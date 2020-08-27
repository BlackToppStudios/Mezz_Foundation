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
#ifndef Mezz_Foundation_FunctionalPipe_h
#define Mezz_Foundation_FunctionalPipe_h

/// @file
/// @brief

#ifndef SWIG
    #include "DataTypes.h"
    #include "ContainerTools.h"
    #include "MezzException.h"
#endif

namespace Mezzanine
{
namespace Functional {

    // This is garbage I am experimenting with.
    template<typename CVContainerType>
    class Pipe {
    private:
        CVContainerType PipeData;
    public:
        Pipe(CVContainerType DataToPipe) : PipeData{DataToPipe}
        {
            // Static asserts around type of Data
        }

        ~Pipe() = default;
        Pipe(const Pipe&) = default;

        [[nodiscard]] constexpr
        CVContainerType Value() const noexcept
            { return PipeData; }

        /*
        // operator precendence after ==
        template<typename FunctionType>
        [[nodiscard]] constexpr
        auto operator | (FunctionType&& FunctionOrDrain) const
        {
            using namespace ContainerDetect;
            if constexpr(HasDrain<FunctionType>())
                //{ return FunctionOrDrain.Drain(PipeData); }
                { return std::invoke(FunctionOrDrain.Drain(), PipeData); }
            else
                { return Pipe{ std::invoke(std::forward<FunctionType>(FunctionOrDrain), PipeData) }; }
        }

        // operator precendence before ==
        template<typename FunctionType>
        [[nodiscard]] constexpr
        auto operator >> (FunctionType&& FunctionOrDrain) const
        {
            using namespace ContainerDetect;
            if constexpr(HasDrain<FunctionType>())
                //{ return FunctionOrDrain.Drain(PipeData); }
                { return std::invoke(FunctionOrDrain.Drain(), PipeData); }
            else
                { return Pipe{ std::invoke(std::forward<FunctionType>(FunctionOrDrain), PipeData) }; }
        }
*/
        template<typename FunctorType>
        [[nodiscard]] constexpr
        auto operator >> (FunctorType&& Functor) const
        {
            return Pipe{ std::invoke(std::forward<FunctorType>(Functor), PipeData) };
        }

        template<typename FunctorType>
        [[nodiscard]] constexpr
        auto operator > (FunctorType&& Functor) const
        {
            return std::invoke(std::forward<FunctorType>(Functor), PipeData);
        }

        // Convert this when assiging to strongly typed storage.
        [[nodiscard]] constexpr
        operator auto() const
            { return PipeData; }

    };
/*
    template <typename T, typename PipedDataType>
    [[nodiscard]]
    Boole operator==(const T& t, const Pipe<PipedDataType>& MyPipeIsReady)
    {
        static_assert(std::is_convertible_v<T,PipedDataType>,
            "The data contained in the Pipe is not implicitly convertible to T for operator==(T,Pipe).");
        return t == MyPipeIsReady.Value();
    }

    template <typename T, typename PipedDataType>
    [[nodiscard]]
    Boole operator==(const Pipe<PipedDataType>& MyPipeIsReady, const T& t)
    {
        static_assert(std::is_convertible_v<T,PipedDataType>,
            "The data contained in the Pipe is not implicitly convertible to T for operator==(Pipe,T).");
        return t == MyPipeIsReady.Value();
    }

    template <typename T, typename PipedDataType>
    [[nodiscard]]
    Boole operator!=(const T& t, const Pipe<PipedDataType>& MyPipeIsReady)
    {
        static_assert(std::is_convertible_v<T,PipedDataType>,
            "The data contained in the Pipe is not implicitly convertible to T for operator!=(T,Pipe).");
        return t != MyPipeIsReady.Value();
    }

    template <typename T, typename PipedDataType>
    [[nodiscard]]
    Boole operator!=(const Pipe<PipedDataType>& MyPipeIsReady, const T& t)
    {
        static_assert(std::is_convertible_v<T,PipedDataType>,
            "The data contained in the Pipe is not implicitly convertible to T for operator!=(Pipe,T).");
        return t != MyPipeIsReady.Value();
    }
*/

} // Functional
} // Mezzanine

#endif // Include guard
