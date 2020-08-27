// © Copyright 2010 - 2020 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef Mezz_Foundation_FunctionalFunctorsContainerToData_h
#define Mezz_Foundation_FunctionalFunctorsContainerToData_h

/// @file
/// @brief A collection of functors for creating a single piece of data from containers.

#ifndef SWIG
    #include "ContainerTools.h"
    #include "MezzException.h"
#endif

namespace Mezzanine
{
namespace Functional
{

    // Pipe drains are functions that get the value out of the pipe. The must implemented a "Drain" member function
    // which returns a functor. That functor accepts the final value of the pipe and returns some answer.

#ifndef START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY

    #define START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(StructName)                                                            \
        template <typename MaybePredicateType = std::false_type>                                                       \
        struct StructName                                                                                              \
        {                                                                                                              \
            MaybePredicateType MaybePredicate;
#endif

#ifndef END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY

    /// @def END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY
    /// @brief Terminate a default function that autocurries if given a predicate
    /// @param StructName
    /// @param Name

    #define END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(StructName,Name)                                                         \
            template<typename CVIncomingContainerType>                                                                 \
            [[nodiscard]] constexpr                                                                                    \
            decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer) const                         \
                { return operator()(IncomingContainer, MaybePredicate); }                                              \
        }; /* Terminate StructName */                                                                                  \
                                                                                                                       \
        /* Immediately invoking entry point */                                                                         \
        template<typename CVIncomingContainerType,  typename PredicateType>                                            \
        [[nodiscard]] constexpr                                                                                        \
        decltype(auto) Name(const CVIncomingContainerType& IncomingContainer, PredicateType Predicate)                 \
        {                                                                                                              \
            return StructName<>{}                                                                                      \
                (std::forward<const CVIncomingContainerType&>(IncomingContainer),                                      \
                 std::forward<PredicateType>(Predicate));                                                              \
        }                                                                                                              \
                                                                                                                       \
        /* Auto-Currying Entry Point */                                                                                \
        template<typename IncomingType = std::false_type>                                                              \
        [[nodiscard]] constexpr                                                                                        \
        decltype(auto) Name(IncomingType&& PredicateOrData = std::false_type{})                                        \
        {                                                                                                              \
            if constexpr(IsRange<IncomingType>()) /* Data or Predicate? */                                             \
                { return StructName<>{}(std::forward<IncomingType>(PredicateOrData)); } /* Data */                     \
            else                                                                                                       \
                { return StructName<IncomingType>{std::forward<IncomingType>(PredicateOrData)}; } /* Predicate */      \
        }
#endif

#ifndef START_FUNCTOR_END_CONTAINER_TO_DATA

    #define START_FUNCTOR_END_CONTAINER_TO_DATA(StructName)                                                            \
        struct StructName                                                                                              \
        {
#endif

#ifndef END_FUNCTOR_END_CONTAINER_TO_DATA

    /// @def END_FUNCTOR_END_CONTAINER_TO_DATA
    /// @brief Terminate a default function
    /// @param StructName
    /// @param Name

