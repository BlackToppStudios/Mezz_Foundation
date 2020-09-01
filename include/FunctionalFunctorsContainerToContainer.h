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
#ifndef Mezz_Foundation_FunctionalFunctorsContainerToContainer_h
#define Mezz_Foundation_FunctionalFunctorsContainerToContainer_h

/// @file
/// @brief A collection of functors for creating new containers from other containers.

#ifndef SWIG
    #include "DataTypes.h"
    #include "ContainerTools.h"
    #include "MezzException.h"

    #include <iterator>
#endif


namespace Mezzanine
{
namespace Functional
{


#ifndef START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY

    /// @def START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY
    /// @brief Start a default container to container functor that autocurries if given a predicate.
    /// @param StructName What is the struct called, this is an internal implementation detail.

    #define START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(StructName)                                             \
        template <typename CVReturnContainerType = std::false_type, typename MaybePredicateType = std::false_type>     \
        struct StructName                                                                                              \
        {                                                                                                              \
            MaybePredicateType MaybePredicate;
#endif


#ifndef END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY

    /// @def END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY
    /// @brief Terminate a default functor that accepts and returns a container and autocurries if given a predicate.
    /// @param StructName This must match the above StructName, it is an implementation detail.
    /// @param Name These are the names of the API functions.

#define END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(StructName,Name)                                          \
            template<typename CVIncomingContainerType>                                                                 \
            [[nodiscard]] constexpr                                                                                    \
            decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer) const                         \
                {  return operator()(IncomingContainer, MaybePredicate); }                                             \
        };                                                                                                             \
                                                                                                                       \
        /* Immediately invoking entry point */                                                                         \
        template<typename CVReturnContainerType = std::false_type,                                                     \
                 typename CVIncomingContainerType,                                                                     \
                 typename PredicateType>                                                                               \
        [[nodiscard]] constexpr                                                                                        \
        decltype(auto) Name(const CVIncomingContainerType& IncomingContainer, PredicateType&& Predicate)               \
        {                                                                                                              \
            return StructName<CVReturnContainerType,std::false_type>()                                                 \
                (IncomingContainer, std::forward<PredicateType>(Predicate));                                           \
        }                                                                                                              \
                                                                                                                       \
        /* Auto-Currying Entry Point */                                                                                \
        template<typename CVReturnContainerType = std::false_type, typename PredicateType>                             \
        [[nodiscard]] constexpr                                                                                        \
        decltype(auto) Name(PredicateType&& Predicate)                                                                 \
            { return StructName<CVReturnContainerType,PredicateType>{std::forward<PredicateType>(Predicate)}; }
#endif

    // The select make a new method by checking if the predicate is true when called for each item in the incoming data.
    // if it is true, it is included, if not it is excluded.

    /// @struct SelectStruct
    /// @details This is an implementation detail for @ref Select

    /// @fn Select
    /// @details This accepts a container of data and a predicate. The predicate accepts one value from the container
    /// and returns true or false. This is applied to every item in the container and each item that results in true is
    /// copied into the resulting container. If data is supplied this returns a function which accepts data and uses the
    /// previously passed predicate.
    /// @return This returns a container that is a subset of the previous container.

    /// @struct FilterStruct
    /// @details This is an implementation detail for @ref Filter

