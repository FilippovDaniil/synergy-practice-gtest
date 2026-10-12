#pragma once

// Эталонная (вспомогательная) реализация интерфейсов.
// Не является сдаваемой частью задания: нужна только для того, чтобы доказать,
// что тесты корректны (на ней все тесты зелёные).

#include <cstddef>
#include <deque>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

#include "ds/binary_tree.hpp"
#include "ds/heap.hpp"
#include "ds/queue.hpp"

namespace ds::reference {

// ---------------------------------------------------------------- Queue (FIFO)

template <typename T>
class Queue final : public IQueue<T> {
public:
    void push(const T& value) override { items_.push_back(value); }

    T pop() override {
        if (items_.empty()) {
            throw std::out_of_range("Queue::pop on empty queue");
        }
        T front = std::move(items_.front());
        items_.pop_front();
        return front;
    }

    bool empty() const override { return items_.empty(); }
    std::size_t size() const override { return items_.size(); }

private:
    std::deque<T> items_;
};

// ------------------------------------------------- Heap (двоичная куча на массиве)

template <typename T, typename Compare = std::less<T>>
class Heap final : public IHeap<T, Compare> {
public:
    void push(const T& value) override {
        data_.push_back(value);
        siftUp(data_.size() - 1);
    }

    T pop() override {
        if (data_.empty()) {
            throw std::out_of_range("Heap::pop on empty heap");
        }
        T top = std::move(data_.front());
        data_.front() = std::move(data_.back());
        data_.pop_back();
        if (!data_.empty()) {
            siftDown(0);
        }
        return top;
    }

    bool empty() const override { return data_.empty(); }
    std::size_t size() const override { return data_.size(); }

private:
    void siftUp(std::size_t i) {
        while (i > 0) {
            const std::size_t parent = (i - 1) / 2;
            if (!less_(data_[i], data_[parent])) {
                break;
            }
            std::swap(data_[i], data_[parent]);
            i = parent;
        }
    }

    void siftDown(std::size_t i) {
        const std::size_t n = data_.size();
        while (true) {
            const std::size_t left = 2 * i + 1;
            const std::size_t right = left + 1;
            std::size_t best = i;
            if (left < n && less_(data_[left], data_[best])) {
                best = left;
            }
            if (right < n && less_(data_[right], data_[best])) {
                best = right;
            }
            if (best == i) {
                break;
            }
            std::swap(data_[i], data_[best]);
            i = best;
        }
    }

    std::vector<T> data_;
    Compare less_;
};

// ------------------------------------------------ BinaryTree (BST без дубликатов)

template <typename T>
class BinaryTree final : public IBinaryTree<T> {
public:
    BinaryTree() = default;
    BinaryTree(const BinaryTree&) = delete;
    BinaryTree& operator=(const BinaryTree&) = delete;

    // Итеративное разрушение: вырожденное дерево не переполнит стек
    ~BinaryTree() override {
        std::vector<std::unique_ptr<Node>> stack;
        if (root_) {
            stack.push_back(std::move(root_));
        }
        while (!stack.empty()) {
            std::unique_ptr<Node> node = std::move(stack.back());
            stack.pop_back();
            if (node->left) {
                stack.push_back(std::move(node->left));
            }
            if (node->right) {
                stack.push_back(std::move(node->right));
            }
        }
    }

    bool push(const T& value) override {
        std::unique_ptr<Node>* link = &root_;
        while (*link) {
            if (value < (*link)->value) {
                link = &(*link)->left;
            } else if ((*link)->value < value) {
                link = &(*link)->right;
            } else {
                return false;  // дубликат
            }
        }
        *link = std::make_unique<Node>(value);
        ++size_;
        return true;
    }

    bool pop(const T& value) override {
        std::unique_ptr<Node>* link = findLink(value);
        if (!*link) {
            return false;
        }
        Node* node = link->get();
        if (!node->left) {
            *link = std::move(node->right);           // лист или только правый потомок
        } else if (!node->right) {
            *link = std::move(node->left);            // только левый потомок
        } else {
            // Два потомка: заменить значение минимумом правого поддерева
            std::unique_ptr<Node>* succ = &node->right;
            while ((*succ)->left) {
                succ = &(*succ)->left;
            }
            node->value = std::move((*succ)->value);
            *succ = std::move((*succ)->right);
        }
        --size_;
        return true;
    }

    bool search(const T& value) const override {
        const Node* node = root_.get();
        while (node) {
            if (value < node->value) {
                node = node->left.get();
            } else if (node->value < value) {
                node = node->right.get();
            } else {
                return true;
            }
        }
        return false;
    }

    bool empty() const override { return size_ == 0; }
    std::size_t size() const override { return size_; }

private:
    struct Node {
        explicit Node(const T& v) : value(v) {}
        T value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

    // Ссылка (владеющий указатель) на узел со значением value, либо на пустое место
    std::unique_ptr<Node>* findLink(const T& value) {
        std::unique_ptr<Node>* link = &root_;
        while (*link) {
            if (value < (*link)->value) {
                link = &(*link)->left;
            } else if ((*link)->value < value) {
                link = &(*link)->right;
            } else {
                break;
            }
        }
        return link;
    }

    std::unique_ptr<Node> root_;
    std::size_t size_ = 0;
};

} // namespace ds::reference
