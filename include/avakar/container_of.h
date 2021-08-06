#include "../../src/bitcast.h"

#include <cassert>
#include <cstdint>
#include <type_traits>

namespace avakar {
namespace _container_of {

#ifdef _MSC_VER

// MSVC allows pointer-to-member objects to refer to members of virtual bases.
// Additionally, the size of `T X::*` depends on whether `X` has virtual bases.
//
// When `X` has no virtual bases, `sizeof(T X::*) == 4` and it is a signed
// integer representing the offset of the member from the beginning of the
// structure. The value -1 is used to represent null pointer.
//
// Otherwise, `sizeof(T X::*) == 8`, and it consists of two signed integers,
// * the offset from the beginning of the virtual base table containing
//   the signed offset from the beginning of `X` to the beginning of the
//   base object `B` containing the member, and
// * the offset of the member in `B`.
//
// The virtual base table is an array of 32-bit signed integers, one for each
// virtual base. The first entry refers to the the class itself
// and is therefore always 0.
//
// It is not possible to walk the virtual base hierarchy upwards, therefore
// the `container_of` operation is only defined when the vbtable offset is 0.
	
struct pointer_to_data_member
{
	std::int32_t offset;
};

struct wide_pointer_to_data_member
{
	std::int32_t offset;
	std::int32_t offset_in_vbtable;
};

template <typename C, typename T>
auto container_of(T * p, T C::* mp) noexcept
	-> typename std::enable_if<sizeof(T C::*) == sizeof(pointer_to_data_member), C *>::type
{
	auto const & pdm = bit_cast<pointer_to_data_member>(mp);
	assert(pdm.offset >= 0);
	return (C *)((char *)p - pdm.offset);
}

template <typename C, typename T>
auto container_of(T * p, T C::* mp) noexcept
	-> typename std::enable_if<sizeof(T C::*) == sizeof(wide_pointer_to_data_member), C *>::type
{
	auto const & pdm = bit_cast<wide_pointer_to_data_member>(mp);
	assert(pdm.offset_in_vbtable == 0);
	assert(pdm.offset >= 0);
	return (C *)((char *)p - pdm.offset);
}

#else

// Non-msvc compilers won't let you form pointers to members of virtual bases,
// so there is only one form of pointer-to-member objects. They are always
// pointer-sized signed integers representing the member offset. The null pointer
// value is stored as -1.

template <typename C, typename T>
auto container_of(T * p, T C::* mp) noexcept
	-> typename std::enable_if<sizeof(T C::*) == sizeof(std::intptr_t), C *>::type
{
	auto const & offset = bit_cast<std::intptr_t>(mp);
	assert(offset >= 0);
	return (C *)((char *)p - offset);
}

#endif

template <typename C, typename T>
C const * container_of(T const * p, T C::* mp) noexcept
{
	return container_of(const_cast<T *>(p), mp);
}

}

using _container_of::container_of;

}

#pragma once
