// © Copyright 2010 - 2021 BlackTopp Studios Inc.
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
#ifndef Mezz_Foundation_SortedVectorTests_h
#define Mezz_Foundation_SortedVectorTests_h

/// @file
/// @brief This file tests the functionality of the SortedVector class.

#include "MezzTest.h"

#include "SortedVector.h"

DEFAULT_TEST_GROUP(SortedVectorTests,SortedVector)
{
    using namespace Mezzanine;

    {//Construction
        SortedVector<int> DefaultTest;
        TEST_EQUAL("SortedVector()-ResultSize",size_t(0),DefaultTest.size())
        TEST_EQUAL("SortedVector()-Iterators",0,DefaultTest.end() - DefaultTest.begin())

        SortedVector<int> InitializerTest = { 3, 14, 15, 92, 65, 35 };
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element1",3,InitializerTest[0])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element2",14,InitializerTest[1])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element3",15,InitializerTest[2])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element4",35,InitializerTest[3])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element5",65,InitializerTest[4])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element6",92,InitializerTest[5])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-ResultSize",
                   size_t(6),InitializerTest.size())

        SortedVector<int,std::less<int>> InitializerLess = { 3, 14, 15, 92, 65, 35 };
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element1Less",3,InitializerLess[0])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element2Less",14,InitializerLess[1])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element3Less",15,InitializerLess[2])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element4Less",35,InitializerLess[3])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element5Less",65,InitializerLess[4])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element6Less",92,InitializerLess[5])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-ResultSizeLess",
                   size_t(6),InitializerLess.size())

        SortedVector<int,std::greater<int>> InitializerGreater = { 3, 14, 15, 92, 65, 35 };
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element1Greater",92,InitializerGreater[0])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element2Greater",65,InitializerGreater[1])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element3Greater",35,InitializerGreater[2])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element4Greater",15,InitializerGreater[3])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element5Greater",14,InitializerGreater[4])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-Element6Greater",3,InitializerGreater[5])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-ResultSizeGreater",
                   size_t(6),InitializerGreater.size())

        SortedVector<int> CopyTest(InitializerTest);
        TEST_EQUAL("SortedVector(const_SortedVector&)-ResultSize",size_t(6),CopyTest.size())
        TEST_EQUAL("SortedVector(const_SortedVector&)-Element1",3,CopyTest[0])
        TEST_EQUAL("SortedVector(const_SortedVector&)-Element2",14,CopyTest[1])
        TEST_EQUAL("SortedVector(const_SortedVector&)-Element3",15,CopyTest[2])
        TEST_EQUAL("SortedVector(const_SortedVector&)-Element4",35,CopyTest[3])
        TEST_EQUAL("SortedVector(const_SortedVector&)-Element5",65,CopyTest[4])
        TEST_EQUAL("SortedVector(const_SortedVector&)-Element6",92,CopyTest[5])

        SortedVector<int> MoveSourceTest = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
        SortedVector<int> MoveDestTest( std::move( MoveSourceTest ) );
        TEST_EQUAL("SortedVector(SortedVector&&)-ResultSize",size_t(10),MoveDestTest.size())
        TEST_EQUAL("SortedVector(SortedVector&&)-SourceSize",size_t(0),MoveSourceTest.size())
        TEST_EQUAL("SortedVector(SortedVector&&)-Element1",1,MoveDestTest[0])
        TEST_EQUAL("SortedVector(SortedVector&&)-Element2",1,MoveDestTest[1])
        TEST_EQUAL("SortedVector(SortedVector&&)-Element3",2,MoveDestTest[2])
        TEST_EQUAL("SortedVector(SortedVector&&)-Element4",3,MoveDestTest[3])
        TEST_EQUAL("SortedVector(SortedVector&&)-Element5",5,MoveDestTest[4])
        TEST_EQUAL("SortedVector(SortedVector&&)-Element6",8,MoveDestTest[5])
        TEST_EQUAL("SortedVector(SortedVector&&)-Element7",13,MoveDestTest[6])
        TEST_EQUAL("SortedVector(SortedVector&&)-Element8",21,MoveDestTest[7])
        TEST_EQUAL("SortedVector(SortedVector&&)-Element9",34,MoveDestTest[8])
        TEST_EQUAL("SortedVector(SortedVector&&)-Element10",55,MoveDestTest[9])
    }//Construction End

    {//Operators
        SortedVector<int> CopyAssignSource = { 1, 3, 5, 7, 9 };
        SortedVector<int> CopyAssignDest;
        CopyAssignDest = CopyAssignSource;
        TEST_EQUAL("operator=(const_SortedVector&)-SourceSize",size_t(5),CopyAssignSource.size())
        TEST_EQUAL("operator=(const_SortedVector&)-DestSize",size_t(5),CopyAssignDest.size())
        TEST_EQUAL("operator=(const_SortedVector&)-SourceElement1",1,CopyAssignSource[0])
        TEST_EQUAL("operator=(const_SortedVector&)-SourceElement2",3,CopyAssignSource[1])
        TEST_EQUAL("operator=(const_SortedVector&)-SourceElement3",5,CopyAssignSource[2])
        TEST_EQUAL("operator=(const_SortedVector&)-SourceElement4",7,CopyAssignSource[3])
        TEST_EQUAL("operator=(const_SortedVector&)-SourceElement5",9,CopyAssignSource[4])
        TEST_EQUAL("operator=(const_SortedVector&)-DestElement1",1,CopyAssignDest[0])
        TEST_EQUAL("operator=(const_SortedVector&)-DestElement2",3,CopyAssignDest[1])
        TEST_EQUAL("operator=(const_SortedVector&)-DestElement3",5,CopyAssignDest[2])
        TEST_EQUAL("operator=(const_SortedVector&)-DestElement4",7,CopyAssignDest[3])
        TEST_EQUAL("operator=(const_SortedVector&)-DestElement5",9,CopyAssignDest[4])

        SortedVector<int> MoveAssignSource = { 0, 1, 3, 5, 7, 9 };
        SortedVector<int> MoveAssignDest;
        MoveAssignDest = std::move( MoveAssignSource );
        TEST_EQUAL("operator=(SortedVector&&)-DestSize",size_t(6),MoveAssignDest.size())
        TEST_EQUAL("operator=(SortedVector&&)-DestElement1",0,MoveAssignDest[0])
        TEST_EQUAL("operator=(SortedVector&&)-DestElement2",1,MoveAssignDest[1])
        TEST_EQUAL("operator=(SortedVector&&)-DestElement3",3,MoveAssignDest[2])
        TEST_EQUAL("operator=(SortedVector&&)-DestElement4",5,MoveAssignDest[3])
        TEST_EQUAL("operator=(SortedVector&&)-DestElement5",7,MoveAssignDest[4])
        TEST_EQUAL("operator=(SortedVector&&)-DestElement6",9,MoveAssignDest[5])
        TEST_EQUAL("operator=(SortedVector&&)-SourceSize",size_t(0),MoveAssignSource.size())
        TEST_EQUAL("operator=(SortedVector&&)-SourceEmpty",true,MoveAssignSource.empty())
    }//Operators End

    {//Iterators
        SortedVector<int> TestVector = { 3, 14, 15, 92, 65, 35, 89, 79, 32, 38 };
        const SortedVector<int> ConstTestVector = { 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 120 };

        SortedVector<int>::iterator BeginIter = TestVector.begin();
        SortedVector<int>::const_iterator CBeginIter = TestVector.cbegin();
        SortedVector<int>::const_iterator ConstBeginIter = ConstTestVector.begin();
        TEST_EQUAL("begin()",3,*BeginIter)
        TEST_EQUAL("cbegin()",3,*CBeginIter)
        TEST_EQUAL("begin()_const",1,*ConstBeginIter)

        SortedVector<int>::iterator EndIter = TestVector.end();
        SortedVector<int>::const_iterator CEndIter = TestVector.cend();
        SortedVector<int>::const_iterator ConstEndIter = ConstTestVector.end();
        TEST_EQUAL("end()",10,EndIter - BeginIter)
        TEST_EQUAL("cend()",10,CEndIter - CBeginIter)
        TEST_EQUAL("end()_const",15,ConstEndIter - ConstBeginIter)

        SortedVector<int>::reverse_iterator BeginRevIter = TestVector.rbegin();
        SortedVector<int>::const_reverse_iterator CBeginRevIter = TestVector.crbegin();
        SortedVector<int>::const_reverse_iterator ConstBeginRevIter = ConstTestVector.rbegin();
        TEST_EQUAL("rbegin()",92,*BeginRevIter)
        TEST_EQUAL("crbegin()",92,*CBeginRevIter)
        TEST_EQUAL("rbegin()_const",120,*ConstBeginRevIter)

        SortedVector<int>::reverse_iterator EndRevIter = TestVector.rend();
        SortedVector<int>::const_reverse_iterator CEndRevIter = TestVector.crend();
        SortedVector<int>::const_reverse_iterator ConstEndRevIter = ConstTestVector.rend();
        TEST_EQUAL("rend()",10,EndRevIter - BeginRevIter)
        TEST_EQUAL("crend()",10,CEndRevIter - CBeginRevIter)
        TEST_EQUAL("rend()_const",15,ConstEndRevIter - ConstBeginRevIter)
    }//Iterators End

    {//Query and Access
        SortedVector<int> EmptyVector;
        SortedVector<int> QueryVector = { 0, 1, 4, 9, 16, 25, 36, 49, 64, 81 };
        const SortedVector<int> ConstQueryVector = { 1, 8, 27, 64, 125, 216, 343, 512 };

        TEST_EQUAL("size()_const-Empty",size_t(0),EmptyVector.size())
        TEST_EQUAL("size()_const-Query",size_t(10),QueryVector.size())
        TEST_EQUAL("size()_const-ConstQuery",size_t(8),ConstQueryVector.size())

        TEST_EQUAL("capacity()_const-Query",true,QueryVector.capacity() >= size_t(10))
        TEST_EQUAL("capacity()_const-ConstQuery",true,ConstQueryVector.capacity() >= size_t(8))

        EmptyVector.reserve(31);
        SortedVector<int>::size_type QueryVectorOldCap = QueryVector.capacity();
        QueryVector.reserve(2);

        TEST_EQUAL("reserve()-IncreasedCap",true,EmptyVector.capacity() >= size_t(31))
        TEST_EQUAL("reserve()-NoIncreasedCap",QueryVectorOldCap,QueryVector.capacity())

        TEST_EQUAL("empty()_const-Empty",true,EmptyVector.empty())
        TEST_EQUAL("empty()_const-Query",false,QueryVector.empty())
        TEST_EQUAL("empty()_const-ConstQuery",false,ConstQueryVector.empty())

        TEST_EQUAL("operator[](size_t)-Element1",0,QueryVector[0])
        TEST_EQUAL("operator[](size_t)-Element3",4,QueryVector[2])
        TEST_EQUAL("operator[](size_t)-Element5",16,QueryVector[4])
        TEST_EQUAL("operator[](size_t)-Element7",36,QueryVector[6])
        TEST_EQUAL("operator[](size_t)-Element9",64,QueryVector[8])

        TEST_EQUAL("operator[](size_t)_const-Element1",1,ConstQueryVector[0])
        TEST_EQUAL("operator[](size_t)_const-Element3",27,ConstQueryVector[2])
        TEST_EQUAL("operator[](size_t)_const-Element5",125,ConstQueryVector[4])
        TEST_EQUAL("operator[](size_t)_const-Element7",343,ConstQueryVector[6])

        TEST_EQUAL("at(size_t)-Element2",1,QueryVector[1])
        TEST_EQUAL("at(size_t)-Element4",9,QueryVector[3])
        TEST_EQUAL("at(size_t)-Element6",25,QueryVector[5])
        TEST_EQUAL("at(size_t)-Element8",49,QueryVector[7])
        TEST_EQUAL("at(size_t)-Element10",81,QueryVector[9])

        TEST_EQUAL("at(size_t)_const-Element2",8,ConstQueryVector[1])
        TEST_EQUAL("at(size_t)_const-Element4",64,ConstQueryVector[3])
        TEST_EQUAL("at(size_t)_const-Element6",216,ConstQueryVector[5])
        TEST_EQUAL("at(size_t)_const-Element8",512,ConstQueryVector[7])

        TEST_THROW("at(size_t)-Throw",
                   Mezzanine::Exception::OutOfRange,
                   [&QueryVector](){ QueryVector.at(100); })
        TEST_THROW("at(size_t)_const-Throw",
                   Mezzanine::Exception::OutOfRange,
                   [&ConstQueryVector](){ ConstQueryVector.at(500); })

        TEST_EQUAL("contains(const_value_type&)_const-Pass",true,QueryVector.contains(36))
        TEST_EQUAL("contains(const_value_type&)_const-Fail",false,QueryVector.contains(50))

        SortedVector<int>::iterator Find4 = QueryVector.find(4);
        SortedVector<int>::iterator Find9 = QueryVector.find(9);
        SortedVector<int>::iterator Find64 = QueryVector.find(64);
        SortedVector<int>::iterator FindFail = QueryVector.find(999);
        SortedVector<int>::iterator FindEnd = QueryVector.end();
        SortedVector<int>::const_iterator ConstFind1 = ConstQueryVector.find(1);
        SortedVector<int>::const_iterator ConstFind8 = ConstQueryVector.find(8);
        SortedVector<int>::const_iterator ConstFind64 = ConstQueryVector.find(64);
        SortedVector<int>::const_iterator ConstFindFail = ConstQueryVector.find(999);
        SortedVector<int>::const_iterator ConstFindEnd = ConstQueryVector.end();

        TEST_EQUAL("find()-4",4,*Find4)
        TEST_EQUAL("find()-9",9,*Find9)
        TEST_EQUAL("find()-64",64,*Find64)
        TEST_EQUAL("find()-Fail",0,FindFail - FindEnd)
        TEST_EQUAL("find()_const-1",1,*ConstFind1)
        TEST_EQUAL("find()_const-8",8,*ConstFind8)
        TEST_EQUAL("find()-const_64",64,*ConstFind64)
        TEST_EQUAL("find()-const_Fail",0,ConstFindFail - ConstFindEnd)

        SortedVector<int>::iterator Findif4 = QueryVector.find_if([](const int Num){ return Num == 4; });
        SortedVector<int>::iterator Findif9 = QueryVector.find_if([](const int Num){ return Num == 9; });
        SortedVector<int>::iterator Findif64 = QueryVector.find_if([](const int Num){ return Num == 64; });
        SortedVector<int>::iterator FindifFail = QueryVector.find_if([](const int Num){ return Num == 999; });
        SortedVector<int>::iterator FindifEnd = QueryVector.end();

        TEST_EQUAL("findif()-4",4,*Findif4)
        TEST_EQUAL("findif()-8",9,*Findif9)
        TEST_EQUAL("findif()-64",64,*Findif64)
        TEST_EQUAL("findif()-Fail",0,FindifFail - FindifEnd)
    }//Query and Access End

    {//Sequence Modifiers
        SortedVector<int> ModifierVector = { 1, 10 };

        ModifierVector.add(0);
        ModifierVector.add(5);
        ModifierVector.add(50);

        TEST_EQUAL("add()-size",size_t(5),ModifierVector.size())
        // {0, 1, 5, 10, 50}
        TEST_EQUAL("add()-Element1",0,ModifierVector[0])
        TEST_EQUAL("add()-Element2",1,ModifierVector[1])
        TEST_EQUAL("add()-Element3",5,ModifierVector[2])
        TEST_EQUAL("add()-Element4",10,ModifierVector[3])
        TEST_EQUAL("add()-Element5",50,ModifierVector[4])

        std::vector<int> AddRangeTest = { 11, 2 };
        ModifierVector.add_range(AddRangeTest.begin(),AddRangeTest.end());

        TEST_EQUAL("add_range()-size",size_t(7),ModifierVector.size())
        // {0, 1, 2, 5, 10, 11, 50}
        TEST_EQUAL("add_range()-Element1",0,ModifierVector[0])
        TEST_EQUAL("add_range()-Element2",1,ModifierVector[1])
        TEST_EQUAL("add_range()-Element3",2,ModifierVector[2])
        TEST_EQUAL("add_range()-Element4",5,ModifierVector[3])
        TEST_EQUAL("add_range()-Element5",10,ModifierVector[4])
        TEST_EQUAL("add_range()-Element6",11,ModifierVector[5])
        TEST_EQUAL("add_range()-Element7",50,ModifierVector[6])

        ModifierVector.erase(ModifierVector.begin());

        TEST_EQUAL("erase()-size",size_t(6),ModifierVector.size())
        // {1, 2, 5, 10, 11, 50}
        TEST_EQUAL("erase()-FirstElement",1,ModifierVector[0])
        TEST_EQUAL("erase()-LastElement",50,ModifierVector[5])

        ModifierVector.erase(ModifierVector.begin()+1,ModifierVector.begin()+3);

        TEST_EQUAL("erase()-EraseRange",size_t(4),ModifierVector.size())
        // {1, 10, 11, 50}
        TEST_EQUAL("erase()-PostEraseRangeElement1",1,ModifierVector[0])
        TEST_EQUAL("erase()-PostEraseRangeElement2",10,ModifierVector[1])
        TEST_EQUAL("erase()-PostEraseRangeElement3",11,ModifierVector[2])
        TEST_EQUAL("erase()-PostEraseRangeElement4",50,ModifierVector[3])

        ModifierVector.clear();

        TEST_EQUAL("clear()-size",size_t(0),ModifierVector.size())
    }//Sequence Modifiers End

    {//Alternate Element Types
        SortedVector<String> StringTest = { "A", "C", "B" };

        TEST_EQUAL("size()-StringInitial",size_t(3),StringTest.size())
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-StringElement1","A",StringTest[0])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-StringElement2","B",StringTest[1])
        TEST_EQUAL("SortedVector(std::initializer_list<ElementType>)-StringElement3","C",StringTest[2])

        StringTest.erase(StringTest.begin());
        TEST_EQUAL("size()-StringErase",size_t(2),StringTest.size())
        TEST_EQUAL("erase()-Strings",String("B"),*StringTest.begin())

        StringTest.add("D");
        TEST_EQUAL("size()-StringAdd",size_t(3),StringTest.size())
        TEST_EQUAL("add()-Strings",String("D"),StringTest[2])

        SortedVector<String>::iterator FindC = StringTest.find("C");
        TEST_EQUAL("find()-C",String("C"),*FindC)

        StringTest.clear();
        TEST_EQUAL("clear()-Strings",size_t(0),StringTest.size())
    }//Alternate Element Types End

}

#endif
