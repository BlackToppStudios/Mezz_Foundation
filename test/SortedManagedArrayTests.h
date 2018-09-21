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
#ifndef MEZZ_TEST_SortedManagedArray_h
#define MEZZ_TEST_SortedManagedArray_h

/// @file
/// @brief This file tests the functionality of the SortedManagedArray class.

#include "MezzTest.h"

#include "ManagedArray.h"
#include "SortedManagedArray.h"

DEFAULT_TEST_GROUP(SortedManagedArrayTests,SortedManagedArray)
{
    using namespace Mezzanine;

    {//Construction
        SortedManagedArray<int,5> DefaultTest;
        TEST_EQUAL("SortedManagedArray::SortedManagedArray()-ResultSize",size_t(0),DefaultTest.size());
        TEST_EQUAL("SortedManagedArray::SortedManagedArray()-Iterators",DefaultTest.begin(),DefaultTest.end());

        SortedManagedArray<int,10> InitializerTest = { 3, 14, 15, 92, 65, 35 };
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-ResultSize",size_t(6),InitializerTest.size());
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element1",3,InitializerTest[0]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element2",14,InitializerTest[1]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element3",15,InitializerTest[2]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element4",35,InitializerTest[3]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element5",65,InitializerTest[4]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element6",92,InitializerTest[5]);

        SortedManagedArray<int,10,std::less<int>> InitializerTestLess = { 3, 14, 15, 92, 65, 35 };
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-ResultSizeLess",size_t(6),InitializerTestLess.size());
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element1Less",3,InitializerTestLess[0]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element2Less",14,InitializerTestLess[1]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element3Less",15,InitializerTestLess[2]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element4Less",35,InitializerTestLess[3]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element5Less",65,InitializerTestLess[4]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element6Less",92,InitializerTestLess[5]);

        SortedManagedArray<int,10,std::greater<int>> InitializerTestGreater = { 3, 14, 15, 92, 65, 35 };
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-ResultSizeGreater",size_t(6),InitializerTestGreater.size());
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element1Greater",92,InitializerTestGreater[0]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element2Greater",65,InitializerTestGreater[1]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element3Greater",35,InitializerTestGreater[2]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element4Greater",15,InitializerTestGreater[3]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element5Greater",14,InitializerTestGreater[4]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-Element6Greater",3,InitializerTestGreater[5]);

        SortedManagedArray<int,10> CopyTest(InitializerTest);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(const_SortedManagedArray&)-ResultSize",size_t(6),CopyTest.size());
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(const_SortedManagedArray&)-Element1",3,CopyTest[0]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(const_SortedManagedArray&)-Element2",14,CopyTest[1]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(const_SortedManagedArray&)-Element3",15,CopyTest[2]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(const_SortedManagedArray&)-Element4",35,CopyTest[3]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(const_SortedManagedArray&)-Element5",65,CopyTest[4]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(const_SortedManagedArray&)-Element6",92,CopyTest[5]);

        SortedManagedArray<int,15> MoveSourceTest = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
        SortedManagedArray<int,15> MoveDestTest( std::move( MoveSourceTest ) );
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(SortedManagedArray&&)-ResultSize",size_t(10),MoveDestTest.size());
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(SortedManagedArray&&)-Element1",1,MoveDestTest[0]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(SortedManagedArray&&)-Element2",1,MoveDestTest[1]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(SortedManagedArray&&)-Element3",2,MoveDestTest[2]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(SortedManagedArray&&)-Element4",3,MoveDestTest[3]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(SortedManagedArray&&)-Element5",5,MoveDestTest[4]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(SortedManagedArray&&)-Element6",8,MoveDestTest[5]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(SortedManagedArray&&)-Element7",13,MoveDestTest[6]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(SortedManagedArray&&)-Element8",21,MoveDestTest[7]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(SortedManagedArray&&)-Element9",34,MoveDestTest[8]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(SortedManagedArray&&)-Element10",55,MoveDestTest[9]);
    }//Construction End

    {//Operators
        SortedManagedArray<int,12> CopyAssignSource = { 1, 3, 5, 7, 9 };
        SortedManagedArray<int,12> CopyAssignDest = { 0, 0, 0, 0, 0, 0 };
        CopyAssignDest = CopyAssignSource;
        TEST_EQUAL("SortedManagedArray::operator=(const_SortedManagedArray&)-SourceSize",size_t(5),CopyAssignSource.size());
        TEST_EQUAL("SortedManagedArray::operator=(const_SortedManagedArray&)-DestSize",size_t(5),CopyAssignDest.size());
        TEST_EQUAL("SortedManagedArray::operator=(const_SortedManagedArray&)-SourceElement1",1,CopyAssignSource[0]);
        TEST_EQUAL("SortedManagedArray::operator=(const_SortedManagedArray&)-SourceElement2",3,CopyAssignSource[1]);
        TEST_EQUAL("SortedManagedArray::operator=(const_SortedManagedArray&)-SourceElement3",5,CopyAssignSource[2]);
        TEST_EQUAL("SortedManagedArray::operator=(const_SortedManagedArray&)-SourceElement4",7,CopyAssignSource[3]);
        TEST_EQUAL("SortedManagedArray::operator=(const_SortedManagedArray&)-SourceElement5",9,CopyAssignSource[4]);
        TEST_EQUAL("SortedManagedArray::operator=(const_SortedManagedArray&)-DestElement1",1,CopyAssignDest[0]);
        TEST_EQUAL("SortedManagedArray::operator=(const_SortedManagedArray&)-DestElement2",3,CopyAssignDest[1]);
        TEST_EQUAL("SortedManagedArray::operator=(const_SortedManagedArray&)-DestElement3",5,CopyAssignDest[2]);
        TEST_EQUAL("SortedManagedArray::operator=(const_SortedManagedArray&)-DestElement4",7,CopyAssignDest[3]);
        TEST_EQUAL("SortedManagedArray::operator=(const_SortedManagedArray&)-DestElement5",9,CopyAssignDest[4]);

        SortedManagedArray<int,12> MoveAssignSource = { 0, 1, 3, 5, 7, 9 };
        SortedManagedArray<int,12> MoveAssignDest = { 0, 0, 0, 0, 0 };
        MoveAssignDest = std::move( MoveAssignSource );
        TEST_EQUAL("SortedManagedArray::operator=(SortedManagedArray&&)-SourceSize",size_t(0),MoveAssignSource.size());
        TEST_EQUAL("SortedManagedArray::operator=(SortedManagedArray&&)-DestSize",size_t(6),MoveAssignDest.size());
        TEST_EQUAL("SortedManagedArray::operator=(SortedManagedArray&&)-SourceElement1",0,MoveAssignSource[0]);
        TEST_EQUAL("SortedManagedArray::operator=(SortedManagedArray&&)-SourceElement2",1,MoveAssignSource[1]);
        TEST_EQUAL("SortedManagedArray::operator=(SortedManagedArray&&)-SourceElement3",3,MoveAssignSource[2]);
        TEST_EQUAL("SortedManagedArray::operator=(SortedManagedArray&&)-SourceElement4",5,MoveAssignSource[3]);
        TEST_EQUAL("SortedManagedArray::operator=(SortedManagedArray&&)-SourceElement5",7,MoveAssignSource[4]);
        TEST_EQUAL("SortedManagedArray::operator=(SortedManagedArray&&)-SourceElement6",9,MoveAssignSource[5]);
        TEST_EQUAL("SortedManagedArray::operator=(SortedManagedArray&&)-DestElement1",0,MoveAssignDest[0]);
        TEST_EQUAL("SortedManagedArray::operator=(SortedManagedArray&&)-DestElement2",1,MoveAssignDest[1]);
        TEST_EQUAL("SortedManagedArray::operator=(SortedManagedArray&&)-DestElement3",3,MoveAssignDest[2]);
        TEST_EQUAL("SortedManagedArray::operator=(SortedManagedArray&&)-DestElement4",5,MoveAssignDest[3]);
        TEST_EQUAL("SortedManagedArray::operator=(SortedManagedArray&&)-DestElement5",7,MoveAssignDest[4]);
        TEST_EQUAL("SortedManagedArray::operator=(SortedManagedArray&&)-DestElement6",9,MoveAssignDest[5]);
    }//Operators End

    {//Iterators
        SortedManagedArray<int,10> TestArray = { 3, 14, 15, 92, 65, 35, 89, 79, 32, 38 };
        const SortedManagedArray<int,15> ConstTestArray = { 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 120 };

        SortedManagedArray<int,10>::iterator BeginIter = TestArray.begin();
        SortedManagedArray<int,15>::const_iterator ConstBeginIter = ConstTestArray.begin();
        TEST_EQUAL("SortedManagedArray::begin()",3,*BeginIter);
        TEST_EQUAL("SortedManagedArray::begin()_const",1,*ConstBeginIter);

        SortedManagedArray<int,10>::iterator EndIter = TestArray.end();
        SortedManagedArray<int,15>::const_iterator ConstEndIter = ConstTestArray.end();
        TEST_EQUAL("SortedManagedArray::end()",10,EndIter - BeginIter);
        TEST_EQUAL("SortedManagedArray::end()_const",15,ConstEndIter - ConstBeginIter);

        SortedManagedArray<int,10>::reverse_iterator BeginRevIter = TestArray.rbegin();
        SortedManagedArray<int,15>::const_reverse_iterator ConstBeginRevIter = ConstTestArray.rbegin();
        TEST_EQUAL("SortedManagedArray::rbegin()",92,*BeginRevIter);
        TEST_EQUAL("SortedManagedArray::rbegin()_const",120,*ConstBeginRevIter);

        SortedManagedArray<int,10>::reverse_iterator EndRevIter = TestArray.rend();
        SortedManagedArray<int,15>::const_reverse_iterator ConstEndRevIter = ConstTestArray.rend();
        TEST_EQUAL("SortedManagedArray::rend()",10,EndRevIter - BeginRevIter);
        TEST_EQUAL("SortedManagedArray::rend()_const",15,ConstEndRevIter - ConstBeginRevIter);
    }//Iterators End

    {//Query and Access
        SortedManagedArray<int,8> EmptyArray;
        SortedManagedArray<int,16> QueryArray = { 0, 1, 4, 9, 16, 25, 36, 49, 64, 81 };
        const SortedManagedArray<int,24> ConstQueryArray = { 1, 8, 27, 64, 125, 216, 343, 512 };

        TEST_EQUAL("SortedManagedArray::size()_const-Empty",size_t(0),EmptyArray.size());
        TEST_EQUAL("SortedManagedArray::size()_const-Query",size_t(10),QueryArray.size());
        TEST_EQUAL("SortedManagedArray::size()_const-ConstQuery",size_t(8),ConstQueryArray.size());

        TEST_EQUAL("SortedManagedArray::capacity()_const-Empty",size_t(8),EmptyArray.capacity());
        TEST_EQUAL("SortedManagedArray::capacity()_const-Query",size_t(16),QueryArray.capacity());
        TEST_EQUAL("SortedManagedArray::capacity()_const-ConstQuery",size_t(24),ConstQueryArray.capacity());

        TEST_EQUAL("SortedManagedArray::empty()_const-Empty",true,EmptyArray.empty());
        TEST_EQUAL("SortedManagedArray::empty()_const-Query",false,QueryArray.empty());
        TEST_EQUAL("SortedManagedArray::empty()_const-ConstQuery",false,ConstQueryArray.empty());

        TEST_EQUAL("SortedManagedArray::operator[](size_t)-Element1",0,QueryArray[0]);
        TEST_EQUAL("SortedManagedArray::operator[](size_t)-Element3",4,QueryArray[2]);
        TEST_EQUAL("SortedManagedArray::operator[](size_t)-Element5",16,QueryArray[4]);
        TEST_EQUAL("SortedManagedArray::operator[](size_t)-Element7",36,QueryArray[6]);
        TEST_EQUAL("SortedManagedArray::operator[](size_t)-Element9",64,QueryArray[8]);

        TEST_EQUAL("SortedManagedArray::operator[](size_t)_const-Element1",1,ConstQueryArray[0]);
        TEST_EQUAL("SortedManagedArray::operator[](size_t)_const-Element3",27,ConstQueryArray[2]);
        TEST_EQUAL("SortedManagedArray::operator[](size_t)_const-Element5",125,ConstQueryArray[4]);
        TEST_EQUAL("SortedManagedArray::operator[](size_t)_const-Element7",343,ConstQueryArray[6]);

        TEST_EQUAL("SortedManagedArray::at(size_t)-Element2",1,QueryArray[1]);
        TEST_EQUAL("SortedManagedArray::at(size_t)-Element4",9,QueryArray[3]);
        TEST_EQUAL("SortedManagedArray::at(size_t)-Element6",25,QueryArray[5]);
        TEST_EQUAL("SortedManagedArray::at(size_t)-Element8",49,QueryArray[7]);
        TEST_EQUAL("SortedManagedArray::at(size_t)-Element10",81,QueryArray[9]);

        TEST_EQUAL("SortedManagedArray::at(size_t)_const-Element2",8,ConstQueryArray[1]);
        TEST_EQUAL("SortedManagedArray::at(size_t)_const-Element4",64,ConstQueryArray[3]);
        TEST_EQUAL("SortedManagedArray::at(size_t)_const-Element6",216,ConstQueryArray[5]);
        TEST_EQUAL("SortedManagedArray::at(size_t)_const-Element8",512,ConstQueryArray[7]);

        TEST_THROW("SortedManagedArray::at(size_t)-Throw",std::out_of_range,[&QueryArray](){ QueryArray.at(100); });
        TEST_THROW("SortedManagedArray::at(size_t)_const-Throw",std::out_of_range,[&ConstQueryArray](){ ConstQueryArray.at(500); });

        TEST_EQUAL("SortedManagedArray::contains(const_value_type&)_const-Pass",true,QueryArray.contains(36));
        TEST_EQUAL("SortedManagedArray::contains(const_value_type&)_const-Fail",false,QueryArray.contains(50));

        SortedManagedArray<int,16>::iterator Find4 = QueryArray.find(4);
        SortedManagedArray<int,16>::iterator Find9 = QueryArray.find(9);
        SortedManagedArray<int,16>::iterator Find64 = QueryArray.find(64);
        SortedManagedArray<int,16>::iterator FindFail = QueryArray.find(999);
        SortedManagedArray<int,16>::iterator FindEnd = QueryArray.end();
        SortedManagedArray<int,24>::const_iterator ConstFind1 = ConstQueryArray.find(1);
        SortedManagedArray<int,24>::const_iterator ConstFind8 = ConstQueryArray.find(8);
        SortedManagedArray<int,24>::const_iterator ConstFind64 = ConstQueryArray.find(64);
        SortedManagedArray<int,24>::const_iterator ConstFindFail = ConstQueryArray.find(999);
        SortedManagedArray<int,24>::const_iterator ConstFindEnd = ConstQueryArray.end();

        TEST_EQUAL("SortedManagedArray::find()-4",4,*Find4);
        TEST_EQUAL("SortedManagedArray::find()-9",9,*Find9);
        TEST_EQUAL("SortedManagedArray::find()-64",64,*Find64);
        TEST_EQUAL("SortedManagedArray::find()-Fail",FindFail,FindEnd);
        TEST_EQUAL("SortedManagedArray::find()_const-1",1,*ConstFind1);
        TEST_EQUAL("SortedManagedArray::find()_const-8",8,*ConstFind8);
        TEST_EQUAL("SortedManagedArray::find()-const_64",64,*ConstFind64);
        TEST_EQUAL("SortedManagedArray::find()-const_Fail",ConstFindFail,ConstFindEnd);

        SortedManagedArray<int,16>::iterator Findif4 = QueryArray.find_if([](const int Num){ return Num == 4; });
        SortedManagedArray<int,16>::iterator Findif9 = QueryArray.find_if([](const int Num){ return Num == 9; });
        SortedManagedArray<int,16>::iterator Findif64 = QueryArray.find_if([](const int Num){ return Num == 64; });
        SortedManagedArray<int,16>::iterator FindifFail = QueryArray.find_if([](const int Num){ return Num == 999; });
        SortedManagedArray<int,16>::iterator FindifEnd = QueryArray.end();

        TEST_EQUAL("SortedManagedArray::findif()-4",4,*Findif4);
        TEST_EQUAL("SortedManagedArray::findif()-8",9,*Findif9);
        TEST_EQUAL("SortedManagedArray::findif()-64",64,*Findif64);
        TEST_EQUAL("SortedManagedArray::findif()-Fail",FindifFail,FindifEnd);
    }//Query and Access End

    {//Sequence Modifiers
        SortedManagedArray<int,7> ModifierArray = { 1, 10 };

        ModifierArray.add(0);
        ModifierArray.add(5);
        ModifierArray.add(50);

        TEST_EQUAL("SortedManagedArray::add()-size",size_t(5),ModifierArray.size());
        // {0, 1, 5, 10, 50}
        TEST_EQUAL("SortedManagedArray::add()-Element1",0,ModifierArray[0]);
        TEST_EQUAL("SortedManagedArray::add()-Element2",1,ModifierArray[1]);
        TEST_EQUAL("SortedManagedArray::add()-Element3",5,ModifierArray[2]);
        TEST_EQUAL("SortedManagedArray::add()-Element4",10,ModifierArray[3]);
        TEST_EQUAL("SortedManagedArray::add()-Element5",50,ModifierArray[4]);

        std::vector<Whole> AddRangeTest = { 2, 11 };
        ModifierArray.add_range(AddRangeTest.begin(),AddRangeTest.end());

        TEST_EQUAL("SortedManagedArray::add_range()-size",size_t(7),ModifierArray.size());
        // {0, 1, 2, 5, 10, 11, 50}
        TEST_EQUAL("SortedManagedArray::add_range()-Element1",0,ModifierArray[0]);
        TEST_EQUAL("SortedManagedArray::add_range()-Element2",1,ModifierArray[1]);
        TEST_EQUAL("SortedManagedArray::add_range()-Element3",2,ModifierArray[2]);
        TEST_EQUAL("SortedManagedArray::add_range()-Element4",5,ModifierArray[3]);
        TEST_EQUAL("SortedManagedArray::add_range()-Element5",10,ModifierArray[4]);
        TEST_EQUAL("SortedManagedArray::add_range()-Element6",11,ModifierArray[5]);
        TEST_EQUAL("SortedManagedArray::add_range()-Element7",50,ModifierArray[6]);

        TEST_THROW("SortedManagedArray::add()-Throw",
                std::out_of_range,
                [&ModifierArray](){ ModifierArray.add(999); });
        TEST_THROW("SortedManagedArray::add_range()-Throw",
                std::out_of_range,
                [&](){ ModifierArray.add_range(AddRangeTest.begin(),AddRangeTest.end()); });

        ModifierArray.erase(ModifierArray.begin());

        TEST_EQUAL("SortedManagedArray::erase()-size",size_t(6),ModifierArray.size());
        // {1, 2, 5, 10, 11, 50}
        TEST_EQUAL("SortedManagedArray::erase()-FirstElement",1,ModifierArray[0]);
        TEST_EQUAL("SortedManagedArray::erase()-LastElement",50,ModifierArray[5]);

        ModifierArray.erase(ModifierArray.begin()+1,ModifierArray.begin()+3);

        TEST_EQUAL("SortedManagedArray::erase()-Multiple",size_t(4),ModifierArray.size());
        // {1, 10, 11, 50}
        TEST_EQUAL("SortedManagedArray::add_range()-PostEraseElement1",1,ModifierArray[0]);
        TEST_EQUAL("SortedManagedArray::add_range()-PostEraseElement2",10,ModifierArray[1]);
        TEST_EQUAL("SortedManagedArray::add_range()-PostEraseElement3",11,ModifierArray[2]);
        TEST_EQUAL("SortedManagedArray::add_range()-PostEraseElement4",50,ModifierArray[3]);

        ModifierArray.clear();

        TEST_EQUAL("SortedManagedArray::clear()-size",size_t(0),ModifierArray.size());
    }//Sequence Modifiers End

    {
        SortedManagedArray<String,10> StringTest = { "A", "C", "B" };

        TEST_EQUAL("SortedManagedArray::size()-initial",size_t(3),StringTest.size())
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-StringElement1","A",StringTest[0]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-StringElement2","B",StringTest[1]);
        TEST_EQUAL("SortedManagedArray::SortedManagedArray(std::initializer_list<ElementType>)-StringElement3","C",StringTest[2]);

        StringTest.erase(StringTest.begin());
        TEST_EQUAL("SortedManagedArray::size()-StringErase",size_t(2),StringTest.size())
        TEST_EQUAL("SortedManagedArray::erase()-Strings",String("B"),*StringTest.begin());

        StringTest.add("D");
        TEST_EQUAL("SortedManagedArray::size()-StringAdd",size_t(3),StringTest.size())
        TEST_EQUAL("SortedManagedArray::add()-Strings",String("D"),StringTest[2]);

        SortedManagedArray<String,10>::iterator FindC = StringTest.find("C");
        TEST_EQUAL("SortedManagedArray::find()-C",String("C"),*FindC);

        StringTest.clear();
        TEST_EQUAL("SortedManagedArray::clear()-Strings",size_t(0),StringTest.size());
    }
}//sortedmanagedarraytests

#endif