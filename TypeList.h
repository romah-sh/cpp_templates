#pragma once

#include <utility>
#include <type_traits>

/**
 * @file
 * @brief Contains functions that defines TypeList
 */

namespace TypeTraits {

/**
 * @brief The TypeList<Args...> struct
 * @param Args Types stored in list
 * @variable Head - first element of list
 * @variable Tail - List of rest elements
 * @variable length - length of list
 * @details List of types
 */
template <typename... Args>
struct TypeList;

template <typename H, typename... T>
struct TypeList<H, T...> {
    using Head = H;
    using Tail = TypeList<T...>;
    static constexpr size_t length = 1u + sizeof...(T);
};

template <>
struct TypeList<> {
    static constexpr size_t length = 0u;
};


/**
 * @brief The TypeChecker<Type, List> struct
 * @param Type - check if Type element stored in TypeList
 * @param List - TypeList<Args...>
 * @variable atHead return if first element in List is Type
 * @variable hasType return if Type element exists in List
 * @details Checks if List contains Type element
 */
template <typename Type, typename List>
struct TypeChecker;

template <typename Type, typename... ListElements>
struct TypeChecker<Type, TypeList<ListElements...>> {
    using List = TypeList<ListElements...>;
    static constexpr bool atHead = std::is_same<
            Type, typename List::Head
            >::value;
    static constexpr bool hasType = atHead
    || TypeChecker<Type, typename List::Tail>::hasType;
};

template <typename Type>
struct TypeChecker<Type, TypeList<>> {
    static constexpr bool hasType = false;
};


/**
 * @brief The ListIndex<Type, List> struct
 * @param Type - search of Type element index in TypeList
 * @param List - TypeList<Args...>
 * @variable value return first index of element
 * @details value enabled only if Type exists in TypeList,
 * if type not appear in list compiler error will thrown
 */
template <typename Type, typename List>
struct ListIndex;

template <typename Type, typename... ListElements>
struct ListIndex<Type, TypeList<ListElements...>> {
private:
    using List = TypeList<ListElements...>;
    using Checker = TypeChecker<Type, List>;
    static constexpr bool hasType = Checker::hasType;

    static constexpr size_t getTypeIndex() {
        if constexpr (Checker::atHead) {
            return 0u;
        }
        else {
            return 1u + ListIndex<Type, typename List::Tail>::value;
        }
    }
public:
    static constexpr
    std::enable_if_t<hasType, size_t> value = getTypeIndex();
};

/**
 * @brief ListIndexV<Type, List>
 * @details Shortcut for ListIndex<Type, List>::value
 */
template <typename Type, typename List>
inline constexpr size_t ListIndexV = ListIndex<Type, List>::value;

/**
 * @brief TupleTypeList<Tuple> struct
 * @param Tuple - std::tuple<Args...>
 * @variable Value return TypeList<Args...>
 * @details convert std::tuple<Args...> to TypeList<Args...>,
 * if type not appear in list compiler error will thrown
 */
template <typename Tuple>
struct TupleTypeList;

template <typename... Args>
struct TupleTypeList<std::tuple<Args...>> {
    using Value = TypeList<Args...>;
};

/**
 * @brief ValidIndex<List, index> struct
 * @param List - TypeList<Args...>
 * @param index - check index
 * @variable value return if List contains element with at index position
 * @details Return if List can accept index
 */
template<typename List, size_t index>
struct ValidIndex {
    constexpr static bool value = index < List::length;
};

/**
 * @brief ValidIndexV<List, index>
 * @details Shortcut for ValidIndex<List, index>::value
 */
template <typename List, size_t index>
inline constexpr bool ValidIndexV = ValidIndex<List, index>::value;

/**
 * @brief TypeAt<List, index> struct
 * @param List - TypeList<Args...>
 * @param index - check index
 * @variable Type return type at index in List
 * @details Return type stored in TypeList at index
 */
template<typename List, size_t index>
struct TypeAt;
 
template<typename... ListElements>
struct TypeAt<TypeList<ListElements...>, 0> {
    using List = TypeList<ListElements...>;
    using Type = typename List::Head;
};

template<size_t index, typename... ListElements>
struct TypeAt<TypeList<ListElements...>, index> {
    using List = TypeList<ListElements...>;
    using Type = typename TypeAt<typename List::Tail, index - 1u>::Type;
};

/**
 * @brief TypeAtT<List, index>
 * @details Shortcut for TypeAtT<List, index>::Type
 */
template<typename List, size_t index>
using TypeAtT = typename TypeAt<List, index>::Type;


/**
 * @brief SameTypeAt<Type, List, index> struct
 * @param Type - Checked type
 * @param List - TypeList<Args...>
 * @param index - check index
 * @variable value return if Type equals type stored in List at index position
 * @details Return value that true if Type equals type stored in List
 * at index position, false if not equals
 */
template <typename T, typename List, size_t index>
struct SameTypeAt {
    constexpr static bool value = std::is_same_v<T, TypeAtT<List, index>>;
};

/**
 * @brief SameTypeAtV<Type, List, index>
 * @details Shortcut for SameTypeAt<T, List, index>::value
 */
template <typename T, typename List, size_t index>
inline constexpr bool SameTypeAtV = SameTypeAt<T, List, index>::value;

template <typename List>
struct HasRepeats;
template <typename... T>
// TODO add realization
struct HasRepeats<TypeList<T...>>;


} // namespace Conversions
