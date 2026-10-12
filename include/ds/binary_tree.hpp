#pragma once

#include <cstddef>

namespace ds {

/**
 * Бинарное дерево поиска (BST) без дубликатов.
 *
 * Контракт:
 *  - push(v) вставляет значение; возвращает false, если оно уже есть (дерево не меняется);
 *  - pop(v) удаляет значение; возвращает false, если его нет (дерево не меняется);
 *    удаление корректно для листа, узла с одним потомком, узла с двумя потомками и корня;
 *  - search(v) возвращает true, если значение присутствует в дереве;
 *  - empty()/size() — вспомогательные методы для проверки состояния.
 */
template <typename T>
class IBinaryTree {
public:
    virtual ~IBinaryTree() = default;

    /// Вставить значение. false — если значение уже есть.
    virtual bool push(const T& value) = 0;

    /// Удалить значение. false — если значения нет.
    virtual bool pop(const T& value) = 0;

    /// Проверить наличие значения.
    virtual bool search(const T& value) const = 0;

    /// true, если дерево пусто.
    virtual bool empty() const = 0;

    /// Количество узлов в дереве.
    virtual std::size_t size() const = 0;
};

} // namespace ds
