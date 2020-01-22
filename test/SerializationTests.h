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
#ifndef Mezz_Foundation_SerializationTests_h
#define Mezz_Foundation_SerializationTests_h

/// @file
/// @brief This file tests the functionality of the Serialization classes.

#include "MezzTest.h"

#include "Serialization.h"

namespace SerializationTest {
    using namespace Mezzanine;

    struct Simple
    {
        Integer IntVarOne = -100;
        Integer IntVarTwo = 100;

        static StringView RegisterName()
            { return "Simple"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            return Members(
                MakeMemberAccessor("IntVarOne",&Simple::IntVarOne),
                MakeMemberAccessor("IntVarTwo",&Simple::IntVarTwo)
            );
        }
    };//Simple

    struct SimpleBase
    {
        String StringVar = "Base";

        static StringView RegisterName()
            { return "SimpleBase"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            return Members(
                MakeMemberAccessor("StringVar",&SimpleBase::StringVar)
            );
        }
    };//SimpleBase

    struct SimpleDerivedOne : public SimpleBase
    {
        double DoubleVar = 3.1415926;
        float FloatVarOne = -3.14f;
        float FloatVarTwo = 3.14f;

        static StringView RegisterName()
            { return "SimpleDerivedOne"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            return MergeMembers(
                SimpleBase::RegisterMembers(),
                Members(
                    MakeMemberAccessor("DoubleVar",&SimpleDerivedOne::DoubleVar),
                    MakeMemberAccessor("FloatVarOne",&SimpleDerivedOne::FloatVarOne),
                    MakeMemberAccessor("FloatVarTwo",&SimpleDerivedOne::FloatVarTwo)
                )
            );
        }
    };//SimpleDerivedOne

    struct SimpleDerivedTwo : public SimpleBase
    {
        UInt32 UIntVar = 120000;
        Int32 IntVar = -120000;
        UInt16 ShortUIntVar = 42;
        Int16 ShortIntVar = -42;

        static StringView RegisterName()
            { return "SimpleDerivedTwo"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            return MergeMembers(
                SimpleBase::RegisterMembers(),
                Members(
                    MakeMemberAccessor("UIntVar",&SimpleDerivedTwo::UIntVar),
                    MakeMemberAccessor("IntVar",&SimpleDerivedTwo::IntVar),
                    MakeMemberAccessor("ShortUIntVar",&SimpleDerivedTwo::ShortUIntVar),
                    MakeMemberAccessor("ShortIntVar",&SimpleDerivedTwo::ShortIntVar)
                )
            );
        }
    };//SimpleDerivedTwo

    class Composed
    {
    protected:
        Simple SimpleObj;
        SimpleBase* SimplePtr;
    public:
        Composed(SimpleBase* Ptr) :
            SimplePtr(Ptr)
            {  }

        static StringView RegisterName()
            { return "Composed"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            return Members(
                MakeMemberAccessor("SimpleObj",&Composed::SimpleObj),
                MakeMemberAccessor("SimplePtr",&Composed::SimplePtr)
            );
        }
    };//Composed
}//SerializationTest

AUTOMATIC_TEST_GROUP(SerializationTests,Serialization)
{
    using namespace Mezzanine;
    using namespace SerializationTest;

    RegisterCasters<SimpleBase,SimpleDerivedOne,SimpleDerivedTwo>();
}

#endif
