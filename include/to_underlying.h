#pragma once
#include <type_traits>

template <typename Enum>
constexpr typename std::enable_if< std::is_enum<Enum>::value, typename std::underlying_type<Enum>::type >::type
to_underlying(const Enum value) noexcept {
	return static_cast< typename std::underlying_type<Enum>::type >(value);
}