    /// @fn Filter
    /// @copydoc Select

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(SelectStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, PredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using ActualCVReturnType = std::conditional_t<
                std::is_same_v<CVReturnContainerType, std::false_type>, IncomingContainerType, CVReturnContainerType
            >;
            using ReturnContainerType = std::decay_t<ActualCVReturnType>;
            using namespace ContainerDetect;

            static_assert(IsRange<IncomingContainerType>(),
                    "Mezzanine::Select only selects from containers or ranges.");
            static_assert(IsContainer<ReturnContainerType>(),
                    "Mezzanine::Select only selects into containers.");
            static_assert(HasPushBackValue<ReturnContainerType>() ||
                          HasInsertValue<ReturnContainerType>() ||
                          HasAddValue<ReturnContainerType>(),
                    "Mezzanine::Select only selects into containers with push_back(value_type), add(value_type) or "
                    "insert(value_type).");
            static_assert(std::is_default_constructible<ReturnContainerType>(),
                    "Mezzanine::Select only selects into containers that can be default constructed.");

            ReturnContainerType Results;

            if constexpr(HasReserve<ReturnContainerType>())
                { Results.reserve(IncomingContainer.size()); }

            for(const auto& item : IncomingContainer)
            {
                if(std::invoke(std::forward<PredicateType>(Predicate),item))
                {
                    if constexpr(HasPushBackValue<ReturnContainerType>())
                        { Results.push_back(item); }
                    else if constexpr(HasInsertValue<ReturnContainerType>())
                        { Results.insert(item); }
                    else if constexpr(HasAddValue<ReturnContainerType>())
                        { Results.add(item); }
                }
            }

            return Results;
        }
    END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(SelectStruct, Select)

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(FilterStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, PredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using ActualCVReturnType = std::conditional_t<
                std::is_same_v<CVReturnContainerType, std::false_type>, IncomingContainerType, CVReturnContainerType
            >;
            using ReturnContainerType = std::decay_t<ActualCVReturnType>;
            using namespace ContainerDetect;

            static_assert(IsRange<IncomingContainerType>(),
                    "Mezzanine::Filter only filters from containers or ranges.");
            static_assert(IsContainer<ReturnContainerType>(),
                    "Mezzanine::Filter only filters into containers.");
            static_assert(HasPushBackValue<ReturnContainerType>() ||
                          HasInsertValue<ReturnContainerType>() ||
                          HasAddValue<ReturnContainerType>(),
                    "Mezzanine::Filter only filters into containers with push_back(value_type), add(value_type) or "
                    "insert(value_type).");
            static_assert(std::is_default_constructible<ReturnContainerType>(),
                    "Mezzanine::Filter only filters into containers that can be default constructed.");

            ReturnContainerType Results;

            if constexpr(HasReserve<ReturnContainerType>())
                { Results.reserve(IncomingContainer.size()); }

            for(const auto& item : IncomingContainer)
            {
                if(std::invoke(std::forward<PredicateType>(Predicate),item))
                {
                    if constexpr(HasPushBackValue<ReturnContainerType>())
                        { Results.push_back(item); }
                    else if constexpr(HasInsertValue<ReturnContainerType>())
                        { Results.insert(item); }
                    else if constexpr(HasAddValue<ReturnContainerType>())
                        { Results.add(item); }
                }
            }

            return Results;
        }
    END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(FilterStruct, Filter)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Reject is the inverse of select. It makes a new collection by applying a function to each item in the incoming
    // data and keeping it if false (copying into new collection).

    /// @struct RejectStruct
    /// @details This is an implementation detail for @ref Reject

    /// @fn Reject
    /// @details This accepts a container of data and a predicate. The predicate accepts one value from the container
    /// and returns true or false. This is applied to every item in the container and each item that results in false is
    /// copied into the resulting container. If data is not supplied this returns a function which accepts data and uses
    /// the previously passed predicate.
    /// @return This returns a container that is a subset of the previous container.

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(RejectStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, PredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using ActualCVReturnType = std::conditional_t<
                std::is_same_v<CVReturnContainerType, std::false_type>, IncomingContainerType, CVReturnContainerType
            >;
            using ReturnContainerType = std::decay_t<ActualCVReturnType>;
            using namespace ContainerDetect;

            static_assert(IsRange<IncomingContainerType>(),
                    "Mezzanine::Reject only rejects from containers or ranges.");
            static_assert(IsContainer<ReturnContainerType>(),
                    "Mezzanine::Reject only rejects into containers.");
            static_assert(HasPushBackValue<ReturnContainerType>() ||
                          HasInsertValue<ReturnContainerType>() ||
                          HasAddValue<ReturnContainerType>(),
                    "Mezzanine::Reject only rejects into containers with push_back(value_type), add(value_type) or "
                    "insert(value_type).");
            static_assert(std::is_default_constructible<ReturnContainerType>(),
                    "Mezzanine::Reject only rejects into containers that can be default constructed.");

            ReturnContainerType Results;

            if constexpr( HasReserve<ReturnContainerType>())
                { Results.reserve(IncomingContainer.size()); }

            for(const auto& item : IncomingContainer)
            {
                if(!std::invoke(std::forward<PredicateType>(Predicate),item))
                {
                    if constexpr(HasPushBackValue<ReturnContainerType>())
                        { Results.push_back(item); }
                    else if constexpr(HasInsertValue<ReturnContainerType>())
                        { Results.insert(item); }
                    else if constexpr(HasAddValue<ReturnContainerType>())
                        { Results.add(item); }
                }
            }

            return Results;
        }
    END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(RejectStruct, Reject)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // The "map" operation, this creates a new collection by capturing the values returned when apply a function to the
    // incoming data set.

