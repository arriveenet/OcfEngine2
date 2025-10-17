# メモリアロケーション

## 概要

HandleAllocatorは、3段階のメモリプーリング戦略を使用して、効率的なメモリ管理を実現しています。この設計により、小さなオブジェクトの高速な割り当てと解放が可能になります。

## HandleAllocatorの構造

```cpp
template <size_t P0, size_t P1, size_t P2>
class HandleAllocator {
    // P0, P1, P2: 3つのプールのサイズ（バイト）
    // 例: HandleAllocatorGL = HandleAllocator<32, 96, 184>
};
```

### 型定義

```cpp
using HandleAllocatorGL = HandleAllocator<32, 96, 184>;
```

OpenGL用のハンドルアロケーターは、以下の3つのプールを持ちます：
- **プール0**: 32バイト以下のオブジェクト
- **プール1**: 33～96バイトのオブジェクト
- **プール2**: 97～184バイトのオブジェクト

## メモリプール階層

### プールサイズの決定

コンパイル時に、オブジェクトのサイズに基づいて適切なプールが選択されます：

```cpp
template <typename D> 
static constexpr size_t getBucketSize() noexcept {
    if constexpr (sizeof(D) <= P0) return P0;
    if constexpr (sizeof(D) <= P1) return P1;
    static_assert(sizeof(D) <= P2, "Handle type is too large");
    return P2;
}
```

### リソース型とプールサイズ

各リソース型は、そのサイズに応じて適切なプールに配置されます：

| リソース型 | 構造体サイズ | 使用プール |
|-----------|------------|----------|
| HwVertexBufferInfo | 4バイト | プール0 (32) |
| HwVertexBuffer | 12バイト | プール0 (32) |
| HwIndexBuffer | 8バイト | プール0 (32) |
| HwRenderPrimitive | 4バイト | プール0 (32) |
| HwProgram | 小 | プール0 (32) |
| HwTexture | 16バイト | プール0 (32) |

## 内部Allocatorクラス

### 構造

```cpp
class Allocator {
    static constexpr size_t MIN_ALIGNMENT = alignof(std::max_align_t);
    
    struct Node {
        uint8_t age;  // 世代カウンター
    };
    
    template <size_t SIZE>
    using Pool = MemoryPool<SIZE, MIN_ALIGNMENT, sizeof(Node)>;
    
    Pool<P0> m_pool0;  // 32バイトプール
    Pool<P1> m_pool1;  // 96バイトプール
    Pool<P2> m_pool2;  // 184バイトプール
    
    const AreaPolicy::HeapArea& m_area;
};
```

### 初期化

```cpp
Allocator::Allocator(const AreaPolicy::HeapArea& area)
    : m_area(area)
{
    const size_t heapSizeMax = area.size();
    memset(area.data(), 0, heapSizeMax);
    
    // 均等に3つのプールに分割
    const size_t count = heapSizeMax / (P0 + P1 + P2);
    char* p0 = static_cast<char*>(area.begin());
    char* p1 = p0 + count * P0;
    char* p2 = p1 + count * P1;
    
    m_pool0 = Pool<P0>(p0, count * P0);
    m_pool1 = Pool<P1>(p1, count * P1);
    m_pool2 = Pool<P2>(p2, count * P2);
}
```

この初期化により、ヒープ領域を3つのプールに均等に分割します。例えば、1MBのヒープがある場合：
- プール0: 約32/312 ≈ 10.3%
- プール1: 約96/312 ≈ 30.8%
- プール2: 約184/312 ≈ 58.9%

## メモリ割り当ての流れ

### 1. ハンドルの割り当て

```cpp
template <typename D, typename... Args>
Handle<D> allocateAndConstruct(Args&&... args) {
    // 1. ハンドルIDを割り当て
    Handle<D> handle{allocateHandle<D>()};
    
    // 2. ハンドルからポインタを取得
    D* addr = handle_cast<D*>(handle);
    
    // 3. placement newでオブジェクトを構築
    new (addr) D(std::forward<Args>(args)...);
    
    return handle;
}
```

### 2. プールからの割り当て

```cpp
template <size_t SIZE>
HandleBase::HandleId allocateHandleInPool() noexcept {
    uint8_t age;
    void* p = m_allocator.alloc(SIZE, alignof(std::max_align_t), 0, &age);
    
    if (p != nullptr) {
        // プールから割り当て成功
        return linearPoolPointerToHandle(p, age);
    }
    
    // プールが満杯の場合はヒープから割り当て
    return allocateHandleMap(SIZE);
}
```

### 3. 実際のメモリ割り当て

```cpp
void* Allocator::alloc(size_t size, size_t, size_t, uint8_t* outAge) noexcept {
    void* p = nullptr;
    
    // サイズに応じて適切なプールから割り当て
    if      (size <= m_pool0.getSize()) p = m_pool0.alloc(size);
    else if (size <= m_pool1.getSize()) p = m_pool1.alloc(size);
    else if (size <= m_pool2.getSize()) p = m_pool2.alloc(size);
    
    if (p != nullptr) {
        // 世代カウンターを取得（メモリブロックの直前に保存されている）
        const Node* const pNode = static_cast<Node const*>(p);
        *outAge = pNode[-1].age;
    }
    
    return p;
}
```

## メモリ解放の流れ

### 1. ハンドルの解放

