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
                   true,is_range_v<TestArray>);
        TEST_EQUAL("is_range-Map",
                   true,is_range_v<TestMap>);
        TEST_EQUAL("is_range-String",
                   true,is_range_v<TestString>);
        TEST_EQUAL("is_range-SharedPtr",
                   false,is_range_v<TestSharedPtr>);
    }//is_range

    {//is_container
        TEST_EQUAL("is_container-Array",
                   true,is_container_v<TestArray>);
        TEST_EQUAL("is_container-Vector",
                   true,is_container_v<TestVector>);
        TEST_EQUAL("is_container-Deque",
                   true,is_container_v<TestDeque>);
        TEST_EQUAL("is_container-List",
                   true,is_container_v<TestList>);
        TEST_EQUAL("is_container-Set",
                   true,is_container_v<TestSet>);
        TEST_EQUAL("is_container-MultiSet",
                   true,is_container_v<TestMultiSet>);
        TEST_EQUAL("is_container-Map",
                   true,is_container_v<TestMap>);
        TEST_EQUAL("is_container-MultiMap",
                   true,is_container_v<TestMultiMap>);
        TEST_EQUAL("is_container-UnorderedSet",
                   true,is_container_v<TestUnorderedSet>);
        TEST_EQUAL("is_container-UnorderedMultiSet",
                   true,is_container_v<TestUnorderedMultiSet>);
        TEST_EQUAL("is_container-UnorderedMap",
                   true,is_container_v<TestUnorderedMap>);
        TEST_EQUAL("is_container-UnorderedMultiMap",
                   true,is_container_v<TestUnorderedMultiMap>);
        TEST_EQUAL("is_container-FlatMap",
                   true,is_container_v<TestFlatMap>);
        TEST_EQUAL("is_container-ManagedArray",
                   true,is_container_v<TestManagedArray>);
        TEST_EQUAL("is_container-SortedManagedArray",
                   true,is_container_v<TestSortedManagedArray>);
        TEST_EQUAL("is_container-SortedVector",
                   true,is_container_v<TestSortedVector>);
        TEST_EQUAL("is_container-String",
                   true,is_container_v<TestString>);
        TEST_EQUAL("is_container-SharedPtr",
                   false,is_container_v<TestSharedPtr>);
    }//is_container

    {//is_key_value_pair
        TEST_EQUAL("is_key_value_pair-Vector",
                   false,is_key_value_pair_v<TestVector::value_type>);
        TEST_EQUAL("is_key_value_pair-Map",
                   true,is_key_value_pair_v<TestMap::value_type>);
        TEST_EQUAL("is_key_value_pair-ManagedArray",
                   false,is_key_value_pair_v<TestManagedArray::value_type>);
        TEST_EQUAL("is_key_value_pair-UnorderedMultiMap",
                   true,is_key_value_pair_v<TestUnorderedMultiMap::value_type>);
        TEST_EQUAL("is_key_value_pair-SharedPtr",
                   false,is_key_value_pair_v<TestSharedPtr>);
    }//is_key_value_pair

    {//is_associative_container
        TEST_EQUAL("is_associative_container-Array",
                   false,is_associative_container_v<TestArray>);
        TEST_EQUAL("is_associative_container-Vector",
                   false,is_associative_container_v<TestVector>);
        TEST_EQUAL("is_associative_container-Deque",
                   false,is_associative_container_v<TestDeque>);
        TEST_EQUAL("is_associative_container-List",
                   false,is_associative_container_v<TestList>);
        TEST_EQUAL("is_associative_container-Set",
                   false,is_associative_container_v<TestSet>);
        TEST_EQUAL("is_associative_container-MultiSet",
                   false,is_associative_container_v<TestMultiSet>);
        TEST_EQUAL("is_associative_container-Map",
                   true,is_associative_container_v<TestMap>);
        TEST_EQUAL("is_associative_container-MultiMap",
                   true,is_associative_container_v<TestMultiMap>);
        TEST_EQUAL("is_associative_container-UnorderedSet",
                   false,is_associative_container_v<TestUnorderedSet>);
        TEST_EQUAL("is_associative_container-UnorderedMultiSet",
                   false,is_associative_container_v<TestUnorderedMultiSet>);
        TEST_EQUAL("is_associative_container-UnorderedMap",
                   true,is_associative_container_v<TestUnorderedMap>);
        TEST_EQUAL("is_associative_container-UnorderedMultiMap",
                   true,is_associative_container_v<TestUnorderedMultiMap>);
        TEST_EQUAL("is_associative_container-FlatMap",
                   true,is_associative_container_v<TestFlatMap>);
        TEST_EQUAL("is_associative_container-ManagedArray",
                   false,is_associative_container_v<TestManagedArray>);
        TEST_EQUAL("is_associative_container-SortedManagedArray",
                   false,is_associative_container_v<TestSortedManagedArray>);
        TEST_EQUAL("is_associative_container-SortedVector",
                   false,is_associative_container_v<TestSortedVector>);
        TEST_EQUAL("is_associative_container-String",
                   false,is_associative_container_v<TestString>);
        TEST_EQUAL("is_associative_container-SharedPtr",
                   false,is_associative_container_v<TestSharedPtr>);
    }//is_associative_container

    {//is_non_associative_container
        TEST_EQUAL("is_non_associative_container-Array",
                   true,is_non_associative_container_v<TestArray>);
        TEST_EQUAL("is_non_associative_container-Vector",
                   true,is_non_associative_container_v<TestVector>);
        TEST_EQUAL("is_non_associative_container-Deque",
                   true,is_non_associative_container_v<TestDeque>);
        TEST_EQUAL("is_non_associative_container-List",
                   true,is_non_associative_container_v<TestList>);
        TEST_EQUAL("is_non_associative_container-Set",
                   true,is_non_associative_container_v<TestSet>);
        TEST_EQUAL("is_non_associative_container-MultiSet",
                   true,is_non_associative_container_v<TestMultiSet>);
        TEST_EQUAL("is_non_associative_container-Map",
                   false,is_non_associative_container_v<TestMap>);
        TEST_EQUAL("is_non_associative_container-MultiMap",
                   false,is_non_associative_container_v<TestMultiMap>);
        TEST_EQUAL("is_non_associative_container-UnorderedSet",
                   true,is_non_associative_container_v<TestUnorderedSet>);
        TEST_EQUAL("is_non_associative_container-UnorderedMultiSet",
                   true,is_non_associative_container_v<TestUnorderedMultiSet>);
        TEST_EQUAL("is_non_associative_container-UnorderedMap",
                   false,is_non_associative_container_v<TestUnorderedMap>);
        TEST_EQUAL("is_non_associative_container-UnorderedMultiMap",
                   false,is_non_associative_container_v<TestUnorderedMultiMap>);
        TEST_EQUAL("is_non_associative_container-FlatMap",
                   false,is_non_associative_container_v<TestFlatMap>);
        TEST_EQUAL("is_non_associative_container-ManagedArray",
                   true,is_non_associative_container_v<TestManagedArray>);
        TEST_EQUAL("is_non_associative_container-SortedManagedArray",
                   true,is_non_associative_container_v<TestSortedManagedArray>);
        TEST_EQUAL("is_non_associative_container-SortedVector",
                   true,is_non_associative_container_v<TestSortedVector>);
        TEST_EQUAL("is_non_associative_container-String",
                   true,is_non_associative_container_v<TestString>);
        TEST_EQUAL("is_non_associative_container-SharedPtr",
                   false,is_non_associative_container_v<TestSharedPtr>);
    }//is_non_associative_container
}

#endif
