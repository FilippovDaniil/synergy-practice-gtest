// Юнит-тесты очереди ds::IQueue (push / pop).
// Тесты работают только через интерфейс; реализация берётся из фабрики.

#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>

#include "factory.hpp"
#include "test_values.hpp"

using ds::testing::value;

template <typename T>
class QueueTest : public ::testing::Test {
protected:
    void SetUp() override { queue = ds::testing::makeQueue<T>(); }

    std::unique_ptr<ds::IQueue<T>> queue;
};

using QueueTypes = ::testing::Types<int, std::string>;
TYPED_TEST_SUITE(QueueTest, QueueTypes, ds::testing::TypeNames);

// 1. Новая очередь пуста, size == 0
TYPED_TEST(QueueTest, Queue_New_IsEmptyWithZeroSize) {
    EXPECT_TRUE(this->queue->empty());
    EXPECT_EQ(this->queue->size(), 0u);
}

// 2. push одного элемента -> size 1, очередь не пуста
TYPED_TEST(QueueTest, Queue_PushOne_SizeIsOneAndNotEmpty) {
    this->queue->push(value<TypeParam>(42));

    EXPECT_FALSE(this->queue->empty());
    EXPECT_EQ(this->queue->size(), 1u);
}

// 3. pop возвращает элементы в порядке FIFO
TYPED_TEST(QueueTest, Queue_PopSeveral_ReturnsFifoOrder) {
    this->queue->push(value<TypeParam>(3));
    this->queue->push(value<TypeParam>(1));
    this->queue->push(value<TypeParam>(2));

    EXPECT_EQ(this->queue->pop(), value<TypeParam>(3));
    EXPECT_EQ(this->queue->pop(), value<TypeParam>(1));
    EXPECT_EQ(this->queue->pop(), value<TypeParam>(2));
    EXPECT_TRUE(this->queue->empty());
}

// 4. pop из пустой очереди -> std::out_of_range
TYPED_TEST(QueueTest, Queue_PopEmpty_ThrowsOutOfRange) {
    EXPECT_THROW(this->queue->pop(), std::out_of_range);

    this->queue->push(value<TypeParam>(1));
    this->queue->pop();
    EXPECT_THROW(this->queue->pop(), std::out_of_range);
}

// 5. push после полного опустошения работает
TYPED_TEST(QueueTest, Queue_PushAfterDrain_Works) {
    this->queue->push(value<TypeParam>(1));
    this->queue->push(value<TypeParam>(2));
    this->queue->pop();
    this->queue->pop();
    ASSERT_TRUE(this->queue->empty());

    this->queue->push(value<TypeParam>(7));

    EXPECT_EQ(this->queue->size(), 1u);
    EXPECT_EQ(this->queue->pop(), value<TypeParam>(7));
    EXPECT_TRUE(this->queue->empty());
}

// 6. Чередование push/pop сохраняет порядок FIFO
TYPED_TEST(QueueTest, Queue_InterleavedPushPop_KeepsOrder) {
    this->queue->push(value<TypeParam>(1));
    this->queue->push(value<TypeParam>(2));
    EXPECT_EQ(this->queue->pop(), value<TypeParam>(1));

    this->queue->push(value<TypeParam>(3));
    EXPECT_EQ(this->queue->pop(), value<TypeParam>(2));

    this->queue->push(value<TypeParam>(4));
    this->queue->push(value<TypeParam>(5));
    EXPECT_EQ(this->queue->pop(), value<TypeParam>(3));
    EXPECT_EQ(this->queue->pop(), value<TypeParam>(4));
    EXPECT_EQ(this->queue->size(), 1u);
    EXPECT_EQ(this->queue->pop(), value<TypeParam>(5));
    EXPECT_TRUE(this->queue->empty());
}

// 7. Большой объём (10 000 элементов) — порядок и size
TYPED_TEST(QueueTest, Queue_LargeVolume_KeepsOrderAndSize) {
    constexpr int kCount = 10000;
    for (int i = 0; i < kCount; ++i) {
        this->queue->push(value<TypeParam>(i));
    }
    ASSERT_EQ(this->queue->size(), static_cast<std::size_t>(kCount));

    for (int i = 0; i < kCount; ++i) {
        ASSERT_EQ(this->queue->pop(), value<TypeParam>(i)) << "at index " << i;
    }
    EXPECT_TRUE(this->queue->empty());
    EXPECT_EQ(this->queue->size(), 0u);
}

// 8. Дубликаты сохраняются (не схлопываются)
TYPED_TEST(QueueTest, Queue_Duplicates_ArePreserved) {
    this->queue->push(value<TypeParam>(5));
    this->queue->push(value<TypeParam>(5));
    this->queue->push(value<TypeParam>(5));

    EXPECT_EQ(this->queue->size(), 3u);
    EXPECT_EQ(this->queue->pop(), value<TypeParam>(5));
    EXPECT_EQ(this->queue->pop(), value<TypeParam>(5));
    EXPECT_EQ(this->queue->pop(), value<TypeParam>(5));
    EXPECT_TRUE(this->queue->empty());
}
