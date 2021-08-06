#include <avakar/container_of.h>
#include <type_traits>

#include <stdexcept>
#ifndef PP_STRINGIFY
#define PP_STRINGIFY2(x) #x
#define PP_STRINGIFY(x) PP_STRINGIFY2(x)
#endif
struct assertion_failed
	: std::runtime_error
{
	using runtime_error::runtime_error;
};
#define require(cond) do { if (!(cond)) throw ::assertion_failed(__FILE__ "(" PP_STRINGIFY(__LINE__) "): assertion failed: " #cond); } while (0)

using avakar::container_of;

struct A
{
	int a1;
	int a2;
};

static void test_simple()
{
	A a;
	require(container_of(&a.a1, &A::a1) == &a);
	require(container_of(&a.a2, &A::a2) == &a);

	require((std::is_same<decltype(container_of(&a.a1, &A::a1)), A *>::value));
}

static void test_const()
{
	A const a = {};
	require(container_of(&a.a1, &A::a1) == &a);
	require(container_of(&a.a2, &A::a2) == &a);

	require((std::is_same<decltype(container_of(&a.a1, &A::a1)), A const *>::value));
}

struct B
{
	int b1;
	int b2;
};

struct C
	: A, B
{
	int c1;
	int c2;
};

static void test_inherit()
{
	C c;
	require(container_of(&c.a1, &C::a1) == &c);
	require(container_of(&c.a2, &C::a2) == &c);

	require(container_of(&c.b1, &C::b1) == &c);
	require(container_of(&c.b2, &C::b2) == &c);

	require(container_of(&c.c1, &C::c1) == &c);
	require(container_of(&c.c2, &C::c2) == &c);
}

struct D
	: virtual A, B
{
	int d1;
	int d2;
};

static void test_virtual_inherit()
{
	D d;
	require(container_of(&d.d1, &D::d1) == &d);
	require(container_of(&d.d2, &D::d2) == &d);

	require(container_of(&d.b1, &D::b1) == &d);
	require(container_of(&d.b2, &D::b2) == &d);
}

#include <iostream>
int main()
{
	try
	{
		test_simple();
		test_inherit();
		test_virtual_inherit();
		test_const();
	}
	catch (assertion_failed const & e)
	{
		std::cerr << e.what() << "\n";
		return 1;
	}

	return 0;
}
