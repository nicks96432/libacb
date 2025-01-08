#ifndef CGSS_TYPE_TRAITS_H_
#define CGSS_TYPE_TRAITS_H_

#include <type_traits>

#include "cgss_env_ns.h"

CGSS_NS_BEGIN

template<typename T>
struct is_numeric
    : public std::
          integral_constant<bool, std::is_integral<T>::value || std::is_floating_point<T>::value> {
};

#if defined(__cplusplus) && __cplusplus >= 201500L
template<typename T>
inline constexpr bool is_numeric_v = std::is_integral<T>::value;
#endif

CGSS_NS_END

#endif
