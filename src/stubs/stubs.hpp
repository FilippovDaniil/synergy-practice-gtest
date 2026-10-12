#pragma once

// Заглушки: реализуют интерфейсы, но каждый метод бросает std::logic_error.
// Нужны, чтобы тесты компилировались и запускались до появления реализации (TDD: red).

#include <cstddef>
#include <functional>
#include <stdexcept>

#include "ds/binary_tree.hpp"
#include "ds/heap.hpp"
#include "ds/queue.hpp"

namespace ds::stubs {

[[noreturn]] inline void notImplemented() {
    throw std::logic_error("not implemented");
}

template <typename T>
class Queue final : public IQueue<T> {
public:
    void push(const T&) override { notImplemented(); }
    T pop() override { notImplemented(); }
    bool empty() const override { notImplemented(); }
    std::size_t size() const override { notImplemented(); }
};

template <typename T, typename Compare = std::less<T>>
class Heap final : public IHeap<T, Compare> {
public:
    void push(const T&) override { notImplemented(); }
    T pop() override { notImplemented(); }
    bool empty() const override { notImplemented(); }
    std::size_t size() const override { notImplemented(); }
};

template <typename T>
class BinaryTree final : public IBinaryTree<T> {
public:
    bool push(const T&) override { notImplemented(); }
    bool pop(const T&) override { notImplemented(); }
    bool search(const T&) const override { notImplemented(); }
    bool empty() const override { notImplemented(); }
    std::size_t size() const override { notImplemented(); }
};

} // namespace ds::stubs
