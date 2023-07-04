// concepts1.cpp : This file contains the 'main' function. Program execution
// begins and ends there.

// https://devblogs.microsoft.com/cppblog/c20-concepts-are-here-in-visual-studio-2019-version-16-3/

#include <iostream>
#include <concepts>

// This concept tests whether 'T::type' is a valid type
template <typename T>
concept has_type_member = requires { typename T::type; };

struct S1 {};
struct S2 { using type = int; };

static_assert(!has_type_member<S1>);
static_assert(has_type_member<S2>);


// Before, MSVC did not support requires-expression everywhere; in VS2022 it does.
// This was the previous work around.
// template <class T> constexpr bool has_type_member_f(T) { return has_type_member<T>; }

// This is how you do it without a workaround.
// requires-expression work everywhere in VS2022.
template <class T> constexpr bool has_type_member_f(T) { return requires { typename T::type; }; }

static_assert(has_type_member_f(S1{}));
static_assert(!has_type_member_f(S1{}));

static_assert(has_type_member_f(S2{}));
static_assert(!has_type_member_f(S2{}));


// This concept tests whether 'T::value' is a valid expression which can be implicitly converted to bool
// 'std::convertible_to' is a concept defined in <concepts>
template <typename T>
concept has_bool_value_member = requires { { T::value } -> std::convertible_to<bool>; };

struct S3 {};
struct S4 { static constexpr bool value = true; };
struct S5 { static constexpr S3 value{}; };

static_assert(has_bool_value_member<S3>);
static_assert(has_bool_value_member<S4>);
static_assert(has_bool_value_member<S5>);

// test has_bool_value_member<> via levels of inheritance
struct S6 : private S3 {};
struct S7 : protected S3 {};
struct S8 : public S3 {};

static_assert(has_bool_value_member<S6>);
static_assert(has_bool_value_member<S7>);
static_assert(has_bool_value_member<S8>);

// test has_bool_value_member<> via levels of inheritance
struct S9 : private S4 {};
struct S10 : protected S4 {};
struct S11 : public S4 {};

static_assert(has_bool_value_member<S9>);
static_assert(has_bool_value_member<S10>);
static_assert(has_bool_value_member<S11>);

// The function is only a viable candidate if 'T::value' is a valid expression which can 
// be implicitly converted to bool
template<has_bool_value_member T>
bool get_value()
{
    return T::value;
}

// This concept tests whether 't + u' is a valid expression
template <typename T, typename U>
concept can_add = requires(T t, U u) { t + u; };

// The function is only viable candidate if 't + u' is a valid expression
template <typename T, typename U> requires can_add<T, U>
auto add(T t, U u)
{
    return t + u;
}

struct T {};
const T operator+(const T& t1, const T& t2)
{
    T t3;
    return t3;
}

std::ostream& operator<<(std::ostream& os, const T& t2)
{
    return os;
}

int main()
{
    std::cout << "Hello World!\n";
    std::cout << "add(2,3) = " << add(2, 3) << "\n";

    // type T supports operator+ so concept can_add is valid
    T t1;
    T t2;
    std::cout << "add(t1,t2) = " << add(t1, t2) << "\n";
    std::cin.get();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
