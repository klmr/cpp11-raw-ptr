#include <type_traits>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "ptr.hpp"

using base::ptr;
using base::raw_ptr;
using base::static_pointer_cast;

namespace std {
    ostream& operator <<(ostream& out, nullptr_t) { return out << "nullptr"; }
}

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
}

TEST_CASE("deref", "dereference a pointer") {
    int x = 101;

    ptr<int> px = raw_ptr(&x);

    REQUIRE(*px == 101);

    *px = 12;

    REQUIRE(x == 12);
}

TEST_CASE("swap", "swap pointers") {
    int a = 23;
    int b = 42;

    ptr<int> pa = raw_ptr(&a);
    ptr<int> pb = raw_ptr(&b);

    REQUIRE(*pa == 23);
    REQUIRE(*pb == 42);

    swap(pa, pb);

    REQUIRE(*pa == 42);
    REQUIRE(*pb == 23);
    // Original values untouched.
    REQUIRE(a == 23);
    REQUIRE(b == 42);
}

TEST_CASE("static_cast", "Casting") {
    struct a_t { };
    struct b_t : a_t { };

    b_t b;

    ptr<b_t> pb = raw_ptr(&b);
    ptr<a_t> pa1 = raw_ptr(&b);
    ptr<a_t> pa2 = static_pointer_cast<a_t>(pb);

    REQUIRE(pa1 == pa2);
    REQUIRE(pa1 == pb);
    REQUIRE(pa2 == pb);
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
