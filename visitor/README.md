# Usage
1. Create base class `A` and child classes `B`, `C`, `D`
2. Account child classes in Accept list
3. Create functor that accepts needed types
4. .. or callable object
5. Create objects & visitors
```cpp
#include <iostream>
#include "Visitable.h"
// 1
struct B;
struct C;
struct D;

//2
using AcceptList = TypeTraits::TypeList<B, C, D>;

struct A : Visitor::BaseVisitable<AcceptList> {};

struct B  : Visitor::OverriddenVisitable<A, B, AcceptList> {
    char b = 'B';
};
struct C  : Visitor::OverriddenVisitable<A, C, AcceptList> {
    char c = 'C';
};
struct D : Visitor::OverriddenVisitable<A, D, AcceptList> {
    char d = 'D';
};

// 3
auto func = [](auto&& _p) {
    if constexpr (std::is_same_v<std::decay_t<decltype(_p)>, B>) {
        std::cout << "B " << _p.b << std::endl;
    }
    else if constexpr (std::is_same_v<std::decay_t<decltype(_p)>, D>) {
        std::cout << "D " << _p.d << std::endl;
    } 
    else {
        std::cout << "other " << std::endl;
    }
};

// 4
struct ConcreteVisitor {
    void operator()(const C& a) {
        std::cout << "C " << a.c << std::endl;
    }
    template <typename T>
    void operator()(const T& t) {
        std::cout << "other" << std::endl;
    }
};

// 5
D d;
A& ad = d;
C c;
A& ac = c;

Visitor::Generic<AcceptList, decltype(func)> gv(func);
ad.accept(gv); // prints "D D"
ac.accept(gv); // prints "other"

Visitor::Generic<AcceptList, ConcreteVisitor> cv(ConcreteVisitor{});
ad.accept(cv); // prints "other"
ac.accept(cv); // prints "C C"
```