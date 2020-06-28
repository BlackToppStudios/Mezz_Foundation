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
#ifndef Mezz_Foundation_ManagedArrayTests_h
#define Mezz_Foundation_ManagedArrayTests_h

/// @file
/// @brief This file tests the functionality of the ManagedArray class. Focused on Basic operations.

#include "MezzTest.h"

#include "MezzException.h"
#include "ManagedArray.h"

DEFAULT_TEST_GROUP(ManagedArrayTests,ManagedArray)
{
    using namespace Mezzanine;

    {//Construction
        ManagedArray<int,5> DefaultTest;
        TEST_EQUAL("ManagedArray()-ResultSize",size_t(0),DefaultTest.size());
        TEST_EQUAL("ManagedArray()-Iterators",DefaultTest.begin(),DefaultTest.end());

        ManagedArray<int,10> InitializerTest = { 3, 14, 15, 92, 65, 35 };
        TEST_EQUAL("ManagedArray(std::initializer_list<ElementType>)-ResultSize",size_t(6),InitializerTest.size());
        TEST_EQUAL("ManagedArray(std::initializer_list<ElementType>)-Element1",3,InitializerTest[0]);
        TEST_EQUAL("ManagedArray(std::initializer_list<ElementType>)-Element2",14,InitializerTest[1]);
        TEST_EQUAL("ManagedArray(std::initializer_list<ElementType>)-Element3",15,InitializerTest[2]);
        TEST_EQUAL("ManagedArray(std::initializer_list<ElementType>)-Element4",92,InitializerTest[3]);
        TEST_EQUAL("ManagedArray(std::initializer_list<ElementType>)-Element5",65,InitializerTest[4]);
        TEST_EQUAL("ManagedArray(std::initializer_list<ElementType>)-Element6",35,InitializerTest[5]);

        ManagedArray<int,10> CopyTest(InitializerTest);
        TEST_EQUAL("ManagedArray(const_ManagedArray&)-ResultSize",size_t(6),CopyTest.size());
        TEST_EQUAL("ManagedArray(const_ManagedArray&)-Element1",3,CopyTest[0]);
        TEST_EQUAL("ManagedArray(const_ManagedArray&)-Element2",14,CopyTest[1]);
        TEST_EQUAL("ManagedArray(const_ManagedArray&)-Element3",15,CopyTest[2]);
        TEST_EQUAL("ManagedArray(const_ManagedArray&)-Element4",92,CopyTest[3]);
        TEST_EQUAL("ManagedArray(const_ManagedArray&)-Element5",65,CopyTest[4]);
        TEST_EQUAL("ManagedArray(const_ManagedArray&)-Element6",35,CopyTest[5]);

        ManagedArray<int,15> MoveSourceTest = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
        ManagedArray<int,15> MoveDestTest( std::move( MoveSourceTest ) );
        TEST_EQUAL("ManagedArray(ManagedArray&&)-ResultSize",size_t(10),MoveDestTest.size());
        TEST_EQUAL("ManagedArray(ManagedArray&&)-Element1",1,MoveDestTest[0]);
        TEST_EQUAL("ManagedArray(ManagedArray&&)-Element2",1,MoveDestTest[1]);
        TEST_EQUAL("ManagedArray(ManagedArray&&)-Element3",2,MoveDestTest[2]);
        TEST_EQUAL("ManagedArray(ManagedArray&&)-Element4",3,MoveDestTest[3]);
        TEST_EQUAL("ManagedArray(ManagedArray&&)-Element5",5,MoveDestTest[4]);
        TEST_EQUAL("ManagedArray(ManagedArray&&)-Element6",8,MoveDestTest[5]);
        TEST_EQUAL("ManagedArray(ManagedArray&&)-Element7",13,MoveDestTest[6]);
        TEST_EQUAL("ManagedArray(ManagedArray&&)-Element8",21,MoveDestTest[7]);
        TEST_EQUAL("ManagedArray(ManagedArray&&)-Element9",34,MoveDestTest[8]);
        TEST_EQUAL("ManagedArray(ManagedArray&&)-Element10",55,MoveDestTest[9]);

    }//Construction

    {//Operators
        ManagedArray<int,12> CopyAssignSource = { 1, 3, 5, 7, 9 };
        ManagedArray<int,12> CopyAssignDest = { 9, 7, 5, 3, 1, 0 };
        CopyAssignDest = CopyAssignSource;
        TEST_EQUAL("operator=(const_ManagedArray&)-SourceSize",size_t(5),CopyAssignSource.size());
        TEST_EQUAL("operator=(const_ManagedArray&)-DestSize",size_t(5),CopyAssignDest.size());
        TEST_EQUAL("operator=(const_ManagedArray&)-SourceElement1",1,CopyAssignSource[0]);
        TEST_EQUAL("operator=(const_ManagedArray&)-SourceElement2",3,CopyAssignSource[1]);
        TEST_EQUAL("operator=(const_ManagedArray&)-SourceElement3",5,CopyAssignSource[2]);
        TEST_EQUAL("operator=(const_ManagedArray&)-SourceElement4",7,CopyAssignSource[3]);
        TEST_EQUAL("operator=(const_ManagedArray&)-SourceElement5",9,CopyAssignSource[4]);
        TEST_EQUAL("operator=(const_ManagedArray&)-DestElement1",1,CopyAssignDest[0]);
        TEST_EQUAL("operator=(const_ManagedArray&)-DestElement2",3,CopyAssignDest[1]);
        TEST_EQUAL("operator=(const_ManagedArray&)-DestElement3",5,CopyAssignDest[2]);
        TEST_EQUAL("operator=(const_ManagedArray&)-DestElement4",7,CopyAssignDest[3]);
        TEST_EQUAL("operator=(const_ManagedArray&)-DestElement5",9,CopyAssignDest[4]);

        // Should the source array zero out after a move?
        ManagedArray<int,12> MoveAssignSource = { 9, 7, 5, 3, 1, 0 };
        ManagedArray<int,12> MoveAssignDest = { 1, 3, 5, 7, 9 };
        MoveAssignDest = std::move( MoveAssignSource );
        TEST_EQUAL("operator=(ManagedArray&&)-SourceSize",size_t(0),MoveAssignSource.size());
        TEST_EQUAL("operator=(ManagedArray&&)-DestSize",size_t(6),MoveAssignDest.size());
        TEST_EQUAL("operator=(ManagedArray&&)-SourceElement1",9,MoveAssignSource[0]);
        TEST_EQUAL("operator=(ManagedArray&&)-SourceElement2",7,MoveAssignSource[1]);
        TEST_EQUAL("operator=(ManagedArray&&)-SourceElement3",5,MoveAssignSource[2]);
        TEST_EQUAL("operator=(ManagedArray&&)-SourceElement4",3,MoveAssignSource[3]);
        TEST_EQUAL("operator=(ManagedArray&&)-SourceElement5",1,MoveAssignSource[4]);
        TEST_EQUAL("operator=(ManagedArray&&)-SourceElement6",0,MoveAssignSource[5]);
        TEST_EQUAL("operator=(ManagedArray&&)-DestElement1",9,MoveAssignDest[0]);
        TEST_EQUAL("operator=(ManagedArray&&)-DestElement2",7,MoveAssignDest[1]);
        TEST_EQUAL("operator=(ManagedArray&&)-DestElement3",5,MoveAssignDest[2]);
        TEST_EQUAL("operator=(ManagedArray&&)-DestElement4",3,MoveAssignDest[3]);
        TEST_EQUAL("operator=(ManagedArray&&)-DestElement5",1,MoveAssignDest[4]);
        TEST_EQUAL("operator=(ManagedArray&&)-DestElement6",0,MoveAssignDest[5]);
    }//Operators

    {//Iterators
        ManagedArray<int,10> TestArray = { 3, 14, 15, 92, 65, 35, 89, 79, 32, 38 };
        const ManagedArray<int,15> ConstTestArray = { 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 120 };

        ManagedArray<int,10>::iterator BeginIter = TestArray.begin();
        ManagedArray<int,10>::const_iterator CBeginIter = TestArray.cbegin();
        ManagedArray<int,15>::const_iterator ConstBeginIter = ConstTestArray.begin();
        TEST_EQUAL("begin()",3,*BeginIter);
        TEST_EQUAL("cbegin()",3,*CBeginIter);
        TEST_EQUAL("begin()_const",1,*ConstBeginIter);

        ManagedArray<int,10>::iterator EndIter = TestArray.end();
        ManagedArray<int,10>::const_iterator CEndIter = TestArray.cend();
        ManagedArray<int,15>::const_iterator ConstEndIter = ConstTestArray.end();
        TEST_EQUAL("end()",10,EndIter - BeginIter);
        TEST_EQUAL("cend()",10,CEndIter - CBeginIter);
        TEST_EQUAL("end()_const",15,ConstEndIter - ConstBeginIter);

        ManagedArray<int,10>::reverse_iterator BeginRevIter = TestArray.rbegin();
        ManagedArray<int,10>::const_reverse_iterator CBeginRevIter = TestArray.crbegin();
        ManagedArray<int,15>::const_reverse_iterator ConstBeginRevIter = ConstTestArray.rbegin();
        TEST_EQUAL("rbegin()",38,*BeginRevIter);
        TEST_EQUAL("crbegin()",38,*CBeginRevIter);
        TEST_EQUAL("rbegin()_const",120,*ConstBeginRevIter);

        ManagedArray<int,10>::reverse_iterator EndRevIter = TestArray.rend();
        ManagedArray<int,10>::const_reverse_iterator CEndRevIter = TestArray.crend();
        ManagedArray<int,15>::const_reverse_iterator ConstEndRevIter = ConstTestArray.rend();
        TEST_EQUAL("rend()",10,EndRevIter - BeginRevIter);
        TEST_EQUAL("crend()",10,CEndRevIter - CBeginRevIter);
        TEST_EQUAL("rend()_const",15,ConstEndRevIter - ConstBeginRevIter);
    }//Iterators

    {//Query and Access
        ManagedArray<int,8> EmptyArray;
        ManagedArray<int,16> QueryArray = { 0, 1, 4, 9, 16, 25, 36, 49, 64, 81 };
        const ManagedArray<int,24> ConstQueryArray = { 1, 8, 27, 64, 125, 216, 343, 512 };

        TEST_EQUAL("size()_const-Empty",size_t(0),EmptyArray.size());
        TEST_EQUAL("size()_const-Query",size_t(10),QueryArray.size());
        TEST_EQUAL("size()_const-ConstQuery",size_t(8),ConstQueryArray.size());

        TEST_EQUAL("capacity()_const-Empty",size_t(8),EmptyArray.capacity());
        TEST_EQUAL("capacity()_const-Query",size_t(16),QueryArray.capacity());
        TEST_EQUAL("capacity()_const-ConstQuery",size_t(24),ConstQueryArray.capacity());

        TEST_EQUAL("empty()_const-Empty",true,EmptyArray.empty());
        TEST_EQUAL("empty()_const-Query",false,QueryArray.empty());
        TEST_EQUAL("empty()_const-ConstQuery",false,ConstQueryArray.empty());

        TEST_EQUAL("operator[](size_t)-Element1",0,QueryArray[0]);
        TEST_EQUAL("operator[](size_t)-Element3",4,QueryArray[2]);
        TEST_EQUAL("operator[](size_t)-Element5",16,QueryArray[4]);
        TEST_EQUAL("operator[](size_t)-Element7",36,QueryArray[6]);
        TEST_EQUAL("operator[](size_t)-Element9",64,QueryArray[8]);

        TEST_EQUAL("operator[](size_t)_const-Element1",1,ConstQueryArray[0]);
        TEST_EQUAL("operator[](size_t)_const-Element3",27,ConstQueryArray[2]);
        TEST_EQUAL("operator[](size_t)_const-Element5",125,ConstQueryArray[4]);
        TEST_EQUAL("operator[](size_t)_const-Element7",343,ConstQueryArray[6]);

        TEST_EQUAL("at(size_t)-Element2",1,QueryArray[1]);
        TEST_EQUAL("at(size_t)-Element4",9,QueryArray[3]);
        TEST_EQUAL("at(size_t)-Element6",25,QueryArray[5]);
        TEST_EQUAL("at(size_t)-Element8",49,QueryArray[7]);
        TEST_EQUAL("at(size_t)-Element10",81,QueryArray[9]);

        TEST_EQUAL("at(size_t)_const-Element2",8,ConstQueryArray[1]);
        TEST_EQUAL("at(size_t)_const-Element4",64,ConstQueryArray[3]);
        TEST_EQUAL("at(size_t)_const-Element6",216,ConstQueryArray[5]);
        TEST_EQUAL("at(size_t)_const-Element8",512,ConstQueryArray[7]);

        TEST_THROW("at(size_t)-Throw",
                   Mezzanine::Exception::OutOfRange,
                   [&QueryArray](){ QueryArray.at(100); });
        TEST_THROW("at(size_t)_const-Throw",
                   Mezzanine::Exception::OutOfRange,
                   [&ConstQueryArray](){ ConstQueryArray.at(500); });

        TEST_EQUAL("contains(const_value_type&)_const-Pass",true,QueryArray.contains(36));
        TEST_EQUAL("contains(const_value_type&)_const-Fail",false,QueryArray.contains(50));
    }//Query and Access

}

#endif
