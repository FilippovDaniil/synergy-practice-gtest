#pragma once

#include <cstddef>

namespace ds {

/**
 * Очередь FIFO (first in — first out).
 *
 * Контракт:
 *  - push(v) добавляет элемент в конец очереди;
 *  - pop() извлекает и возвращает элемент из начала очереди;
 *    на пустой очереди бросает std::out_of_range;
 *  - дубликаты допускаются и сохраняются;
 *  - empty()/size() — вспомогательные методы для проверки состояния.
 */
template <typename T>
class IQueue {
public:
    virtual ~IQueue() = default;

    /// Добавить элемент в конец очереди.
    virtual void push(const T& value) = 0;

    /// Извлечь элемент из начала очереди. Пустая очередь -> std::out_of_range.
    virtual T pop() = 0;

    /// true, если в очереди нет элементов.
    virtual bool empty() const = 0;

    /// Количество элементов в очереди.
    virtual std::size_t size() const = 0;
};

} // namespace ds
