// © Copyright 2010 - 2019 BlackTopp Studios Inc.
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
#ifndef Mezz_Foundation_FunctionalToolsTests_h
#define Mezz_Foundation_FunctionalToolsTests_h

#include <deque>

#include "MezzTest.h"

#include "FunctionalTools.h"
#include "SortedVector.h"

/// @file
/// @brief

DEFAULT_TEST_GROUP(FunctionalToolsTests,FunctionalTools)
{
    using namespace Mezzanine;

    // This meme is floating around r/ProgrammerHumor so I might as well.
    const auto IsEven = [](Integer i) noexcept { return i%2 == 0; };
    const auto IsOdd = [IsEven](Integer i) noexcept { return !IsEven(i); };
    const auto Increment = [](Integer i) noexcept { return i+1; };

    const std::vector<Integer> OneToTen {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const std::vector<Integer> OneToTenOnlyEven {2, 4, 6, 8, 10};
    const std::vector<Integer> OneToTenOnlyOdd {1, 3, 5, 7, 9};
    const std::vector<Integer> OnlyZeroForSure{0};
    const std::vector<Integer> ZeroToTenOnlyEven {0, 2, 4, 6, 8, 10};
    const std::vector<Integer> ZeroAndTwoToEleven {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

    const std::list<Integer> ListOneToTenOnlyEven {2, 4, 6, 8, 10};
    const std::set<Integer> SetOneToTenOnlyOdd {1, 3, 5, 7, 9};
    const std::deque<Integer> DequeOneToTenOnlyEven {2, 4, 6, 8, 10};

    {
        // Simple tests
        TEST("Select<Integer>(IsEven)", OneToTenOnlyEven == Select(OneToTen, IsEven))
        TEST("Reject<Integer>(IsEven)", OneToTenOnlyOdd == Reject(OneToTen, IsEven))
        TEST("Convert<Integer>(Increment)", OneToTenOnlyEven == Convert(OneToTenOnlyOdd, Increment))

        // Specified Return Types
        TEST("Select<List,Integer>(IsEven)",
             ListOneToTenOnlyEven == Select<std::list<Integer>>(OneToTen, IsEven))
        TEST("Reject<Set,Integer>(IsEven)",
             SetOneToTenOnlyOdd == Reject<std::set<Integer>>(OneToTen, IsEven))
        TEST("Convert<Deque,Integer>(Increment)",
             DequeOneToTenOnlyEven == Convert<std::deque<Integer>>(OneToTenOnlyOdd, Increment))
    }

    { // AutoCurrying Predicates
        const auto SelectEvenIntoVector = Select<std::vector<Integer>>(IsEven);
        TEST("SelectAutoCurried<vectorConstruction>(IsEven)", OneToTenOnlyEven == SelectEvenIntoVector(OneToTen))

        const auto SelectEven = Select(IsEven);
        TEST("SelectAutoCurried<Integer>(IsEven)", OneToTenOnlyEven == SelectEven(OneToTen))

    }

    {
        // Simple tests
        TEST("Select3<Integer>(IsEven)", OneToTenOnlyEven == Select3(OneToTen, IsEven))
        TEST("Reject3<Integer>(IsEven)", OneToTenOnlyOdd == Reject3(OneToTen, IsEven))
        TEST("Convert3<Integer>(Increment)", OneToTenOnlyEven == Convert3(OneToTenOnlyOdd, Increment))

        // Specified Return Types
        TEST("Select3<List,Integer>(IsEven)",
             ListOneToTenOnlyEven == Select3(OneToTen, IsEven, std::list<Integer>{}))
        TEST("Select3<Set,Integer>(IsEven)",
             SetOneToTenOnlyOdd == Reject3(OneToTen, IsEven, std::set<Integer>{}))
        TEST("Convert3<Deque,Integer>(Increment)",
             DequeOneToTenOnlyEven == Convert3(OneToTenOnlyOdd, Increment, std::deque<Integer>()))

        {
            std::vector<Integer> OnlyZeroMutable{0};
            TEST("Select3<List,Integer,{0}>(IsEven)",
                 ZeroToTenOnlyEven == Select3(OneToTen, IsEven,std::vector<Integer>{0}))
            TEST("Select3<List,Integer,{0}>(IsEven)-InputUnchanged", OnlyZeroForSure == OnlyZeroMutable)

            TEST("Select3<List,Integer,const&{0}>(IsEven)",
                 ZeroToTenOnlyEven == Select3(OneToTen, IsEven, OnlyZeroForSure))
            TEST("Select3<List,Integer,const&{0}>(IsEven)-InputUnchanged", OnlyZeroForSure == OnlyZeroMutable)

            TEST("Select3<List,Integer,&{0}>(IsEven)",
                 ZeroToTenOnlyEven == Select3(OneToTen, IsEven, OnlyZeroMutable))
            TEST("Select3<List,Integer,&{0}>(IsEven)-InputUnchanged", OnlyZeroForSure == OnlyZeroMutable)
        }

        {
            std::vector<Integer> OnlyZeroMutable{0};
            TEST("Reject3<List,Integer,{0}>(IsOdd)",
                 ZeroToTenOnlyEven == Reject3(OneToTen, IsOdd,std::vector<Integer>{0}))
            TEST("Reject3<List,Integer,{0}>(IsOdd)-InputUnchanged", OnlyZeroForSure == OnlyZeroMutable)

            TEST("Reject3<List,Integer,const&{0}>(IsOdd)",
                 ZeroToTenOnlyEven == Reject3(OneToTen, IsOdd, OnlyZeroForSure))
            TEST("Reject3<List,Integer,const&{0}>(IsOdd)-InputUnchanged", OnlyZeroForSure == OnlyZeroMutable)

            TEST("Reject3<List,Integer,&{0}>(IsOdd)",
                 ZeroToTenOnlyEven == Reject3(OneToTen, IsOdd, OnlyZeroMutable))
            TEST("Reject3<List,Integer,&{0}>(IsOdd)-InputUnchanged", OnlyZeroForSure == OnlyZeroMutable)
        }

        {
            std::vector<Integer> OnlyZeroMutable{0};
            TEST("Convert3<List,Integer,{0}>(Increment)",
                 ZeroAndTwoToEleven == Convert3(OneToTen, Increment, std::vector<Integer>{0}))
            TEST("Convert3<List,Integer,{0}>(Increment)-InputUnchanged", OnlyZeroForSure == OnlyZeroMutable)

            TEST("Convert3<List,Integer,const&{0}>(Increment)",
                 ZeroAndTwoToEleven == Convert3(OneToTen, Increment, OnlyZeroForSure))
            TEST("Convert3<List,Integer,const&{0}>(IsEven)-InputUnchanged", OnlyZeroForSure == OnlyZeroMutable)

            TEST("Convert3<List,Integer,&{0}>(Increment)",
                 ZeroAndTwoToEleven == Convert3(OneToTen, Increment, OnlyZeroMutable))
            TEST("Convert3<List,Integer,&{0}>(IsEven)-InputUnchanged", OnlyZeroForSure == OnlyZeroMutable)
        }

    }

    {
        const auto UpCase = [](String::value_type c) noexcept
        {
            if('a'<=c && c<='z')
                { return static_cast<String::value_type>(c-32); }
            return c;
        };
        const String Word{"Word"};
        const String UpperCaseWord{"WORD"};

        TEST("Convert<String>(UpCase)", UpperCaseWord == Convert(Word, UpCase))

    }

    {
        const auto OnlyReturnsTrue = Curry(IsEven, 2);
        const auto OnlyReturnsFalse = Curry(IsEven, 1);
        const auto AlsoOnlyReturnsTrue = CurryBack(IsEven, 2);
        const auto AlsoOnlyReturnsFalse = CurryBack(IsEven, 1);

        TEST_EQUAL("CurryReturnsTrue", true, OnlyReturnsTrue())
        TEST_EQUAL("CurryReturnsFalse", false, OnlyReturnsFalse())
        TEST_EQUAL("CurryBackReturnsTrue", true, AlsoOnlyReturnsTrue())
        TEST_EQUAL("CurryBackReturnsFalse", false, AlsoOnlyReturnsFalse())

        const auto SubtractFromTen = Curry(std::minus<int>(), 10);
        TEST_EQUAL("CurryStdFunctions", 3, SubtractFromTen(7))

        const auto SubtractFive = CurryBack(std::minus<int>(), 5);
        TEST_EQUAL("CurryBackStdFunctions", 2, SubtractFive(7))
    }

    // Garbage that is still in progress

    SAVE_WARNING_STATE
    SUPPRESS_CLANG_WARNING("-Wctad-maybe-unsupported")
    {
        Pipe Working{OneToTen};

        TEST("PipeSimpleGetValue", OneToTen == Working.Value())
        TEST("PipeInlineGetValue", OneToTen == Pipe{OneToTen}.Value())

        Pipe{OneToTen}.f();
        Pipe<std::vector<int>>::f();
        Pipe{OneToTen}.next(IsEven);
        Pipe{OneToTen}.next(SelectStruct<>());
        //Pipe{OneToTen}.next(Select<>());
    }
    RESTORE_WARNING_STATE
}

#endif
