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

    #include <numeric>
#endif

namespace Mezzanine
{
namespace Functional
{

    // Pipe drains are functions that get the value out of the pipe. The must implemented a "Drain" member function
    // which returns a functor. That functor accepts the final value of the pipe and returns some answer.

    #ifndef START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY

        /// @def START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY
        /// @brief Start a default function that autocurries if given a predicate
        /// @param StructName What is the struct called, this is an internal implementation detail.

        #define START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(StructName)                                              \
            template <typename MaybePredicateType = std::false_type>                                                   \
            struct StructName                                                                                          \
            {                                                                                                          \
                MaybePredicateType MaybePredicate;
    #endif

    #ifndef END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY

        /// @def END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY
        /// @brief Terminate a default function that autocurries if given a predicate
        /// @param StructName This is an internal implementation detail and needs to match the starting macro used.
        /// @param Name What are the names of the API functions?

        #define END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(StructName,Name)                                           \
                template<typename CVIncomingContainerType>                                                             \
                [[nodiscard]] constexpr                                                                                \
                decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer) const                     \
                    { return operator()(IncomingContainer, MaybePredicate); }                                          \
            }; /* Terminate StructName */                                                                              \
                                                                                                                       \
            /* Immediately invoking entry point */                                                                     \
            template<typename CVIncomingContainerType, typename PredicateType>                                         \
            [[nodiscard]] constexpr                                                                                    \
            decltype(auto) Name(const CVIncomingContainerType& IncomingContainer, PredicateType Predicate)             \
                { return StructName<>{}(IncomingContainer, std::forward<PredicateType>(Predicate));  }                 \
                                                                                                                       \
            /* Auto-Currying Entry Point */                                                                            \
            template<typename IncomingType = std::false_type>                                                          \
            [[nodiscard]] constexpr                                                                                    \
            decltype(auto) Name(IncomingType&& PredicateOrData = std::false_type{})                                    \
            {                                                                                                          \
                if constexpr(IsRange<IncomingType>()) /* Data or Predicate? */                                         \
                    { return StructName<>{}(std::forward<IncomingType>(PredicateOrData)); } /* Data */                 \
                else                                                                                                   \
                    { return StructName<IncomingType>{std::forward<IncomingType>(PredicateOrData)}; } /* Predicate */  \
            }
    #endif

    #ifndef START_FUNCTOR_END_CONTAINER_TO_DATA

        /// @def START_FUNCTOR_END_CONTAINER_TO_DATA
        /// @brief Start a default function that doesn't autocurry because it only accepts data.
        /// @param StructName What is the struct called, this is an internal implementation detail.

        #define START_FUNCTOR_END_CONTAINER_TO_DATA(StructName)                                                        \
            struct StructName                                                                                          \
            {
    #endif

    #ifndef END_FUNCTOR_END_CONTAINER_TO_DATA

        /// @def END_FUNCTOR_END_CONTAINER_TO_DATA
        /// @brief Terminate a default function that doesn't autocurry because it only accepts data.
        /// @param StructName This is an internal implementation detail and needs to match the starting macro used.
        /// @param Name What are the names of the API functions?

        #define END_FUNCTOR_END_CONTAINER_TO_DATA(StructName,Name)                                                     \
            }; /* Terminate StructName */                                                                              \
                                                                                                                       \
            /* Immediately invoking entry point */                                                                     \
            template<typename CVIncomingContainerType>                                                                 \
            [[nodiscard]] constexpr                                                                                    \
            decltype(auto) Name(const CVIncomingContainerType& IncomingContainer)                                      \
            {                                                                                                          \
                return StructName{}(std::forward<const CVIncomingContainerType&>(IncomingContainer));                  \
            }                                                                                                          \
                                                                                                                       \
            /* Auto-Currying Entry Point */                                                                            \
            template<typename IncomingType = std::false_type>                                                          \
            [[nodiscard]] constexpr                                                                                    \
            decltype(auto) Name(IncomingType&& DataOrNot = std::false_type{})                                          \
            {                                                                                                          \
                if constexpr(IsRange<IncomingType>()) /* Data or Not? */                                               \
                    { return StructName{}(std::forward<IncomingType>(DataOrNot)); } /* Data */                         \
                else                                                                                                   \
                    { return StructName{}; } /* Not */                                                                 \
            }
    #endif

