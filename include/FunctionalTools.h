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
#ifndef Mezz_Foundation_FunctionalTools_h
#define Mezz_Foundation_FunctionalTools_h

/// @file
/// @brief

#ifndef SWIG
    #include "DataTypes.h"
    #include "ContainerTools.h"

    #include <iostream>
#endif

namespace Mezzanine
{


    template<typename CVReturnContainerType = void,
             typename CVIncomingContainerType,
             typename PredicateType>
    [[nodiscard]] constexpr
    decltype(auto) Select(const CVIncomingContainerType& IncomingContainer, PredicateType Predicate)
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using ActualCVReturnType = std::conditional_t<
            std::is_void_v<CVReturnContainerType>, IncomingContainerType, CVReturnContainerType
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

        if constexpr( HasReserve<ReturnContainerType>())
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


    template<typename CVIncomingContainerType,
             typename PredicateType,
             typename CVReturnContainerType = CVIncomingContainerType>
    constexpr
    decltype(auto) Select3(const CVIncomingContainerType& IncomingContainer,
                          PredicateType Predicate,
                          CVReturnContainerType&& ResultsContainer = CVIncomingContainerType())
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using ReturnContainerType = std::decay_t<CVReturnContainerType>;
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

        ReturnContainerType Results{ResultsContainer};

        if constexpr( HasReserve<ReturnContainerType>())
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template<typename CVReturnContainerType = void,
             typename CVIncomingContainerType,
             typename PredicateType>
    [[nodiscard]] constexpr
    decltype(auto) Reject(const CVIncomingContainerType& IncomingContainer, PredicateType Predicate)
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using ActualCVReturnType = std::conditional_t<
            std::is_void_v<CVReturnContainerType>, IncomingContainerType, CVReturnContainerType
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


    template<typename CVIncomingContainerType,
             typename PredicateType,
             typename CVReturnContainerType = CVIncomingContainerType>
    [[nodiscard]] constexpr
    decltype(auto) Reject3(const CVIncomingContainerType& IncomingContainer,
                          PredicateType Predicate,
                          CVReturnContainerType&& ResultsContainer = CVIncomingContainerType())
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using ReturnContainerType = std::decay_t<CVReturnContainerType>;
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

        ReturnContainerType Results{ResultsContainer};

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    template<typename CVReturnContainerType = void,
             typename CVIncomingContainerType,
             typename PredicateType>
    [[nodiscard]] constexpr
    decltype(auto) Convert(const CVIncomingContainerType& IncomingContainer, PredicateType Predicate)
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using ActualCVReturnType = std::conditional_t<
            std::is_void_v<CVReturnContainerType>, IncomingContainerType, CVReturnContainerType
        >;
        using ReturnContainerType = std::decay_t<ActualCVReturnType>;
        using namespace ContainerDetect;

        static_assert(IsRange<IncomingContainerType>(),
                "Mezzanine::Convert only coonverts from containers or ranges.");
        static_assert(IsContainer<ReturnContainerType>(),
                "Mezzanine::Convert only coonverts into containers.");
        static_assert(HasPushBackValue<ReturnContainerType>() ||
                      HasInsertValue<ReturnContainerType>() ||
                      HasAddValue<ReturnContainerType>(),
                "Mezzanine::Convert only coonverts into containers with push_back(value_type), add(value_type) or "
                "insert(value_type).");
        static_assert(std::is_default_constructible<ReturnContainerType>(),
                "Mezzanine::Convert only coonverts into containers that can be default constructed.");

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


    template<typename CVIncomingContainerType,
             typename PredicateType,
             typename CVReturnContainerType = CVIncomingContainerType>
    [[nodiscard]] constexpr
    decltype(auto) Convert3(const CVIncomingContainerType& IncomingContainer,
                           PredicateType Predicate,
                           CVReturnContainerType&& ResultsContainer = CVIncomingContainerType())
    {
        using IncomingContainerType = std::decay_t<CVIncomingContainerType>;
        using ReturnContainerType = std::decay_t<CVReturnContainerType>;
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

        ReturnContainerType Results{ResultsContainer};

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

} //Mezzanine

#endif // Include guard
