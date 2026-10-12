#include <gtest/gtest.h>

#include <string>

#include "factory.hpp"

TEST(Smoke, FrameworkWorks) {
    EXPECT_EQ(2 + 2, 4);
}

// Фабрика создаёт объекты всех трёх интерфейсов (конструкторы не бросают)
TEST(Smoke, FactoryCreatesAllStructures) {
    EXPECT_NE(ds::testing::makeQueue<int>(), nullptr);
    EXPECT_NE(ds::testing::makeHeap<std::string>(), nullptr);
    EXPECT_NE(ds::testing::makeBinaryTree<int>(), nullptr);
}
