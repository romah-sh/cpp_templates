#pragma once

#include "../TypeList.h"
#include "Visitor.h"

namespace Visitor {
    using TypeTraits::TypeList;

    namespace Detail {
        template <typename List>
        struct BaseVisitable;

        template <typename... T>
        struct BaseVisitable<TypeList<T...>> {
            virtual ~BaseVisitable() { }
            using ArgType = Abstract<TypeList<T...>>;
            virtual void accept(ArgType& p) = 0;
        };
    } // namespace Detail

    template <typename List>
    struct BaseVisitable;

    template <typename... T>
    struct BaseVisitable<TypeList<T...>> 
        : Detail::BaseVisitable<TypeList<T...>> {
    };

    template <typename B, typename C, typename List>
    struct OverriddenVisitable;

    template <typename B, typename C, typename... T>
    struct OverriddenVisitable<B, C, TypeList<T...>> : B {
        template<typename... Args>
        OverriddenVisitable(Args&&... args) : B(std::forward<Args>(args)...) {}
        using ArgType = Abstract<TypeList<T...>>;
        void accept(ArgType& _visitor) override
        {
            _visitor.visit(static_cast<C&>(*this));
        }
    };

} // namespace Visitor
