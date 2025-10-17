# ハンドル使用例

## 概要

このドキュメントでは、OcfEngine2のハンドルシステムを使用した実際のコード例を示します。

## 基本的な使い方

### HandleAllocatorの初期化

```cpp
#include "renderer/backend/HandleAllocator.h"

// HandleAllocatorを作成（10MBのメモリプールを確保）
ocf::backend::HandleAllocatorGL handleAllocator("GLHandles", 10 * 1024 * 1024);
```

### ハンドルの作成と破棄

#### 方法1: allocateAndConstruct（推奨）

```cpp
// 頂点バッファを作成（コンストラクタ引数を直接渡す）
auto vbHandle = handleAllocator.allocateAndConstruct<HwVertexBuffer>(
    1000,    // vertexCount
    12000    // byteCount
);

// 使用
HwVertexBuffer* vb = handleAllocator.handle_cast<HwVertexBuffer*>(vbHandle);
vb->bufferObjectVertion = 1;

// 破棄
handleAllocator.deallocate(vbHandle, vb);
```

#### 方法2: allocate + placement new

```cpp
// ハンドルのみを割り当て
auto vbHandle = handleAllocator.allocate<HwVertexBuffer>();

// ポインタ取得
HwVertexBuffer* vb = handleAllocator.handle_cast<HwVertexBuffer*>(vbHandle);

// placement newで初期化
new (vb) HwVertexBuffer(1000, 12000);

// 使用
vb->bufferObjectVertion = 1;

// 破棄
handleAllocator.deallocate(vbHandle, vb);
```

## 各リソースタイプの使用例

### 頂点バッファ情報（VertexBufferInfo）

```cpp
// 8つの属性を持つ頂点バッファ情報を作成
auto vbiHandle = handleAllocator.allocateAndConstruct<HwVertexBufferInfo>(8);

// アクセス
HwVertexBufferInfo* vbi = handleAllocator.handle_cast<HwVertexBufferInfo*>(vbiHandle);
std::cout << "Attribute count: " << vbi->attributeCount << std::endl;

// 破棄
handleAllocator.deallocate(vbiHandle, vbi);
```

### インデックスバッファ

```cpp
// 16ビットインデックス、3000個の要素
auto ibHandle = handleAllocator.allocateAndConstruct<HwIndexBuffer>(
    2,      // elementSize (16ビット = 2バイト)
    3000    // indexCount
);

HwIndexBuffer* ib = handleAllocator.handle_cast<HwIndexBuffer*>(ibHandle);
std::cout << "Index count: " << ib->count << std::endl;
std::cout << "Element size: " << ib->elementSize << " bytes" << std::endl;

handleAllocator.deallocate(ibHandle, ib);
```

### レンダープリミティブ

```cpp
// 三角形プリミティブを作成
auto rpHandle = handleAllocator.allocateAndConstruct<HwRenderPrimitive>();

HwRenderPrimitive* rp = handleAllocator.handle_cast<HwRenderPrimitive*>(rpHandle);
rp->type = PrimitiveType::TRIANGLES;

handleAllocator.deallocate(rpHandle, rp);
```

### シェーダープログラム

```cpp
// プログラムを作成
auto programHandle = handleAllocator.allocateAndConstruct<HwProgram>();

HwProgram* program = handleAllocator.handle_cast<HwProgram*>(programHandle);
// プログラムの設定...

handleAllocator.deallocate(programHandle, program);
```

### テクスチャ

```cpp
// 2Dテクスチャを作成
auto texHandle = handleAllocator.allocateAndConstruct<HwTexture>();

HwTexture* tex = handleAllocator.handle_cast<HwTexture*>(texHandle);
tex->width = 512;
tex->height = 512;
tex->depth = 1;
tex->target = SamplerType::SAMPLER_2D;

handleAllocator.deallocate(texHandle, tex);
```

## 高度な使用パターン

### ハンドルの有効性チェック

```cpp
ocf::backend::VertexBufferHandle vbHandle;

// ハンドルの有効性チェック
if (!vbHandle) {
    std::cout << "ハンドルは無効です" << std::endl;
}

// ハンドルを作成
vbHandle = handleAllocator.allocateAndConstruct<HwVertexBuffer>(100, 1200);

if (vbHandle) {
    std::cout << "ハンドルは有効です" << std::endl;
}

// クリア
vbHandle.clear();

if (!vbHandle) {
    std::cout << "ハンドルは再び無効になりました" << std::endl;
}
```