    /// @struct ConvertStruct
    /// @details This is an implementation detail for @ref Convert

    /// @fn Convert
    /// @details This accepts a container of data and a predicate. The predicate accepts one value from the container
    /// and returns any value. The values that result from applying the predicate to every value in the container are
    /// used to produce a new container. If data is not supplied this returns a function which accepts data and uses
    /// the previously passed predicate.
    /// @return A fresh container construct by applying a predicate or a functor that can convert data in this way.

    /// @struct MapStruct
    /// @details This is an implementation detail for @ref Map

    /// @fn Map
    /// @copydoc Convert

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(ConvertStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, PredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using ActualCVReturnType = std::conditional_t<
                std::is_same_v<CVReturnContainerType, std::false_type>, IncomingContainerType, CVReturnContainerType
            >;
            using ReturnContainerType = std::decay_t<ActualCVReturnType>;
            using namespace ContainerDetect;

            static_assert(IsRange<IncomingContainerType>(),
                    "Mezzanine::Convert only converts from containers or ranges.");
            static_assert(IsContainer<ReturnContainerType>(),
                    "Mezzanine::Convert only converts into containers.");
            static_assert(HasPushBackValue<ReturnContainerType>() ||
                          HasInsertValue<ReturnContainerType>() ||
                          HasAddValue<ReturnContainerType>(),
                    "Mezzanine::Convert only converts into containers with push_back(value_type), add(value_type) or "
                    "insert(value_type).");
            static_assert(std::is_default_constructible<ReturnContainerType>(),
                    "Mezzanine::Convert only converts into containers that can be default constructed.");

            ReturnContainerType Results;

            if constexpr( HasReserve<ReturnContainerType>())
                { Results.reserve(IncomingContainer.size()); }

            for(const auto& item : IncomingContainer)
            {
                if constexpr(HasPushBackValue<ReturnContainerType>())
                    { Results.push_back(std::invoke(std::forward<PredicateType>(Predicate),item)); }
                else if constexpr(HasInsertValue<ReturnContainerType>())
                    { Results.insert(std::invoke(std::forward<PredicateType>(Predicate),item)); }
                else if constexpr(HasAddValue<ReturnContainerType>())
                    { Results.add(std::invoke(std::forward<PredicateType>(Predicate),item)); }
            }

            return Results;
        }
    END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(ConvertStruct, Convert)

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(MapStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, PredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using ActualCVReturnType = std::conditional_t<
                std::is_same_v<CVReturnContainerType, std::false_type>, IncomingContainerType, CVReturnContainerType
            >;
            using ReturnContainerType = std::decay_t<ActualCVReturnType>;
            using namespace ContainerDetect;

            static_assert(IsRange<IncomingContainerType>(),
                    "Mezzanine::Map only converts from containers or ranges.");
            static_assert(IsContainer<ReturnContainerType>(),
                    "Mezzanine::Map only converts into containers.");
            static_assert(HasPushBackValue<ReturnContainerType>() ||
                          HasInsertValue<ReturnContainerType>() ||
                          HasAddValue<ReturnContainerType>(),
                    "Mezzanine::Map only converts into containers with push_back(value_type), add(value_type) or "
                    "insert(value_type).");
            static_assert(std::is_default_constructible<ReturnContainerType>(),
                    "Mezzanine::Map only converts into containers that can be default constructed.");

            ReturnContainerType Results;

            if constexpr( HasReserve<ReturnContainerType>())
                { Results.reserve(IncomingContainer.size()); }

            for(const auto& item : IncomingContainer)
            {
                if constexpr(HasPushBackValue<ReturnContainerType>())
                    { Results.push_back(std::invoke(std::forward<PredicateType>(Predicate),item)); }
                else if constexpr(HasInsertValue<ReturnContainerType>())
                    { Results.insert(std::invoke(std::forward<PredicateType>(Predicate),item)); }
                else if constexpr(HasAddValue<ReturnContainerType>())
                    { Results.add(std::invoke(std::forward<PredicateType>(Predicate),item)); }
            }

            return Results;
        }
    END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(MapStruct, Map)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// @struct TakeNStruct
    /// @details This is an implementation detail for @ref TakeN

