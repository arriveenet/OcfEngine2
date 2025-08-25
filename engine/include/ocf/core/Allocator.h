#pragma once
#include <cstddef>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>

namespace ocf {

namespace pointermath {

template <typename P, typename T>
static inline P* add(P* a, T b)
{
    return reinterpret_cast<P*>(reinterpret_cast<uintptr_t>(a) + static_cast<uintptr_t>(b));
}

template <typename P>
static inline P* align(P* p, size_t alignment)
{
    // alignment must be a power of two
    assert((alignment & (alignment - 1)) == 0);
    return reinterpret_cast<P*>((reinterpret_cast<uintptr_t>(p) + alignment - 1) &
                                ~(alignment - 1));
}

template <typename P>
static inline P* align(P* p, size_t alignment, size_t offset)
{
    P* const r = align(add(p, offset), alignment);
    assert(r >= add(p, offset));
    return r;
}

} // namespace pointermath

namespace AreaPolicy {

class StaticArea {
public:
    explicit StaticArea(void* begin, void* end)
        : m_begin(begin)
        , m_end(end)
    {
    }

    ~StaticArea() = default;

    void* data() const { return m_begin; }
    void* begin() const { return m_begin; }
    void* end() const { return m_end; }
    size_t size() const { return uintptr_t(m_end) - uintptr_t(m_begin); }

private:
    void* m_begin;
    void* m_end;

};

class HeapArea {
public:
    explicit HeapArea(size_t size)
        : m_begin(nullptr)
        , m_end(nullptr)
    {
        if (size > 0) {
            m_begin = ::malloc(size);
            m_end = pointermath::add(m_begin, size);
        }
    }

    ~HeapArea() { free(m_begin); }

    void* data() const { return m_begin; }
    void* begin() const { return m_begin; }
    void* end() const { return m_end; }
    size_t size() const { return uintptr_t(m_end) - uintptr_t(m_begin); }

private:
    void* m_begin;
    void* m_end;
};

} // namespace AreaPolicy

class FreeList {
public:
    struct Node {
        Node* next;
    };

    FreeList(void* begin, void* end, size_t elementSize, size_t alignment, size_t offset);

    void* pop()
    {
        Node* const head = m_head;
        m_head = head ? head->next : nullptr;
        return head;
    }

    void push(void* p)
    {
        Node* node = static_cast<Node*>(p);
        node->next = m_head;
        m_head = node;
    }

    void* begin() const { return m_begin; }
    void* end() const { return m_end; }

private:
    static Node* init(void* begin, void* end, size_t elementSize, size_t alignment, size_t offset);

    Node* m_head;
    void* m_begin;
    void* m_end;
};

template <size_t ELEMENT_SIZE,
          size_t ALIGNMENT = alignof(std::max_align_t),
          size_t OFFSET = 0 >
class MemoryPool {
public:
    MemoryPool(void* begin, void* end)
        : m_freeList(begin, end, ELEMENT_SIZE, ALIGNMENT, OFFSET)
    {
    }

    MemoryPool(void* begin, size_t size)
        : m_freeList(begin, static_cast<char*>(begin) + size, ELEMENT_SIZE, ALIGNMENT, OFFSET)
    {
    }

    void* alloc()
    {
        return m_freeList.pop();
    }

    void free(void* p, size_t = ELEMENT_SIZE)
    {
        m_freeList.push(p);
    }

private:
    FreeList m_freeList;
};

template <typename Allocator,
          typename AreaPolicy = AreaPolicy::HeapArea>
class LinearAllocator {
public:
    LinearAllocator(const char* name, size_t size)
        : m_name(name)
        , m_area(size)
        , m_allocator(m_area.begin(), m_area.end())
    {
    }

    void* alloc()
    {
        return m_allocator.alloc();
    }

    void free(void* p)
    {
        m_allocator.free(p);
    }

private:
    const char* m_name;
    AreaPolicy m_area;
    Allocator m_allocator;
};

} // namespace ocf
