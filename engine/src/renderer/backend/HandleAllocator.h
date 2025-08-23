#pragma once
#include "renderer/backend/Handle.h"
#include <unordered_map>

namespace ocf::backend {

template <size_t P0, size_t P1, size_t P2>
class HandleAllocator {
public:
    HandleAllocator();
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
        std::is_base_of_v<B, std::remove_pointer<Dp>>, Dp>
    handle_cast(Handle<B> handle) const noexcept
    {
        auto [p, tag] = handleToPointer(handle.getId());

        static_cast<Dp>(p);
    }

private:

    template <typename D> static constexpr size_t getBucketSize() noexcept
    {
        if constexpr (sizeof(D) <= P0) return P0;
        if constexpr (sizeof(D) <= P1) return P1;
        static_assert(sizeof(D) <= P2, "Handle type is too large");
        return P2;
    }


    template <typename D>
    HandleBase::HandleId allocateHandle() noexcept
    {
        constexpr size_t BUCKET_SIZE = getBucketSize<D>();
        return allocateHandleInPool<BUCKET_SIZE>();
    }

    template <size_t SIZE>
    HandleBase::HandleId allocateHandleInPool() noexcept
    {
        return allocateHandleMap(SIZE);
    }

    std::pair<void*, uint32_t> handleToPointer(HandleBase::HandleId id) const noexcept
    {
        return {handleToPointerHandleMap(id), 0};
    }

    HandleBase::HandleId allocateHandleMap(size_t size);

    void* handleToPointerHandleMap(HandleBase::HandleId id) const noexcept;

private:
    std::unordered_map<HandleBase::HandleId, void*> m_handleMap;
    HandleBase::HandleId m_id = 0;
};

using HandleAllocatorGL = HandleAllocator<32, 96, 184>;

} // namespace ocf::backend
