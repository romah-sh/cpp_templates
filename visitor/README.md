# Visitor pattern
## Used classes
1. List to store visitable types
```
TypeTraits::TypeList<VisitableTypes...>
```
2. Base visitable class
```
using VisitableList = TypeTraits::TypeList<VisitableTypes...>
Visitor::BaseVisitable<VisitableList> {};
```
Inherit base visitable class from `Visitor::BaseVisitable`

3. Parent class for all inherited visitable classes
```
Visitor::OverriddenVisitable<BaseVisitable, CurrentVisitable, VisitableList>
```
where
* `BaseVisitable` - base class for all visitable classes
* `CurrentVisitable` - current visitable class
* `VisitableList` - list of all visitable child classes

Inherit all visitable classes from `Visitor::OverriddenVisitable`

4. Visitor class
```
Visitor::Generic<VisitableList, ConcreteVisitor>
```
where
* `VisitableList` - list of all visitable child classes
* `ConcreteVisitor` - callable object, which accepts all types from `VisitableList`

If `ConcreteVisitor` accepts not all visitable classes, it must contain template function, which accept all remained types
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
struct ChildA;
struct ChildB;
struct ChildC;

//2
using AcceptList = TypeTraits::TypeList<ChildA, ChildB, ChildC>;

struct BaseParent : Visitor::BaseVisitable<AcceptList> {};

struct ChildA : Visitor::OverriddenVisitable<BaseParent, ChildA, AcceptList> {
    char a = 'A';
};
struct ChildB : Visitor::OverriddenVisitable<BaseParent, ChildB, AcceptList> {
    char b = 'B';
};
struct ChildC : Visitor::OverriddenVisitable<BaseParent, ChildC, AcceptList> {
    char c = 'C';
};

// 3
auto func = [](auto&& _p) {
    if constexpr (std::is_same_v<std::decay_t<decltype(_p)>, ChildA>) {
        std::cout << "ChildA " << _p.a << std::endl;
    }
    else if constexpr (std::is_same_v<std::decay_t<decltype(_p)>, ChildC>) {
        std::cout << "ChildC " << _p.c << std::endl;
    } 
    else {
        // ChildB
        std::cout << "other" << std::endl;
    }
};

// 4
struct ConcreteVisitor {
    void operator()(const ChildB& a) {
        std::cout << "B " << a.b << std::endl;
    }
    template <typename T>
    void operator()(const T& t) {
        // ChildA, ChildC
        std::cout << "other" << std::endl;
    }
};

// 5
ChildC c;
BaseParent& bpc = c;
ChildB b;
BaseParent& bpb = b;

Visitor::Generic<AcceptList, decltype(func)> gv(func);
bpc.accept(gv); // prints "ChildC C"
bpb.accept(gv); // prints "other"

Visitor::Generic<AcceptList, ConcreteVisitor> cv(ConcreteVisitor{});
bpc.accept(cv); // prints "other"
bpb.accept(cv); // prints "ChildB B"
```