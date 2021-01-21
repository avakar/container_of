# container_of

Type-safe inverse of pointer-to-member operators for C++11.

## Motivation

In C++, one can create a pointer to non-static data member of a structure.
The type of such a pointer is `T X::*`, where `X` is the type of the structure
and `T` is the type of the member. For example,

```cpp
struct point_t
{
    int x;
    int y;
};

int point_t::* mp = &point_t::y;
```

The pointer-to-member object effectively behaves as a type-safe offset,
you can use the `.*` operator to move from a pointer to the structure
to the pointer to its member.

```cpp
point_t point;
int * coord = &point.*mp;
assert(coord == &point.y);
```

However, C++ doesn't provide any way to recover the pointer to the structure,
given a pointer to its member and the appropriate pointer-to-member object.
This library provides `container_of` function that does exactly that.

```cpp
point_t * pt = container_of(coord, mp);
assert(pt == &point);
```

The function has therefore the same effect as
the `container_of` macro in the Linux kernel or `CONTAINING_RECORD` in Windows,
except that the member specification is an object that can be passed around.

You can now pass pointer-to-member objects around instead of member offsets.

## Getting started

The library is header-only. To use it, clone the repo somewhere
and add the `include` directory to your include path.

If you're using CMake, use the FetchContent module.

```cmake
FetchContent_Declare(
    avakar.container_of
    GIT_REPOSITORY https://github.com/avakar/container_of.git
    GIT_TAG main
    GIT_SHALLOW 1
    )
FetchContent_MakeAvailable(avakar.container_of)

target_link_libraries(my_target PUBLIC avakar::container_of)
```

## Usage

Include `<avakar/container_of.h>`, which provides the following function.

```cpp
template <typename X, typename T>
X * avakar::container_of(T * p, T X::* mp) noexcept;
```

Both `p` and `mp` shall be non-null pointers.
The object pointed to by `p` shall be a member of an object of type `X`
corresponding to the member referred to by `mp`. `mp` shall not
refer to a member of a virtual base of `X` (such pointers cannot be
formed in standard C++, but Microsoft Visual Studio compiler
allows it as an extension). Otherwise, the behavior is undefined.

