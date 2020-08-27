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


    #define START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(StructName)                                             \
        template <typename CVReturnContainerType = std::false_type, typename MaybePredicateType = std::false_type>     \
        struct StructName                                                                                              \
        {                                                                                                              \
            MaybePredicateType MaybePredicate;
#endif


#ifndef END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY

    /// @def END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY
    /// @brief Terminate a default functor that accepts and returns a container and autocurries if given a predicate.
    /// @param StructName
    /// @param Name

    #define END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(StructName,Name)                                          \
            template<typename CVIncomingContainerType>                                                                 \
            [[nodiscard]] constexpr                                                                                    \
            decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer) const                         \
            {                                                                                                          \
                return operator()(IncomingContainer, MaybePredicate);                                                  \
            }                                                                                                          \
        };                                                                                                             \
                                                                                                                       \
        /* Immediately invoking entry point */                                                                         \
        template<typename CVReturnContainerType = std::false_type,                                                     \
                 typename CVIncomingContainerType,                                                                     \
                 typename PredicateType>                                                                               \
        [[nodiscard]] constexpr                                                                                        \
        decltype(auto) Name(const CVIncomingContainerType& IncomingContainer, PredicateType&& Predicate)               \
        {                                                                                                              \
        return StructName<CVReturnContainerType,std::false_type>()                                                     \
                (std::forward<const CVIncomingContainerType&>(IncomingContainer),                                      \
                 std::forward<PredicateType>(Predicate));                                                              \
        }                                                                                                              \
                                                                                                                       \
        /* Auto-Currying Entry Point */                                                                                \
        template<typename CVReturnContainerType = std::false_type,                                                     \
             typename PredicateType>                                                                                   \
        [[nodiscard]] constexpr                                                                                        \
        decltype(auto) Name(PredicateType&& Predicate)                                                                 \
            { return StructName<CVReturnContainerType,PredicateType>{std::forward<PredicateType>(Predicate)}; }

#endif

    // The select make a new method by checking if the predicate is true when called for each item in the incoming data.
    // if it is true, it is included, if not it is excluded.

    /// @struct SelectStruct
    /// @details This is an implementation detail for @ref Select

    /// @fn Select
    /// @details todo
    /// @return todo

    /// @struct FilterStruct
    /// @details This is an implementation detail for @ref Filter

    /// @fn Filter
    /// @copydoc Select alias of Select

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(SelectStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, PredicateType Predicate) const
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
                if(std::invoke(Predicate,item))
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
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, PredicateType Predicate) const
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
                if(std::invoke(Predicate,item))
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
    /// @details todo
    /// @return todo

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(RejectStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, PredicateType Predicate) const
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
                if(!std::invoke(Predicate,item))
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
    /// @details todo
    /// @return todo

    /// @struct MapStruct
    /// @details This is an implementation detail for @ref Map

    /// @fn Map
    /// @copydoc Convert alias of Convert

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(ConvertStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, PredicateType Predicate) const
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
                    { Results.push_back(std::invoke(Predicate,item)); }
                else if constexpr(HasInsertValue<ReturnContainerType>())
                    { Results.insert(std::invoke(Predicate,item)); }
                else if constexpr(HasAddValue<ReturnContainerType>())
                    { Results.add(std::invoke(Predicate,item)); }
            }

            return Results;
        }
    END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(ConvertStruct, Convert)

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(MapStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, PredicateType Predicate) const
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
                    { Results.push_back(std::invoke(Predicate,item)); }
                else if constexpr(HasInsertValue<ReturnContainerType>())
                    { Results.insert(std::invoke(Predicate,item)); }
                else if constexpr(HasAddValue<ReturnContainerType>())
                    { Results.add(std::invoke(Predicate,item)); }
            }

            return Results;
        }
    END_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(MapStruct, Map)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// @struct TakeNStruct
    /// @details This is an implementation detail for @ref TakeN

    /// @fn TakeN
    /// @details todo
    /// @return todo

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(TakeNStruct)
        template<typename CVIncomingContainerType, typename CountType>
        [[nodiscard]] constexpr
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, CountType CountToTake) const
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
    /// @details todo returns reversed end of container
    /// @return todo

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(TakeBackNStruct)
        template<typename CVIncomingContainerType, typename CountType>
        [[nodiscard]] constexpr
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, CountType CountToTake) const
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
    /// @details todo
    /// @return todo

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(TakeWhileStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, PredicateType Predicate) const
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
            while(IncomingContainer.cend() != Copying && std::invoke(Predicate, *Copying))
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
    /// @details todo
    /// @return todo

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(TakeBackWhileStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, PredicateType Predicate) const
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
            while(IncomingContainer.crend() != Copying && std::invoke(Predicate, *Copying))
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
    /// @details todo
    /// @return todo

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(DropNStruct)
        template<typename CVIncomingContainerType, typename CountType>
        [[nodiscard]] constexpr
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, CountType CountToDrop) const
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
    /// @details todo returns reversed end of container
    /// @return todo

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(DropBackNStruct)
        template<typename CVIncomingContainerType, typename CountType>
        [[nodiscard]] constexpr
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, CountType CountToDrop) const
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
    /// @details todo
    /// @return todo

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(DropWhileStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, PredicateType Predicate) const
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
            while(IncomingContainer.cend() != Copying && std::invoke(Predicate, *Copying))
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
    /// @details todo
    /// @return todo

    START_FUNCTOR_END_CONTAINER_TO_CONTAINER_AUTOCURRY(DropBackWhileStruct)
        template<typename CVIncomingContainerType, typename PredicateType>
        [[nodiscard]] constexpr
        decltype(auto) operator() (const CVIncomingContainerType& IncomingContainer, PredicateType Predicate) const
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
            while(IncomingContainer.crend() != Copying && std::invoke(Predicate, *Copying))
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




} // Functional
} // Mezzanine

#endif // Include guard
