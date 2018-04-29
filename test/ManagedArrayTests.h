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
#ifndef Mezz_Test_ManagedArray_h
#define Mezz_Test_ManagedArray_h

/// @file
/// @brief This file tests the functionality of the ManagedArray class.

#include "MezzTest.h"

#include "ManagedArray.h"

DEFAULT_TEST_GROUP(ManagedArrayTests,ManagedArray)
{
    using namespace Mezzanine;

    {//Construction
        ManagedArray<int,5> DefaultTest;
        TEST_EQUAL("ManagedArray::ManagedArray()-ResultSize",size_t(0),DefaultTest.size());
        TEST_EQUAL("ManagedArray::ManagedArray()-Iterators",DefaultTest.begin(),DefaultTest.end());

        ManagedArray<int,10> InitializerTest = { 3, 14, 15, 92, 65, 35 };
        TEST_EQUAL("ManagedArray::ManagedArray(std::initializer_list<ElementType>)-ResultSize",size_t(6),InitializerTest.size());
        TEST_EQUAL("ManagedArray::ManagedArray(std::initializer_list<ElementType>)-Element1",3,InitializerTest[0]);
        TEST_EQUAL("ManagedArray::ManagedArray(std::initializer_list<ElementType>)-Element2",14,InitializerTest[1]);
        TEST_EQUAL("ManagedArray::ManagedArray(std::initializer_list<ElementType>)-Element3",15,InitializerTest[2]);
        TEST_EQUAL("ManagedArray::ManagedArray(std::initializer_list<ElementType>)-Element4",92,InitializerTest[3]);
        TEST_EQUAL("ManagedArray::ManagedArray(std::initializer_list<ElementType>)-Element5",65,InitializerTest[4]);
        TEST_EQUAL("ManagedArray::ManagedArray(std::initializer_list<ElementType>)-Element6",35,InitializerTest[5]);

        ManagedArray<int,10> CopyTest(InitializerTest);
        TEST_EQUAL("ManagedArray::ManagedArray(const_ManagedArray&)-ResultSize",size_t(6),CopyTest.size());
        TEST_EQUAL("ManagedArray::ManagedArray(const_ManagedArray&)-Element1",3,CopyTest[0]);
        TEST_EQUAL("ManagedArray::ManagedArray(const_ManagedArray&)-Element2",14,CopyTest[1]);
        TEST_EQUAL("ManagedArray::ManagedArray(const_ManagedArray&)-Element3",15,CopyTest[2]);
        TEST_EQUAL("ManagedArray::ManagedArray(const_ManagedArray&)-Element4",92,CopyTest[3]);
        TEST_EQUAL("ManagedArray::ManagedArray(const_ManagedArray&)-Element5",65,CopyTest[4]);
        TEST_EQUAL("ManagedArray::ManagedArray(const_ManagedArray&)-Element6",35,CopyTest[5]);

        ManagedArray<int,15> MoveSourceTest = { 1, 1, 2, 3, 5, 8, 13, 21, 34, 55 };
        ManagedArray<int,15> MoveDestTest( std::move( MoveSourceTest ) );
        TEST_EQUAL("ManagedArray::ManagedArray(ManagedArray&&)-ResultSize",size_t(10),MoveDestTest.size());
        TEST_EQUAL("ManagedArray::ManagedArray(ManagedArray&&)-Element1",1,MoveDestTest[0]);
        TEST_EQUAL("ManagedArray::ManagedArray(ManagedArray&&)-Element2",1,MoveDestTest[1]);
        TEST_EQUAL("ManagedArray::ManagedArray(ManagedArray&&)-Element3",2,MoveDestTest[2]);
        TEST_EQUAL("ManagedArray::ManagedArray(ManagedArray&&)-Element4",3,MoveDestTest[3]);
        TEST_EQUAL("ManagedArray::ManagedArray(ManagedArray&&)-Element5",5,MoveDestTest[4]);
        TEST_EQUAL("ManagedArray::ManagedArray(ManagedArray&&)-Element6",8,MoveDestTest[5]);
        TEST_EQUAL("ManagedArray::ManagedArray(ManagedArray&&)-Element7",13,MoveDestTest[6]);
        TEST_EQUAL("ManagedArray::ManagedArray(ManagedArray&&)-Element8",21,MoveDestTest[7]);
        TEST_EQUAL("ManagedArray::ManagedArray(ManagedArray&&)-Element9",34,MoveDestTest[8]);
        TEST_EQUAL("ManagedArray::ManagedArray(ManagedArray&&)-Element10",55,MoveDestTest[9]);
    }//Construction

    {//Operators
        ManagedArray<int,12> CopyAssignSource = { 1, 3, 5, 7, 9 };
        ManagedArray<int,12> CopyAssignDest = { 9, 7, 5, 3, 1, 0 };
        CopyAssignDest = CopyAssignSource;
        TEST_EQUAL("ManagedArray::operator=(const_ManagedArray&)-SourceSize",size_t(5),CopyAssignSource.size());
        TEST_EQUAL("ManagedArray::operator=(const_ManagedArray&)-DestSize",size_t(5),CopyAssignDest.size());
        TEST_EQUAL("ManagedArray::operator=(const_ManagedArray&)-SourceElement1",1,CopyAssignSource[0]);
        TEST_EQUAL("ManagedArray::operator=(const_ManagedArray&)-SourceElement2",3,CopyAssignSource[1]);
        TEST_EQUAL("ManagedArray::operator=(const_ManagedArray&)-SourceElement3",5,CopyAssignSource[2]);
        TEST_EQUAL("ManagedArray::operator=(const_ManagedArray&)-SourceElement4",7,CopyAssignSource[3]);
        TEST_EQUAL("ManagedArray::operator=(const_ManagedArray&)-SourceElement5",9,CopyAssignSource[4]);
        TEST_EQUAL("ManagedArray::operator=(const_ManagedArray&)-DestElement1",1,CopyAssignDest[0]);
        TEST_EQUAL("ManagedArray::operator=(const_ManagedArray&)-DestElement2",3,CopyAssignDest[1]);
        TEST_EQUAL("ManagedArray::operator=(const_ManagedArray&)-DestElement3",5,CopyAssignDest[2]);
        TEST_EQUAL("ManagedArray::operator=(const_ManagedArray&)-DestElement4",7,CopyAssignDest[3]);
        TEST_EQUAL("ManagedArray::operator=(const_ManagedArray&)-DestElement5",9,CopyAssignDest[4]);

        // Should the source array zero out after a move?
        ManagedArray<int,12> MoveAssignSource = { 9, 7, 5, 3, 1, 0 };
        ManagedArray<int,12> MoveAssignDest = { 1, 3, 5, 7, 9 };
        MoveAssignDest = std::move( MoveAssignSource );
        TEST_EQUAL("ManagedArray::operator=(ManagedArray&&)-SourceSize",size_t(0),MoveAssignSource.size());
        TEST_EQUAL("ManagedArray::operator=(ManagedArray&&)-DestSize",size_t(6),MoveAssignDest.size());
        TEST_EQUAL("ManagedArray::operator=(ManagedArray&&)-SourceElement1",9,MoveAssignSource[0]);
        TEST_EQUAL("ManagedArray::operator=(ManagedArray&&)-SourceElement2",7,MoveAssignSource[1]);
        TEST_EQUAL("ManagedArray::operator=(ManagedArray&&)-SourceElement3",5,MoveAssignSource[2]);
        TEST_EQUAL("ManagedArray::operator=(ManagedArray&&)-SourceElement4",3,MoveAssignSource[3]);
        TEST_EQUAL("ManagedArray::operator=(ManagedArray&&)-SourceElement5",1,MoveAssignSource[4]);
        TEST_EQUAL("ManagedArray::operator=(ManagedArray&&)-SourceElement6",0,MoveAssignSource[5]);
        TEST_EQUAL("ManagedArray::operator=(ManagedArray&&)-DestElement1",9,MoveAssignDest[0]);
        TEST_EQUAL("ManagedArray::operator=(ManagedArray&&)-DestElement2",7,MoveAssignDest[1]);
        TEST_EQUAL("ManagedArray::operator=(ManagedArray&&)-DestElement3",5,MoveAssignDest[2]);
        TEST_EQUAL("ManagedArray::operator=(ManagedArray&&)-DestElement4",3,MoveAssignDest[3]);
        TEST_EQUAL("ManagedArray::operator=(ManagedArray&&)-DestElement5",1,MoveAssignDest[4]);
        TEST_EQUAL("ManagedArray::operator=(ManagedArray&&)-DestElement6",0,MoveAssignDest[5]);
    }//Operators

    {//Iterators
        ManagedArray<int,10> TestArray = { 3, 14, 15, 92, 65, 35, 89, 79, 32, 38 };
        const ManagedArray<int,15> ConstTestArray = { 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78, 91, 105, 120 };

        ManagedArray<int,10>::iterator BeginIter = TestArray.begin();
        ManagedArray<int,15>::const_iterator ConstBeginIter = ConstTestArray.begin();
        TEST_EQUAL("ManagedArray::begin()",3,*BeginIter);
        TEST_EQUAL("ManagedArray::begin()_const",1,*ConstBeginIter);

        ManagedArray<int,10>::iterator EndIter = TestArray.end();
        ManagedArray<int,15>::const_iterator ConstEndIter = ConstTestArray.end();
        TEST_EQUAL("ManagedArray::end()",10,EndIter - BeginIter);
        TEST_EQUAL("ManagedArray::end()_const",15,ConstEndIter - ConstBeginIter);

        ManagedArray<int,10>::reverse_iterator BeginRevIter = TestArray.rbegin();
        ManagedArray<int,15>::const_reverse_iterator ConstBeginRevIter = ConstTestArray.rbegin();
        TEST_EQUAL("ManagedArray::rbegin()",38,*BeginRevIter);
        TEST_EQUAL("ManagedArray::rbegin()_const",120,*ConstBeginRevIter);

        ManagedArray<int,10>::reverse_iterator EndRevIter = TestArray.rend();
        ManagedArray<int,15>::const_reverse_iterator ConstEndRevIter = ConstTestArray.rend();
        TEST_EQUAL("ManagedArray::rend()",10,EndRevIter - BeginRevIter);
        TEST_EQUAL("ManagedArray::rend()_const",15,ConstEndRevIter - ConstBeginRevIter);
    }//Iterators

    {//Query and Access
        ManagedArray<int,8> EmptyArray;
        ManagedArray<int,16> QueryArray = { 0, 1, 4, 9, 16, 25, 36, 49, 64, 81 };
        const ManagedArray<int,24> ConstQueryArray = { 1, 8, 27, 64, 125, 216, 343, 512 };

        TEST_EQUAL("ManagedArray::size()_const-Empty",size_t(0),EmptyArray.size());
        TEST_EQUAL("ManagedArray::size()_const-Query",size_t(10),QueryArray.size());
        TEST_EQUAL("ManagedArray::size()_const-ConstQuery",size_t(8),ConstQueryArray.size());

        TEST_EQUAL("ManagedArray::capacity()_const-Empty",size_t(8),EmptyArray.capacity());
        TEST_EQUAL("ManagedArray::capacity()_const-Query",size_t(16),QueryArray.capacity());
        TEST_EQUAL("ManagedArray::capacity()_const-ConstQuery",size_t(24),ConstQueryArray.capacity());

        TEST_EQUAL("ManagedArray::empty()_const-Empty",true,EmptyArray.empty());
        TEST_EQUAL("ManagedArray::empty()_const-Query",false,QueryArray.empty());
        TEST_EQUAL("ManagedArray::empty()_const-ConstQuery",false,ConstQueryArray.empty());

        TEST_EQUAL("ManagedArray::operator[](size_t)-Element1",0,QueryArray[0]);
        TEST_EQUAL("ManagedArray::operator[](size_t)-Element3",4,QueryArray[2]);
        TEST_EQUAL("ManagedArray::operator[](size_t)-Element5",16,QueryArray[4]);
        TEST_EQUAL("ManagedArray::operator[](size_t)-Element7",36,QueryArray[6]);
        TEST_EQUAL("ManagedArray::operator[](size_t)-Element9",64,QueryArray[8]);

        TEST_EQUAL("ManagedArray::operator[](size_t)_const-Element1",1,ConstQueryArray[0]);
        TEST_EQUAL("ManagedArray::operator[](size_t)_const-Element3",27,ConstQueryArray[2]);
        TEST_EQUAL("ManagedArray::operator[](size_t)_const-Element5",125,ConstQueryArray[4]);
        TEST_EQUAL("ManagedArray::operator[](size_t)_const-Element7",343,ConstQueryArray[6]);

        TEST_EQUAL("ManagedArray::at(size_t)-Element2",1,QueryArray[1]);
        TEST_EQUAL("ManagedArray::at(size_t)-Element4",9,QueryArray[3]);
        TEST_EQUAL("ManagedArray::at(size_t)-Element6",25,QueryArray[5]);
        TEST_EQUAL("ManagedArray::at(size_t)-Element8",49,QueryArray[7]);
        TEST_EQUAL("ManagedArray::at(size_t)-Element10",81,QueryArray[9]);

        TEST_EQUAL("ManagedArray::at(size_t)_const-Element2",8,ConstQueryArray[1]);
        TEST_EQUAL("ManagedArray::at(size_t)_const-Element4",64,ConstQueryArray[3]);
        TEST_EQUAL("ManagedArray::at(size_t)_const-Element6",216,ConstQueryArray[5]);
        TEST_EQUAL("ManagedArray::at(size_t)_const-Element8",512,ConstQueryArray[7]);

        TEST_THROW("ManagedArray::at(size_t)-Throw",std::out_of_range,[&QueryArray](){ QueryArray.at(100); });
        TEST_THROW("ManagedArray::at(size_t)_const-Throw",std::out_of_range,[&ConstQueryArray](){ ConstQueryArray.at(500); });

        TEST_EQUAL("ManagedArray::contains(const_value_type&)_const-Pass",true,QueryArray.contains(36));
        TEST_EQUAL("ManagedArray::contains(const_value_type&)_const-Fail",false,QueryArray.contains(50));
    }//Query and Access

    {//Sequence Modifiers
        ManagedArray<int,1> FirstPushBack;
        const int PushBackCopyValue = 66;
        FirstPushBack.push_back(PushBackCopyValue);
        TEST_EQUAL("ManagedArray::push_back(const_value_type&)-Size",size_t(1),FirstPushBack.size());
        TEST_EQUAL("ManagedArray::push_back(const_value_type&)-Element1",66,FirstPushBack[0]);
        TEST_THROW("ManagedArray::push_back(const_value_type&)-Throw",
                   std::out_of_range,
                   [&FirstPushBack](){ FirstPushBack.push_back(99); });

        ManagedArray<int,1> SecondPushBack;
        SecondPushBack.push_back( std::move(77) );
        TEST_EQUAL("ManagedArray::push_back(value_type&&)-Size",size_t(1),SecondPushBack.size());
        TEST_EQUAL("ManagedArray::push_back(value_type&&)-Element1",77,SecondPushBack[0]);
        TEST_THROW("ManagedArray::push_back(value_type&&)-Throw",
                   std::out_of_range,
                   [&SecondPushBack](){ SecondPushBack.push_back( std::move(99) ); });

        ManagedArray<int,10> AppendArray = { 1, 3, 5 };
        std::vector<int> AppendSource = { 7, 9, 11};
        AppendArray.append(AppendSource.begin(),AppendSource.end());
        TEST_EQUAL("ManagedArray::append(InputIterator,InputIterator)-Size",size_t(6),AppendArray.size());
        TEST_EQUAL("ManagedArray::append(InputIterator,InputIterator)-Element1",1,AppendArray[0]);
        TEST_EQUAL("ManagedArray::append(InputIterator,InputIterator)-Element2",3,AppendArray[1]);
        TEST_EQUAL("ManagedArray::append(InputIterator,InputIterator)-Element3",5,AppendArray[2]);
        TEST_EQUAL("ManagedArray::append(InputIterator,InputIterator)-Element4",7,AppendArray[3]);
        TEST_EQUAL("ManagedArray::append(InputIterator,InputIterator)-Element5",9,AppendArray[4]);
        TEST_EQUAL("ManagedArray::append(InputIterator,InputIterator)-Element6",11,AppendArray[5]);

        std::vector<int> ThrowAppendSource = { 1, 1, 1, 1, 1, 1 };
        TEST_THROW("ManagedArray::append(InputIterator,InputIterator)-Throw",
                   std::out_of_range,
                   [&](){ AppendArray.append(ThrowAppendSource.begin(),ThrowAppendSource.end()); });

        ManagedArray<int,4> EmplaceArray = { 1, 3, 7 };
        EmplaceArray.emplace(EmplaceArray.begin() + 1, 3 );
        EmplaceArray.emplace(EmplaceArray.end(),1337);
        TEST_EQUAL("ManagedArray::emplace(const_iterator,ArgTypes&&...)-Size",size_t(5),EmplaceArray.size());
        TEST_EQUAL("ManagedArray::emplace(const_iterator,ArgTypes&&...)-Element1",1,EmplaceArray[0]);
        TEST_EQUAL("ManagedArray::emplace(const_iterator,ArgTypes&&...)-Element2",3,EmplaceArray[1]);
        TEST_EQUAL("ManagedArray::emplace(const_iterator,ArgTypes&&...)-Element3",3,EmplaceArray[2]);
        TEST_EQUAL("ManagedArray::emplace(const_iterator,ArgTypes&&...)-Element4",7,EmplaceArray[3]);
        TEST_EQUAL("ManagedArray::emplace(const_iterator,ArgTypes&&...)-Element5",1337,EmplaceArray[4]);

        TEST_THROW("ManagedArray::emplace(const_iterator,ArgTypes&&...)-Throw",
                   std::out_of_range,
                   [&EmplaceArray](){ EmplaceArray.emplace(EmplaceArray.begin(),1337); });

        ManagedArray<String,4> EmplaceBackArray = { "This ", "is ", "a " };
        EmplaceBackArray.emplace_back("test.");
        TEST_EQUAL("ManagedArray::emplace_back(ArgTypes&&...)-Size",size_t(4),EmplaceBackArray.size());
        TEST_EQUAL("ManagedArray::emplace_back(ArgTypes&&...)-Element1",String("This "),EmplaceBackArray[0]);
        TEST_EQUAL("ManagedArray::emplace_back(ArgTypes&&...)-Element2",String("is "),EmplaceBackArray[1]);
        TEST_EQUAL("ManagedArray::emplace_back(ArgTypes&&...)-Element3",String("a "),EmplaceBackArray[2]);
        TEST_EQUAL("ManagedArray::emplace_back(ArgTypes&&...)-Element4",String("test."),EmplaceBackArray[3]);

        TEST_THROW("ManagedArray::emplace_back(ArgTypes&&...)-Throw",
                   std::out_of_range,
                   [&EmplaceBackArray](){ EmplaceBackArray.emplace_back("NOT!"); });

        ManagedArray<int,4> InsertCopyArray = { 4, 6 };
        const int InsertCopyValue = 5;
        InsertCopyArray.insert( InsertCopyArray.begin() + 1, InsertCopyValue );
        InsertCopyArray.insert( InsertCopyArray.end(), 7 );
        TEST_EQUAL("ManagedArray::insert(const_iterator,const_value_type&)-Size",size_t(3),InsertCopyArray.size());
        TEST_EQUAL("ManagedArray::insert(const_iterator,const_value_type&)-Element1",4,InsertCopyArray[0]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,const_value_type&)-Element2",5,InsertCopyArray[1]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,const_value_type&)-Element3",6,InsertCopyArray[2]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,const_value_type&)-Element4",7,InsertCopyArray[3]);

        TEST_THROW("ManagedArray::insert(const_iterator,const_value_type&)-Throw",
                   std::out_of_range,
                   [&InsertCopyArray](){ InsertCopyArray.insert(InsertCopyArray.begin(),7331); });

        ManagedArray<int,4> InsertMoveArray = { 9, 11 };
        InsertMoveArray.insert( InsertMoveArray.begin() + 1, std::move(10) );
        InsertMoveArray.insert( InsertMoveArray.end(), std::move(12) );
        TEST_EQUAL("ManagedArray::insert(const_iterator,value_type&&)-Size",size_t(3),InsertMoveArray.size());
        TEST_EQUAL("ManagedArray::insert(const_iterator,value_type&&)-Element1",9,InsertMoveArray[0]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,value_type&&)-Element2",10,InsertMoveArray[1]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,value_type&&)-Element3",11,InsertMoveArray[2]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,value_type&&)-Element4",12,InsertMoveArray[3]);

        TEST_THROW("ManagedArray::insert(const_iterator,value_type&&)-Throw",
                   std::out_of_range,
                   [&InsertMoveArray](){ InsertMoveArray.insert(InsertMoveArray.begin(),3317); });

        ManagedArray<int,14> InsertRangeArray = { 0, 55, 89 };//{ 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89 };
        ManagedArray<int,4> FirstInsertSource = { 8, 13, 21, 34 };
        ManagedArray<int,5> SecondInsertSource = { 1, 1, 2, 3, 5 };
        InsertRangeArray.insert(InsertRangeArray.begin() + 1,FirstInsertSource.begin(),FirstInsertSource.end());
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-FirstInsertSize",size_t(7),InsertRangeArray.size());
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-FirstInsertElement1",0,InsertRangeArray[0]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-FirstInsertElement2",8,InsertRangeArray[1]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-FirstInsertElement3",13,InsertRangeArray[2]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-FirstInsertElement4",21,InsertRangeArray[3]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-FirstInsertElement5",34,InsertRangeArray[4]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-FirstInsertElement6",55,InsertRangeArray[5]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-FirstInsertElement7",89,InsertRangeArray[6]);
        InsertRangeArray.insert(InsertRangeArray.begin() + 1,SecondInsertSource.begin(),SecondInsertSource.end());
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-SecondInsertSize",size_t(12),InsertRangeArray.size());
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement1",0,InsertRangeArray[0]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement2",1,InsertRangeArray[1]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement3",1,InsertRangeArray[2]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement4",2,InsertRangeArray[3]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement5",3,InsertRangeArray[4]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement6",5,InsertRangeArray[5]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement7",8,InsertRangeArray[6]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement8",13,InsertRangeArray[7]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement9",21,InsertRangeArray[8]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement10",34,InsertRangeArray[9]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement11",55,InsertRangeArray[10]);
        TEST_EQUAL("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement12",89,InsertRangeArray[11]);

        TEST_THROW("ManagedArray::insert(const_iterator,InputIterator,InputIterator)-Throw",
                   std::out_of_range,
                   [&](){ InsertRangeArray.insert(InsertRangeArray.begin() + 6,FirstInsertSource.begin(),FirstInsertSource.end()); });

        ManagedArray<int,2> PopBackArray = { 5, 10 };
        PopBackArray.pop_back();
        TEST_EQUAL("ManagedArray::pop_back()-Size",size_t(1),PopBackArray.size());
        TEST_EQUAL("ManagedArray::pop_back()-Element1",5,PopBackArray[0]);

        ManagedArray<int,3> EraseArray = { 333, 555, 777 };
        EraseArray.erase(EraseArray.begin() + 1);
        TEST_EQUAL("ManagedArray::erase(const_iterator)-Size",size_t(2),EraseArray.size());
        TEST_EQUAL("ManagedArray::erase(const_iterator)-Element1",333,EraseArray[0]);
        TEST_EQUAL("ManagedArray::erase(const_iterator)-Element2",777,EraseArray[1]);

        TEST_THROW("ManagedArray::erase(const_iterator)-Throw",
                   std::out_of_range,
                   [&EraseArray](){ EraseArray.erase(EraseArray.begin() + 9); });

        ManagedArray<int,6> EraseRangeArray = { 1, 22, 333, 4444, 55555, 666666 };
        EraseRangeArray.erase(EraseRangeArray.begin() + 2, EraseRangeArray.begin() + 5);

        TEST_EQUAL("ManagedArray::erase(const_iterator,const_iterator)-Size",size_t(3),EraseRangeArray.size());
        TEST_EQUAL("ManagedArray::erase(const_iterator,const_iterator)-Element1",1,EraseRangeArray[0]);
        TEST_EQUAL("ManagedArray::erase(const_iterator,const_iterator)-Element2",22,EraseRangeArray[1]);
        TEST_EQUAL("ManagedArray::erase(const_iterator,const_iterator)-Element3",666666,EraseRangeArray[2]);

        TEST_THROW("ManagedArray::erase(const_iterator,const_iterator)-Throw",
                   std::out_of_range,
                   [&EraseRangeArray](){ EraseRangeArray.erase(EraseRangeArray.begin() + 7, EraseRangeArray.begin() + 9); });

        ManagedArray<int,3> FirstSwapArray = { 123, 234, 345 };
        ManagedArray<int,3> SecondSwapArray = { 567, 678 };
        FirstSwapArray.swap(SecondSwapArray);
        TEST_EQUAL("ManagedArray::swap()-FirstSize",size_t(2),FirstSwapArray.size());
        TEST_EQUAL("ManagedArray::swap()-SecondSize",size_t(3),SecondSwapArray.size());
        TEST_EQUAL("ManagedArray::swap()-FirstArrayElement1",567,FirstSwapArray[0]);
        TEST_EQUAL("ManagedArray::swap()-FirstArrayElement2",678,FirstSwapArray[1]);
        TEST_EQUAL("ManagedArray::swap()-SecondArrayElement1",123,SecondSwapArray[0]);
        TEST_EQUAL("ManagedArray::swap()-SecondArrayElement2",234,SecondSwapArray[1]);
        TEST_EQUAL("ManagedArray::swap()-SecondArrayElement3",345,SecondSwapArray[2]);

        ManagedArray<int,6> ClearArray = { 1, 2, 3, 4, 5, 6 };
        ClearArray.clear();
        TEST_EQUAL("ManagedArray::clear()",true,ClearArray.empty());
    }//Sequence Modifiers
}

#endif