    #define END_FUNCTOR_END_CONTAINER_TO_DATA(StructName,Name)                                                         \
        }; /* Terminate StructName */                                                                                  \
                                                                                                                       \
        /* Immediately invoking entry point */                                                                         \
        template<typename CVIncomingContainerType>                                                                     \
        [[nodiscard]] constexpr                                                                                        \
        decltype(auto) Name(const CVIncomingContainerType& IncomingContainer)                                          \
        {                                                                                                              \
            return StructName{}(std::forward<const CVIncomingContainerType&>(IncomingContainer));                      \
        }                                                                                                              \
                                                                                                                       \
        /* Auto-Currying Entry Point */                                                                                \
        template<typename IncomingType = std::false_type>                                                              \
        [[nodiscard]] constexpr                                                                                        \
        decltype(auto) Name(IncomingType&& DataOrNot = std::false_type{})                                              \
        {                                                                                                              \
            if constexpr(IsRange<IncomingType>()) /* Data or Not? */                                                   \
                { return StructName{}(std::forward<IncomingType>(DataOrNot)); } /* Data */                             \
            else                                                                                                       \
                { return StructName{}; } /* Not */      \
        }
#endif

/// @struct MaxStruct
/// @details This is an implementation detail for @ref Max

/// @fn Max()
/// @details This Function accepts an optional container and an optional a predicate. This scans the container for the
/// maximum value and returns it. This needs to compare many value in the container and does so with std::less, if a
/// predicate is passed that is used instead. If container is provided this function is immediately invoked, if it is
/// not provided this function returns a function that can be called later. This can be used to create a function that
/// has any predicate to use in determining the max.
/// @return This returns either a function (if no data is provide) that can find the max in data passed to it or it
/// returns the max if passed data.

START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(MaxStruct)
    template<typename CVIncomingContainerType, typename PredicateType>
    [[nodiscard]] constexpr
    decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, const PredicateType& Predicate) const
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using namespace ContainerDetect;
        static_assert(IsRange<IncomingContainerType>(),
                "Mezzanine::Max only determines the Max from containers or ranges.");

        if constexpr(std::is_same_v<std::false_type, PredicateType>)
            { return *std::max_element(IncomingContainer.cbegin(), IncomingContainer.cend()); }
        else
            { return *std::max_element(IncomingContainer.cbegin(), IncomingContainer.cend(), Predicate); }
    }
END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(MaxStruct,Max)

/// @struct MinStruct
/// @details This is an implementation detail for @ref Min

/// @fn Min()
/// @details This Function accepts an optional container and an optional a predicate. This scans the container for the
/// minimum value and returns it. This needs to compare many value in the container and does so with std::less, if a
/// predicate is passed that is used instead. If container is provided this function is immediately invoked, if it is
/// not provided this function returns a function that can be called later. This can be used to create a function that
/// has any predicate to use in determining the min.
/// @return This returns either a function (if no data is provide) that can find the min in data passed to it or it
/// returns the min if passed data.

START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(MinStruct)
    template<typename CVIncomingContainerType, typename PredicateType>
    [[nodiscard]] constexpr
    decltype(auto) operator() (CVIncomingContainerType&& IncomingContainer, PredicateType Predicate) const
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using namespace ContainerDetect;
        static_assert(IsRange<IncomingContainerType>(),
                "Mezzanine::MinMax only determines the Min from containers or ranges.");

        if constexpr(std::is_same_v<std::false_type, PredicateType>)
            { return *std::min_element(IncomingContainer.cbegin(), IncomingContainer.cend()); }
        else
        {
            return *std::min_element(IncomingContainer.cbegin(), IncomingContainer.cend(),
                                     std::forward<PredicateType>(Predicate));
        }
    }
END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(MinStruct,Min)


/// @struct MinMaxStruct
/// @details This is an implementation detail for @ref MinMax

/// @fn MinMax()
/// @details todo
/// @return todo

START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(MinMaxStruct)
    template<typename CVIncomingContainerType, typename PredicateType>
    [[nodiscard]] constexpr
    decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, PredicateType Predicate) const
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using namespace ContainerDetect;
        static_assert(IsRange<IncomingContainerType>(),
                "Mezzanine::MinMax only determines the MinMax from containers or ranges.");

        if constexpr(std::is_same_v<std::false_type, PredicateType>)
        {
            auto Results = std::minmax_element(IncomingContainer.cbegin(), IncomingContainer.cend());
            return std::pair{*Results.first, *Results.second};
        }
        else
        {
            auto Results = std::minmax_element(IncomingContainer.cbegin(), IncomingContainer.cend(), Predicate);
            return std::pair{*Results.first, *Results.second};
        }
    }
END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(MinMaxStruct,MinMax)


/// @struct FindStruct
/// @details This is an implementation detail for @ref MinMax

/// @fn Find()
/// @details todo
/// @return todo std::optional<container::value_type>