```cpp
template <typename B, typename D>
void deallocate(Handle<B>& handle, D const* p) noexcept {
    if (p) {
        // デストラクタを呼び出し
        p->~D();
        
        // ハンドルを解放
        deallocateHandle<D>(handle.getId());
    }
}
```

### 2. プールまたはヒープへの返却

```cpp
template <size_t SIZE>
void deallocateHandleInPool(HandleBase::HandleId id) noexcept {
    if (isPoolHandle(id)) {
        // プールに返却
        auto [p, tag] = handleToPointer(id);
        uint8_t age = (tag & HANDLE_AGE_MASK) >> HANDLE_AGE_SHIFT;
        m_allocator.free(p, SIZE, age);
    } else {
        // ヒープから解放
        deallocateHandleMap(id, SIZE);
    }
}
```

### 3. 世代カウンターの更新

```cpp
void Allocator::free(void* p, size_t size, uint8_t) noexcept {
    Node* const pNode = static_cast<Node*>(p);
    uint8_t& expectedAge = pNode[-1].age;
    
    // 世代カウンターをインクリメント（0-15で循環）
    expectedAge = (expectedAge + 1) & 0xF;
    
    // 適切なプールに返却
    if (size <= m_pool0.getSize()) { m_pool0.free(p); return; }
    if (size <= m_pool1.getSize()) { m_pool1.free(p); return; }
    if (size <= m_pool2.getSize()) { m_pool2.free(p); return; }
}
```

## フォールバックヒープメカニズム

プールが満杯の場合、動的メモリ（ヒープ）にフォールバックします。

### ヒープ割り当て

```cpp
HandleBase::HandleId allocateHandleMap(size_t size) {
    // 標準のmallocを使用
    void* p = ::malloc(size);
    
    // 一意なIDを生成（ビット31=1でヒープフラグ）
    HandleBase::HandleId id = ++m_id;
    
    // マップに登録
    m_handleMap.emplace(id, p);
    
    return id;
}
```

### ヒープ解放

```cpp
void deallocateHandleMap(HandleBase::HandleId id, size_t) noexcept {
    void* p = nullptr;
    
    // マップから検索
    const auto& iter = m_handleMap.find(id);
    if (iter != m_handleMap.end()) {
        p = iter->second;
        m_handleMap.erase(iter);
    }
    
    // 標準のfreeを使用
    ::free(p);
}
```

## MemoryPoolの実装

### FreeListベースの管理

```cpp
template <size_t ELEMENT_SIZE, size_t ALIGNMENT = alignof(std::max_align_t), size_t OFFSET = 0>
class MemoryPool {
    FreeList m_freeList;
    
public:
    void* alloc(size_t size = ELEMENT_SIZE, ...) {
        assert(size <= ELEMENT_SIZE);
        return m_freeList.pop();  // フリーリストから取得
    }
    
    void free(void* p, size_t = ELEMENT_SIZE) {
        m_freeList.push(p);  // フリーリストに返却
    }
};
```

### FreeListの構造

```cpp
class FreeList {
    struct Node {
        Node* next;  // 次の空きブロックへのポインタ
    };
    
    Node* m_head;  // フリーリストの先頭
    void* m_begin; // プールの開始アドレス
    void* m_end;   // プールの終了アドレス
    
public:
    void* pop() {
        Node* const head = m_head;
        m_head = head ? head->next : nullptr;
        return head;
    }
    
    void push(void* p) {
        Node* node = static_cast<Node*>(p);
        node->next = m_head;
        m_head = node;
    }
};
```

## パフォーマンス特性

### プール割り当ての利点

1. **O(1)の割り当て・解放**: フリーリストのpush/pop操作のみ
2. **キャッシュ局所性**: 連続したメモリ領域を使用
3. **フラグメンテーション削減**: 固定サイズブロックの再利用
4. **プリアロケーション**: 起動時に一度だけメモリを確保

### ヒープフォールバックの用途

- プールが満杯の場合の安全策
- 非常に大きなオブジェクト（184バイト超）
- 動的なリソース管理

## アライメント要件

すべてのメモリブロックは、`alignof(std::max_align_t)`でアライメントされます：

```cpp
static constexpr size_t MIN_ALIGNMENT = alignof(std::max_align_t);
```

これにより、以下が保証されます：
- すべての標準型に対して適切なアライメント
- SIMD命令での効率的なアクセス
- プラットフォーム間の一貫性

## メモリレイアウト

各メモリブロックのレイアウト：

```
┌────────────┬─────────────────────────┐
│   Node     │    Object Data          │
│  (age)     │    (ELEMENT_SIZE)       │
└────────────┴─────────────────────────┘
    1 byte          可変サイズ
```

- **Node**: 世代カウンター（1バイト）
- **Object Data**: 実際のオブジェクトデータ

ポインタは常にObject Dataの開始位置を指し、`pNode[-1].age`で世代カウンターにアクセスします。

## 使用例

### 初期化

```cpp
// 10MBのハンドルアロケーターを作成
HandleAllocatorGL allocator("GLHandles", 10 * 1024 * 1024);
```

### 割り当て

```cpp
// 頂点バッファを作成
auto vbHandle = allocator.allocateAndConstruct<HwVertexBuffer>(100, 1200);

// ポインタ取得
HwVertexBuffer* vb = allocator.handle_cast<HwVertexBuffer*>(vbHandle);
```

### 解放

```cpp
// オブジェクトを破棄
allocator.deallocate(vbHandle, vb);
```