### ムーブセマンティクス

```cpp
// ハンドルを作成
auto vbHandle1 = handleAllocator.allocateAndConstruct<HwVertexBuffer>(100, 1200);

// ムーブ
auto vbHandle2 = std::move(vbHandle1);

// vbHandle1は無効になる
assert(!vbHandle1);
assert(vbHandle2);

// vbHandle2を使用して破棄
HwVertexBuffer* vb = handleAllocator.handle_cast<HwVertexBuffer*>(vbHandle2);
handleAllocator.deallocate(vbHandle2, vb);
```

### ハンドルIDの取得

```cpp
auto vbHandle = handleAllocator.allocateAndConstruct<HwVertexBuffer>(100, 1200);

// ハンドルIDを取得
uint32_t handleId = vbHandle.getId();
std::cout << "Handle ID: 0x" << std::hex << handleId << std::endl;

// ハンドルIDから再構築（注意：通常は推奨されない）
ocf::backend::VertexBufferHandle reconstructedHandle(handleId);
```

### オブジェクトの再構築（destroyAndConstruct）

```cpp
// 基底クラスのハンドル
ocf::backend::Handle<HwBase> baseHandle = 
    handleAllocator.allocateAndConstruct<HwVertexBuffer>(100, 1200);

// 別の型で再構築（同じメモリ位置）
HwIndexBuffer* ib = handleAllocator.destroyAndConstruct<HwIndexBuffer>(
    baseHandle,
    2,      // elementSize
    3000    // indexCount
);

// 使用
std::cout << "Index count: " << ib->count << std::endl;

// 破棄
handleAllocator.deallocate(baseHandle, ib);
```

## OpenGLDriverでの使用例

OpenGLDriverクラスでの実際の使用例：

```cpp
class OpenGLDriver {
    HandleAllocatorGL m_handleAllocator;
    
public:
    OpenGLDriver() 
        : m_handleAllocator("OpenGLHandles", 10 * 1024 * 1024) 
    {
    }
    
    // 頂点バッファの作成
    VertexBufferHandle createVertexBuffer(
        uint32_t vertexCount, 
        uint32_t byteCount) 
    {
        return m_handleAllocator.allocateAndConstruct<HwVertexBuffer>(
            vertexCount, 
            byteCount
        );
    }
    
    // 頂点バッファの破棄
    void destroyVertexBuffer(VertexBufferHandle handle) {
        HwVertexBuffer* vb = m_handleAllocator.handle_cast<HwVertexBuffer*>(handle);
        m_handleAllocator.deallocate(handle, vb);
    }
    
    // 頂点バッファへのアクセス
    void updateVertexBuffer(VertexBufferHandle handle, const void* data, size_t size) {
        HwVertexBuffer* vb = m_handleAllocator.handle_cast<HwVertexBuffer*>(handle);
        // OpenGLのバッファ更新処理...
    }
};
```

## エラーハンドリング

### プールが満杯の場合

プールが満杯の場合、自動的にヒープにフォールバックします：

```cpp
// 多数のハンドルを作成
std::vector<VertexBufferHandle> handles;

for (int i = 0; i < 100000; ++i) {
    auto handle = handleAllocator.allocateAndConstruct<HwVertexBuffer>(100, 1200);
    handles.push_back(handle);
    
    // プールが満杯になると、自動的にヒープから割り当て
    // ハンドルIDのビット31が1になる
    if (handle.getId() & 0x80000000) {
        std::cout << "ヒープから割り当て: " << i << std::endl;
    }
}

// すべて破棄
for (auto& handle : handles) {
    HwVertexBuffer* vb = handleAllocator.handle_cast<HwVertexBuffer*>(handle);
    handleAllocator.deallocate(handle, vb);
}
```

### ハンドルの型安全性

コンパイル時の型チェックにより、誤った型のキャストは防止されます：

