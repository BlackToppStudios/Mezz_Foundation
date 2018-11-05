// © Copyright 2010 - 2018 BlackTopp Studios Inc.
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
#ifndef Mezz_Foundation_FlatMapSequenceTests_h
#define Mezz_Foundation_FlatMapSequenceTests_h

#include "MezzTest.h"

#include "FlatMap.h"

/// @file
/// @brief This file tests the sequence modification functionality of the FlatMap class.

DEFAULT_TEST_GROUP(FlatMapSequenceTests,FlatMapSequence)
{
    using namespace Mezzanine;

    using SequenceMapType = FlatMap<int,std::string>;
    using SequenceMapIter = SequenceMapType::iterator;
    using SequenceMapValue = SequenceMapType::value_type;
    using SequenceListType = std::list<SequenceMapValue>;
    using SequenceInitListType = std::initializer_list<SequenceMapValue>;
    using InsertPairResult = std::pair<SequenceMapIter,bool>;

    {// Sequence Modifiers - Copy Insert
        const SequenceMapValue CopyInsertValueOne(1,"One");
        const SequenceMapValue CopyInsertValueTwo(2,"Two");
        const SequenceMapValue CopyInsertValueThree(3,"Three");
        const SequenceMapValue FailCopyInsertValue(3,"Twelve");
        SequenceMapType CopyInsertMap = { CopyInsertValueOne, CopyInsertValueThree };
        TEST_EQUAL( "insert(const_value_type&)-BeforeCount",
                    2u, CopyInsertMap.size() );
        InsertPairResult NewCopyInsertResult = CopyInsertMap.insert(CopyInsertValueTwo);
        InsertPairResult OldCopyInsertResult = CopyInsertMap.insert(FailCopyInsertValue);
        SequenceMapIter CopyInsertBegin = CopyInsertMap.begin();
        TEST_EQUAL( "insert(const_value_type&)-AfterCount",
                    3u, CopyInsertMap.size() );
        TEST_EQUAL( "insert(const_value_type&)-NewResult-First",
                    1, std::distance( CopyInsertBegin, NewCopyInsertResult.first ) );
        TEST_EQUAL( "insert(const_value_type&)-NewResult-Second",
                    true, NewCopyInsertResult.second );
        TEST_EQUAL( "insert(const_value_type&)-OldResult-First",
                    2, std::distance( CopyInsertBegin, OldCopyInsertResult.first ) );
        TEST_EQUAL( "insert(const_value_type&)-OldResult-Second",
                    false, OldCopyInsertResult.second );
        TEST_EQUAL( "insert(const_value_type&)-Element1-First",
                    CopyInsertValueOne.first, CopyInsertBegin->first );
        TEST_EQUAL( "insert(const_value_type&)-Element1-Second",
                    CopyInsertValueOne.second, CopyInsertBegin->second );
        TEST_EQUAL( "insert(const_value_type&)-Element2-First",
                    CopyInsertValueTwo.first, ( CopyInsertBegin + 1 )->first );
        TEST_EQUAL( "insert(const_value_type&)-Element2-Second",
                    CopyInsertValueTwo.second, ( CopyInsertBegin + 1 )->second );
        TEST_EQUAL( "insert(const_value_type&)-Element3-First",
                    CopyInsertValueThree.first, ( CopyInsertBegin + 2 )->first );
        TEST_EQUAL( "insert(const_value_type&)-Element3-Second",
                    CopyInsertValueThree.second, ( CopyInsertBegin + 2 )->second );
    }// Sequence Modifiers - Copy Insert

    {// Sequence Modifiers - Move Insert
        SequenceMapValue MoveInsertValueFour(4,"Four");
        SequenceMapValue MoveInsertValueFive(5,"Five");
        SequenceMapValue MoveInsertValueSix(6,"Six");
        SequenceMapValue FailMoveInsertValue(4,"Sixteen");
        SequenceMapType MoveInsertMap = { MoveInsertValueFour, MoveInsertValueSix };
        TEST_EQUAL( "insert(value_type&&)-BeforeCount",
                    2u, MoveInsertMap.size() );
        InsertPairResult NewMoveInsertResult = MoveInsertMap.insert( std::move(MoveInsertValueFive) );
        InsertPairResult OldMoveInsertResult = MoveInsertMap.insert( std::move(FailMoveInsertValue) );
        SequenceMapIter MoveInsertBegin = MoveInsertMap.begin();
        TEST_EQUAL( "insert(value_type&&)-AfterCount",
                    3u, MoveInsertMap.size() );
        TEST_EQUAL( "insert(value_type&&)-NewResult-First",
                    1, std::distance( MoveInsertBegin, NewMoveInsertResult.first ) );
        TEST_EQUAL( "insert(value_type&&)-NewResult-Second",
                    true, NewMoveInsertResult.second );
        TEST_EQUAL( "insert(value_type&&)-OldResult-First",
                    0, std::distance( MoveInsertBegin, OldMoveInsertResult.first ) );
        TEST_EQUAL( "insert(value_type&&)-OldResult-Second",
                    false, OldMoveInsertResult.second );
        TEST_EQUAL( "insert(value_type&&)-Element1-First",
                    MoveInsertValueFour.first, MoveInsertBegin->first );
        TEST_EQUAL( "insert(value_type&&)-Element1-Second",
                    "Four", MoveInsertBegin->second );
        TEST_EQUAL( "insert(value_type&&)-Element2-First",
                    MoveInsertValueFive.first, ( MoveInsertBegin + 1 )->first );
        TEST_EQUAL( "insert(value_type&&)-Element2-Second",
                    "Five", ( MoveInsertBegin + 1 )->second );
        TEST_EQUAL( "insert(value_type&&)-Element3-First",
                    MoveInsertValueSix.first, ( MoveInsertBegin + 2 )->first );
        TEST_EQUAL( "insert(value_type&&)-Element3-Second",
                    "Six", ( MoveInsertBegin + 2 )->second );
        TEST_EQUAL( "insert(value_type&&)-VerifyMove-New",
                    true, MoveInsertValueFive.second.empty() );
        TEST_EQUAL( "insert(value_type&&)-VerifyMove-Old",
                    false, FailMoveInsertValue.second.empty() );
    }// Sequence Modifiers - Move Insert

    {// Sequence Modifiers - Hint Copy Insert
        SequenceMapType HintCopyInsertMap;
        const SequenceMapValue CopyHintValueOne(1,"Mercury");
        const SequenceMapValue CopyHintValueTwo(2,"Venus");
        const SequenceMapValue CopyHintValueThree(3,"Earth");
        const SequenceMapValue CopyHintValueFour(4,"Mars");
        const SequenceMapValue CopyHintValueFive(5,"Asteroids");
        const SequenceMapValue FailCopyHintValue(3,"Earf");
        SequenceMapIter CopyHint = HintCopyInsertMap.begin();
        SequenceMapIter CopyHintResult = HintCopyInsertMap.insert(CopyHint,CopyHintValueTwo);
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-EmptyInsert-Position",
                    0, std::distance( HintCopyInsertMap.begin(), CopyHintResult ) );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-EmptyInsert-First",
                    CopyHintValueTwo.first, CopyHintResult->first );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-EmptyInsert-Second",
                    CopyHintValueTwo.second, CopyHintResult->second );
        CopyHint = HintCopyInsertMap.begin();
        CopyHintResult = HintCopyInsertMap.insert(CopyHint,CopyHintValueOne);
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-BeginInsert-Position",
                    0, std::distance( HintCopyInsertMap.begin(), CopyHintResult ) );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-BeginInsert-First",
                    CopyHintValueOne.first, CopyHintResult->first );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-BeginInsert-Second",
                    CopyHintValueOne.second, CopyHintResult->second );
        CopyHint = HintCopyInsertMap.end();
        CopyHintResult = HintCopyInsertMap.insert(CopyHint,CopyHintValueFive);
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-EndInsert-Position",
                    2, std::distance( HintCopyInsertMap.begin(), CopyHintResult ) );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-EndInsert-First",
                    CopyHintValueFive.first, CopyHintResult->first );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-EndInsert-Second",
                    CopyHintValueFive.second, CopyHintResult->second );
        CopyHint = HintCopyInsertMap.begin() + 2;
        CopyHintResult = HintCopyInsertMap.insert(CopyHint,CopyHintValueThree);
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-MiddleInsert-Position",
                    2, std::distance( HintCopyInsertMap.begin(), CopyHintResult ) );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-MiddleInsert-First",
                    CopyHintValueThree.first, CopyHintResult->first );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-MiddleInsert-Second",
                    CopyHintValueThree.second, CopyHintResult->second );
        CopyHint = HintCopyInsertMap.begin();
        CopyHintResult = HintCopyInsertMap.insert(CopyHint,CopyHintValueFour);
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-FallBackInsert-Position",
                    3, std::distance( HintCopyInsertMap.begin(), CopyHintResult ) );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-FallBackInsert-First",
                    CopyHintValueFour.first, CopyHintResult->first);
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-FallBackInsert-Second",
                    CopyHintValueFour.second, CopyHintResult->second );
        CopyHint = HintCopyInsertMap.begin() + 2;
        CopyHintResult = HintCopyInsertMap.insert(CopyHint,FailCopyHintValue);
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-FailInsert-Position",
                    2, std::distance( HintCopyInsertMap.begin(), CopyHintResult ) );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-FailInsert-First",
                    CopyHintValueThree.first, CopyHintResult->first );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-FailInsert-Second",
                    CopyHintValueThree.second, CopyHintResult->second );
        // Sequence check the entire container
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-FinalSize",
                    5u, HintCopyInsertMap.size() );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-Element1-First",
                    CopyHintValueOne.first, HintCopyInsertMap.begin()->first );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-Element1-Second",
                    CopyHintValueOne.second, HintCopyInsertMap.begin()->second );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-Element2-First",
                    CopyHintValueTwo.first, ( HintCopyInsertMap.begin() + 1 )->first );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-Element2-Second",
                    CopyHintValueTwo.second, ( HintCopyInsertMap.begin() + 1 )->second );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-Element3-First",
                    CopyHintValueThree.first, ( HintCopyInsertMap.begin() + 2 )->first );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-Element3-Second",
                    CopyHintValueThree.second, ( HintCopyInsertMap.begin() + 2 )->second );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-Element4-First",
                    CopyHintValueFour.first, ( HintCopyInsertMap.begin() + 3 )->first );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-Element4-Second",
                    CopyHintValueFour.second, ( HintCopyInsertMap.begin() + 3 )->second );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-Element5-First",
                    CopyHintValueFive.first, ( HintCopyInsertMap.begin() + 4 )->first );
        TEST_EQUAL( "insert(const_iterator,const_value_type&)-Element5-Second",
                    CopyHintValueFive.second, ( HintCopyInsertMap.begin() + 4 )->second );
    }// Sequence Modifiers - Hint Copy Insert

    {// Sequence Modifiers - Hint Move Insert
        SequenceMapType HintMoveInsertMap;
        SequenceMapValue MoveHintValueFive(5,"Jupiter");
        SequenceMapValue MoveHintValueSix(6,"Saturn");
        SequenceMapValue MoveHintValueSeven(7,"Uranus");
        SequenceMapValue MoveHintValueEight(8,"Neptune");
        SequenceMapValue MoveHintValueNine(9,"Pluto");
        SequenceMapValue FailMoveHintValue(9,"Planet X");
        SequenceMapIter MoveHint = HintMoveInsertMap.begin();
        SequenceMapIter MoveHintResult = HintMoveInsertMap.insert(MoveHint,std::move(MoveHintValueSeven));
        TEST_EQUAL( "insert(const_iterator,value_type&&)-EmptyInsert-Position",
                    0, std::distance( HintMoveInsertMap.begin(), MoveHintResult ) );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-EmptyInsert-First",
                    MoveHintValueSeven.first, MoveHintResult->first );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-EmptyInsert-Second",
                    "Uranus", MoveHintResult->second );
        MoveHint = HintMoveInsertMap.begin();
        MoveHintResult = HintMoveInsertMap.insert(MoveHint,std::move(MoveHintValueFive));
        TEST_EQUAL( "insert(const_iterator,value_type&&)-BeginInsert-Position",
                    0, std::distance( HintMoveInsertMap.begin(), MoveHintResult ) );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-BeginInsert-First",
                    MoveHintValueFive.first, MoveHintResult->first );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-BeginInsert-Second",
                    "Jupiter", MoveHintResult->second );
        MoveHint = HintMoveInsertMap.end();
        MoveHintResult = HintMoveInsertMap.insert(MoveHint,std::move(MoveHintValueEight));
        TEST_EQUAL( "insert(const_iterator,value_type&&)-EndInsert-Position",
                    2, std::distance( HintMoveInsertMap.begin(), MoveHintResult ) );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-EndInsert-First",
                    MoveHintValueEight.first, MoveHintResult->first );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-EndInsert-Second",
                    "Neptune", MoveHintResult->second );
        MoveHint = HintMoveInsertMap.begin() + 1;
        MoveHintResult = HintMoveInsertMap.insert(MoveHint,std::move(MoveHintValueSix));
        TEST_EQUAL( "insert(const_iterator,value_type&&)-MiddleInsert-Position",
                    1, std::distance( HintMoveInsertMap.begin(), MoveHintResult ) );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-MiddleInsert-First",
                    MoveHintValueSix.first, MoveHintResult->first );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-MiddleInsert-Second",
                    "Saturn", MoveHintResult->second );
        MoveHint = HintMoveInsertMap.begin();
        MoveHintResult = HintMoveInsertMap.insert(MoveHint,std::move(MoveHintValueNine));
        TEST_EQUAL( "insert(const_iterator,value_type&&)-FallBackInsert-Position",
                    4, std::distance( HintMoveInsertMap.begin(), MoveHintResult ) );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-FallBackInsert-First",
                    MoveHintValueNine.first, MoveHintResult->first );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-FallBackInsert-Second",
                    "Pluto", MoveHintResult->second );
        MoveHint = HintMoveInsertMap.begin() + 4;
        MoveHintResult = HintMoveInsertMap.insert(MoveHint,std::move(FailMoveHintValue));
        TEST_EQUAL( "insert(const_iterator,value_type&&)-FailInsert-Position",
                    4, std::distance( HintMoveInsertMap.begin(), MoveHintResult ) );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-FailInsert-First",
                    MoveHintValueNine.first, MoveHintResult->first );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-FailInsert-Second",
                    "Pluto", MoveHintResult->second );
        // Sequence check the entire container
        TEST_EQUAL( "insert(const_iterator,value_type&&)-FinalSize",
                    5u, HintMoveInsertMap.size() );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-Element1-First",
                    MoveHintValueFive.first, HintMoveInsertMap.begin()->first );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-Element1-Second",
                    "Jupiter",HintMoveInsertMap.begin()->second );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-Element2-First",
                    MoveHintValueSix.first, ( HintMoveInsertMap.begin() + 1 )->first );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-Element2-Second",
                    "Saturn", ( HintMoveInsertMap.begin() + 1 )->second );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-Element3-First",
                    MoveHintValueSeven.first, ( HintMoveInsertMap.begin() + 2 )->first );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-Element3-Second",
                    "Uranus", ( HintMoveInsertMap.begin() + 2 )->second );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-Element4-First",
                    MoveHintValueEight.first, ( HintMoveInsertMap.begin() + 3 )->first );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-Element4-Second",
                    "Neptune", ( HintMoveInsertMap.begin() + 3 )->second );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-Element5-First",
                    MoveHintValueNine.first, ( HintMoveInsertMap.begin() + 4 )->first );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-Element5-Second",
                    "Pluto", ( HintMoveInsertMap.begin() + 4 )->second );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-VerifyMove-First",
                    true, MoveHintValueFive.second.empty() );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-VerifyMove-Second",
                    true, MoveHintValueSix.second.empty() );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-VerifyMove-Third",
                    true, MoveHintValueSeven.second.empty() );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-VerifyMove-Fourth",
                    true, MoveHintValueEight.second.empty() );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-VerifyMove-Fifth",
                    true, MoveHintValueNine.second.empty() );
        TEST_EQUAL( "insert(const_iterator,value_type&&)-VerifyMove-Fail",
                    false, FailMoveHintValue.second.empty() );
    }// Sequence Modifiers - Hint Move Insert

    {// Sequence Modifiers - Range Insert
        const SequenceMapValue RangeInsertValueOne(1,"Eins");
        const SequenceMapValue RangeInsertValueTwo(2,"Zwei");
        const SequenceMapValue RangeInsertValueFive(5,"Funf");
        const SequenceMapValue RangeInsertValueSix(6,"Sechs");
        const SequenceMapValue RangeInsertValueSeven(7,"Seiben");
        const SequenceMapValue RangeInsertValueNine(9,"Neun");
        SequenceListType RangeInsertList = { RangeInsertValueTwo, RangeInsertValueSix, RangeInsertValueSeven };
        SequenceMapType RangeInsertMap = { RangeInsertValueOne, RangeInsertValueFive, RangeInsertValueNine };
        TEST_EQUAL( "insert(ItType,ItType)-BeforeCount",
                    3u, RangeInsertMap.size() );
        RangeInsertMap.insert(RangeInsertList.begin(),RangeInsertList.end());
        TEST_EQUAL( "insert(ItType,ItType)-AfterCount",
                    6u, RangeInsertMap.size() );
        TEST_EQUAL( "insert(ItType,ItType)-Element1-First",
                    RangeInsertValueOne.first, RangeInsertMap.begin()->first );
        TEST_EQUAL( "insert(ItType,ItType)-Element1-Second",
                    RangeInsertValueOne.second, RangeInsertMap.begin()->second );
        TEST_EQUAL( "insert(ItType,ItType)-Element2-First",
                    RangeInsertValueTwo.first, ( RangeInsertMap.begin() + 1 )->first );
        TEST_EQUAL( "insert(ItType,ItType)-Element2-Second",
                    RangeInsertValueTwo.second, ( RangeInsertMap.begin() + 1 )->second );
        TEST_EQUAL( "insert(ItType,ItType)-Element3-First",
                    RangeInsertValueFive.first, ( RangeInsertMap.begin() + 2 )->first );
        TEST_EQUAL( "insert(ItType,ItType)-Element3-Second",
                    RangeInsertValueFive.second, ( RangeInsertMap.begin() + 2 )->second );
        TEST_EQUAL( "insert(ItType,ItType)-Element4-First",
                    RangeInsertValueSix.first, ( RangeInsertMap.begin() + 3 )->first );
        TEST_EQUAL( "insert(ItType,ItType)-Element4-Second",
                    RangeInsertValueSix.second, ( RangeInsertMap.begin() + 3 )->second );
        TEST_EQUAL( "insert(ItType,ItType)-Element5-First",
                    RangeInsertValueSeven.first, ( RangeInsertMap.begin() + 4 )->first );
        TEST_EQUAL( "insert(ItType,ItType)-Element5-Second",
                    RangeInsertValueSeven.second, ( RangeInsertMap.begin() + 4 )->second );
        TEST_EQUAL( "insert(ItType,ItType)-Element6-First",
                    RangeInsertValueNine.first, ( RangeInsertMap.begin() + 5 )->first );
        TEST_EQUAL( "insert(ItType,ItType)-Element6-Second",
                    RangeInsertValueNine.second, ( RangeInsertMap.begin() + 5 )->second );
    }// Sequence Modifiers - Range Insert

    {// Sequence Modifiers - Initialization List Insert
        const SequenceMapValue InitListInsertValueOne(48227,"Naiad");
        const SequenceMapValue InitListInsertValueTwo(50074,"Thalassa");
        const SequenceMapValue InitListInsertValueThree(52526,"Despina");
        const SequenceMapValue InitListInsertValueFour(61953,"Galatea");
        const SequenceMapValue InitListInsertValueFive(73548,"Larissa");
        const SequenceMapValue InitListInsertValueSix(117646,"Proteus");
        const SequenceMapValue InitListInsertValueSeven(354759,"Triton");
        const SequenceMapValue InitListInsertValueEight(5513818,"Nereid");
        SequenceMapType InitListInsertMap = { InitListInsertValueTwo,
                                              InitListInsertValueSix };
        SequenceInitListType InsertInitList = { InitListInsertValueFive,
                                                InitListInsertValueEight,
                                                InitListInsertValueFour };
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-ConstructCount",
                    2u, InitListInsertMap.size() );
        InitListInsertMap.insert( InsertInitList );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-ExplicitInsertCount",
                    5u, InitListInsertMap.size() );
        InitListInsertMap.insert( { InitListInsertValueThree,
                                    InitListInsertValueOne,
                                    InitListInsertValueSeven } );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-ImplicitInsertCount",
                    8u, InitListInsertMap.size() );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element1-First",
                    InitListInsertValueOne.first, InitListInsertMap.begin()->first );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element1-Second",
                    InitListInsertValueOne.second, InitListInsertMap.begin()->second );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element2-First",
                    InitListInsertValueTwo.first, ( InitListInsertMap.begin() + 1 )->first );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element2-Second",
                    InitListInsertValueTwo.second, ( InitListInsertMap.begin() + 1 )->second );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element3-First",
                    InitListInsertValueThree.first, ( InitListInsertMap.begin() + 2 )->first );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element3-Second",
                    InitListInsertValueThree.second, ( InitListInsertMap.begin() + 2 )->second );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element4-First",
                    InitListInsertValueFour.first, ( InitListInsertMap.begin() + 3 )->first );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element4-Second",
                    InitListInsertValueFour.second, ( InitListInsertMap.begin() + 3 )->second );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element5-First",
                    InitListInsertValueFive.first, ( InitListInsertMap.begin() + 4 )->first );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element5-Second",
                    InitListInsertValueFive.second, ( InitListInsertMap.begin() + 4 )->second );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element6-First",
                    InitListInsertValueSix.first, ( InitListInsertMap.begin() + 5 )->first );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element6-Second",
                    InitListInsertValueSix.second, ( InitListInsertMap.begin() + 5 )->second );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element7-First",
                    InitListInsertValueSeven.first, ( InitListInsertMap.begin() + 6 )->first );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element7-Second",
                    InitListInsertValueSeven.second, ( InitListInsertMap.begin() + 6 )->second );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element8-First",
                    InitListInsertValueEight.first, ( InitListInsertMap.begin() + 7 )->first );
        TEST_EQUAL( "insert(std::initializer_list<value_type>)-Element8-Second",
                    InitListInsertValueEight.second, ( InitListInsertMap.begin() + 7 )->second );
    }// Sequence Modifiers - Initialization List Insert

    {// Sequence Modifiers - Emplace
        const SequenceMapValue EmplaceValueSeven(7,"Seven");
        const SequenceMapValue EmplaceValueEight(8,"Eight");
        const SequenceMapValue EmplaceValueNine(9,"Nine");
        const SequenceMapValue FailEmplaceValue(7,"Fourteen");
        SequenceMapType EmplaceMap = { EmplaceValueSeven, EmplaceValueNine };
        TEST_EQUAL( "emplace(ArgTypes&&...)-BeforeCount",
                    2u, EmplaceMap.size() );
        InsertPairResult NewEmplaceResult = EmplaceMap.emplace(EmplaceValueEight.first,EmplaceValueEight.second);
        InsertPairResult OldEmplaceResult = EmplaceMap.emplace(FailEmplaceValue.first,FailEmplaceValue.second);
        TEST_EQUAL( "emplace(ArgTypes&&...)-AfterCount",
                    3u, EmplaceMap.size() );
        TEST_EQUAL( "emplace(ArgTypes&&...)-NewResult-First",
                    1, std::distance( EmplaceMap.begin(), NewEmplaceResult.first ) );
        TEST_EQUAL( "emplace(ArgTypes&&...)-NewResult-Second",
                    true, NewEmplaceResult.second );
        TEST_EQUAL( "emplace(ArgTypes&&...)-OldResult-First",
                    0, std::distance( EmplaceMap.begin(), OldEmplaceResult.first ) );
        TEST_EQUAL( "emplace(ArgTypes&&...)-OldResult-Second",
                    false, OldEmplaceResult.second );
        TEST_EQUAL( "emplace(ArgTypes&&...)-Element1-First",
                    EmplaceValueSeven.first, EmplaceMap.begin()->first );
        TEST_EQUAL( "emplace(ArgTypes&&...)-Element1-Second",
                    EmplaceValueSeven.second, EmplaceMap.begin()->second );
        TEST_EQUAL( "emplace(ArgTypes&&...)-Element2-First",
                    EmplaceValueEight.first, ( EmplaceMap.begin() + 1 )->first );
        TEST_EQUAL( "emplace(ArgTypes&&...)-Element2-Second",
                    EmplaceValueEight.second, ( EmplaceMap.begin() + 1 )->second );
        TEST_EQUAL( "emplace(ArgTypes&&...)-Element3-First",
                    EmplaceValueNine.first, ( EmplaceMap.begin() + 2 )->first );
        TEST_EQUAL( "emplace(ArgTypes&&...)-Element3-Second",
                    EmplaceValueNine.second, ( EmplaceMap.begin() + 2 )->second );
    }// Sequence Modifiers - Emplace

    {// Sequence Modifiers - Hint Emplace
        SequenceMapType HintEmplaceMap;
        //SequenceMapValue FirstEmplaceHintValue(129390,"Miranda");
        //SequenceMapValue SecondEmplaceHintValue(191020,"Ariel");
        //SequenceMapValue ThirdEmplaceHintValue(266300,"Umbriel");
        //SequenceMapValue FourthEmplaceHintValue(435910,"Titania");
        //SequenceMapValue FifthEmplaceHintValue(583520,"Oberon");
        //SequenceMapValue FailEmplaceHintValue(129390,"Earf");
        SequenceMapIter EmplaceHint = HintEmplaceMap.begin();
        SequenceMapIter EmplaceHintResult = HintEmplaceMap.emplace_hint(EmplaceHint,191020,"Ariel");
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-EmptyInsert-Position",
                    0, std::distance( HintEmplaceMap.begin(), EmplaceHintResult ) );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-EmptyInsert-First",
                    191020, EmplaceHintResult->first );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-EmptyInsert-Second",
                    "Ariel", EmplaceHintResult->second );
        EmplaceHint = HintEmplaceMap.begin();
        EmplaceHintResult = HintEmplaceMap.emplace_hint(EmplaceHint,129390,"Miranda");
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-BeginInsert-Position",
                    0, std::distance( HintEmplaceMap.begin(), EmplaceHintResult ) );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-BeginInsert-First",
                    129390, EmplaceHintResult->first );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-BeginInsert-Second",
                    "Miranda", EmplaceHintResult->second );
        EmplaceHint = HintEmplaceMap.end();
        EmplaceHintResult = HintEmplaceMap.emplace_hint(EmplaceHint,583520,"Oberon");
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-EndInsert-Position",
                    2, std::distance( HintEmplaceMap.begin(), EmplaceHintResult ) );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-EndInsert-First",
                    583520, EmplaceHintResult->first );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-EndInsert-Second",
                    "Oberon", EmplaceHintResult->second );
        EmplaceHint = HintEmplaceMap.begin() + 2;
        EmplaceHintResult = HintEmplaceMap.emplace_hint(EmplaceHint,266300,"Umbriel");
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-MiddleInsert-Position",
                    2, std::distance( HintEmplaceMap.begin(), EmplaceHintResult ) );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-MiddleInsert-First",
                    266300, EmplaceHintResult->first );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-MiddleInsert-Second",
                    "Umbriel", EmplaceHintResult->second );
        EmplaceHint = HintEmplaceMap.begin();
        EmplaceHintResult = HintEmplaceMap.emplace_hint(EmplaceHint,435910,"Titania");
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-FallBackInsert-Position",
                    3, std::distance( HintEmplaceMap.begin(), EmplaceHintResult ) );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-FallBackInsert-First",
                    435910, EmplaceHintResult->first );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-FallBackInsert-Second",
                    "Titania", EmplaceHintResult->second );
        EmplaceHint = HintEmplaceMap.begin() + 2;
        EmplaceHintResult = HintEmplaceMap.emplace_hint(EmplaceHint,129390,"Earf");
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-FailInsert-Position",
                    0, std::distance( HintEmplaceMap.begin(), EmplaceHintResult ) );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-FailInsert-First",
                    129390, EmplaceHintResult->first );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-FailInsert-Second",
                    "Miranda", EmplaceHintResult->second );
        // Sequence check the entire container
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-FinalSize",
                    5u, HintEmplaceMap.size() );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-Element1-First",
                    129390, HintEmplaceMap.begin()->first );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-Element1-Second",
                    "Miranda", HintEmplaceMap.begin()->second );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-Element2-First",
                    191020, ( HintEmplaceMap.begin() + 1 )->first );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-Element2-Second",
                    "Ariel", ( HintEmplaceMap.begin() + 1 )->second );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-Element3-First",
                    266300, ( HintEmplaceMap.begin() + 2 )->first );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-Element3-Second",
                    "Umbriel", ( HintEmplaceMap.begin() + 2 )->second );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-Element4-First",
                    435910, ( HintEmplaceMap.begin() + 3 )->first );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-Element4-Second",
                    "Titania", ( HintEmplaceMap.begin() + 3 )->second );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-Element5-First",
                    583520, ( HintEmplaceMap.begin() + 4 )->first );
        TEST_EQUAL( "emplace_hint(const_iterator,ArgTypes&&...)-Element5-Second",
                    "Oberon", ( HintEmplaceMap.begin() + 4 )->second );
    }// Sequence Modifiers - Hint Emplace

    {// Sequence Modifiers - Swap
        const SequenceMapValue SwapValueTen(10,"Ten");
        const SequenceMapValue SwapValueTwenty(20,"Twenty");
        const SequenceMapValue SwapValueThirty(30,"Thirty");
        const SequenceMapValue SwapValueFourty(40,"Forty");
        const SequenceMapValue SwapValueFifty(50,"Fifty");
        SequenceMapType SwapSrcMap = { SwapValueTen, SwapValueTwenty, SwapValueThirty };
        SequenceMapType SwapDestMap = { SwapValueFourty, SwapValueFifty };
        SwapDestMap.swap(SwapSrcMap);
        TEST_EQUAL( "swap(SelfType&)-SrcCount",
                    2u, SwapSrcMap.size() );
        TEST_EQUAL( "swap(SelfType&)-DestCount",
                    3u, SwapDestMap.size() );
        TEST_EQUAL( "swap(SelfType&)-SrcMap-Element1-First",
                    SwapValueFourty.first, SwapSrcMap.begin()->first );
        TEST_EQUAL( "swap(SelfType&)-SrcMap-Element1-Second",
                    SwapValueFourty.second, SwapSrcMap.begin()->second );
        TEST_EQUAL( "swap(SelfType&)-SrcMap-Element2-First",
                    SwapValueFifty.first, ( SwapSrcMap.begin() + 1 )->first );
        TEST_EQUAL( "swap(SelfType&)-SrcMap-Element2-Second",
                    SwapValueFifty.second, ( SwapSrcMap.begin() + 1 )->second );
        TEST_EQUAL( "swap(SelfType&)-DestMap-Element1-First",
                    SwapValueTen.first, SwapDestMap.begin()->first );
        TEST_EQUAL( "swap(SelfType&)-DestMap-Element1-Second",
                    SwapValueTen.second, SwapDestMap.begin()->second );
        TEST_EQUAL( "swap(SelfType&)-DestMap-Element2-First",
                    SwapValueTwenty.first, ( SwapDestMap.begin() + 1 )->first );
        TEST_EQUAL( "swap(SelfType&)-DestMap-Element2-Second",
                    SwapValueTwenty.second, ( SwapDestMap.begin() + 1 )->second );
        TEST_EQUAL( "swap(SelfType&)-DestMap-Element3-First",
                    SwapValueThirty.first, ( SwapDestMap.begin() + 2 )->first  );
        TEST_EQUAL( "swap(SelfType&)-DestMap-Element3-Second",
                    SwapValueThirty.second, ( SwapDestMap.begin() + 2 )->second );
    }// Sequence Modifiers - Swap

    {// Sequence Modifiers - Erase
        const SequenceMapValue EraseValueOne(1,"Uno");
        const SequenceMapValue EraseValueTwo(2,"Dos");
        const SequenceMapValue EraseValueThree(3,"Tres");
        const SequenceMapValue EraseValueFour(4,"Cuatro");
        SequenceMapType EraseMap = { EraseValueFour, EraseValueThree, EraseValueTwo, EraseValueOne };
        TEST_EQUAL( "erase((const_)iterator)-BeforeSize",
                    4u, EraseMap.size() );
        SequenceMapType::iterator FirstErase = EraseMap.begin() + 1;
        SequenceMapType::iterator FirstEraseResult = EraseMap.erase(FirstErase);
        TEST_EQUAL( "erase((const_)iterator)-AfterFirstEraseSize",
                    3u, EraseMap.size() );
        TEST_EQUAL( "erase((const_)iterator)-FirstEraseResultPosition",
                    1, std::distance( EraseMap.begin(), FirstEraseResult ) );
        SequenceMapType::const_iterator SecondErase = EraseMap.begin() + 2;
        SequenceMapType::const_iterator SecondEraseResult = EraseMap.erase(SecondErase);
        TEST_EQUAL( "erase((const_)iterator)-AfterSecondEraseSize",
                    2u, EraseMap.size() );
        TEST_EQUAL( "erase((const_)iterator)-SecondEraseResultPosition",
                    2, std::distance( EraseMap.cbegin(), SecondEraseResult ) );
        TEST_EQUAL( "erase((const_)iterator)-Element1-First",
                    EraseValueOne.first, EraseMap.begin()->first );
        TEST_EQUAL( "erase((const_)iterator)-Element1-Second",
                    EraseValueOne.second, EraseMap.begin()->second );
        TEST_EQUAL( "erase((const_)iterator)-Element2-First",
                    EraseValueThree.first, ( EraseMap.begin() + 1 )->first );
        TEST_EQUAL( "erase((const_)iterator)-Element2-Second",
                    EraseValueThree.second, ( EraseMap.begin() + 1 )->second );
    }// Sequence Modifiers - Erase

    {// Sequence Modifiers - RangeErase
        const SequenceMapValue RangeEraseValueFifteen(15,"30");
        const SequenceMapValue RangeEraseValueThirty(30,"60");
        const SequenceMapValue RangeEraseValueFourtyFive(45,"90");
        const SequenceMapValue RangeEraseValueSixty(60,"120");
        const SequenceMapValue RangeEraseValueSeventyFive(75,"150");
        const SequenceMapValue RangeEraseValueNinty(90,"180");
        SequenceMapType RangeEraseMap = { RangeEraseValueFifteen,
                                          RangeEraseValueThirty,
                                          RangeEraseValueFourtyFive,
                                          RangeEraseValueSixty,
                                          RangeEraseValueSeventyFive,
                                          RangeEraseValueNinty };
        SequenceMapIter RangeEraseBegin = RangeEraseMap.begin();
        TEST_EQUAL( "erase(const_iterator,const_iterator)-BeforeCount",
                    6u, RangeEraseMap.size() );
        SequenceMapIter RangeRetIt = RangeEraseMap.erase(RangeEraseBegin + 2,RangeEraseBegin + 5);
        TEST_EQUAL( "erase(const_iterator,const_iterator)-AfterCount",
                    3u, RangeEraseMap.size() );
        TEST_EQUAL( "erase(const_iterator,const_iterator)-ReturnPosition",
                    2, std::distance( RangeEraseMap.begin(), RangeRetIt ) );
        TEST_EQUAL( "erase(const_iterator,const_iterator)-Element1-First",
                    RangeEraseValueFifteen.first, RangeEraseMap.begin()->first );
        TEST_EQUAL( "erase(const_iterator,const_iterator)-Element1-Second",
                    RangeEraseValueFifteen.second, RangeEraseMap.begin()->second );
        TEST_EQUAL( "erase(const_iterator,const_iterator)-Element2-First",
                    RangeEraseValueThirty.first, ( RangeEraseMap.begin() + 1 )->first );
        TEST_EQUAL( "erase(const_iterator,const_iterator)-Element2-Second",
                    RangeEraseValueThirty.second, ( RangeEraseMap.begin() + 1 )->second );
        TEST_EQUAL( "erase(const_iterator,const_iterator)-Element3-First",
                    RangeEraseValueNinty.first, ( RangeEraseMap.begin() + 2 )->first );
        TEST_EQUAL( "erase(const_iterator,const_iterator)-Element3-Second",
                    RangeEraseValueNinty.second, ( RangeEraseMap.begin() + 2 )->second );
    }// Sequence Modifiers - RangeErase

    {// Sequence Modifiers - Key Erase
        const SequenceMapValue KeyEraseValueFive(5,"Cinq");
        const SequenceMapValue KeyEraseValueSix(6,"Six");
        const SequenceMapValue KeyEraseValueSeven(7,"Sept");
        SequenceMapType KeyEraseMap = { KeyEraseValueSeven, KeyEraseValueSix, KeyEraseValueFive };
        TEST_EQUAL( "erase(const_key_type&)-BeforeCount",
                    3u, KeyEraseMap.size() );
        TEST_EQUAL( "erase(const_key_type&)-Pass",
                    1u, KeyEraseMap.erase(KeyEraseValueSix.first) );
        TEST_EQUAL( "erase(const_key_type&)-AfterCount",
                    2u, KeyEraseMap.size() );
        TEST_EQUAL( "erase(const_key_type&)-Fail",
                    0u, KeyEraseMap.erase(9) );
        TEST_EQUAL( "erase(const_key_type&)-Element1-First",
                    KeyEraseValueFive.first, KeyEraseMap.begin()->first );
        TEST_EQUAL( "erase(const_key_type&)-Element1-Second",
                    KeyEraseValueFive.second, KeyEraseMap.begin()->second );
        TEST_EQUAL( "erase(const_key_type&)-Element2-First",
                    KeyEraseValueSeven.first, ( KeyEraseMap.begin() + 1 )->first );
        TEST_EQUAL( "erase(const_key_type&)-Element2-Second",
                    KeyEraseValueSeven.second, ( KeyEraseMap.begin() + 1 )->second );
    }// Sequence Modifiers - Key Erase

    {// Sequence Modifiers - Clear
        SequenceMapType ClearMap = { {4,"Quatre"}, {3,"Trois"}, {2,"Deux"}, {1,"Un"} };
        TEST_EQUAL( "clear()-BeforeCount", 4u, ClearMap.size() );
        ClearMap.clear();
        TEST_EQUAL( "clear()-Empty", true, ClearMap.empty() );
    }// Sequence Modifiers - Clear
}

#endif