    /// @fn TakeN
    /// @details This contructs a smaller container by copying the first N items from the data container. If an amount
    /// is passed but not data is a functor is returned that will return copies of the passed amount of data that
    /// functor is passed.
    /// @return A subset of the data or a functor that returns a subset of the data.

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(TakeNStruct)
        template<typename CVIncomingContainerType, typename CountType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, CountType CountToTake) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using SizeType = typename IncomingContainerType::size_type;
            using ActualCVReturnType = std::conditional_t<
                std::is_same_v<CVReturnContainerType, std::false_type>, IncomingContainerType, CVReturnContainerType
            >;
            using ReturnContainerType = std::decay_t<ActualCVReturnType>;
            using namespace ContainerDetect;

            static_assert(IsRange<IncomingContainerType>(),
                    "Mezzanine::TakeN only takes from containers or ranges.");
            static_assert(IsContainer<ReturnContainerType>(),
                    "Mezzanine::TakeN only takes into containers.");
            static_assert(HasPushBackValue<ReturnContainerType>() ||
                          HasInsertValue<ReturnContainerType>() ||
                          HasAddValue<ReturnContainerType>(),
                    "Mezzanine::TakeN only takes into containers with push_back(value_type), add(value_type) or "
                    "insert(value_type).");
            static_assert(std::is_default_constructible<ReturnContainerType>(),
                    "Mezzanine::TakeN only takes into containers that can be default constructed.");


            auto Copying = IncomingContainer.cbegin();
            const SizeType HowMany = static_cast<SizeType>(CountToTake);

            ReturnContainerType Results;
            if constexpr( HasReserve<ReturnContainerType>())
                { Results.reserve(HowMany); }

            for(SizeType Counter{0}; Counter<HowMany; Counter++, Copying++)
            {
                if constexpr(HasPushBackValue<ReturnContainerType>())
                    { Results.push_back(*Copying); }
                else if constexpr(HasInsertValue<ReturnContainerType>())
                    { Results.insert(*Copying); }
                else if constexpr(HasAddValue<ReturnContainerType>())
                    { Results.add(*Copying); }
            }

            return Results;
        }
    END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(TakeNStruct, TakeN)


    /// @struct TakeBackNStruct
    /// @details This is an implementation detail for @ref TakeBackN

    /// @fn TakeBackN
    /// @details This contructs a smaller container by copying the last N items in reverse order from the data
    /// container. If an amount is passed but not data is a functor is returned that will return copies of the passed
    /// amount of data that functor is passed.
    /// @return A subset of the data or a functor that returns a subset of the data.

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(TakeBackNStruct)
        template<typename CVIncomingContainerType, typename CountType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, CountType CountToTake) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using SizeType = typename IncomingContainerType::size_type;
            using ActualCVReturnType = std::conditional_t<
                std::is_same_v<CVReturnContainerType, std::false_type>, IncomingContainerType, CVReturnContainerType
            >;
            using ReturnContainerType = std::decay_t<ActualCVReturnType>;
            using namespace ContainerDetect;

            static_assert(IsRange<IncomingContainerType>(),
                    "Mezzanine::TakeBackN only takes from containers or ranges.");
            static_assert(IsContainer<ReturnContainerType>(),
                    "Mezzanine::TakeBackN only takes into containers.");
            static_assert(HasPushBackValue<ReturnContainerType>() ||
                          HasInsertValue<ReturnContainerType>() ||
                          HasAddValue<ReturnContainerType>(),
                    "Mezzanine::TakeBackN only takes into containers with push_back(value_type), add(value_type) or "
                    "insert(value_type).");
            static_assert(std::is_default_constructible<ReturnContainerType>(),
                    "Mezzanine::TakeBackN only takes into containers that can be default constructed.");


            auto Copying = IncomingContainer.crbegin();
            const SizeType HowMany = static_cast<SizeType>(CountToTake);

            ReturnContainerType Results;
            if constexpr( HasReserve<ReturnContainerType>())
                { Results.reserve(HowMany); }

            for(SizeType Counter{0}; Counter<HowMany; Counter++, Copying++)
            {
                if constexpr(HasPushBackValue<ReturnContainerType>())
                    { Results.push_back(*Copying); }
                else if constexpr(HasInsertValue<ReturnContainerType>())
                    { Results.insert(*Copying); }
                else if constexpr(HasAddValue<ReturnContainerType>())
                    { Results.add(*Copying); }
            }

            return Results;
        }
    END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(TakeBackNStruct, TakeBackN)

    /// @struct TakeWhileStruct
    /// @details This is an implementation detail for @ref TakeWhile

    /// @fn TakeWhile
    /// @details This contructs a smaller container by copying items from the beginning of the container as long as the
    /// predicate returns true when passed that item. When the predicate returns false this returns what has been copied
    /// so far. If a predicate is passed but nt data is passed, then a functor is returned that will return copies of
    /// the data it recieves based on the predicate.
    /// @return A subset of the data or a functor that returns a subset of the data.

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(TakeWhileStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, PredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using ActualCVReturnType = std::conditional_t<
                std::is_same_v<CVReturnContainerType, std::false_type>, IncomingContainerType, CVReturnContainerType
            >;
            using ReturnContainerType = std::decay_t<ActualCVReturnType>;
            using namespace ContainerDetect;

            static_assert(IsRange<IncomingContainerType>(),
                    "Mezzanine::TakeWhile only takes from containers or ranges.");
            static_assert(IsContainer<ReturnContainerType>(),
                    "Mezzanine::TakeWhile only takes into containers.");
            static_assert(HasPushBackValue<ReturnContainerType>() ||
                          HasInsertValue<ReturnContainerType>() ||
                          HasAddValue<ReturnContainerType>(),
                    "Mezzanine::TakeWhile only takes into containers with push_back(value_type), add(value_type) or "
                    "insert(value_type).");
            static_assert(std::is_default_constructible<ReturnContainerType>(),
                    "Mezzanine::TakeWhile only takes into containers that can be default constructed.");

            ReturnContainerType Results;

            if constexpr( HasReserve<ReturnContainerType>())
                { Results.reserve(IncomingContainer.size()); }

            auto Copying = IncomingContainer.cbegin();
            while(IncomingContainer.cend() != Copying && std::invoke(std::forward<PredicateType>(Predicate), *Copying))
            {
                if constexpr(HasPushBackValue<ReturnContainerType>())
                    { Results.push_back(*Copying); }
                else if constexpr(HasInsertValue<ReturnContainerType>())
                    { Results.insert(*Copying); }
                else if constexpr(HasAddValue<ReturnContainerType>())
                    { Results.add(*Copying); }

                Copying++;
            }

            return Results;
        }
    END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(TakeWhileStruct, TakeWhile)

    /// @struct TakeBackWhileStruct
    /// @details This is an implementation detail for @ref TakeBackWhile

    /// @fn TakeBackWhile
    /// @details This contructs a smaller container by copying items from the end of the container in reverse order as
    /// long as the predicate returns true when passed that item. When the predicate returns false this returns what has
    /// been copied so far. If a predicate is passed but no data is, then a functor is returned that will return copies
    /// using the predicate as described above.
    /// @return A subset of the data or a functor that returns a subset of the data.

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(TakeBackWhileStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, PredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using ActualCVReturnType = std::conditional_t<
                std::is_same_v<CVReturnContainerType, std::false_type>, IncomingContainerType, CVReturnContainerType
            >;
            using ReturnContainerType = std::decay_t<ActualCVReturnType>;
            using namespace ContainerDetect;

            static_assert(IsRange<IncomingContainerType>(),
                    "Mezzanine::TakeBackWhile only takes from containers or ranges.");
            static_assert(IsContainer<ReturnContainerType>(),
                    "Mezzanine::TakeBackWhile only takes into containers.");
            static_assert(HasPushBackValue<ReturnContainerType>() ||
                          HasInsertValue<ReturnContainerType>() ||
                          HasAddValue<ReturnContainerType>(),
                    "Mezzanine::TakeBackWhile only takes into containers with push_back(value_type), add(value_type) "
                    "or insert(value_type).");
            static_assert(std::is_default_constructible<ReturnContainerType>(),
                    "Mezzanine::TakeBackWhile only takes into containers that can be default constructed.");

            ReturnContainerType Results;

            if constexpr( HasReserve<ReturnContainerType>())
                { Results.reserve(IncomingContainer.size()); }

            auto Copying = IncomingContainer.crbegin();
            while(IncomingContainer.crend() != Copying && std::invoke(std::forward<PredicateType>(Predicate), *Copying))
            {
                if constexpr(HasPushBackValue<ReturnContainerType>())
                    { Results.push_back(*Copying); }
                else if constexpr(HasInsertValue<ReturnContainerType>())
                    { Results.insert(*Copying); }
                else if constexpr(HasAddValue<ReturnContainerType>())
                    { Results.add(*Copying); }

                Copying++;
            }

            return Results;
        }
    END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(TakeBackWhileStruct, TakeBackWhile)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// @struct DropNStruct
    /// @details This is an implementation detail for @ref DropN

    /// @fn DropN
    /// @details This skips the first N items them copies items to the start of the container. If not given data this
    /// returns a functor that does the described copying.
    /// @return A subset of the data with some of the data omitted.

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(DropNStruct)
        template<typename CVIncomingContainerType, typename CountType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, CountType CountToDrop) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using SizeType = typename IncomingContainerType::size_type;
            using DiffType = typename IncomingContainerType::difference_type;
            using ActualCVReturnType = std::conditional_t<
                std::is_same_v<CVReturnContainerType, std::false_type>, IncomingContainerType, CVReturnContainerType
            >;
            using ReturnContainerType = std::decay_t<ActualCVReturnType>;
            using namespace ContainerDetect;

            static_assert(IsRange<IncomingContainerType>(),
                    "Mezzanine::DropN only drops from containers or ranges.");
            static_assert(IsContainer<ReturnContainerType>(),
                    "Mezzanine::DropN only drops into containers.");
            static_assert(HasPushBackValue<ReturnContainerType>() ||
                          HasInsertValue<ReturnContainerType>() ||
                          HasAddValue<ReturnContainerType>(),
                    "Mezzanine::DropN only drops into containers with push_back(value_type), add(value_type) or "
                    "insert(value_type).");
            static_assert(std::is_default_constructible<ReturnContainerType>(),
                    "Mezzanine::DropN only drops into containers that can be default constructed.");

            const SizeType HowMany = static_cast<SizeType>(CountToDrop);
            const DiffType Difference = static_cast<DiffType>(CountToDrop);

            // Don't do work if we are dropping more than we have.
            ReturnContainerType Results;
            if(IncomingContainer.size() < HowMany)
                { return Results; }

            if constexpr( HasReserve<ReturnContainerType>())
                { Results.reserve(IncomingContainer.size()-HowMany); }

            for(auto Copying = IncomingContainer.cbegin() + Difference;
                IncomingContainer.cend() != Copying;
                Copying++)
            {
                if constexpr(HasPushBackValue<ReturnContainerType>())
                    { Results.push_back(*Copying); }
                else if constexpr(HasInsertValue<ReturnContainerType>())
                    { Results.insert(*Copying); }
                else if constexpr(HasAddValue<ReturnContainerType>())
                    { Results.add(*Copying); }
            }

            return Results;
        }
    END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(DropNStruct, DropN)


    /// @struct DropBackNStruct
    /// @details This is an implementation detail for @ref DropBackN

    /// @fn DropBackN
    /// @details This skips the last N items then copies items to the end of the container. If not given data this
    /// returns a functor that does the described copying.
    /// @return A subset of the data with some of the data omitted.

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(DropBackNStruct)
        template<typename CVIncomingContainerType, typename CountType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, CountType CountToDrop) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using SizeType = typename IncomingContainerType::size_type;
            using DiffType = typename IncomingContainerType::difference_type;
            using ActualCVReturnType = std::conditional_t<
                std::is_same_v<CVReturnContainerType, std::false_type>, IncomingContainerType, CVReturnContainerType
            >;
            using ReturnContainerType = std::decay_t<ActualCVReturnType>;
            using namespace ContainerDetect;

            static_assert(IsRange<IncomingContainerType>(),
                    "Mezzanine::DropBackN only Drops from containers or ranges.");
            static_assert(IsContainer<ReturnContainerType>(),
                    "Mezzanine::DropBackN only Drops into containers.");
            static_assert(HasPushBackValue<ReturnContainerType>() ||
                          HasInsertValue<ReturnContainerType>() ||
                          HasAddValue<ReturnContainerType>(),
                    "Mezzanine::DropBackN only Drops into containers with push_back(value_type), add(value_type) or "
                    "insert(value_type).");
            static_assert(std::is_default_constructible<ReturnContainerType>(),
                    "Mezzanine::DropBackN only Drops into containers that can be default constructed.");

            const SizeType HowMany = static_cast<SizeType>(CountToDrop);
            const DiffType Difference = static_cast<DiffType>(CountToDrop);

            // Don't do work if we are dropping more than we have.
            ReturnContainerType Results;
            if(IncomingContainer.size() < HowMany)
                { return Results; }

            if constexpr( HasReserve<ReturnContainerType>())
                { Results.reserve(IncomingContainer.size()-HowMany); }

            for(auto Copying = IncomingContainer.crbegin() + Difference;
                IncomingContainer.crend() != Copying;
                Copying++)
            {
                if constexpr(HasPushBackValue<ReturnContainerType>())
                    { Results.push_back(*Copying); }
                else if constexpr(HasInsertValue<ReturnContainerType>())
                    { Results.insert(*Copying); }
                else if constexpr(HasAddValue<ReturnContainerType>())
                    { Results.add(*Copying); }
            }

            return Results;
        }
    END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(DropBackNStruct, DropBackN)

    /// @struct DropWhileStruct
    /// @details This is an implementation detail for @ref DropWhile

    /// @fn DropWhile
    /// @details Skips items from the beginning as long as a predicate returns true then copies the rest of the data
    /// from the incoming container. If not given data this returns a functor that uses the predicate to skip and copy
    /// data.
    /// @return A subset of the data with some of the data omitted.

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(DropWhileStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, PredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using ActualCVReturnType = std::conditional_t<
                std::is_same_v<CVReturnContainerType, std::false_type>, IncomingContainerType, CVReturnContainerType
            >;
            using ReturnContainerType = std::decay_t<ActualCVReturnType>;
            using namespace ContainerDetect;

            static_assert(IsRange<IncomingContainerType>(),
                    "Mezzanine::DropWhile only drops from containers or ranges.");
            static_assert(IsContainer<ReturnContainerType>(),
                    "Mezzanine::DropWhile only drops into containers.");
            static_assert(HasPushBackValue<ReturnContainerType>() ||
                          HasInsertValue<ReturnContainerType>() ||
                          HasAddValue<ReturnContainerType>(),
                    "Mezzanine::DropWhile only drops into containers with push_back(value_type), add(value_type) or "
                    "insert(value_type).");
            static_assert(std::is_default_constructible<ReturnContainerType>(),
                    "Mezzanine::DropWhile only drops into containers that can be default constructed.");

            ReturnContainerType Results;

            if constexpr( HasReserve<ReturnContainerType>())
                { Results.reserve(IncomingContainer.size()); }

            auto Copying = IncomingContainer.cbegin();
            while(IncomingContainer.cend() != Copying && std::invoke(std::forward<PredicateType>(Predicate), *Copying))
                { Copying++; }

            while(IncomingContainer.cend() != Copying)
            {
                if constexpr(HasPushBackValue<ReturnContainerType>())
                    { Results.push_back(*Copying); }
                else if constexpr(HasInsertValue<ReturnContainerType>())
                    { Results.insert(*Copying); }
                else if constexpr(HasAddValue<ReturnContainerType>())
                    { Results.add(*Copying); }

                Copying++;
            }

            return Results;
        }
    END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(DropWhileStruct, DropWhile)

    /// @struct DropBackWhileStruct
    /// @details This is an implementation detail for @ref DropBackWhile

    /// @fn DropBackWhile
    /// @details Skips items from the end as long as a predicate returns true then copies the rest of the data
    /// from the incoming container. If not given data this returns a functor that uses the predicate to skip and copy
    /// data.
    /// @return A subset of the data with some of the data omitted.

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(DropBackWhileStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        auto operator() (const CVIncomingContainerType& IncomingContainer, PredicateType&& Predicate) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using ActualCVReturnType = std::conditional_t<
                std::is_same_v<CVReturnContainerType, std::false_type>, IncomingContainerType, CVReturnContainerType
            >;
            using ReturnContainerType = std::decay_t<ActualCVReturnType>;
            using namespace ContainerDetect;

            static_assert(IsRange<IncomingContainerType>(),
                    "Mezzanine::DropBackWhile only drops from containers or ranges.");
            static_assert(IsContainer<ReturnContainerType>(),
                    "Mezzanine::DropBackWhile only drops into containers.");
            static_assert(HasPushBackValue<ReturnContainerType>() ||
                          HasInsertValue<ReturnContainerType>() ||
                          HasAddValue<ReturnContainerType>(),
                    "Mezzanine::DropBackWhile only drops into containers with push_back(value_type), add(value_type) "
                    "or insert(value_type).");
            static_assert(std::is_default_constructible<ReturnContainerType>(),
                    "Mezzanine::DropBackWhile only drops into containers that can be default constructed.");

            ReturnContainerType Results;

            if constexpr( HasReserve<ReturnContainerType>())
                { Results.reserve(IncomingContainer.size()); }

            auto Copying = IncomingContainer.crbegin();
            while(IncomingContainer.crend() != Copying && std::invoke(std::forward<PredicateType>(Predicate), *Copying))
                { Copying++; }

            while(IncomingContainer.crend() != Copying)
            {
                if constexpr(HasPushBackValue<ReturnContainerType>())
                    { Results.push_back(*Copying); }
                else if constexpr(HasInsertValue<ReturnContainerType>())
                    { Results.insert(*Copying); }
                else if constexpr(HasAddValue<ReturnContainerType>())
                    { Results.add(*Copying); }

                Copying++;
            }

            return Results;
        }
    END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(DropBackWhileStruct, DropBackWhile)

    /// @details This is an implementation detail for @ref Reverse
    template <typename CVReturnContainerType = std::false_type>
    struct ReverseStruct
    {
        template<typename CVIncomingContainerType>
        [[nodiscard]] constexpr
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer) const
        {
            using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
            using ActualCVReturnType = std::conditional_t<
                std::is_same_v<CVReturnContainerType, std::false_type>, IncomingContainerType, CVReturnContainerType
            >;
            using ReturnContainerType = std::decay_t<ActualCVReturnType>;
            using namespace ContainerDetect;

            static_assert(IsConstReverseRange<IncomingContainerType>(),
                    "Mezzanine::Reverse only reverses from reversible const containers or ranges.");
            static_assert(IsContainer<ReturnContainerType>(),
                    "Mezzanine::Reverse only reverses into containers.");
            static_assert(HasPushBackValue<ReturnContainerType>() ||
                          HasInsertValue<ReturnContainerType>() ||
                          HasAddValue<ReturnContainerType>(),
                    "Mezzanine::Reverse only reverses into containers with push_back(value_type), add(value_type) "
                    "or insert(value_type).");
            static_assert(std::is_default_constructible<ReturnContainerType>(),
                    "Mezzanine::Reverse only drops into containers that can be default constructed.");

            ReturnContainerType Results;

            if constexpr( HasReserve<ReturnContainerType>())
                { Results.reserve(IncomingContainer.size()); }

            for(auto Copying = IncomingContainer.crbegin();
                Copying != IncomingContainer.crend();
                Copying++)
            {
                if constexpr(HasPushBackValue<ReturnContainerType>())
                    { Results.push_back(*Copying); }
                else if constexpr(HasInsertValue<ReturnContainerType>())
                    { Results.insert(*Copying); }
                else if constexpr(HasAddValue<ReturnContainerType>())
                    { Results.add(*Copying); }
            }

            return Results;
        }
    };

    /// @copydoc Reverse
    template<typename CVReturnContainerType = std::false_type,
             typename CVIncomingContainerType>
    [[nodiscard]] constexpr
    decltype(auto) Reverse(const CVIncomingContainerType& IncomingContainer)
        { return ReverseStruct<CVReturnContainerType>{}(IncomingContainer); }

    /// @details Copies items in reverse order to create a new container. If not given data returns a functor that
    /// reverses its input.
    /// @return A copy of the data in reverse order.
    template<typename CVReturnContainerType = std::false_type>
    [[nodiscard]] constexpr
    decltype(auto) Reverse()
        { return ReverseStruct<CVReturnContainerType>{}; }



} // Functional
} // Mezzanine

#endif // Include guard
