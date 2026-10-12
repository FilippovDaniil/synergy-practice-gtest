#pragma once

// Фабрика: единственное место, где тесты узнают о конкретной реализации.
// Сами тесты работают только через интерфейсы ds::IQueue / ds::IHeap / ds::IBinaryTree.
// Выбор реализации — CMake-опция DS_USE_REFERENCE (ON — эталон, OFF — заглушки).

#include <functional>
#include <memory>

#include "ds/binary_tree.hpp"
#include "ds/heap.hpp"
#include "ds/queue.hpp"

#if DS_USE_REFERENCE
#include "reference/reference.hpp"
namespace ds_impl = ds::reference;
#else
#include "stubs/stubs.hpp"
namespace ds_impl = ds::stubs;
#endif

namespace ds::testing {

template <typename T>
std::unique_ptr<IQueue<T>> makeQueue() {
    return std::make_unique<ds_impl::Queue<T>>();
}

template <typename T, typename Compare = std::less<T>>
std::unique_ptr<IHeap<T, Compare>> makeHeap() {
    return std::make_unique<ds_impl::Heap<T, Compare>>();
}

template <typename T>
std::unique_ptr<IBinaryTree<T>> makeBinaryTree() {
    return std::make_unique<ds_impl::BinaryTree<T>>();
}

} // namespace ds::testing
