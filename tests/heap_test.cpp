// Юнит-тесты кучи ds::IHeap (push / pop).
// Тесты работают только через интерфейс; реализация берётся из фабрики.

#include <gtest/gtest.h>

#include <algorithm>
#include <functional>
#include <memory>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#include "factory.hpp"
#include "test_values.hpp"

using ds::testing::value;

template <typename T>
class HeapTest : public ::testing::Test {
protected:
    void SetUp() override { heap = ds::testing::makeHeap<T>(); }

    // Извлечь все элементы кучи в порядке pop()
    std::vector<T> drain() {
        std::vector<T> out;
        while (!heap->empty()) {
            out.push_back(heap->pop());
        }
        return out;
    }

    std::unique_ptr<ds::IHeap<T>> heap;
};

using HeapTypes = ::testing::Types<int, std::string>;
TYPED_TEST_SUITE(HeapTest, HeapTypes, ds::testing::TypeNames);

// 1. Новая куча пуста
TYPED_TEST(HeapTest, Heap_New_IsEmptyWithZeroSize) {
    EXPECT_TRUE(this->heap->empty());
    EXPECT_EQ(this->heap->size(), 0u);
}

// 2. pop из пустой кучи -> std::out_of_range
TYPED_TEST(HeapTest, Heap_PopEmpty_ThrowsOutOfRange) {
    EXPECT_THROW(this->heap->pop(), std::out_of_range);

    this->heap->push(value<TypeParam>(1));
    this->heap->pop();
    EXPECT_THROW(this->heap->pop(), std::out_of_range);
}

// 3. pop возвращает минимум независимо от порядка вставки
TYPED_TEST(HeapTest, Heap_Pop_ReturnsMinimum) {
    this->heap->push(value<TypeParam>(5));
    this->heap->push(value<TypeParam>(2));
    this->heap->push(value<TypeParam>(8));
    this->heap->push(value<TypeParam>(1));
    this->heap->push(value<TypeParam>(9));
    ASSERT_EQ(this->heap->size(), 5u);

    EXPECT_EQ(this->heap->pop(), value<TypeParam>(1));
    EXPECT_EQ(this->heap->size(), 4u);
}

// 4. Последовательные pop дают отсортированную по возрастанию последовательность
TYPED_TEST(HeapTest, Heap_SequentialPops_ReturnSortedSequence) {
    for (int v : {7, 3, 9, 1, 5, 8, 2, 6, 4, 0}) {
        this->heap->push(value<TypeParam>(v));
    }

    std::vector<TypeParam> expected;
    for (int v = 0; v <= 9; ++v) {
        expected.push_back(value<TypeParam>(v));
    }
    EXPECT_EQ(this->drain(), expected);
}

// 5. Дубликаты извлекаются все
TYPED_TEST(HeapTest, Heap_Duplicates_AllExtracted) {
    for (int v : {4, 2, 4, 2, 4}) {
        this->heap->push(value<TypeParam>(v));
    }
    ASSERT_EQ(this->heap->size(), 5u);

    const std::vector<TypeParam> expected = {
        value<TypeParam>(2), value<TypeParam>(2),
        value<TypeParam>(4), value<TypeParam>(4), value<TypeParam>(4)};
    EXPECT_EQ(this->drain(), expected);
}

// 6. Отрицательные числа (только для int)
TEST(HeapIntTest, Heap_NegativeNumbers_OrderedCorrectly) {
    auto heap = ds::testing::makeHeap<int>();
    for (int v : {0, -5, 3, -100, 42, -1}) {
        heap->push(v);
    }

    std::vector<int> actual;
    while (!heap->empty()) {
        actual.push_back(heap->pop());
    }
    EXPECT_EQ(actual, (std::vector<int>{-100, -5, -1, 0, 3, 42}));
}

// 7. Пользовательский компаратор std::greater -> max-heap
TYPED_TEST(HeapTest, Heap_GreaterComparator_BehavesAsMaxHeap) {
    auto maxHeap = ds::testing::makeHeap<TypeParam, std::greater<TypeParam>>();
    for (int v : {3, 10, 1, 7}) {
        maxHeap->push(value<TypeParam>(v));
    }

    EXPECT_EQ(maxHeap->pop(), value<TypeParam>(10));
    EXPECT_EQ(maxHeap->pop(), value<TypeParam>(7));
    EXPECT_EQ(maxHeap->pop(), value<TypeParam>(3));
    EXPECT_EQ(maxHeap->pop(), value<TypeParam>(1));
    EXPECT_TRUE(maxHeap->empty());
}

// 8. 1 000 случайных чисел (фиксированный seed) — результат совпадает с std::sort
TYPED_TEST(HeapTest, Heap_RandomThousand_MatchesStdSort) {
    std::mt19937 rng(20261012u);
    std::uniform_int_distribution<int> dist(-100000, 100000);

    std::vector<TypeParam> expected;
    for (int i = 0; i < 1000; ++i) {
        TypeParam v = value<TypeParam>(dist(rng));
        expected.push_back(v);
        this->heap->push(v);
    }
    ASSERT_EQ(this->heap->size(), 1000u);

    std::sort(expected.begin(), expected.end());
    EXPECT_EQ(this->drain(), expected);
}
