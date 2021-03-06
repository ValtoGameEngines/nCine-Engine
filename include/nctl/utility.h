#ifndef NCTL_UTILITY
#define NCTL_UTILITY

#include "type_traits.h"

namespace nctl {

/// It is used to indicate that an object may be "moved from"
template <class T>
inline typename removeReference<T>::type &&move(T &&arg)
{
	return static_cast<typename removeReference<T>::type &&>(arg);
}

//// Forwards lvalues as either lvalues or as rvalues, depending on T
template <typename T>
inline constexpr T &&forward(typename removeReference<T>::type &arg)
{
	return static_cast<T &&>(arg);
}

template <typename T>
inline constexpr T &&forward(typename removeReference<T>::type &&arg)
{
	static_assert(!isLValueReference<T>::value, "Invalid lvalue to rvalue conversion");
	return static_cast<T &&>(arg);
}

/// Swaps the content of two objects of the same type
template <class T>
inline void swap(T &a, T &b)
{
	T temp = nctl::move(a);
	a = nctl::move(b);
	b = nctl::move(temp);
}

/// Compare two objects of the same type
template <class T>
inline bool equalTo(const T &a, const T &b)
{
	return (a == b);
}

/// Compare two objects of the same type
template <class T>
inline bool equalTo(const T *a, const T *b)
{
	return (a == b);
}

template <>
inline bool equalTo<char>(const char *a, const char *b)
{
	// `strcmp` reimplementation to avoid additional dependencies
	while (*a && (*a == *b))
	{
		a++;
		b++;
	}
	const unsigned char result = *reinterpret_cast<const unsigned char *>(a) -
	                             *reinterpret_cast<const unsigned char *>(b);

	return (result == 0);
}

}

#endif
