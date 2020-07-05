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
#ifndef Mezz_Foundation_ManagedArraySequenceTests_h
#define Mezz_Foundation_ManagedArraySequenceTests_h

/// @file
/// @brief This file tests the functionality of the ManagedArray class related to sequences.

#include "MezzTest.h"

#include "MezzException.h"
#include "ManagedArray.h"

DEFAULT_TEST_GROUP(ManagedArraySequenceTests,ManagedArraySequence)
{
    using namespace Mezzanine;
    {//Sequence Modifiers

        ManagedArray<int,1> FirstPushBack;
        const int PushBackCopyValue = 66;
        FirstPushBack.push_back(PushBackCopyValue);
        TEST_EQUAL("push_back(const_value_type&)-Size",size_t(1),FirstPushBack.size())
        TEST_EQUAL("push_back(const_value_type&)-Element1",66,FirstPushBack[0])
        TEST_THROW("push_back(const_value_type&)-Throw",
                   Mezzanine::Exception::CapacityConsumed,
                   [&FirstPushBack](){ FirstPushBack.push_back(99); })

        ManagedArray<int,1> SecondPushBack;
        SecondPushBack.push_back( std::move(77) );
        TEST_EQUAL("push_back(value_type&&)-Size",size_t(1),SecondPushBack.size())
        TEST_EQUAL("push_back(value_type&&)-Element1",77,SecondPushBack[0])
        TEST_THROW("push_back(value_type&&)-Throw",
                   Mezzanine::Exception::CapacityConsumed,
                   [&SecondPushBack](){ SecondPushBack.push_back( std::move(99) ); })

        ManagedArray<int,10> AppendArray = { 1, 3, 5 };
        std::vector<int> AppendSource = { 7, 9, 11};
        AppendArray.append(AppendSource.begin(),AppendSource.end());
        TEST_EQUAL("append(InputIterator,InputIterator)-Size",size_t(6),AppendArray.size())
        TEST_EQUAL("append(InputIterator,InputIterator)-Element1",1,AppendArray[0])
        TEST_EQUAL("append(InputIterator,InputIterator)-Element2",3,AppendArray[1])
        TEST_EQUAL("append(InputIterator,InputIterator)-Element3",5,AppendArray[2])
        TEST_EQUAL("append(InputIterator,InputIterator)-Element4",7,AppendArray[3])
        TEST_EQUAL("append(InputIterator,InputIterator)-Element5",9,AppendArray[4])
        TEST_EQUAL("append(InputIterator,InputIterator)-Element6",11,AppendArray[5])

        std::vector<int> ThrowAppendSource = { 1, 1, 1, 1, 1, 1 };
        TEST_THROW("append(InputIterator,InputIterator)-Throw",
                   Mezzanine::Exception::CapacityConsumed,
                   [&](){ AppendArray.append(ThrowAppendSource.begin(),ThrowAppendSource.end()); })

        ManagedArray<int,5> EmplaceArray = { 1, 3, 7 };
        EmplaceArray.emplace(EmplaceArray.begin() + 1, 3 );
        EmplaceArray.emplace(EmplaceArray.end(),1337);
        TEST_EQUAL("emplace(const_iterator,ArgTypes&&...)-Size",size_t(5),EmplaceArray.size())
        TEST_EQUAL("emplace(const_iterator,ArgTypes&&...)-Element1",1,EmplaceArray[0])
        TEST_EQUAL("emplace(const_iterator,ArgTypes&&...)-Element2",3,EmplaceArray[1])
        TEST_EQUAL("emplace(const_iterator,ArgTypes&&...)-Element3",3,EmplaceArray[2])
        TEST_EQUAL("emplace(const_iterator,ArgTypes&&...)-Element4",7,EmplaceArray[3])
        TEST_EQUAL("emplace(const_iterator,ArgTypes&&...)-Element5",1337,EmplaceArray[4])

        TEST_THROW("emplace(const_iterator,ArgTypes&&...)-Throw",
                   Mezzanine::Exception::CapacityConsumed,
                   [&EmplaceArray](){ EmplaceArray.emplace(EmplaceArray.begin(),1337); })

        ManagedArray<String,4> EmplaceBackArray = { "This ", "is ", "a " };
        EmplaceBackArray.emplace_back("test.");
        TEST_EQUAL("emplace_back(ArgTypes&&...)-Size",size_t(4),EmplaceBackArray.size())
        TEST_EQUAL("emplace_back(ArgTypes&&...)-Element1",String("This "),EmplaceBackArray[0])
        TEST_EQUAL("emplace_back(ArgTypes&&...)-Element2",String("is "),EmplaceBackArray[1])
        TEST_EQUAL("emplace_back(ArgTypes&&...)-Element3",String("a "),EmplaceBackArray[2])
        TEST_EQUAL("emplace_back(ArgTypes&&...)-Element4",String("test."),EmplaceBackArray[3])

        TEST_THROW("emplace_back(ArgTypes&&...)-Throw",
                   Mezzanine::Exception::CapacityConsumed,
                   [&EmplaceBackArray](){ EmplaceBackArray.emplace_back("NOT!"); })

        ManagedArray<int,4> InsertCopyArray = { 4, 6 };
        const int InsertCopyValue1 = 5;
        const int InsertCopyValue2 = 7;
        InsertCopyArray.insert( InsertCopyArray.begin() + 1, InsertCopyValue1 );
        InsertCopyArray.insert( InsertCopyArray.end(), InsertCopyValue2 );
        TEST_EQUAL("insert(const_iterator,const_value_type&)-Size",size_t(4),InsertCopyArray.size())
        TEST_EQUAL("insert(const_iterator,const_value_type&)-Element1",4,InsertCopyArray[0])
        TEST_EQUAL("insert(const_iterator,const_value_type&)-Element2",5,InsertCopyArray[1])
        TEST_EQUAL("insert(const_iterator,const_value_type&)-Element3",6,InsertCopyArray[2])
        TEST_EQUAL("insert(const_iterator,const_value_type&)-Element4",7,InsertCopyArray[3])

        TEST_THROW("insert(const_iterator,const_value_type&)-Throw",
                   Mezzanine::Exception::CapacityConsumed,
                   [&InsertCopyArray](){ InsertCopyArray.insert(InsertCopyArray.begin(),7331); })

        ManagedArray<int,4> InsertMoveArray = { 9, 11 };
        InsertMoveArray.insert( InsertMoveArray.begin() + 1, std::move(10) );
        InsertMoveArray.insert( InsertMoveArray.end(), std::move(12) );
        TEST_EQUAL("insert(const_iterator,value_type&&)-Size",size_t(4),InsertMoveArray.size())
        TEST_EQUAL("insert(const_iterator,value_type&&)-Element1",9,InsertMoveArray[0])
        TEST_EQUAL("insert(const_iterator,value_type&&)-Element2",10,InsertMoveArray[1])
        TEST_EQUAL("insert(const_iterator,value_type&&)-Element3",11,InsertMoveArray[2])
        TEST_EQUAL("insert(const_iterator,value_type&&)-Element4",12,InsertMoveArray[3])

        TEST_THROW("insert(const_iterator,value_type&&)-Throw",
                   Mezzanine::Exception::CapacityConsumed,
                   [&InsertMoveArray](){ InsertMoveArray.insert(InsertMoveArray.begin(),3317); })

        ManagedArray<int,14> InsertRangeArray = { 0, 55, 89 };//{ 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89 };
        ManagedArray<int,4> FirstInsertSource = { 8, 13, 21, 34 };
        ManagedArray<int,5> SecondInsertSource = { 1, 1, 2, 3, 5 };
        InsertRangeArray.insert(InsertRangeArray.begin() + 1,FirstInsertSource.begin(),FirstInsertSource.end());
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-FirstInsertSize",
                   size_t(7),
                   InsertRangeArray.size())
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-FirstInsertElement1",0,InsertRangeArray[0])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-FirstInsertElement2",8,InsertRangeArray[1])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-FirstInsertElement3",13,InsertRangeArray[2])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-FirstInsertElement4",21,InsertRangeArray[3])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-FirstInsertElement5",34,InsertRangeArray[4])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-FirstInsertElement6",55,InsertRangeArray[5])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-FirstInsertElement7",89,InsertRangeArray[6])

        InsertRangeArray.insert(InsertRangeArray.begin() + 1u,
                                SecondInsertSource.begin(),
                                SecondInsertSource.end());
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-SecondInsertSize",
                   size_t(12),
                   InsertRangeArray.size())
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement1",0,InsertRangeArray[0])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement2",1,InsertRangeArray[1])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement3",1,InsertRangeArray[2])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement4",2,InsertRangeArray[3])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement5",3,InsertRangeArray[4])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement6",5,InsertRangeArray[5])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement7",8,InsertRangeArray[6])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement8",13,InsertRangeArray[7])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement9",21,InsertRangeArray[8])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement10",34,InsertRangeArray[9])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement11",55,InsertRangeArray[10])
        TEST_EQUAL("insert(const_iterator,InputIterator,InputIterator)-SecondInsertElement12",89,InsertRangeArray[11])

        TEST_THROW("insert(const_iterator,CapacityConsumed,InputIterator)-Throw",
                   Mezzanine::Exception::CapacityConsumed,
                   [&](){
                        InsertRangeArray.insert(InsertRangeArray.begin() + 6,
                                                FirstInsertSource.begin(),
                                                FirstInsertSource.end());
                   })

        ManagedArray<int,2> PopBackArray = { 5, 10 };
        PopBackArray.pop_back();
        TEST_EQUAL("pop_back()-Size",size_t(1),PopBackArray.size())
        TEST_EQUAL("pop_back()-Element1",5,PopBackArray[0])

        ManagedArray<int,3> EraseArray = { 333, 555, 777 };
        EraseArray.erase(EraseArray.begin() + 1);
        TEST_EQUAL("erase(const_iterator)-Size",size_t(2),EraseArray.size())
        TEST_EQUAL("erase(const_iterator)-Element1",333,EraseArray[0])
        TEST_EQUAL("erase(const_iterator)-Element2",777,EraseArray[1])

        TEST_THROW("erase(const_iterator)-Throw",
                   Mezzanine::Exception::OutOfRange,
                   [&EraseArray](){ EraseArray.erase(EraseArray.begin() + 9); })

        ManagedArray<int,6> EraseRangeArray = { 1, 22, 333, 4444, 55555, 666666 };
        EraseRangeArray.erase(EraseRangeArray.begin() + 2, EraseRangeArray.begin() + 5);

        TEST_EQUAL("erase(const_iterator,const_iterator)-Size",size_t(3),EraseRangeArray.size())
        TEST_EQUAL("erase(const_iterator,const_iterator)-Element1",1,EraseRangeArray[0])
        TEST_EQUAL("erase(const_iterator,const_iterator)-Element2",22,EraseRangeArray[1])
        TEST_EQUAL("erase(const_iterator,const_iterator)-Element3",666666,EraseRangeArray[2])

        TEST_THROW("erase(const_iterator,const_iterator)-Throw",
                   Mezzanine::Exception::OutOfRange,
                   [&EraseRangeArray](){
                        EraseRangeArray.erase(EraseRangeArray.begin() + 7, EraseRangeArray.begin() + 9);
                   })

        ManagedArray<int,3> FirstSwapArray = { 123, 234, 345 };
        ManagedArray<int,3> SecondSwapArray = { 567, 678 };
        FirstSwapArray.swap(SecondSwapArray);
        TEST_EQUAL("swap()-FirstSize",size_t(2),FirstSwapArray.size())
        TEST_EQUAL("swap()-SecondSize",size_t(3),SecondSwapArray.size())
        TEST_EQUAL("swap()-FirstArrayElement1",567,FirstSwapArray[0])
        TEST_EQUAL("swap()-FirstArrayElement2",678,FirstSwapArray[1])
        TEST_EQUAL("swap()-SecondArrayElement1",123,SecondSwapArray[0])
        TEST_EQUAL("swap()-SecondArrayElement2",234,SecondSwapArray[1])
        TEST_EQUAL("swap()-SecondArrayElement3",345,SecondSwapArray[2])

        ManagedArray<int,6> ClearArray = { 1, 2, 3, 4, 5, 6 };
        ClearArray.clear();
        TEST_EQUAL("clear()",true,ClearArray.empty())
    }//Sequence Modifiers

}

#endif