```cpp
auto vbHandle = handleAllocator.allocateAndConstruct<HwVertexBuffer>(100, 1200);

// OK: 正しい型
HwVertexBuffer* vb = handleAllocator.handle_cast<HwVertexBuffer*>(vbHandle);

// コンパイルエラー: 無関係な型へのキャスト
// HwTexture* tex = handleAllocator.handle_cast<HwTexture*>(vbHandle);

// OK: 基底クラスへのキャスト
HwBase* base = handleAllocator.handle_cast<HwBase*>(vbHandle);
```

## ベストプラクティス

### 1. RAII パターンの使用

```cpp
class VertexBufferWrapper {
    HandleAllocatorGL& m_allocator;
    VertexBufferHandle m_handle;
    
public:
    VertexBufferWrapper(HandleAllocatorGL& allocator, uint32_t count, uint32_t size)
        : m_allocator(allocator)
        , m_handle(allocator.allocateAndConstruct<HwVertexBuffer>(count, size))
    {
    }
    
    ~VertexBufferWrapper() {
        if (m_handle) {
            HwVertexBuffer* vb = m_allocator.handle_cast<HwVertexBuffer*>(m_handle);
            m_allocator.deallocate(m_handle, vb);
        }
    }
    
    // コピー禁止
    VertexBufferWrapper(const VertexBufferWrapper&) = delete;
    VertexBufferWrapper& operator=(const VertexBufferWrapper&) = delete;
    
    // ムーブ許可
    VertexBufferWrapper(VertexBufferWrapper&& other) noexcept
        : m_allocator(other.m_allocator)
        , m_handle(std::move(other.m_handle))
    {
    }
    
    VertexBufferHandle getHandle() const { return m_handle; }
};
```

### 2. ハンドルのコンテナ管理

```cpp
class ResourceManager {
    HandleAllocatorGL m_allocator;
    std::vector<VertexBufferHandle> m_vertexBuffers;
    std::vector<TextureHandle> m_textures;
    
public:
    ResourceManager() : m_allocator("Resources", 20 * 1024 * 1024) {}
    
    VertexBufferHandle createVertexBuffer(uint32_t count, uint32_t size) {
        auto handle = m_allocator.allocateAndConstruct<HwVertexBuffer>(count, size);
        m_vertexBuffers.push_back(handle);
        return handle;
    }
    
    void destroyAllResources() {
        // すべての頂点バッファを破棄
        for (auto& handle : m_vertexBuffers) {
            HwVertexBuffer* vb = m_allocator.handle_cast<HwVertexBuffer*>(handle);
            m_allocator.deallocate(handle, vb);
        }
        m_vertexBuffers.clear();
        
        // すべてのテクスチャを破棄
        for (auto& handle : m_textures) {
            HwTexture* tex = m_allocator.handle_cast<HwTexture*>(handle);
            m_allocator.deallocate(handle, tex);
        }
        m_textures.clear();
    }
};
```

### 3. ハンドルの検証

```cpp
bool isValidHandle(const VertexBufferHandle& handle, HandleAllocatorGL& allocator) {
    if (!handle) {
        return false;
    }
    
    // プールハンドルの場合、範囲チェック
    if (!(handle.getId() & 0x80000000)) {
        // 実際の実装では、allocatorの範囲内かチェック
        return true;
    }
    
    // ヒープハンドルの場合、マップに存在するかチェック
    // （実際の実装では内部APIが必要）
    return true;
}
```

## パフォーマンス測定

```cpp
#include <chrono>

void benchmarkHandleAllocation(HandleAllocatorGL& allocator) {
    constexpr int N = 100000;
    std::vector<VertexBufferHandle> handles;
    handles.reserve(N);
    
    // 割り当て時間測定
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < N; ++i) {
        handles.push_back(
            allocator.allocateAndConstruct<HwVertexBuffer>(100, 1200)
        );
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "割り当て時間: " << duration.count() << " μs" << std::endl;
    std::cout << "1回あたり: " << (duration.count() / N) << " μs" << std::endl;
    
    // 解放時間測定
    start = std::chrono::high_resolution_clock::now();
    
    for (auto& handle : handles) {
        HwVertexBuffer* vb = allocator.handle_cast<HwVertexBuffer*>(handle);
        allocator.deallocate(handle, vb);
    }
    
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "解放時間: " << duration.count() << " μs" << std::endl;
    std::cout << "1回あたり: " << (duration.count() / N) << " μs" << std::endl;
}
```
