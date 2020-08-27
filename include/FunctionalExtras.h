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
#ifndef Mezz_Foundation_FunctionalExtras_h
#define Mezz_Foundation_FunctionalExtras_h

/// @file
/// @brief Tools that relate to the core functional tools but are core to the Pipe >> Functor >> Drain pattern.

#ifndef SWIG
    #include "ContainerTools.h"
    #include "MezzException.h"
#endif

namespace Mezzanine
{
namespace Functional
{

SAVE_WARNING_STATE
SUPPRESS_CLANG_WARNING("-Wpadded")
SUPPRESS_GCC_WARNING("-Wpadded")

/// @brief This binds value(s) to the left arguments in a function and returns a function that accepts unbound args.
/// @detailts Currying is "partially applying arguments to a function". These are ways make new functions by
/// bundling old functions and values in to a lambda so that we get new semantics. The tests have a super simple
/// example.
/// @n @n
/// Derived from https://stackoverflow.com/questions/152005/how-can-currying-be-done-in-c
/// With written permission via cc by-sa
/// @tparam FunctionType What was the passed function's type, derived automatically most of th time.
/// @tparam ArgumentsType A pack type of whatever args were passed.
/// @param Function The actual function to have data attached to it.
/// @param Arguments The data to attach to the function
/// @return A functions with 0 or more parameters bound to passed values
template<typename FunctionType, typename... ArgumentsType>
auto Curry(FunctionType Function, ArgumentsType... Arguments) {
    return [=](auto... AppendedArguments) {
        return std::invoke(Function, Arguments..., AppendedArguments...);
    };
}

/// @copydoc Curry
/// @brief As Curry, but binds the rightmost arguments.
template<typename FunctionType, typename... ArgumentsType>
auto CurryBack(FunctionType Function, ArgumentsType... Arguments) {
    return [=](auto... AppendedArguments) {
        return std::invoke(Function, AppendedArguments..., Arguments...);
    };
}

/// @brief Creates a simple range of countable values.
/// @tparam ReturnContainerType The kind of container to return, defaults to std::vector<Integer>.
/// @param First The first item in the container.
/// @param Last The item in the container if the stride doesn't skip over it.
/// @param Stride Count from First to Last by incrementing this amount.
/// @return A container with the computed values.
template<typename ReturnContainerType = std::vector<Integer>>
auto CreateRange(const typename ReturnContainerType::value_type& First,
                 const typename ReturnContainerType::value_type& Last,
                 const typename ReturnContainerType::value_type& Stride = 1)
{
    using namespace ContainerDetect;

    static_assert(IsContainer<ReturnContainerType>(),
            "Mezzanine::CreateRange has a lie for a name, it actually only creates containers.");
    static_assert(HasValueType<ReturnContainerType>(),
            "Mezzanine::CreateRange expects the container to have a value_type.");
    static_assert(HasPushBackValue<ReturnContainerType>() ||
                  HasInsertValue<ReturnContainerType>() ||
                  HasAddValue<ReturnContainerType>(),
            "Mezzanine::ConCreateRangevert only fills containers with push_back(value_type), add(value_type) or "
            "insert(value_type).");

    ReturnContainerType Results;
    if constexpr( HasReserve<ReturnContainerType>())
        { Results.reserve( static_cast<typename ReturnContainerType::size_type>(Last-First)); }

    for(typename ReturnContainerType::value_type Counter = First; Counter<=Last; Counter+=Stride)
    {
        if constexpr(HasPushBackValue<ReturnContainerType>())
            { Results.push_back(Counter); }
        else if constexpr(HasInsertValue<ReturnContainerType>())
            { Results.insert(Counter); }
        else if constexpr(HasAddValue<ReturnContainerType>())
            { Results.add(Counter); }
    }

    return Results;
}

RESTORE_WARNING_STATE

} // Functional
} // Mezzanine

#endif // Include guard
