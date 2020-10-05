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
#ifndef Mezz_Foundation_FunctionalToolsTests_h
#define Mezz_Foundation_FunctionalToolsTests_h

#include <deque>

#include "MezzTest.h"

#include "FunctionalTools.h"
#include "SortedVector.h"

/// @file
/// @brief Test for functional tools

/// @brief This is for testing pipes with named functions
int NamedFunctionThatTriples(int Num);
std::vector<int> NamedFunctionThatDoublesVectorOfInt(std::vector<int> Nums);


int NamedFunctionThatTriples(int Num)
    { return 2 * Num; }

std::vector<int> NamedFunctionThatDoublesVectorOfInt(std::vector<int> Nums)
{
    for(auto& Num : Nums)
        { Num *= 2; }
    return Nums;
}

DEFAULT_TEST_GROUP(FunctionalToolsTests,FunctionalTools)
{
    using namespace Mezzanine;
    using namespace Mezzanine::Functional;

    // This meme is floating around r/ProgrammerHumor so I might as well.
    const auto IsEven = [](Integer i) noexcept { return i%2 == 0; };
    const auto IsOdd = [IsEven](Integer i) noexcept { return !IsEven(i); };
    const auto Increment = [](Integer i) noexcept { return i+1; };
    //const auto IsDivisibleByThree = [](Integer i) noexcept { return i%3 == 0; };

    const std::vector<Integer> OneToTen {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const std::vector<Integer> OneToThree {1, 2, 3};
    const std::vector<Integer> ThreeToOne {3, 2, 1};
    const std::vector<Integer> TenToSeven {10, 9, 8, 7};
    const std::vector<Integer> EightToTen {8, 9, 10};
    const std::vector<Integer> OneToTenOnlyEven {2, 4, 6, 8, 10};
    const std::vector<Integer> OneToTenOnlyOdd {1, 3, 5, 7, 9};
    const std::vector<Integer> TwoToEleven {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    const std::vector<Integer> OneToTenEvenThenOdd {2, 4, 6, 8, 10, 1, 3, 5, 7, 9};
    const std::vector<Integer> FourToOne {4, 3, 2, 1};

    const std::list<Integer> ListOneToTenOnlyEven {2, 4, 6, 8, 10};
    const std::set<Integer> SetOneToTenOnlyOdd {1, 3, 5, 7, 9};
    const std::deque<Integer> DequeOneToTenOnlyEven {2, 4, 6, 8, 10};
    const std::set<Integer> SetTenToEleven {10, 11, 12};
    const std::set<Integer> SetSevenToTen {7, 8, 9, 10};
    const String Alphabet{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};

    const std::vector<Integer> OneToTwenty {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    const std::vector<Integer> DivisibleByTwoAndThree {6, 12, 18};

    {
        // Simple tests of collection to collection functors
        TEST("Select<Integer>(IsEven)", OneToTenOnlyEven == Select(OneToTen, IsEven))
        TEST("FilterIsAliasOfSelect<Integer>(IsEven)", OneToTenOnlyEven == Filter(OneToTen, IsEven))
        TEST("Reject<Integer>(IsEven)", OneToTenOnlyOdd == Reject(OneToTen, IsEven))
        TEST("Convert<Integer>(Increment)", OneToTenOnlyEven == Convert(OneToTenOnlyOdd, Increment))
        TEST("MapIsAliasOfConvert<Integer>(Increment)", OneToTenOnlyEven == Map(OneToTenOnlyOdd, Increment))
        TEST("TakeNSimple", OneToThree == TakeN(OneToTen,3))
        TEST("TakeBackNSimple", TenToSeven == TakeBackN(OneToTen,4))
        TEST("TakeWhileSimple", OneToThree == TakeWhile(OneToTen,[](Integer x){ return x <= 3; }) )
        TEST("TakeBackWhileSimple", TenToSeven == TakeBackWhile(OneToTen,[](Integer x){ return x > 6; }) )
        TEST("DropNSimple", EightToTen == DropN(OneToTen,7))
        TEST("DropBackNSimple", FourToOne == DropBackN(OneToTen,6))
        TEST("DropWhile", EightToTen == DropWhile(OneToTen,[](Integer x){ return x < 8; }))
        TEST("DropBackWhile", FourToOne == DropBackWhile(OneToTen,[](Integer x){ return x > 4; }))
        TEST("Reverse", OneToThree == Reverse(ThreeToOne))
        TEST("Sort", OneToThree == Sort(ThreeToOne, std::less<Integer>{}))
        //TEST("SortDataOnly", OneToThree == Sort(ThreeToOne)) // TODO: Support immediate invocation with default pred.


        // Specified Return Types
        TEST("Select<List,Integer>(IsEven)",
             ListOneToTenOnlyEven == Select<std::list<Integer>>(OneToTen, IsEven))
        TEST("Reject<Set,Integer>(IsEven)",
             SetOneToTenOnlyOdd == Reject<std::set<Integer>>(OneToTen, IsEven))
        TEST("Convert<Deque,Integer>(Increment)",
             DequeOneToTenOnlyEven == Convert<std::deque<Integer>>(OneToTenOnlyOdd, Increment))

        TEST("TakeBackN<Set,Integer>(Increment)",
             SetSevenToTen == TakeBackN<std::set<Integer>>(OneToTen, 4))

        // Does it work on Strings?
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
        // AutoCurrying with Predicates on collection to collection functors
        const auto SelectEvenIntoVector = Select<std::vector<Integer>>(IsEven);
        TEST("SelectAutoCurried<vectorConstruction>(IsEven)", OneToTenOnlyEven == SelectEvenIntoVector(OneToTen))

        const auto SelectEven = Select(IsEven);
        TEST("SelectAutoCurriedOnFunction<Implicit>(IsEven)", OneToTenOnlyEven == SelectEven(OneToTen))

        // It should be possible to support auto-currying on data or predicate. Checout Issue #75 & #76
        //const auto SelectFromOneToTen = Select(OneToTen);
        //TEST("SelectAutoCurriedOnData<Implicit>(OneToTen)", OneToTenOnlyEven == SelectFromOneToTen(IsEven))

        const auto RejectEvenIntoVector = Reject<std::vector<Integer>>(IsOdd);
        TEST("RejectAutoCurried<vectorConstruction>(IsOdd)", OneToTenOnlyEven == RejectEvenIntoVector(OneToTen))
        const auto RejectEven = Reject(IsOdd);
        TEST("RejectAutoCurried<Integer>(IsOdd)", OneToTenOnlyEven == RejectEven(OneToTen))

        const auto ConvertIncIntoVector = Convert<std::vector<Integer>>(Increment);
        TEST("ConvertAutoCurried<vectorConstruction>(Increment)", TwoToEleven == ConvertIncIntoVector(OneToTen))
        const auto ConvertIncrement = Convert(Increment);
        TEST("ConvertAutoCurried<Integer>(Increment)", TwoToEleven == ConvertIncrement(OneToTen))

        const auto Take3 = TakeN(3);
        TEST("TakeNAutoCurried<Integer>(3)", OneToThree == Take3(OneToTen))
        const auto TakeBack4 = TakeBackN(4);
        TEST("TakeBackNAutoCurried<Integer>(4)", TenToSeven == TakeBack4(OneToTen))
        const auto TakeWhileLessThan3 = TakeWhile([](Integer x){ return x <= 3; });
        TEST("TakeWhileAutoCurried<Integer>", OneToThree == TakeWhileLessThan3(OneToTen))
        const auto TakeBackWhileGreaterThan6 = TakeBackWhile([](Integer x){ return x > 6; });
        TEST("TakeBackWhileAutoCurried<Integer>", TenToSeven == TakeBackWhileGreaterThan6(OneToTen))

        const auto Drop7 = DropN(7);
        TEST("DropNAutoCurried<Integer>(7)", EightToTen == Drop7(OneToTen))
        const auto DropBack6 = DropBackN(6);
        TEST("DropBackNAutoCurried<Integer>(6)", FourToOne == DropBack6(OneToTen))
        const auto DropWhileLessThanEight = DropWhile([](Integer x){ return x < 8; });
        TEST("DropWhileAutoCurried<Integer>", EightToTen == DropWhileLessThanEight(OneToTen))
        const auto DropBackWhileGreaterThanFour = DropBackWhile([](Integer x){ return x > 4; });
        TEST("DropBackWhileAutoCurried<Integer>", FourToOne == DropBackWhileGreaterThanFour(OneToTen))

        const auto ReverseFunctor = Reverse<std::vector<Integer>>();
        TEST("ReverseAutoCurry", OneToThree == ReverseFunctor(ThreeToOne))

        const auto SortGreatFunctor = Sort(std::greater<Integer>{});
        TEST("SortAutoCurry", ThreeToOne == SortGreatFunctor(ThreeToOne))
    }

    {
        // Simple Tests of collection to data functors
        TEST_EQUAL("Max", 10, Max(OneToTen))
        TEST_EQUAL("Min", 1, Min(OneToTen))
        std::pair<Integer,Integer> MinMaxResults = MinMax(OneToTen);
        TEST_EQUAL("MinMax-Min", 1, MinMaxResults.first)
        TEST_EQUAL("MinMax-Max", 10, MinMaxResults.second)
        TEST_EQUAL("Size", 10u, Size(OneToTen))
        TEST_EQUAL("Length", 5u, Size(OneToTenOnlyEven))

        TEST_EQUAL("FindValue", 3, Find(OneToTen,3).value_or(false))
        TEST_EQUAL("FindPredicate", 2, Find(OneToTen,IsEven).value_or(false))
        TEST_EQUAL("FindValue-Miss", false, Find(OneToTenOnlyOdd,4).value_or(false))
        TEST_EQUAL("FindPredicate-Miss", false, Find(OneToTenOnlyOdd,IsEven).value_or(false))

        TEST_EQUAL("ContainsValue", true, Contains(OneToTen,3))
        TEST_EQUAL("ContainsPredicate", true, Contains(OneToTen,IsEven))
        TEST_EQUAL("ContainsValue-Miss", false, Contains(OneToTenOnlyOdd,4))
        TEST_EQUAL("ContainsPredicate-Miss", false, Contains(OneToTenOnlyOdd,IsEven))

        TEST_EQUAL("IncludesValue", true, Includes(OneToTen,3))
        TEST_EQUAL("IncludesPredicate", true, Includes(OneToTen,IsEven))
        TEST_EQUAL("IncludesValue-Miss", false, Includes(OneToTenOnlyOdd,4))
        TEST_EQUAL("IncludesPredicate-Miss", false, Includes(OneToTenOnlyOdd,IsEven))

        TEST_EQUAL("AllOf", true, AllOf(OneToTenOnlyEven,IsEven))
        TEST_EQUAL("AllOf-Miss", false, AllOf(OneToTen,IsEven))
        TEST_EQUAL("AnyOf", true, AnyOf(OneToTen,IsEven))
        TEST_EQUAL("AnyOf-Miss", false, AnyOf(OneToTenOnlyOdd,IsEven))
        TEST_EQUAL("NoneOf", true, NoneOf(OneToTenOnlyOdd,IsEven))
        TEST_EQUAL("NoneOf-Miss", false, NoneOf(OneToTen,IsEven))

        TEST_EQUAL("IsSorted", true, IsSorted(OneToTenOnlyOdd,std::less<Integer>{}))
        TEST_EQUAL("IsSorted-Miss", false, IsSorted(OneToTen,std::greater<Integer>{}))
        TEST_EQUAL("IsPartitioned", true, IsPartitioned(OneToTenEvenThenOdd,IsEven))
        TEST_EQUAL("IsPartitioned-Miss", false, IsPartitioned(OneToTenEvenThenOdd,IsOdd))

        TEST_EQUAL("Accumulate", 6, Accumulate(OneToThree,std::plus<Integer>{},0))
        TEST_EQUAL("Accumulate<MaxInt>", MaxInt{6}, Accumulate<MaxInt>(OneToThree,std::plus<Integer>{},0))
        TEST_EQUAL("Collect", 6, Collect(OneToThree,std::plus<Integer>{},0))
        TEST_EQUAL("LeftFold", 6, LeftFold(OneToThree,std::plus<Integer>{},0))
        TEST_EQUAL("Reduce", 6, Reduce(OneToThree,std::plus<Integer>{},0))

        TEST_EQUAL("AccumulateBack", 6, AccumulateBack(OneToThree,std::plus<Integer>{},0))
        TEST_EQUAL("AccumulateBack<MaxInt>", MaxInt{6}, AccumulateBack<MaxInt>(OneToThree,std::plus<Integer>{},0))
        TEST_EQUAL("RightFold", 6, RightFold(OneToThree,std::plus<Integer>{},0))

        auto AsStringAppender = [](String l, Integer r) { return l + std::to_string(r); };
        TEST_EQUAL("AccumulateNonCommutative",
                   String("123"),
                   Accumulate(OneToThree, AsStringAppender, String("")))
        TEST_EQUAL("AccumulateBackNonCommutative",
                   String("321"),
                   AccumulateBack(OneToThree, AsStringAppender, String("")))
    }

    {
        // AutoCurrying with Predicates on collection to data functors
        auto ReverseMax = Max(std::greater<Integer>{})
        TEST_EQUAL("MaxAutoCurry", 1, ReverseMax(OneToTen))

        auto ReverseMin = Min(std::greater<Integer>{})
        TEST_EQUAL("MinAutoCurry", 10, ReverseMin(OneToTen))

        auto Bigness = Size();
        TEST_EQUAL("SizeAutoCurry", 10u, Bigness(OneToTen))
        auto Count = Length();
        TEST_EQUAL("LengthAutoCurry", 5u, Count(OneToTenOnlyOdd))

        auto FindEven = Find(IsEven);
        TEST_EQUAL("FindAutoCurry", 2, FindEven(OneToTen).value_or(false))

        auto AllEven = AllOf(IsEven);
        TEST_EQUAL("AllOfAutoCurry", false, AllEven(OneToTen))
        auto AnyEven = AnyOf(IsEven);
        TEST_EQUAL("AnyOfAutoCurry", true, AnyEven(OneToTen))
        auto NoneAreEven = NoneOf(IsEven);
        TEST_EQUAL("NoneOfAutoCurry", false, NoneAreEven(OneToTen))

        auto IsSortedDescending = IsSorted(std::greater<Integer>{});
        TEST_EQUAL("IsSortedAutoCurry", false, IsSortedDescending(OneToTen))

        auto IsOddPartitioned = IsPartitioned(IsOdd);
        TEST_EQUAL("IsPartitionedAutoCurry", false, IsOddPartitioned(OneToTenEvenThenOdd))


        auto Sum = Accumulate(std::plus<Integer>{},0);
        TEST_EQUAL("AccumulateAutoCurry", 6, Sum(OneToThree))
        auto SumMaxInt = Accumulate(std::plus<Integer>{},0);
        TEST_EQUAL("AccumulateAutoCurry<MaxInt>", MaxInt{6}, SumMaxInt(OneToThree))

        auto SumBack = AccumulateBack(std::plus<Integer>{},0);
        TEST_EQUAL("AccumulateBackAutoCurry", 6, SumBack(OneToThree))
        auto SumBackMaxInt = AccumulateBack(std::plus<Integer>{},0);
        TEST_EQUAL("AccumulateBackAutoCurry<MaxInt>", MaxInt{6}, SumBackMaxInt(OneToThree))

    }





    {
        // Normal Currying
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

    {
        // Creating Ranges
        TEST("CreateRangeDefault", OneToTen == CreateRange(1, 10))
        TEST("CreateRangeSet", SetTenToEleven == CreateRange<std::set<Integer>>(10, 12))
        TEST("CreateStringRange", Alphabet == CreateRange<String>('A', 'Z'))
        TEST("CreateEvenRange", OneToTenOnlyEven == CreateRange(2,10,2))
        TEST("CreateRangeCountBySixPastEnd", DivisibleByTwoAndThree == CreateRange(6,20,6))
    }


    SAVE_WARNING_STATE
    SUPPRESS_CLANG_WARNING("-Wctad-maybe-unsupported")
    {
        // Test the basics of pipes.

        Pipe Working{OneToTen};
        TEST("PipeSimpleGetValue", OneToTen == Working.Value())
        TEST("PipeInlineGetValue", OneToTen == Pipe{OneToTen}.Value())

        auto StreamedResults = Pipe{OneToTen} >> Select(IsEven);
        TEST("PipeOperator>>SelectIsEven", OneToTenOnlyEven == StreamedResults.Value())
        //TEST("PipeOperator>>SelectIsEven>>Value", OneToTenOnlyEven == Pipe{OneToTen} >> Select(IsEven) >> Results() )
        TEST("PipeOperator>>SelectIsEven>>Value", OneToTenOnlyEven == Pipe{OneToTen} > Select(IsEven) )
        std::vector<Integer> StreamedResultsStronglyTyped = Pipe{OneToTen} >> Select(IsEven);
        TEST("PipeOperator>>SelectIsEven(StronglyTypedImplicitConversion)",
             OneToTenOnlyEven == StreamedResultsStronglyTyped)
        const std::vector<Integer> ImplicitlyConverted = Pipe{OneToTen} >> Select(IsEven);
        TEST("PipeOperator>>SelectIsEven(AutoImplicitConversion)", OneToTenOnlyEven == ImplicitlyConverted)



        Integer answer = Pipe{OneToTen}
            >> Select(IsEven)
            >> Convert(Increment)
            > Max();
        TEST_EQUAL("BigPipeExample", 11, answer)


    }

    {
        TestLog << "ASDFASDFASDFASDF" << std::endl;
        for(auto i :
            DropWhile(OneToTen,[](Integer x){ return x < 8; })
            )
        {
            TestLog << i << ' ';
        }
        TestLog << std::endl;
    }
    RESTORE_WARNING_STATE
}

#endif
