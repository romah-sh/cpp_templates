#pragma once

#include "../TypeList.h"

namespace Visitor {
    using TypeTraits::TypeList;

    namespace Detail {
        using TypeTraits::TypeList;

        template <typename List, size_t typeIndex>
        struct TypeVisitImpl;

        template <size_t typeIndex, typename... T>
        struct TypeVisitImpl<TypeList<T...>, typeIndex> {
            using ArgType = TypeTraits::TypeAtT<TypeList<T...>, typeIndex>;
            virtual void visit(ArgType& p) = 0;
        };

        template <typename List, typename IndexSequence>
        struct VisitImpl;

        template <typename... T, size_t... indices>
        struct VisitImpl<TypeList<T...>, std::index_sequence<indices...>>
            : TypeVisitImpl<TypeList<T...>, indices>... {
            using TypeVisitImpl<TypeList<T...>, indices>::visit...;
        };

        template <typename List, size_t listSize>
        struct Base;

        template <typename... T, size_t listSize>
        struct Base<TypeList<T...>, listSize>
            : VisitImpl<TypeList<T...>, std::make_index_sequence<listSize>> {
            virtual ~Base() { }
        };

    } // namespace Detail

    template <typename... T>
    struct Abstract : Detail::Base<TypeList<T...>, TypeList<T...>::length> {
    };

    template <typename... T>
    struct Abstract<TypeList<T...>> : Abstract<T...> {
    };

    namespace Detail {

        template <typename F, typename List>
        struct GenericBase;

        template <typename F, typename... T>
        struct GenericBase<F, TypeList<T...>> : Abstract<TypeList<T...>> {
            GenericBase(F _callable)
                : callable_ { std::move(_callable) }
            {
            }
        protected:
            F callable_;
        };

        template <typename F, typename List, size_t typeIndex>
        struct Generic;
        template <typename F, size_t typeIndex, typename... T>
        struct Generic<F, TypeList<T...>, typeIndex> : Generic<F, TypeList<T...>, typeIndex - 1u> {
            Generic(F _callable)
                : Generic<F, TypeList<T...>, typeIndex - 1u>(std::move(_callable))
            {
            }
            using ArgType = TypeTraits::TypeAtT<TypeList<T...>, typeIndex>;
            using Generic<F, TypeList<T...>, typeIndex - 1u>::visit;
            void visit(ArgType& _p) override
            {
                callable_(_p);
            }
        };

        template <typename F, typename... T>
        struct Generic<F, TypeList<T...>, 0u> : GenericBase<F, TypeList<T...>> {
            Generic(F _callable)
                : GenericBase<F, TypeList<T...>>(std::move(_callable))
            {
            }
            using ArgType = typename TypeList<T...>::Head;
            void visit(ArgType& _p) override
            {
                callable_(_p);
            }
        };
    } // namespace Detail

    template <typename List, typename F>
    struct Generic;

    template <typename F, typename... T>
    struct Generic<TypeList<T...>, F> : Detail::Generic<F, TypeList<T...>, TypeList<T...>::length - 1u> {
        Generic(F _callable)
            : Detail::Generic<F, TypeList<T...>, TypeList<T...>::length - 1u>(std::move(_callable))
        {
        }
    };

} // namespace Visitor
