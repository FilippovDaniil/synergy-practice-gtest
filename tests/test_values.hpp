#pragma once

// Генератор тестовых значений для TYPED_TEST: одно и то же число i
// превращается в значение типа T так, что порядок сохраняется:
//   i < j  <=>  value<T>(i) < value<T>(j)
// Для std::string используется смещение и дополнение нулями до фиксированной длины.

#include <cstdio>
#include <string>
#include <type_traits>

namespace ds::testing {

template <typename T>
T value(int i);

template <>
inline int value<int>(int i) {
    return i;
}

template <>
inline std::string value<std::string>(int i) {
    char buf[32];
    std::snprintf(buf, sizeof(buf), "s%011lld", static_cast<long long>(i) + 1000000000LL);
    return buf;
}

// Читаемые имена типов в выводе TYPED_TEST: QueueTest/int, QueueTest/string
struct TypeNames {
    template <typename T>
    static std::string GetName(int) {
        if constexpr (std::is_same_v<T, int>) {
            return "int";
        } else if constexpr (std::is_same_v<T, std::string>) {
            return "string";
        } else {
            return "unknown";
        }
    }
};

} // namespace ds::testing
