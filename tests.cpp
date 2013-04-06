#include <type_traits>
#include <string>
#include <utility>

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_CPP11_NULLPTR
#include <catch.hpp>

#include "ptr.hpp"

using base::ptr;
using base::raw_ptr;
using base::static_pointer_cast;
using base::dynamic_pointer_cast;
using base::const_pointer_cast;

TEST_CASE("init", "Test pointer initialisation") {
    ptr<int> p1;
    REQUIRE(p1 == nullptr);
    
    ptr<int> p2 = nullptr;
    REQUIRE(p1 == p2);

    int a = 33;
    ptr<int> p3 = raw_ptr(&a);
    REQUIRE(p3 != nullptr);
    REQUIRE(*p3 == a);
    
    assert(p1 == p2);
    assert(p1 == nullptr);

    int b = 33;
    ptr<int> p4 = raw_ptr(&b);
    REQUIRE(p3 != p4);
}

TEST_CASE("deref", "dereference a pointer") {
    int x = 101;

    ptr<int> px = raw_ptr(&x);

    REQUIRE(*px == 101);

    *px = 12;

    REQUIRE(x == 12);
}

TEST_CASE("swap", "Swap pointers") {
    int a = 23;
    int b = 42;

    ptr<int> pa = raw_ptr(&a);
    ptr<int> pb = raw_ptr(&b);

    REQUIRE(*pa == 23);
    REQUIRE(*pb == 42);

    using std::swap;
    swap(pa, pb);

    REQUIRE(*pa == 42);
    REQUIRE(*pb == 23);
    // Original values remain untouched.
    REQUIRE(a == 23);
    REQUIRE(b == 42);
}

TEST_CASE("ordering", "Comparison operators") {
    char ab[] = { 'a', 'b' };

    ptr<char> pa = raw_ptr(&ab[0]);
    ptr<char> pb = raw_ptr(&ab[1]);

    REQUIRE(not (pa < pa));
    REQUIRE(pa <= pa);
    REQUIRE(not (pa > pa));
    REQUIRE(pa >= pa);

    REQUIRE(pa < pb);
    REQUIRE(pa <= pb);
    REQUIRE(not (pa > pb));
    REQUIRE(not (pa >= pb));

    REQUIRE(not (pb < pa));
    REQUIRE(not (pb <= pa));
    REQUIRE(pb > pa);
    REQUIRE(pb >= pa);

    ptr<char> p0 = nullptr;

    REQUIRE(not (p0 < nullptr));
    REQUIRE(p0 <= nullptr);
    REQUIRE(not (p0 > nullptr));
    REQUIRE(p0 >= nullptr);

    REQUIRE(not (nullptr < p0));
    REQUIRE(nullptr <= p0);
    REQUIRE(not (nullptr > p0));
    REQUIRE(nullptr >= p0);

    REQUIRE(not (pa < nullptr));
    REQUIRE(not (pa <= nullptr));
    REQUIRE(pa > nullptr);
    REQUIRE(pa >= nullptr);

    REQUIRE(nullptr < pa);
    REQUIRE(nullptr <= pa);
    REQUIRE(not (nullptr > pa));
    REQUIRE(not (nullptr >= pa));
}

TEST_CASE("static_cast", "Casting") {
    struct a_t { };
    struct b_t : a_t { };

    b_t b;

    ptr<b_t> pb1 = raw_ptr(&b);
    ptr<a_t> pa1 = raw_ptr(&b);
    ptr<a_t> pa2 = static_pointer_cast<a_t>(pb1);
    ptr<b_t> pb2 = static_pointer_cast<b_t>(pa2);

    REQUIRE(pa1 == pa2);
    REQUIRE(pa1 == pb1);
    REQUIRE(pa2 == pb1);
    REQUIRE(pb1 == pb2);
}

TEST_CASE("dynamic_cast", "Casting") {
    struct a_t { virtual ~a_t() { } };
    struct b_t : a_t { };
    struct c_t : a_t { };

    b_t b;

    ptr<a_t> pa = raw_ptr(&b);
    ptr<b_t> pb = dynamic_pointer_cast<b_t>(pa);
    ptr<c_t> pc = dynamic_pointer_cast<c_t>(pa);

    REQUIRE(pa == pb);
    REQUIRE(pa != pc);
    REQUIRE(pc == nullptr);
}

TEST_CASE("const_cast", "Casting") {
    int a;

    ptr<int> pa1 = raw_ptr(&a);
    ptr<int const> pca = pa1;
    ptr<int> pa2 = const_pointer_cast<int>(pca);

    REQUIRE(pa1 == pca);
    REQUIRE(pa2 == pa1);
}

TEST_CASE("auto-generated", "Test whether auto-generated members work") {
    ptr<int> a = nullptr;
    ptr<int> b;

    REQUIRE(a == b);

    ptr<int> c(a);
    ptr<int> d(std::move(d));
}

TEST_CASE("pointer_traits", "Traits") {
    using std::pointer_traits;
    using std::is_same;

    static_assert(
        is_same<
            pointer_traits<int*>::pointer,
            pointer_traits<ptr<int>>::pointer>::value, "Pointer types unequal");

    static_assert(
        is_same<
            pointer_traits<int*>::element_type,
            pointer_traits<ptr<int>>::element_type>::value, "Element types unequal");

    static_assert(
        is_same<
            pointer_traits<int*>::difference_type,
            pointer_traits<ptr<int>>::difference_type>::value, "Difference types unequal");

    static_assert(
        is_same<
            pointer_traits<float*>::rebind<double>,
            pointer_traits<ptr<float>>::rebind<double>::pointer>::value, "Rebind types unequal");
}