START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(FindStruct)
    template<typename CVIncomingContainerType, typename PredicateType>
    [[nodiscard]] constexpr
    decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, PredicateType PredicateOrValue) const
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using ContainerValueType = typename IncomingContainerType::value_type;
        using PossiblePredicateType = std::decay_t<PredicateType>;
        using namespace ContainerDetect;

        static_assert(IsRange<IncomingContainerType>(),
            "Mezzanine::Find only Finds from containers or ranges.");
        static_assert(!std::is_same_v<std::false_type, PossiblePredicateType>,
            "Mezzanine::Find needs either a predicate or value to search for.");

        if constexpr(std::is_same_v<ContainerValueType, PossiblePredicateType>)
        {
            auto Found = std::find(IncomingContainer.cbegin(), IncomingContainer.cend(), PredicateOrValue);
            if(IncomingContainer.cend() == Found)
                { return std::optional<ContainerValueType>{}; }
            return std::optional<ContainerValueType>{*Found};
        } else {
            auto Found = std::find_if(IncomingContainer.cbegin(), IncomingContainer.cend(), PredicateOrValue);
            if(IncomingContainer.cend() == Found)
                { return std::optional<ContainerValueType>{}; }
            return std::optional<ContainerValueType>{*Found};
        }
    }
END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(FindStruct,Find)


/// @struct SizeStruct
/// @details This is an implementation detail for @ref Size

/// @fn Size
/// @details todo
/// @return todo

/// @fn Length
/// @copydoc Size

START_FUNCTOR_END_CONTAINER_TO_DATA(SizeStruct)
    template<typename CVIncomingContainerType>
    [[nodiscard]] constexpr
    decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer) const
    {
        static_assert(IsRange<CVIncomingContainerType>(), "Mezzanine::Size only counts containers or ranges.");
        if constexpr(ContainerDetect::HasSize<decltype(IncomingContainer)>())
            { return IncomingContainer.size(); }
        else
            { return std::size(IncomingContainer); }
    }
END_FUNCTOR_END_CONTAINER_TO_DATA(SizeStruct,Size)

START_FUNCTOR_END_CONTAINER_TO_DATA(LengthStruct)
    template<typename CVIncomingContainerType>
    [[nodiscard]] constexpr
    decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer) const
    {
        static_assert(IsRange<CVIncomingContainerType>(), "Mezzanine::Length only counts containers or ranges.");
        if constexpr(ContainerDetect::HasSize<decltype(IncomingContainer)>())
            { return IncomingContainer.size(); }
        else
            { return std::size(IncomingContainer); }
    }
END_FUNCTOR_END_CONTAINER_TO_DATA(LengthStruct,Length)


/// @struct ContainsStruct
/// @details This is an implementation detail for @ref MinMax

/// @fn Contains()
/// @details todo
/// @return todo

/// @fn Includes
/// @copydoc Contains

START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(ContainsStruct)
    template<typename CVIncomingContainerType, typename PredicateType>
    [[nodiscard]] constexpr
    decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, PredicateType PredicateOrValue) const
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using ContainerValueType = typename IncomingContainerType::value_type;
        using PossiblePredicateType = std::decay_t<PredicateType>;
        using namespace ContainerDetect;

        static_assert(IsRange<IncomingContainerType>(),
            "Mezzanine::Contains only scans containers or ranges.");
        static_assert(!std::is_same_v<std::false_type, PossiblePredicateType>,
            "Mezzanine::Contains needs either a predicate or value to search for.");

        if constexpr(std::is_same_v<ContainerValueType, PossiblePredicateType>)
        {
            return IncomingContainer.cend() !=
                std::find(IncomingContainer.cbegin(), IncomingContainer.cend(), PredicateOrValue);
        } else {
            return IncomingContainer.cend() !=
                std::find_if(IncomingContainer.cbegin(), IncomingContainer.cend(), PredicateOrValue);
        }
    }
END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(ContainsStruct,Contains)

START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(IncludesStruct)
    template<typename CVIncomingContainerType, typename PredicateType>
    [[nodiscard]] constexpr
    decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, PredicateType PredicateOrValue) const
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using ContainerValueType = typename IncomingContainerType::value_type;
        using PossiblePredicateType = std::decay_t<PredicateType>;
        using namespace ContainerDetect;

        static_assert(IsRange<IncomingContainerType>(),
            "Mezzanine::Contains only scans containers or ranges.");
        static_assert(!std::is_same_v<std::false_type, PossiblePredicateType>,
            "Mezzanine::Contains needs either a predicate or value to search for.");

        if constexpr(std::is_same_v<ContainerValueType, PossiblePredicateType>)
        {
            return IncomingContainer.cend() !=
                std::find(IncomingContainer.cbegin(), IncomingContainer.cend(), PredicateOrValue);
        } else {
            return IncomingContainer.cend() !=
                std::find_if(IncomingContainer.cbegin(), IncomingContainer.cend(), PredicateOrValue);
        }
    }
