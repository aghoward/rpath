#pragma once

#include <type_traits>

template <typename T>
struct remove_cvref
{
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

template <size_t L, size_t R>
struct greater_equal
{
    static constexpr bool value = L >= R;
};

template <size_t L, size_t R>
inline constexpr bool greater_equal_v = greater_equal<L, R>::value;

template <typename T>
struct underlying_integral
{
    enum class X { };

    template <typename U, typename = std::enable_if_t<std::is_enum_v<U>>>
    static std::underlying_type_t<std::conditional_t<std::is_enum_v<U>, U, X>> matches(U*);

    template <typename U>
    static U matches(...);

    using type = decltype(matches<std::remove_reference_t<T>>(nullptr));
};

template <typename T>
using underlying_integral_t = typename underlying_integral<T>::type;