    /// @struct MaxStruct
    /// @details This is an implementation detail for @ref Max

    /// @fn Max()
    /// @details This Function accepts an optional container and an optional a predicate. This scans the container for
    /// the maximum value and returns it. This needs to compare many values in the container and does so with std::less
    /// by default, if a predicate is passed that is used instead. If a container is provided this function is
    /// immediately invoked, if a data container is not provided this function returns a function that can be called
    /// later. This can be used to create a function that has any predicate to use in determining the max.
    /// @return This returns either a function (if no data is provide) that can find the max in data passed to it or it
    /// returns the max if passed data.

    START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(MaxStruct)
        template<typename CVIncomingContainerType, typename CVPredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, CVPredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using PredicateType = std::decay_t<CVPredicateType>;
            using namespace ContainerDetect;

            static_assert(IsConstRange<IncomingContainerType>(),
                    "Mezzanine::Max only determines the Max from containers or ranges.");

            if constexpr(std::is_same_v<std::false_type, PredicateType>)
                { return *std::max_element(IncomingContainer.cbegin(), IncomingContainer.cend()); }
            else
            {
                return *std::max_element(IncomingContainer.cbegin(),
                                         IncomingContainer.cend(),
                                         std::forward<CVPredicateType>(Predicate));
            }
        }
    END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(MaxStruct,Max)

    /// @struct MinStruct
    /// @details This is an implementation detail for @ref Min

    /// @fn Min()
    /// @details This function accepts an optional container and an optional predicate. This scans the container for the
    /// minimum value and returns it. This needs to compare many value in the container and does so with std::less, if a
    /// predicate is passed that is used instead. If a container is provided this function is immediately invoked, if it
    /// is not provided this function returns a function that can be called later. This can be used to create a function
    /// that has any predicate to use in determining the min.
    /// @return This returns either a function (if no data is provide) that can find the min in data passed to it or it
    /// returns the min if passed data.

    START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(MinStruct)
        template<typename CVIncomingContainerType, typename CVPredicateType>
        [[nodiscard]] constexpr
        auto operator() (CVIncomingContainerType&& IncomingContainer, CVPredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using PredicateType = std::decay_t<CVPredicateType>;
            using namespace ContainerDetect;
            static_assert(IsConstRange<IncomingContainerType>(),
                    "Mezzanine::Min only determines the Min from containers or ranges.");

            if constexpr(std::is_same_v<std::false_type, PredicateType>)
                { return *std::min_element(IncomingContainer.cbegin(), IncomingContainer.cend()); }
            else
            {
                return *std::min_element(IncomingContainer.cbegin(),
                                         IncomingContainer.cend(),
                                         std::forward<CVPredicateType>(Predicate));
            }
        }
    END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(MinStruct,Min)


    /// @struct MinMaxStruct
    /// @details This is an implementation detail for @ref MinMax

    /// @fn MinMax() Get the Minimum and Maximum from the passed data range in a single pass.
    /// @details This function accepts an optional container and an optional predicate. This scans the container for the
    /// minimum and maximum values and returns them as a std::pair. This needs to compare many values in the container
    /// and does so with std::less, if a predicate is passed that is used instead. If a container is provided this
    /// function is immediately invoked, if it is not provided this function returns a function that can be called later
    /// which accepts a container of data. This can be used to create a function that  has any predicate to use in
    /// determining the min and max values.
    /// @return This returns either a function (if no data is provide) that can find the min and max in data passed to
    /// it or it returns the min and max immediately if passed data.

    START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(MinMaxStruct)
        template<typename CVIncomingContainerType, typename CVPredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, CVPredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using PredicateType = std::decay_t<CVPredicateType>;
            using namespace ContainerDetect;
            static_assert(IsConstRange<IncomingContainerType>(),
                    "Mezzanine::MinMax only determines the MinMax from containers or ranges.");

            if constexpr(std::is_same_v<std::false_type, PredicateType>)
            {
                auto Results = std::minmax_element(IncomingContainer.cbegin(), IncomingContainer.cend());
                return std::make_pair(*Results.first, *Results.second);
            }
            else
            {
                auto Results = std::minmax_element(IncomingContainer.cbegin(),
                                                   IncomingContainer.cend(),
                                                   std::forward<CVPredicateType>(Predicate));
                return std::make_pair(*Results.first, *Results.second);
            }
        }
    END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(MinMaxStruct,MinMax)


    /// @struct FindStruct
    /// @details This is an implementation detail for @ref Find

    /// @fn Find()
    /// @details This scans the passed container by invoking the passed predicate or comparing for equality with the
    /// passed value. Until it finds a value that causes the predicate to return true or matches the passed value. The
    /// data is optional, if not passed data this returns a function that finds a value using the predicate or value
    /// passed here, in a passed container to returned function.
    /// @return This returns an std::optional<container::value_type> that contains the value if it is found.

    START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(FindStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, PredicateType&& PredicateOrValue) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using ContainerValueType = typename IncomingContainerType::value_type;
            using PossiblePredicateType = std::decay_t<PredicateType>;
            using namespace ContainerDetect;

            static_assert(IsConstRange<IncomingContainerType>(),
                "Mezzanine::Find only Finds from containers or ranges.");
            static_assert(!std::is_same_v<std::false_type, PossiblePredicateType>,
                "Mezzanine::Find needs either a predicate or value to search for.");

            if constexpr(std::is_same_v<ContainerValueType, PossiblePredicateType>)
            {
                auto Found = std::find(IncomingContainer.cbegin(),
                                       IncomingContainer.cend(),
                                       std::forward<PredicateType>(PredicateOrValue));
                if(IncomingContainer.cend() == Found)
                    { return std::optional<ContainerValueType>{}; }
                return std::make_optional<ContainerValueType>(*Found);
            } else {
                auto Found = std::find_if(IncomingContainer.cbegin(),
                                          IncomingContainer.cend(),
                                          std::forward<PredicateType>(PredicateOrValue));
                if(IncomingContainer.cend() == Found)
                    { return std::optional<ContainerValueType>{}; }
                return std::make_optional<ContainerValueType>(*Found);
            }
        }
    END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(FindStruct,Find)


    /// @struct SizeStruct
    /// @details This is an implementation detail for @ref Size

    /// @fn Size
    /// @details This calls size() on a container or if no container is passed this returns a function that calls size
    /// on a container passed to it.
    /// @return A size of the type returned by std::size(), or a function that returns that.

    /// @fn Length
    /// @copydoc Size

    START_FUNCTOR_END_CONTAINER_TO_DATA(SizeStruct)
        template<typename CVIncomingContainerType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer) const
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
        auto operator() (const CVIncomingContainerType& IncomingContainer) const
        {
            static_assert(IsRange<CVIncomingContainerType>(), "Mezzanine::Length only counts containers or ranges.");
            if constexpr(ContainerDetect::HasSize<decltype(IncomingContainer)>())
                { return IncomingContainer.size(); }
            else
                { return std::size(IncomingContainer); }
        }
    END_FUNCTOR_END_CONTAINER_TO_DATA(LengthStruct,Length)


    /// @struct ContainsStruct
    /// @details This is an implementation detail for @ref Contains

    /// @fn Contains()
    /// @details Checks if the passed container includes a given value or a value that matches the given predicate. If a
    /// container with data is passed, this is immediately invoked, if not this returns a function. Without data this
    /// returns a function that accepts data an applies the predicate to each item or compares for equality each each.
    /// @return True if the item is in the container and false if absent, or a function that returns these values.

    /// @fn Includes
    /// @copydoc Contains()

    START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(ContainsStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, PredicateType&& PredicateOrValue) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using ContainerValueType = typename IncomingContainerType::value_type;
            using PossiblePredicateType = std::decay_t<PredicateType>;
            using namespace ContainerDetect;

            static_assert(IsConstRange<IncomingContainerType>(),
                "Mezzanine::Contains only scans containers or ranges.");
            static_assert(!std::is_same_v<std::false_type, PossiblePredicateType>,
                "Mezzanine::Contains needs either a predicate or value to search for.");

            if constexpr(std::is_same_v<ContainerValueType, PossiblePredicateType>)
            {
                return IncomingContainer.cend() !=
                    std::find(IncomingContainer.cbegin(),
                              IncomingContainer.cend(),
                              std::forward<PredicateType>(PredicateOrValue));
            } else {
                return IncomingContainer.cend() !=
                    std::find_if(IncomingContainer.cbegin(),
                                 IncomingContainer.cend(),
                                 std::forward<PredicateType>(PredicateOrValue));
            }
        }
    END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(ContainsStruct,Contains)

    START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(IncludesStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, PredicateType&& PredicateOrValue) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using ContainerValueType = typename IncomingContainerType::value_type;
            using PossiblePredicateType = std::decay_t<PredicateType>;
            using namespace ContainerDetect;

            static_assert(IsConstRange<IncomingContainerType>(),
                "Mezzanine::Includes only scans containers or ranges.");
            static_assert(!std::is_same_v<std::false_type, PossiblePredicateType>,
                "Mezzanine::Includes needs either a predicate or value to search for.");

            if constexpr(std::is_same_v<ContainerValueType, PossiblePredicateType>)
            {
                return IncomingContainer.cend() !=
                    std::find(IncomingContainer.cbegin(),
                              IncomingContainer.cend(),
                              std::forward<PredicateType>(PredicateOrValue));
            } else {
                return IncomingContainer.cend() !=
                    std::find_if(IncomingContainer.cbegin(),
                                 IncomingContainer.cend(),
                                 std::forward<PredicateType>(PredicateOrValue));
            }
        }
    END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(IncludesStruct,Includes)


    /// @struct AllOfStruct
    /// @details This is an implementation detail for @ref AllOf

    /// @fn AllOf
    /// @details Do all of the items in the passed container match a given predicate? The predicate should accept on
    /// value and return true or false. If all items in the container cause the predicate to return true then this
    /// returns true. This supports auto-currying if the data is not passed. When no data is passed this return a
    /// function that accepts data and performs the above described work with the predicate.
    /// @return True if all the items pass the predicate, false if at least one doesn't, or a function that does this
    /// if no data is passed.

    START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(AllOfStruct)
        template<typename CVIncomingContainerType, typename CVPredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, CVPredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using namespace ContainerDetect;

            static_assert(IsConstRange<IncomingContainerType>(),
                    "Mezzanine::AllOf only scans containers or ranges.");

            return std::all_of(IncomingContainer.cbegin(),
                               IncomingContainer.cend(),
                               std::forward<CVPredicateType>(Predicate));
        }
    END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(AllOfStruct,AllOf)


    /// @struct AnyOfStruct
    /// @details This is an implementation detail for @ref AnyOf

    /// @fn AnyOf
    /// @details Do any of the items in the passed container match a given predicate? The predicate should accept on
    /// value and return true or false. If any items in the container cause the predicate to return true then this
    /// returns true. This supports auto-currying if the data is not passed. When no data is passed this return a
    /// function that accepts data and performs the above described work with the predicate.
    /// @return True if any the items pass the predicate, false if none do, or a function that does this if no data is
    /// passed.

    START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(AnyOfStruct)
        template<typename CVIncomingContainerType, typename CVPredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, CVPredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using namespace ContainerDetect;

            static_assert(IsConstRange<IncomingContainerType>(),
                    "Mezzanine::AnyOf only scans containers or ranges.");

            return std::any_of(IncomingContainer.cbegin(),
                               IncomingContainer.cend(),
                               std::forward<CVPredicateType>(Predicate));
        }
    END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(AnyOfStruct,AnyOf)


    /// @struct NoneOfStruct
    /// @details This is an implementation detail for @ref NoneOf

    /// @fn NoneOf
    /// @details Do none of the items in the passed container match a given predicate? The predicate should accept on
    /// value and return true or false. If none of the items in the container cause the predicate to return true then
    /// this returns true. This supports auto-currying if the data is not passed. When no data is passed this return a
    /// function that accepts data and performs the above described work with the predicate.
    /// @return True if none of the items pass the predicate, false if any do, or a function that does this if no data
    /// is passed.

    START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(NoneOfStruct)
        template<typename CVIncomingContainerType, typename CVPredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, CVPredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using namespace ContainerDetect;

            static_assert(IsConstRange<IncomingContainerType>(),
                    "Mezzanine::NoneOf only scans containers or ranges.");

            return std::none_of(IncomingContainer.cbegin(),
                                IncomingContainer.cend(),
                                std::forward<CVPredicateType>(Predicate));
        }
    END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(NoneOfStruct,NoneOf)


    /// @struct IsSortedStruct
    /// @details This is an implementation detail for @ref IsSorted

    /// @fn IsSorted
    /// @details Is the given contained sorted according to given predicate? The predicate needs to accept two
    /// parameters and is used to check if each pair of adjacent items all return true when passed to that predicate. if
    /// data is not provided this returns a functor that performs the worked detail.
    /// @return True or false if data is provided, or a function that returns true or false

    START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(IsSortedStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, PredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using namespace ContainerDetect;

            static_assert(IsConstRange<IncomingContainerType>(),
                    "Mezzanine::IsSorted only checks containers or ranges.");

            return std::is_sorted(IncomingContainer.cbegin(),
                                  IncomingContainer.cend(),
                                  std::forward<PredicateType>(Predicate));
        }
    END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(IsSortedStruct,IsSorted)


    /// @struct IsPartitionedStruct
    /// @details This is an implementation detail for @ref IsPartitioned

    /// @fn IsPartitioned
    /// @details Accepts a container with data and a predicate that will be used to checked that data. The predicate is
    /// applied to each item in the container until it can be determined that all items resolving to true come before
    /// all items resolving to false. If data is not provided this returns a function that perform the above with the
    /// passed predicate.
    /// @return True or false, or if no data is provided this returns a function that returns true or false.

    START_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(IsPartitionedStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        auto operator() (CVIncomingContainerType&& IncomingContainer, PredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using namespace ContainerDetect;

            static_assert(IsRange<IncomingContainerType>(),
                    "Mezzanine::IsPartitioned only checks containers or ranges.");

            return std::is_partitioned(IncomingContainer.cbegin(),
                                       IncomingContainer.cend(),
                                       std::forward<PredicateType>(Predicate));
        }
    END_FUNCTOR_END_CONTAINER_TO_DATA_AUTOCURRY(IsPartitionedStruct,IsPartitioned)


    /// @struct AccumulateStruct
    /// @details This is an implementation detail for @ref Accumulate

    /// @fn Accumulate
    /// @details This performs a "Left Fold" operation to get a single value from passed data. This accepts data, a
    /// starting value, and a predicate which accepts two arguments to extract a single value from a collection of data.
    /// Starting with the passed initial value and the first item in the container the predicate is invoked. Using the
    /// result of that and the next item in the container the predicate is invoked again. This is continued until every
    /// value has been folded into a single results. The data parameter is optional, if not passed this returns a
    /// functor that performs the described operation.
    /// @return That returns a single value based on the contents of the data container or a function that will.

    /// @fn Collect
    /// @copydoc Accumulate

    /// @fn LeftFold
    /// @copydoc Accumulate

    /// @fn Reduce
    /// @copydoc Accumulate

    SAVE_WARNING_STATE
    SUPPRESS_CLANG_WARNING("-Wpadded")
    SUPPRESS_GCC_WARNING("-Wpadded")
    template <typename ReturnType = std::false_type,
              typename MaybePredicateType = std::false_type,
              typename MaybeInitialType = std::false_type>
    struct AccumulateStruct
    {
        MaybePredicateType MaybePredicate;
        MaybeInitialType MaybeInitial;

        template<typename CVIncomingContainerType, typename PredicateType, typename InitialType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer,
                         PredicateType&& Predicate,
                         InitialType&& Initial) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using namespace ContainerDetect;

            static_assert(IsRange<IncomingContainerType>(),
                    "Mezzanine::Accumulate/Collect/LeftFold/Reduce only operates on containers or ranges.");

            const auto Results = std::accumulate(IncomingContainer.cbegin(), IncomingContainer.cend(),
                                                 std::forward<InitialType>(Initial),
                                                 std::forward<PredicateType>(Predicate));

            if constexpr(std::is_same_v<ReturnType, std::false_type>)
                { return Results; }
            else
                { return static_cast<ReturnType>(Results); }
        }

        template<typename CVIncomingContainerType>
        [[nodiscard]] constexpr
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer) const
            { return operator()(IncomingContainer, MaybePredicate, MaybeInitial); }
    }; /* Terminate AccumulateStruct */
    RESTORE_WARNING_STATE

    // Immediately invoking entry point
    template<typename ReturnType = std::false_type,
             typename CVIncomingContainerType,
             typename PredicateType,
             typename InitialType>
    [[nodiscard]] constexpr
    decltype(auto) Accumulate(const CVIncomingContainerType& IncomingContainer,
                              PredicateType&& Predicate,
                              InitialType&& Initial)
    {
        return AccumulateStruct<ReturnType>{}
            (IncomingContainer,
             std::forward<PredicateType>(Predicate),
             std::forward<InitialType>(Initial));
    }

    // Auto-Currying Entry Point
    template <typename ReturnType = std::false_type,
              typename PredicateType,
              typename InitialType>
    [[nodiscard]] constexpr
    decltype(auto) Accumulate(PredicateType&& PredicateOrData,
                              InitialType&& Initial)
    {
        return AccumulateStruct<ReturnType, PredicateType, InitialType>
            {std::forward<PredicateType>(PredicateOrData), std::forward<InitialType>(Initial)};
    }

    // Immediately invoking entry point
    template<typename ReturnType = std::false_type,
             typename CVIncomingContainerType,
             typename PredicateType,
             typename InitialType>
    [[nodiscard]] constexpr
    decltype(auto) Collect(const CVIncomingContainerType& IncomingContainer,
                           PredicateType&& Predicate,
                           InitialType&& Initial)
    {
        return AccumulateStruct<ReturnType>{}
            (IncomingContainer,
             std::forward<PredicateType>(Predicate),
             std::forward<InitialType>(Initial));
    }

    // Auto-Currying Entry Point
    template <typename ReturnType = std::false_type,
              typename PredicateType,
              typename InitialType>
    [[nodiscard]] constexpr
    decltype(auto) Collect(PredicateType&& PredicateOrData,
                           InitialType&& Initial)
    {
        return AccumulateStruct<ReturnType, PredicateType, InitialType>
            {std::forward<PredicateType>(PredicateOrData), std::forward<InitialType>(Initial)};
    }

    // Immediately invoking entry point
    template<typename ReturnType = std::false_type,
             typename CVIncomingContainerType,
             typename PredicateType,
             typename InitialType>
    [[nodiscard]] constexpr
    decltype(auto) LeftFold(const CVIncomingContainerType& IncomingContainer,
                            PredicateType&& Predicate,
                            InitialType&& Initial)
    {
        return AccumulateStruct<ReturnType>{}
            (IncomingContainer,
             std::forward<PredicateType>(Predicate),
             std::forward<InitialType>(Initial));
    }

    // Auto-Currying Entry Point
    template <typename ReturnType = std::false_type,
              typename PredicateType,
              typename InitialType>
    [[nodiscard]] constexpr
    decltype(auto) LeftFold(PredicateType&& PredicateOrData,
                            InitialType&& Initial)
    {
        return AccumulateStruct<ReturnType, PredicateType, InitialType>
            {std::forward<PredicateType>(PredicateOrData), std::forward<InitialType>(Initial)};
    }

    // Immediately invoking entry point
    template<typename ReturnType = std::false_type,
             typename CVIncomingContainerType,
             typename PredicateType,
             typename InitialType>
    [[nodiscard]] constexpr
    decltype(auto) Reduce(const CVIncomingContainerType& IncomingContainer,
                          PredicateType&& Predicate,
                          InitialType&& Initial)
    {
        return AccumulateStruct<ReturnType>{}
            (IncomingContainer,
             std::forward<PredicateType>(Predicate),
             std::forward<InitialType>(Initial));
    }

    // Auto-Currying Entry Point
    template <typename ReturnType = std::false_type,
              typename PredicateType,
              typename InitialType>
    [[nodiscard]] constexpr
    decltype(auto) Reduce(PredicateType&& PredicateOrData,
                          InitialType&& Initial)
    {
        return AccumulateStruct<ReturnType, PredicateType, InitialType>
            {std::forward<PredicateType>(PredicateOrData), std::forward<InitialType>(Initial)};
    }



    /// @struct AccumulateBackStruct
    /// @details This is an implementation detail for @ref AccumulateBack

    /// @fn AccumulateBack
    /// @details This performs a "Right Fold" operation to get a single value from passed data. This accepts data, a
    /// starting value, and a predicate which accepts two arguments to extract a single value from a collection of data.
    /// Starting with the passed initial value and the lasst item in the container the predicate is invoked. Using the
    /// result of that and the next to last item in the container the predicate is invoked again. This is continued
    /// until every value has been folded into a single result. The data parameter is optional, if not passed this
    /// returns a functor that performs the described operation.
    /// @return That returns a single value based on the contents of the data container or a function that will.

    /// @fn RightFold
    /// @copydoc AccumulateBack


    SAVE_WARNING_STATE
    SUPPRESS_CLANG_WARNING("-Wpadded")
    SUPPRESS_GCC_WARNING("-Wpadded")
    template <typename ReturnType = std::false_type,
              typename MaybePredicateType = std::false_type,
              typename MaybeInitialType = std::false_type>
    struct AccumulateBackStruct
    {
        MaybePredicateType MaybePredicate;
        MaybeInitialType MaybeInitial;

        template<typename CVIncomingContainerType, typename PredicateType, typename InitialType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer,
                         PredicateType&& Predicate,
                         InitialType&& Initial) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using namespace ContainerDetect;

            static_assert(IsConstReverseRange<IncomingContainerType>(),
                    "Mezzanine::AccumulateBack/RightFold only operates on reversible containers or ranges.");

            const auto Results = std::accumulate(IncomingContainer.crbegin(), IncomingContainer.crend(),
                                                 std::forward<InitialType>(Initial),
                                                 std::forward<PredicateType>(Predicate));

            if constexpr(std::is_same_v<ReturnType, std::false_type>)
                { return Results; }
            else
                { return static_cast<ReturnType>(Results); }
        }

        template<typename CVIncomingContainerType>
        [[nodiscard]] constexpr
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer) const
            { return operator()(IncomingContainer, MaybePredicate, MaybeInitial); }
    }; /* Terminate AccumulateBackStruct */
    RESTORE_WARNING_STATE

    // Immediately invoking entry point
    template<typename ReturnType = std::false_type,
             typename CVIncomingContainerType,
             typename PredicateType,
             typename InitialType>
    [[nodiscard]] constexpr
    decltype(auto) AccumulateBack(const CVIncomingContainerType& IncomingContainer,
                              PredicateType&& Predicate,
                              InitialType&& Initial)
    {
        return AccumulateBackStruct<ReturnType>{}
            (IncomingContainer,
             std::forward<PredicateType>(Predicate),
             std::forward<InitialType>(Initial));
    }

    // Auto-Currying Entry Point
    template <typename ReturnType = std::false_type,
              typename PredicateType,
              typename InitialType>
    [[nodiscard]] constexpr
    decltype(auto) AccumulateBack(PredicateType&& PredicateOrData,
                              InitialType&& Initial)
    {
        return AccumulateBackStruct<ReturnType, PredicateType, InitialType>
            {std::forward<PredicateType>(PredicateOrData), std::forward<InitialType>(Initial)};
    }

    // Immediately invoking entry point
    template<typename ReturnType = std::false_type,
             typename CVIncomingContainerType,
             typename PredicateType,
             typename InitialType>
    [[nodiscard]] constexpr
    decltype(auto) RightFold(const CVIncomingContainerType& IncomingContainer,
                            PredicateType&& Predicate,
                            InitialType&& Initial)
    {
        return AccumulateBackStruct<ReturnType>{}
            (IncomingContainer,
             std::forward<PredicateType>(Predicate),
             std::forward<InitialType>(Initial));
    }

    // Auto-Currying Entry Point
    template <typename ReturnType = std::false_type,
              typename PredicateType,
              typename InitialType>
    [[nodiscard]] constexpr
    decltype(auto) RightFold(PredicateType&& PredicateOrData,
                            InitialType&& Initial)
    {
        return AccumulateBackStruct<ReturnType, PredicateType, InitialType>
            {std::forward<PredicateType>(PredicateOrData), std::forward<InitialType>(Initial)};
    }


} // Functional
} // Mezzanine

#endif // Include guard