END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(IncludesStruct,Includes)

/// @struct AllOfStruct
/// @details This is an implementation detail for @ref AllOf

/// @fn AllOf
/// @details To Do
/// @return todo

START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(AllOfStruct)
    template<typename CVIncomingContainerType, typename PredicateType>
    [[nodiscard]] constexpr
    decltype(auto) operator() (CVIncomingContainerType&& IncomingContainer, PredicateType Predicate) const
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using namespace ContainerDetect;

        static_assert(IsRange<IncomingContainerType>(),
                "Mezzanine::AllOf only scans containers or ranges.");

        return std::all_of(IncomingContainer.cbegin(), IncomingContainer.cend(),
                           std::forward<PredicateType>(Predicate));
    }
END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(AllOfStruct,AllOf)

/// @struct AnyOfStruct
/// @details This is an implementation detail for @ref AnyOf

/// @fn AnyOf
/// @details To Do
/// @return todo

START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(AnyOfStruct)
    template<typename CVIncomingContainerType, typename PredicateType>
    [[nodiscard]] constexpr
    decltype(auto) operator() (CVIncomingContainerType&& IncomingContainer, PredicateType Predicate) const
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using namespace ContainerDetect;

        static_assert(IsRange<IncomingContainerType>(),
                "Mezzanine::AnyOf only scans containers or ranges.");

        return std::any_of(IncomingContainer.cbegin(), IncomingContainer.cend(),
                           std::forward<PredicateType>(Predicate));
    }
END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(AnyOfStruct,AnyOf)

/// @struct NoneOfStruct
/// @details This is an implementation detail for @ref NoneOf

/// @fn NoneOf
/// @details To Do
/// @return todo

START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(NoneOfStruct)
    template<typename CVIncomingContainerType, typename PredicateType>
    [[nodiscard]] constexpr
    decltype(auto) operator() (CVIncomingContainerType&& IncomingContainer, PredicateType Predicate) const
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using namespace ContainerDetect;

        static_assert(IsRange<IncomingContainerType>(),
                "Mezzanine::NoneOf only scans containers or ranges.");

        return std::none_of(IncomingContainer.cbegin(), IncomingContainer.cend(),
                           std::forward<PredicateType>(Predicate));
    }
END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(NoneOfStruct,NoneOf)


/// @struct IsSortedStruct
/// @details This is an implementation detail for @ref IsSorted

/// @fn IsSorted
/// @details To Do
/// @return todo

START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(IsSortedStruct)
    template<typename CVIncomingContainerType, typename PredicateType>
    [[nodiscard]] constexpr
    decltype(auto) operator() (CVIncomingContainerType&& IncomingContainer, PredicateType Predicate) const
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using namespace ContainerDetect;

        static_assert(IsRange<IncomingContainerType>(),
                "Mezzanine::IsSorted only checks containers or ranges.");

        return std::is_sorted(IncomingContainer.cbegin(), IncomingContainer.cend(),
                           std::forward<PredicateType>(Predicate));
    }
END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(IsSortedStruct,IsSorted)


/// @struct IsPartitionedStruct
/// @details This is an implementation detail for @ref IsPartitioned

/// @fn IsPartitioned
/// @details To Do
/// @return todo

START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(IsPartitionedStruct)
    template<typename CVIncomingContainerType, typename PredicateType>
    [[nodiscard]] constexpr
    decltype(auto) operator() (CVIncomingContainerType&& IncomingContainer, PredicateType Predicate) const
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using namespace ContainerDetect;

        static_assert(IsRange<IncomingContainerType>(),
                "Mezzanine::IsPartitioned only checks containers or ranges.");

        return std::is_partitioned(IncomingContainer.cbegin(), IncomingContainer.cend(),
                           std::forward<PredicateType>(Predicate));
    }
END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(IsPartitionedStruct,IsPartitioned)


} // Functional
} // Mezzanine

#endif // Include guard
