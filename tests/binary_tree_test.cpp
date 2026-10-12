// Юнит-тесты бинарного дерева поиска ds::IBinaryTree (push / pop / search).
// Тесты работают только через интерфейс; реализация берётся из фабрики.

#include <gtest/gtest.h>

#include <initializer_list>
#include <memory>
#include <string>

#include "factory.hpp"
#include "test_values.hpp"

using ds::testing::value;

template <typename T>
class BinaryTreeTest : public ::testing::Test {
protected:
    void SetUp() override { tree = ds::testing::makeBinaryTree<T>(); }

    void pushAll(std::initializer_list<int> values) {
        for (int v : values) {
            ASSERT_TRUE(tree->push(value<T>(v))) << "push failed for " << v;
        }
    }

    // Все значения из present найдены, ни одно из absent — нет
    void expectContents(std::initializer_list<int> present, std::initializer_list<int> absent) {
        for (int v : present) {
            EXPECT_TRUE(tree->search(value<T>(v))) << "expected present: " << v;
        }
        for (int v : absent) {
            EXPECT_FALSE(tree->search(value<T>(v))) << "expected absent: " << v;
        }
        EXPECT_EQ(tree->size(), present.size());
    }

    // Эталонное дерево для тестов удаления:
    //
    //   50
    //   ├── 30
    //   │   ├── 20
    //   │   └── 40
    //   │       └── 35 (левый)
    //   └── 70
    //       ├── 60
    //       └── 80
    //
    // 20 — лист, 40 — один потомок, 30 — два потомка, 50 — корень.
    void buildSampleTree() { pushAll({50, 30, 70, 20, 40, 60, 80, 35}); }

    std::unique_ptr<ds::IBinaryTree<T>> tree;
};

using TreeTypes = ::testing::Types<int, std::string>;
TYPED_TEST_SUITE(BinaryTreeTest, TreeTypes, ds::testing::TypeNames);

// 1. Новое дерево пусто, search -> false
TYPED_TEST(BinaryTreeTest, Tree_New_IsEmptyAndSearchFalse) {
    EXPECT_TRUE(this->tree->empty());
    EXPECT_EQ(this->tree->size(), 0u);
    EXPECT_FALSE(this->tree->search(value<TypeParam>(1)));
}

// 2. push -> search true
TYPED_TEST(BinaryTreeTest, Tree_Push_ThenSearchFindsValue) {
    EXPECT_TRUE(this->tree->push(value<TypeParam>(10)));

    EXPECT_TRUE(this->tree->search(value<TypeParam>(10)));
    EXPECT_FALSE(this->tree->search(value<TypeParam>(11)));
    EXPECT_FALSE(this->tree->empty());
    EXPECT_EQ(this->tree->size(), 1u);
}

// 3. push дубликата -> false, size не меняется
TYPED_TEST(BinaryTreeTest, Tree_PushDuplicate_ReturnsFalseSizeUnchanged) {
    this->pushAll({10, 5, 15});
    ASSERT_EQ(this->tree->size(), 3u);

    EXPECT_FALSE(this->tree->push(value<TypeParam>(5)));
    EXPECT_FALSE(this->tree->push(value<TypeParam>(10)));
    EXPECT_EQ(this->tree->size(), 3u);
    EXPECT_TRUE(this->tree->search(value<TypeParam>(5)));
}

// 4. pop существующего -> true, затем search -> false
TYPED_TEST(BinaryTreeTest, Tree_PopExisting_ReturnsTrueAndValueGone) {
    this->pushAll({10, 5, 15});

    EXPECT_TRUE(this->tree->pop(value<TypeParam>(15)));
    EXPECT_FALSE(this->tree->search(value<TypeParam>(15)));
    EXPECT_EQ(this->tree->size(), 2u);
}

// 5. pop несуществующего -> false (и на пустом, и на непустом дереве)
TYPED_TEST(BinaryTreeTest, Tree_PopMissing_ReturnsFalse) {
    EXPECT_FALSE(this->tree->pop(value<TypeParam>(1)));

    this->pushAll({10, 5, 15});
    EXPECT_FALSE(this->tree->pop(value<TypeParam>(7)));
    EXPECT_EQ(this->tree->size(), 3u);
}

// 6a. Удаление листа
TYPED_TEST(BinaryTreeTest, Tree_PopLeaf_OtherNodesIntact) {
    this->buildSampleTree();

    EXPECT_TRUE(this->tree->pop(value<TypeParam>(20)));
    this->expectContents({50, 30, 70, 40, 60, 80, 35}, {20});
}

// 6b. Удаление узла с одним потомком
TYPED_TEST(BinaryTreeTest, Tree_PopNodeWithOneChild_ChildPreserved) {
    this->buildSampleTree();

    EXPECT_TRUE(this->tree->pop(value<TypeParam>(40)));
    this->expectContents({50, 30, 70, 20, 60, 80, 35}, {40});
}

// 6c. Удаление узла с двумя потомками
TYPED_TEST(BinaryTreeTest, Tree_PopNodeWithTwoChildren_SubtreesPreserved) {
    this->buildSampleTree();

    EXPECT_TRUE(this->tree->pop(value<TypeParam>(30)));
    this->expectContents({50, 70, 20, 40, 60, 80, 35}, {30});
}

// 6d. Удаление корня
TYPED_TEST(BinaryTreeTest, Tree_PopRoot_TreeStaysValid) {
    this->buildSampleTree();

    EXPECT_TRUE(this->tree->pop(value<TypeParam>(50)));
    this->expectContents({30, 70, 20, 40, 60, 80, 35}, {50});

    // Дерево остаётся рабочим: вставка и удаление после смены корня
    EXPECT_TRUE(this->tree->push(value<TypeParam>(50)));
    EXPECT_TRUE(this->tree->pop(value<TypeParam>(70)));
    this->expectContents({50, 30, 20, 40, 60, 80, 35}, {70});
}

// 7. Вырожденное дерево (вставка по возрастанию 1 000) — search работает
TYPED_TEST(BinaryTreeTest, Tree_DegenerateAscendingThousand_SearchWorks) {
    constexpr int kCount = 1000;
    for (int i = 0; i < kCount; ++i) {
        ASSERT_TRUE(this->tree->push(value<TypeParam>(i)));
    }
    ASSERT_EQ(this->tree->size(), static_cast<std::size_t>(kCount));

    for (int i = 0; i < kCount; ++i) {
        ASSERT_TRUE(this->tree->search(value<TypeParam>(i))) << "missing " << i;
    }
    EXPECT_FALSE(this->tree->search(value<TypeParam>(-1)));
    EXPECT_FALSE(this->tree->search(value<TypeParam>(kCount)));
}

// 8. После pop всех элементов дерево пусто
TYPED_TEST(BinaryTreeTest, Tree_PopAll_TreeIsEmpty) {
    this->buildSampleTree();

    for (int v : {35, 50, 20, 80, 30, 60, 40, 70}) {
        EXPECT_TRUE(this->tree->pop(value<TypeParam>(v))) << "pop failed for " << v;
    }
    EXPECT_TRUE(this->tree->empty());
    EXPECT_EQ(this->tree->size(), 0u);
    EXPECT_FALSE(this->tree->search(value<TypeParam>(50)));
}
