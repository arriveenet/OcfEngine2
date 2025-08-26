#pragma once
#include "renderer/backend/Handle.h"
#include "ocf/core/Allocator.h"
#include <unordered_map>

namespace ocf::backend {

template <size_t P0, size_t P1, size_t P2>
class HandleAllocator {
public:
    HandleAllocator(const char* name, size_t size);
    ~HandleAllocator();

    template <typename D, typename... Args>
    Handle<D> allocateAndConstruct(Args&&... args)
    {
        Handle<D> handle{allocateHandle<D>()};
        void* p = m_handleMap[handle.getId()];
        new (p) D(std::forward<Args>(args)...);
        return handle;
    }

    template<typename D>
    Handle<D> allocate() noexcept
    {
        Handle<D> handle{allocateHandle<D>()};
        return handle;
    }

    template<typename Dp, typename B>
    inline std::enable_if_t<
        std::is_pointer_v<Dp> &&
        std::is_base_of_v<B, std::remove_pointer_t<Dp>>, Dp>
    handle_cast(Handle<B> handle) const noexcept
    {
        auto [p, tag] = handleToPointer(handle.getId());

       return static_cast<Dp>(p);
    }

private:

    template <typename D> static constexpr size_t getBucketSize() noexcept
    {
        if constexpr (sizeof(D) <= P0) return P0;
        if constexpr (sizeof(D) <= P1) return P1;
        static_assert(sizeof(D) <= P2, "Handle type is too large");
        return P2;
    }

    class Allocator {
        friend class HandleAllocator;
        static constexpr size_t MIN_ALIGNMENT = alignof(std::max_align_t);
        struct Node {
            uint8_t age;
        };
        template <size_t SIZE>
        using Pool = MemoryPool<SIZE, MIN_ALIGNMENT, sizeof(Node)>;
        Pool<P0> m_pool0;
        Pool<P1> m_pool1;
        Pool<P2> m_pool2;
        const AreaPolicy::HeapArea& m_area;

    public:
        explicit Allocator(const AreaPolicy::HeapArea& area);

        static constexpr size_t getAliment() noexcept { return MIN_ALIGNMENT; }

        void* alloc(size_t size, size_t, size_t, uint8_t* outAge) noexcept
        {
            void* p = nullptr;
            if      (size <= m_pool0.getSize()) p = m_pool0.alloc(size);
            else if (size <= m_pool1.getSize()) p = m_pool1.alloc(size);
            else if (size <= m_pool2.getSize()) p = m_pool2.alloc(size);

            if (p != nullptr) {
                const Node* const pNode = static_cast<Node const*>(p);
                *outAge = pNode[-1].age;
            }
            return p;
        }

        void free(void* p, size_t size, size_t, size_t) noexcept
        {
            Node* const pNode = static_cast<Node*>(p);
            uint8_t& expectedAge = pNode[-1].age;
            expectedAge = (expectedAge + 1) & 0xF; // fixme

            if (size <= m_pool0.getSize()) { m_pool0.free(p); return; }
            if (size <= m_pool1.getSize()) { m_pool1.free(p); return; }
            if (size <= m_pool2.getSize()) { m_pool2.free(p); return; }

        }
    };

    using HandleLinerAllocator = LinearAllocator<Allocator>;

    template <typename D>
    HandleBase::HandleId allocateHandle() noexcept
    {
        constexpr size_t BUCKET_SIZE = getBucketSize<D>();
        return allocateHandleInPool<BUCKET_SIZE>();
    }

    template <size_t SIZE>
    HandleBase::HandleId allocateHandleInPool() noexcept
    {
        uint8_t age;
        void* p = m_allocator.alloc(SIZE, alignof(std::max_align_t), 0, &age);
        if (p != nullptr) {
            return linearPoolPointerToHandle(p, age);
        }

        return allocateHandleMap(SIZE);
    }

    std::pair<void*, uint32_t> handleToPointer(HandleBase::HandleId id) const noexcept
    {
        if (isPoolHandle(id)) {
            char* base = static_cast<char*>(m_allocator.getArea().begin());
            uint32_t offset = (id & HANDLE_INDEX_MASK) * Allocator::getAliment();
            return { static_cast<void*>(base + offset), 0 };
        }
        return { handleToPointerHandleMap(id), 0 };
    }

    HandleBase::HandleId linearPoolPointerToHandle(void* p, uint32_t tag) const noexcept
    {
        char* base = static_cast<char*>(m_allocator.getArea().begin());
        size_t offset = static_cast<char*>(p) - base;
        auto id = static_cast<HandleBase::HandleId>(offset / Allocator::getAliment());
        return id;
    }

    HandleBase::HandleId allocateHandleMap(size_t size);

    void* handleToPointerHandleMap(HandleBase::HandleId id) const noexcept;

    static constexpr uint32_t HANDLE_INDEX_MASK = 0x07FFFFFFu;

    static constexpr uint32_t HANDLE_HEAP_FLAG = 0x80000000u;

    static bool isPoolHandle(HandleBase::HandleId id) noexcept
    {
        return (id & HANDLE_HEAP_FLAG) == 0u;
    }

private:
    HandleLinerAllocator m_allocator;
    std::unordered_map<HandleBase::HandleId, void*> m_handleMap;
    HandleBase::HandleId m_id = 0;
};

using HandleAllocatorGL = HandleAllocator<32, 96, 184>;

} // namespace ocf::backend
