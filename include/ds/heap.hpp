#pragma once

#include <cstddef>
#include <functional>

namespace ds {

/**
 * Двоичная куча (очередь с приоритетом).
 *
 * Контракт:
 *  - по умолчанию (Compare = std::less<T>) — min-heap: pop() возвращает минимум;
 *  - с Compare = std::greater<T> — max-heap: pop() возвращает максимум;
 *    в общем случае pop() возвращает элемент x, для которого нет y: Compare(y, x);
 *  - push(v) добавляет элемент, дубликаты допускаются;
 *  - pop() извлекает и возвращает вершину; на пустой куче бросает std::out_of_range;
 *  - empty()/size() — вспомогательные методы для проверки состояния.
 */
template <typename T, typename Compare = std::less<T>>
class IHeap {
public:
    virtual ~IHeap() = default;

    /// Добавить элемент в кучу.
    virtual void push(const T& value) = 0;

    /// Извлечь вершину кучи (минимум по Compare). Пустая куча -> std::out_of_range.
    virtual T pop() = 0;

    /// true, если в куче нет элементов.
    virtual bool empty() const = 0;

    /// Количество элементов в куче.
    virtual std::size_t size() const = 0;
};

} // namespace ds
