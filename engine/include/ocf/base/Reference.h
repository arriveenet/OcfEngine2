#pragma once
#include "ocf/base/Object.h"
#include <atomic>

namespace ocf {

class RefCounted : public Object {
public:
    RefCounted();
    ~RefCounted();

    bool retain();
    bool release();

    uint32_t getReferenceCount() const noexcept { return m_referenceCount; }

private:
    mutable std::atomic<uint32_t> m_referenceCount = 0;
};

template<typename T>
class Ref {
public:
    Ref() = default;

    Ref(T* ptr)
        : m_reference(ptr)
    {
         reference(); 
    }

    Ref(const Ref<T>& rhs)
        : m_reference(rhs)
    {
         reference(); 
    }

    Ref(Ref<T>&& rhs)
        : m_reference(*rhs)
    {
         rhs.m_reference = nullptr;
    }

    ~Ref()
    {
        unreference();
    }

    inline bool operator==(const T* ptr)
    {
        return m_reference == ptr;
    }

    inline bool operator!=(const T* ptr)
    {
        return m_reference != ptr;
    }


    inline T* operator*() const {
        return m_reference;
    }

    inline T* operator->() const {
        return m_reference;
    }

    inline T* ptr() const {
        return m_reference;
    }

    template <typename... Args>
    void instantiate(Args&&... args)
    {
        m_reference = new T(args...);
        reference();
    }

private:
    inline void reference()
    {
        if (m_reference != nullptr)
            m_reference->retain(); 
    }

    inline void unreference()
    {
        if (m_reference != nullptr)
            m_reference->release();
    }

    T* m_reference = nullptr;

};

}// namespace ocf

