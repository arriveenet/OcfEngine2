# ハンドルシステム

## 概要

OcfEngine2のバックエンドは、型安全で効率的なハンドルシステムを採用しています。ハンドルは、リソース（バッファ、テクスチャ、プログラムなど）を間接的に参照するための軽量な識別子です。

## ハンドルの構造

### HandleBase

すべてのハンドルの基底クラスです。

```cpp
class HandleBase {
public:
    using HandleId = uint32_t;
    static constexpr HandleId nullid = UINT32_MAX;
    
    HandleBase() noexcept : object(nullid) {}
    explicit HandleBase(HandleId id) noexcept : object(id) { }
    
    explicit operator bool() const noexcept { return object != nullid; }
    void clear() noexcept { object = nullid; }
    HandleId getId() const { return object; }
    
private:
    HandleId object;  // 32ビットのハンドルID
};
```

#### 主要な特徴

- **32ビット識別子**: `uint32_t`型のIDを使用
- **無効値**: `UINT32_MAX`が無効なハンドルを表す
- **bool変換**: ハンドルの有効性を簡単にチェック可能
- **ムーブセマンティクス**: 効率的な所有権の移動をサポート

### Handle<T> テンプレート

型安全性を提供するテンプレートクラスです。

```cpp
template <typename T>
struct Handle: public HandleBase {
    Handle() noexcept = default;
    explicit Handle(HandleId id) noexcept : HandleBase(id) { }
    
    // コピーとムーブをサポート
    Handle(const Handle& rhs) noexcept = default;
    Handle(Handle&& rhs) noexcept = default;
    Handle& operator=(const Handle& rhs) noexcept;
    Handle& operator=(Handle&& rhs) noexcept;
};
```

#### 型の定義

以下の型定義されたハンドルが用意されています：

```cpp
using VertexBufferInfoHandle = Handle<HwVertexBufferInfo>;
using VertexBufferHandle     = Handle<HwVertexBuffer>;
using IndexBufferHandle      = Handle<HwIndexBuffer>;
using RenderPrimitiveHandle  = Handle<HwRenderPrimitive>;
using ProgramHandle          = Handle<HwProgram>;
using TextureHandle          = Handle<HwTexture>;
```

## ハンドルIDの構造

ハンドルIDは32ビットで、以下のように構成されています：

```
 31 30        27 26                                  0
┌──┬───────────┬─────────────────────────────────────┐
│H │   Age    │           Index                      │
└──┴───────────┴─────────────────────────────────────┘
```

- **ビット31 (H)**: ヒープフラグ（1=ヒープ割り当て、0=プール割り当て）
- **ビット30-27**: 世代カウンター（4ビット、0-15）
- **ビット26-0**: インデックス（メモリ内のオフセット）

### フラグとマスク

```cpp
static constexpr uint32_t HANDLE_AGE_BIT_COUNT = 4u;
static constexpr uint32_t HANDLE_AGE_SHIFT     = 27u;
static constexpr uint32_t HANDLE_AGE_MASK      = 0x78000000u;  // ビット30-27
static constexpr uint32_t HANDLE_INDEX_MASK    = 0x07FFFFFFu;  // ビット26-0
static constexpr uint32_t HANDLE_HEAP_FLAG     = 0x80000000u;  // ビット31
```

## 世代カウンター（Age）の役割

世代カウンターは、**ABA問題**を防ぐために使用されます。

### ABA問題とは

1. スレッドAがハンドルIDを取得
2. そのメモリが解放される
3. 同じメモリ位置が別のオブジェクトに再利用される
4. スレッドAが古いハンドルIDを使ってアクセスしようとする

### 解決方法

メモリが解放されるたびに世代カウンターをインクリメントします（0-15で循環）。これにより、同じメモリ位置でも異なるハンドルIDになり、古いハンドルは無効になります。

```cpp
void free(void* p, size_t size, uint8_t) noexcept {
    Node* const pNode = static_cast<Node*>(p);
    uint8_t& expectedAge = pNode[-1].age;
    expectedAge = (expectedAge + 1) & 0xF;  // 0-15で循環
    // ... プールに返却
}
```

## プールとヒープの判別

ハンドルIDの最上位ビット（ビット31）で、メモリがプールとヒープのどちらから割り当てられたかを判別します：

```cpp
static bool isPoolHandle(HandleBase::HandleId id) noexcept {
    return (id & HANDLE_HEAP_FLAG) == 0u;
}
```

- **ビット31=0**: プールから割り当て（高速、固定サイズ）
- **ビット31=1**: ヒープから割り当て（柔軟、可変サイズ）

## ハンドルとポインタの変換

### ハンドルからポインタへ

```cpp
std::pair<void*, uint32_t> handleToPointer(HandleBase::HandleId id) const noexcept {
    if (isPoolHandle(id)) {
        // プールの場合：ベースアドレス + オフセット
        char* base = static_cast<char*>(m_allocator.getArea().begin());
        size_t offset = (id & HANDLE_INDEX_MASK) * Allocator::getAliment();
        return { static_cast<void*>(base + offset), 0 };
    }
    // ヒープの場合：マップから検索
    return { handleToPointerHandleMap(id), 0 };
}
```

### ポインタからハンドルへ

```cpp
HandleBase::HandleId linearPoolPointerToHandle(void* p, uint32_t tag) const noexcept {
    char* base = static_cast<char*>(m_allocator.getArea().begin());
    size_t offset = static_cast<char*>(p) - base;
    auto id = static_cast<HandleBase::HandleId>(offset / Allocator::getAliment());
    id |= tag & HANDLE_AGE_MASK;  // 世代情報を埋め込む
    return id;
}
```

## 型安全なキャスト

`handle_cast`関数は、ハンドルから型付きポインタへの安全な変換を提供します：

```cpp
template<typename Dp, typename B>
inline std::enable_if_t<
    std::is_pointer_v<Dp> &&
    std::is_base_of_v<B, std::remove_pointer_t<Dp>>, Dp>
handle_cast(Handle<B>& handle) const noexcept {
    auto [p, tag] = handleToPointer(handle.getId());
    return static_cast<Dp>(p);
}
```

この関数は、SFINAEを使用して以下を保証します：
- `Dp`がポインタ型であること
- `Dp`が指す型が`B`の派生クラスであること

## 利点

1. **型安全性**: コンパイル時に型チェックが行われる
2. **メモリ効率**: ポインタ（64ビット）より小さい32ビット
3. **安全性**: 世代カウンターによる不正アクセスの検出
4. **柔軟性**: プールとヒープの両方をサポート
5. **パフォーマンス**: 直接的なポインタ計算で高速アクセス
