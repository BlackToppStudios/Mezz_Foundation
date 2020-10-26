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
#ifndef Mezz_Foundation_ContainerToolsTests_h
#define Mezz_Foundation_ContainerToolsTests_h

/// @file
/// @brief This file tests the functionality of generic support functions and type-traits for working with containers.

#include "MezzTest.h"

#include "ContainerTools.h"

// Standard Sequence Containers
#include <array>
#include <vector>
#include <deque>
#include <list>
// Standard Associative Containers
#include <set>
#include <map>
// Standard Unordered Associative Containers
#include <unordered_set>
#include <unordered_map>
// Incomplete List of Mezzanine Containers
#include "FlatMap.h"
#include "ManagedArray.h"
#include "SortedManagedArray.h"
#include "SortedVector.h"
// For test failure types
#include <memory>

AUTOMATIC_TEST_GROUP(ContainerToolsTests,ContainerTools)
{
    using namespace Mezzanine;

    // List of container types to test.
    using TestArray = std::array<int,5>;
    using TestVector = std::vector<int>;
    using TestDeque = std::deque<int>;
    using TestList = std::list<int>;
    using TestSet = std::set<int>;
    using TestMultiSet = std::multiset<int>;
    using TestMap = std::map<std::string,int>;
    using TestMultiMap = std::multimap<std::string,int>;
    using TestUnorderedSet = std::unordered_set<int>;
    using TestUnorderedMultiSet = std::unordered_multiset<int>;
    using TestUnorderedMap = std::unordered_map<std::string,int>;
    using TestUnorderedMultiMap = std::unordered_multimap<std::string,int>;
    using TestFlatMap = FlatMap<std::string,int>;
    using TestManagedArray = ManagedArray<int,5>;
    using TestSortedManagedArray = SortedManagedArray<int,5>;
    using TestSortedVector = SortedVector<int>;
    using TestString = std::string;
    using TestSharedPtr = std::shared_ptr<int>;

    {//is_range
        TEST_EQUAL("is_range-Array",
                   true,IsRange<TestArray>())
        TEST_EQUAL("is_range-Map",
                   true,IsRange<TestMap>())
        TEST_EQUAL("is_range-String",
                   true,IsRange<TestString>())
        TEST_EQUAL("is_range-SharedPtr",
                   false,IsRange<TestSharedPtr>())
    }//is_range

    {//is_resizable_contiguous
        TEST_EQUAL("is_resizable_contiguous-Array",
                   false,IsResizableContiguous<TestArray>())
        TEST_EQUAL("is_resizable_contiguous-Vector",
                   true,IsResizableContiguous<TestVector>())
        TEST_EQUAL("is_resizable_contiguous-Deque",
                   false,IsResizableContiguous<TestDeque>())
        TEST_EQUAL("is_resizable_contiguous-List",
                   false,IsResizableContiguous<TestList>())
        TEST_EQUAL("is_resizable_contiguous-Set",
                   false,IsResizableContiguous<TestSet>())
        TEST_EQUAL("is_resizable_contiguous-MultiSet",
                   false,IsResizableContiguous<TestMultiSet>())
        TEST_EQUAL("is_resizable_contiguous-Map",
                   false,IsResizableContiguous<TestMap>())
        TEST_EQUAL("is_resizable_contiguous-MultiMap",
                   false,IsResizableContiguous<TestMultiMap>())
        TEST_EQUAL("is_resizable_contiguous-UnorderedSet",
                   false,IsResizableContiguous<TestUnorderedSet>())
        TEST_EQUAL("is_resizable_contiguous-UnorderedMultiSet",
                   false,IsResizableContiguous<TestUnorderedMultiSet>())
        TEST_EQUAL("is_resizable_contiguous-UnorderedMap",
                   false,IsResizableContiguous<TestUnorderedMap>())
        TEST_EQUAL("is_resizable_contiguous-UnorderedMultiMap",
                   false,IsResizableContiguous<TestUnorderedMultiMap>())
        TEST_EQUAL("is_resizable_contiguous-FlatMap",
                   false,IsResizableContiguous<TestFlatMap>())
        TEST_EQUAL("is_resizable_contiguous-ManagedArray",
                   false,IsResizableContiguous<TestManagedArray>())
        TEST_EQUAL("is_resizable_contiguous-SortedManagedArray",
                   false,IsResizableContiguous<TestSortedManagedArray>())
        TEST_EQUAL("is_resizable_contiguous-SortedVector",
                   false,IsResizableContiguous<TestSortedVector>())
        TEST_EQUAL("is_resizable_contiguous-String",
                   true,IsResizableContiguous<TestString>())
        TEST_EQUAL("is_resizable_contiguous-SharedPtr",
                   false,IsResizableContiguous<TestSharedPtr>())
    }//is_resizable_contiguous

    {//is_resizable_noncontiguous
        TEST_EQUAL("is_resizable_noncontiguous-Array",
                   false,IsResizableNonContiguous<TestArray>())
        TEST_EQUAL("is_resizable_noncontiguous-Vector",
                   false,IsResizableNonContiguous<TestVector>())
        TEST_EQUAL("is_resizable_noncontiguous-Deque",
                   true,IsResizableNonContiguous<TestDeque>())
        TEST_EQUAL("is_resizable_noncontiguous-List",
                   true,IsResizableNonContiguous<TestList>())
        TEST_EQUAL("is_resizable_noncontiguous-Set",
                   false,IsResizableNonContiguous<TestSet>())
        TEST_EQUAL("is_resizable_noncontiguous-MultiSet",
                   false,IsResizableNonContiguous<TestMultiSet>())
        TEST_EQUAL("is_resizable_noncontiguous-Map",
                   false,IsResizableNonContiguous<TestMap>())
        TEST_EQUAL("is_resizable_noncontiguous-MultiMap",
                   false,IsResizableNonContiguous<TestMultiMap>())
        TEST_EQUAL("is_resizable_noncontiguous-UnorderedSet",
                   false,IsResizableNonContiguous<TestUnorderedSet>())
        TEST_EQUAL("is_resizable_noncontiguous-UnorderedMultiSet",
                   false,IsResizableNonContiguous<TestUnorderedMultiSet>())
        TEST_EQUAL("is_resizable_noncontiguous-UnorderedMap",
                   false,IsResizableNonContiguous<TestUnorderedMap>())
        TEST_EQUAL("is_resizable_noncontiguous-UnorderedMultiMap",
                   false,IsResizableNonContiguous<TestUnorderedMultiMap>())
        TEST_EQUAL("is_resizable_noncontiguous-FlatMap",
                   false,IsResizableNonContiguous<TestFlatMap>())
        TEST_EQUAL("is_resizable_noncontiguous-ManagedArray",
                   false,IsResizableNonContiguous<TestManagedArray>())
        TEST_EQUAL("is_resizable_noncontiguous-SortedManagedArray",
                   false,IsResizableNonContiguous<TestSortedManagedArray>())
        TEST_EQUAL("is_resizable_noncontiguous-SortedVector",
                   false,IsResizableNonContiguous<TestSortedVector>())
        TEST_EQUAL("is_resizable_noncontiguous-String",
                   false,IsResizableNonContiguous<TestString>())
        TEST_EQUAL("is_resizable_noncontiguous-SharedPtr",
                   false,IsResizableNonContiguous<TestSharedPtr>())
    }//is_resizable_noncontiguous

    {//is_container
        TEST_EQUAL("is_container-Array",
                   true,IsContainer<TestArray>())
        TEST_EQUAL("is_container-Vector",
                   true,IsContainer<TestVector>())
        TEST_EQUAL("is_container-Deque",
                   true,IsContainer<TestDeque>())
        TEST_EQUAL("is_container-List",
                   true,IsContainer<TestList>())
        TEST_EQUAL("is_container-Set",
                   true,IsContainer<TestSet>())
        TEST_EQUAL("is_container-MultiSet",
                   true,IsContainer<TestMultiSet>())
        TEST_EQUAL("is_container-Map",
                   true,IsContainer<TestMap>())
        TEST_EQUAL("is_container-MultiMap",
                   true,IsContainer<TestMultiMap>())
        TEST_EQUAL("is_container-UnorderedSet",
                   true,IsContainer<TestUnorderedSet>())
        TEST_EQUAL("is_container-UnorderedMultiSet",
                   true,IsContainer<TestUnorderedMultiSet>())
        TEST_EQUAL("is_container-UnorderedMap",
                   true,IsContainer<TestUnorderedMap>())
        TEST_EQUAL("is_container-UnorderedMultiMap",
                   true,IsContainer<TestUnorderedMultiMap>())
        TEST_EQUAL("is_container-FlatMap",
                   true,IsContainer<TestFlatMap>())
        TEST_EQUAL("is_container-ManagedArray",
                   true,IsContainer<TestManagedArray>())
        TEST_EQUAL("is_container-SortedManagedArray",
                   true,IsContainer<TestSortedManagedArray>())
        TEST_EQUAL("is_container-SortedVector",
                   true,IsContainer<TestSortedVector>())
        TEST_EQUAL("is_container-String",
                   true,IsContainer<TestString>())
        TEST_EQUAL("is_container-SharedPtr",
                   false,IsContainer<TestSharedPtr>())
    }//is_container

    {//is_key_value_pair
        TEST_EQUAL("is_key_value_pair-Vector",
                   false,IsKeyValuePair<TestVector::value_type>())
        TEST_EQUAL("is_key_value_pair-Map",
                   true,IsKeyValuePair<TestMap::value_type>())
        TEST_EQUAL("is_key_value_pair-ManagedArray",
                   false,IsKeyValuePair<TestManagedArray::value_type>())
        TEST_EQUAL("is_key_value_pair-UnorderedMultiMap",
                   true,IsKeyValuePair<TestUnorderedMultiMap::value_type>())
        TEST_EQUAL("is_key_value_pair-SharedPtr",
                   false,IsKeyValuePair<TestSharedPtr>())
    }//is_key_value_pair

    {//is_associative_container
        TEST_EQUAL("is_associative_container-Array",
                   false,IsAssociativeContainer<TestArray>())
        TEST_EQUAL("is_associative_container-Vector",
                   false,IsAssociativeContainer<TestVector>())
        TEST_EQUAL("is_associative_container-Deque",
                   false,IsAssociativeContainer<TestDeque>())
        TEST_EQUAL("is_associative_container-List",
                   false,IsAssociativeContainer<TestList>())
        TEST_EQUAL("is_associative_container-Set",
                   false,IsAssociativeContainer<TestSet>())
        TEST_EQUAL("is_associative_container-MultiSet",
                   false,IsAssociativeContainer<TestMultiSet>())
        TEST_EQUAL("is_associative_container-Map",
                   true,IsAssociativeContainer<TestMap>())
        TEST_EQUAL("is_associative_container-MultiMap",
                   true,IsAssociativeContainer<TestMultiMap>())
        TEST_EQUAL("is_associative_container-UnorderedSet",
                   false,IsAssociativeContainer<TestUnorderedSet>())
        TEST_EQUAL("is_associative_container-UnorderedMultiSet",
                   false,IsAssociativeContainer<TestUnorderedMultiSet>())
        TEST_EQUAL("is_associative_container-UnorderedMap",
                   true,IsAssociativeContainer<TestUnorderedMap>())
        TEST_EQUAL("is_associative_container-UnorderedMultiMap",
                   true,IsAssociativeContainer<TestUnorderedMultiMap>())
        TEST_EQUAL("is_associative_container-FlatMap",
                   true,IsAssociativeContainer<TestFlatMap>())
        TEST_EQUAL("is_associative_container-ManagedArray",
                   false,IsAssociativeContainer<TestManagedArray>())
        TEST_EQUAL("is_associative_container-SortedManagedArray",
                   false,IsAssociativeContainer<TestSortedManagedArray>())
        TEST_EQUAL("is_associative_container-SortedVector",
                   false,IsAssociativeContainer<TestSortedVector>())
        TEST_EQUAL("is_associative_container-String",
                   false,IsAssociativeContainer<TestString>())
        TEST_EQUAL("is_associative_container-SharedPtr",
                   false,IsAssociativeContainer<TestSharedPtr>())
    }//is_associative_container

    {//is_non_associative_container
        TEST_EQUAL("is_non_associative_container-Array",
                   true,IsNonAssociativeContainer<TestArray>())
        TEST_EQUAL("is_non_associative_container-Vector",
                   true,IsNonAssociativeContainer<TestVector>())
        TEST_EQUAL("is_non_associative_container-Deque",
                   true,IsNonAssociativeContainer<TestDeque>())
        TEST_EQUAL("is_non_associative_container-List",
                   true,IsNonAssociativeContainer<TestList>())
        TEST_EQUAL("is_non_associative_container-Set",
                   true,IsNonAssociativeContainer<TestSet>())
        TEST_EQUAL("is_non_associative_container-MultiSet",
                   true,IsNonAssociativeContainer<TestMultiSet>())
        TEST_EQUAL("is_non_associative_container-Map",
                   false,IsNonAssociativeContainer<TestMap>())
        TEST_EQUAL("is_non_associative_container-MultiMap",
                   false,IsNonAssociativeContainer<TestMultiMap>())
        TEST_EQUAL("is_non_associative_container-UnorderedSet",
                   true,IsNonAssociativeContainer<TestUnorderedSet>())
        TEST_EQUAL("is_non_associative_container-UnorderedMultiSet",
                   true,IsNonAssociativeContainer<TestUnorderedMultiSet>())
        TEST_EQUAL("is_non_associative_container-UnorderedMap",
                   false,IsNonAssociativeContainer<TestUnorderedMap>())
        TEST_EQUAL("is_non_associative_container-UnorderedMultiMap",
                   false,IsNonAssociativeContainer<TestUnorderedMultiMap>())
        TEST_EQUAL("is_non_associative_container-FlatMap",
                   false,IsNonAssociativeContainer<TestFlatMap>())
        TEST_EQUAL("is_non_associative_container-ManagedArray",
                   true,IsNonAssociativeContainer<TestManagedArray>())
        TEST_EQUAL("is_non_associative_container-SortedManagedArray",
                   true,IsNonAssociativeContainer<TestSortedManagedArray>())
        TEST_EQUAL("is_non_associative_container-SortedVector",
                   true,IsNonAssociativeContainer<TestSortedVector>())
        TEST_EQUAL("is_non_associative_container-String",
                   true,IsNonAssociativeContainer<TestString>())
        TEST_EQUAL("is_non_associative_container-SharedPtr",
                   false,IsNonAssociativeContainer<TestSharedPtr>())
    }//is_non_associative_container

    {//AddToContainer
        TestVector ResultVector;
        AddToContainer(ResultVector,10);
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-Vector",
                   10,ResultVector.front())

        TestDeque ResultDeque;
        AddToContainer(ResultDeque,10);
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-Deque",
                   10,ResultDeque.front())

        TestList ResultList;
        AddToContainer(ResultList,10);
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-List",
                   10,ResultList.front())

        TestSet ResultSet;
        AddToContainer(ResultSet,10);
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-Set",
                   10,*ResultSet.begin())

        TestMultiSet ResultMultiSet;
        AddToContainer(ResultMultiSet,10);
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-MultiSet",
                   10,*ResultMultiSet.begin())

        TestMap ResultMap;
        AddToContainer(ResultMap,String("Ten"),10);
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-Map-Key",
                   String("Ten"),(*ResultMap.begin()).first)
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-Map-Mapped",
                   10,(*ResultMap.begin()).second)

        TestMultiMap ResultMultiMap;
        AddToContainer(ResultMultiMap,String("Ten"),10);
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-MultiMap-Key",
                   String("Ten"),(*ResultMultiMap.begin()).first)
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-MultiMap-Mapped",
                   10,(*ResultMultiMap.begin()).second)

        TestUnorderedSet ResultUnorderedSet;
        AddToContainer(ResultUnorderedSet,10);
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-UnorderedSet",
                   10,*ResultUnorderedSet.begin())

        TestUnorderedMultiSet ResultUnorderedMultiSet;
        AddToContainer(ResultUnorderedMultiSet,10);
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-UnorderedMultiSet",
                   10,*ResultUnorderedMultiSet.begin())

        TestUnorderedMap ResultUnorderedMap;
        AddToContainer(ResultUnorderedMap,String("Ten"),10);
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-UnorderedMap-Key",
                   String("Ten"),(*ResultUnorderedMap.begin()).first)
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-UnorderedMap-Mapped",
                   10,(*ResultUnorderedMap.begin()).second)

        TestUnorderedMultiMap ResultUnorderedMultiMap;
        AddToContainer(ResultUnorderedMultiMap,String("Ten"),10);
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-UnorderedMultiMap-Key",
                   String("Ten"),(*ResultUnorderedMultiMap.begin()).first)
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-UnorderedMultiMap-Mapped",
                   10,(*ResultUnorderedMultiMap.begin()).second)

        TestFlatMap ResultFlatMap;
        AddToContainer(ResultFlatMap,String("Ten"),10);
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-FlatMap-Key",
                   String("Ten"),(*ResultFlatMap.begin()).first)
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-FlatMap-Mapped",
                   10,(*ResultFlatMap.begin()).second)

        TestManagedArray ResultManagedArray;
        AddToContainer(ResultManagedArray,10);
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-ManagedArray",
                   10,*ResultManagedArray.begin())

        TestSortedManagedArray ResultSortedManagedArray;
        AddToContainer(ResultSortedManagedArray,10);
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-SortedManagedArray",
                   10,*ResultSortedManagedArray.begin())

        TestSortedVector ResultSortedVector;
        AddToContainer(ResultSortedVector,10);
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-SortedVector",
                   10,*ResultSortedVector.begin())

        TestString ResultString;
        AddToContainer(ResultString,'A');
        TEST_EQUAL("AddToContainer(ContainerType&,ValueType&)-String",
                   'A',ResultString.back())
    }//AddToContainer
}

#endif
