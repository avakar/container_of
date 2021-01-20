#if __has_include(<version>)
#include <version>
#endif

#if __cpp_lib_bit_cast

#include <bit>

namespace avakar {
namespace _container_of {

using std::bit_cast;

}
}

#else

#include <cstring>

namespace avakar {
namespace _container_of {

template <typename To, typename From>
To bit_cast(From const & from) noexcept
{
	To r;
	std::memcpy(&r, &from, sizeof(r));
	return r;
}

}
}

#endif

#pragma once
