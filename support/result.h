#pragma once

#include "typetraits.h"

#include <optional>
#include <type_traits>
#include <variant>

template <typename TSuccess, typename TFailure>
class Result
{
    using TValue = std::variant<TSuccess, TFailure>;
    TValue m_value;

    public:
        template <typename T, typename = std::enable_if_t<std::is_constructible_v<decltype(m_value), T>, void>>
        Result(T&& value)
            : m_value(std::move(value)) {}

        template <typename TSuccessAction, typename TFailureAction>
        auto match(TSuccessAction&& successAction, TFailureAction&& failureAction)
            -> decltype(auto)
        {
            return std::visit(
                [&successAction, &failureAction] (auto&& value)
                {
                    if constexpr (std::is_same_v<remove_cvref_t<TSuccess>, remove_cvref_t<decltype(value)>>)
                        return successAction(std::forward<decltype(value)>(value));
                    else
                        return failureAction(std::forward<decltype(value)>(value));
                },
                std::move(m_value));
        }
};

template <typename TFailure>
class Result<void, TFailure>
{
    using TValue = std::optional<TFailure>;
    TValue m_value;

    public:
        template <typename T, typename = std::enable_if_t<std::is_constructible_v<TValue, T>>>
        Result(T&& value)
            : m_value(std::move(value)) {}

        template <typename TSuccessAction, typename TFailureAction>
        auto match(TSuccessAction&& successAction, TFailureAction&& failureAction)
            -> decltype(auto)
        {
            static_assert(std::is_same_v<std::invoke_result_t<TSuccessAction>, std::invoke_result_t<TFailureAction, TFailure>>);
            if (m_value.has_value())
                return failureAction(std::forward<TFailure>(m_value.value()));
            return successAction();
        }
};
