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
#include "FunctionalTools.h"
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

    {//is_reverse_range
        TEST_EQUAL("is_reverse_range-Array",
                   true,IsReverseRange<TestArray>())
        TEST_EQUAL("is_reverse_range-Map",
                   true,IsReverseRange<TestMap>())
        TEST_EQUAL("is_reverse_range-String",
                   true,IsReverseRange<TestString>())
        TEST_EQUAL("is_reverse_range-SharedPtr",
                   false,IsReverseRange<TestSharedPtr>())
    }//is_reverse_range

    {//is_const_range
        TEST_EQUAL("is_const_range-Array",
                   true,IsConstRange<TestArray>())
        TEST_EQUAL("is_const_range-Map",
                   true,IsConstRange<TestMap>())
        TEST_EQUAL("is_const_range-String",
                   true,IsConstRange<TestString>())
        TEST_EQUAL("is_const_range-SharedPtr",
                   false,IsConstRange<TestSharedPtr>())
    }//is_const_range

    {//is_const_reverse_range
        TEST_EQUAL("is_const_reverse_range-Array",
                   true,IsConstReverseRange<TestArray>())
        TEST_EQUAL("is_const_reverse_range-Map",
                   true,IsConstReverseRange<TestMap>())
        TEST_EQUAL("is_const_reverse_range-String",
                   true,IsConstReverseRange<TestString>())
        TEST_EQUAL("is_const_reverse_range-SharedPtr",
                   false,IsConstReverseRange<TestSharedPtr>())
    }//is_const_reverse_range
/*
    {//is_range_constructible
        TEST_EQUAL("is_range_constructible-Array",
                   false,ContainerDetect::IsRangeConstructible<TestArray>())
        TEST_EQUAL("is_range_constructible-Vector",
                   true,ContainerDetect::IsRangeConstructible<TestVector>())
        TEST_EQUAL("is_range_constructible-Deque",
                   false,ContainerDetect::IsRangeConstructible<TestDeque>())
        TEST_EQUAL("is_range_constructible-List",
                   false,ContainerDetect::IsRangeConstructible<TestList>())
        TEST_EQUAL("is_range_constructible-Set",
                   false,ContainerDetect::IsRangeConstructible<TestSet>())
        TEST_EQUAL("is_range_constructible-MultiSet",
                   false,ContainerDetect::IsRangeConstructible<TestMultiSet>())
        TEST_EQUAL("is_range_constructible-Map",
                   false,ContainerDetect::IsRangeConstructible<TestMap>())
        TEST_EQUAL("is_range_constructible-MultiMap",
                   false,ContainerDetect::IsRangeConstructible<TestMultiMap>())
        TEST_EQUAL("is_range_constructible-UnorderedSet",
                   false,ContainerDetect::IsRangeConstructible<TestUnorderedSet>())
        TEST_EQUAL("is_range_constructible-UnorderedMultiSet",
                   false,ContainerDetect::IsRangeConstructible<TestUnorderedMultiSet>())
        TEST_EQUAL("is_range_constructible-UnorderedMap",
                   false,ContainerDetect::IsRangeConstructible<TestUnorderedMap>())
        TEST_EQUAL("is_range_constructible-UnorderedMultiMap",
                   false,ContainerDetect::IsRangeConstructible<TestUnorderedMultiMap>())
        TEST_EQUAL("is_range_constructible-FlatMap",
                   false,ContainerDetect::IsRangeConstructible<TestFlatMap>())
        TEST_EQUAL("is_range_constructible-ManagedArray",
                   false,ContainerDetect::IsRangeConstructible<TestManagedArray>())
        TEST_EQUAL("is_range_constructible-SortedManagedArray",
                   false,ContainerDetect::IsRangeConstructible<TestSortedManagedArray>())
        TEST_EQUAL("is_range_constructible-SortedVector",
                   false,ContainerDetect::IsRangeConstructible<TestSortedVector>())
        TEST_EQUAL("is_range_constructible-String",
                   true,ContainerDetect::IsRangeConstructible<TestString>())
        TEST_EQUAL("is_range_constructible-SharedPtr",
                   false,ContainerDetect::IsRangeConstructible<TestSharedPtr>())
    }//is_range_constructible
*/
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

    {//has_push_back_value
        TEST_EQUAL("has_push_back_value-Array",
                   false,ContainerDetect::HasPushBackValue<TestArray>())
        TEST_EQUAL("has_push_back_value-Vector",
                   true,ContainerDetect::HasPushBackValue<TestVector>())
        TEST_EQUAL("has_push_back_value-Deque",
                   true,ContainerDetect::HasPushBackValue<TestDeque>())
        TEST_EQUAL("has_push_back_value-List",
                   true,ContainerDetect::HasPushBackValue<TestList>())
        TEST_EQUAL("has_push_back_value-Set",
                   false,ContainerDetect::HasPushBackValue<TestSet>())
        TEST_EQUAL("has_push_back_value-MultiSet",
                   false,ContainerDetect::HasPushBackValue<TestMultiSet>())
        TEST_EQUAL("has_push_back_value-Map",
                   false,ContainerDetect::HasPushBackValue<TestMap>())
        TEST_EQUAL("has_push_back_value-MultiMap",
                   false,ContainerDetect::HasPushBackValue<TestMultiMap>())
        TEST_EQUAL("has_push_back_value-UnorderedSet",
                   false,ContainerDetect::HasPushBackValue<TestUnorderedSet>())
        TEST_EQUAL("has_push_back_value-UnorderedMultiSet",
                   false,ContainerDetect::HasPushBackValue<TestUnorderedMultiSet>())
        TEST_EQUAL("has_push_back_value-UnorderedMap",
                   false,ContainerDetect::HasPushBackValue<TestUnorderedMap>())
        TEST_EQUAL("has_push_back_value-UnorderedMultiMap",
                   false,ContainerDetect::HasPushBackValue<TestUnorderedMultiMap>())
        TEST_EQUAL("has_push_back_value-FlatMap",
                   false,ContainerDetect::HasPushBackValue<TestFlatMap>())
        TEST_EQUAL("has_push_back_value-ManagedArray",
                   true,ContainerDetect::HasPushBackValue<TestManagedArray>())
        TEST_EQUAL("has_push_back_value-SortedManagedArray",
                   false,ContainerDetect::HasPushBackValue<TestSortedManagedArray>())
        TEST_EQUAL("has_push_back_value-SortedVector",
                   false,ContainerDetect::HasPushBackValue<TestSortedVector>())
        TEST_EQUAL("has_push_back_value-String",
                   true,ContainerDetect::HasPushBackValue<TestString>())
        TEST_EQUAL("has_push_back_value-SharedPtr",
                   false,ContainerDetect::HasPushBackValue<TestSharedPtr>())
    }//has_push_back_value

    {//has_insert_value
        TEST_EQUAL("has_insert_value-Array",
                   false,ContainerDetect::HasInsertValue<TestArray>())
        TEST_EQUAL("has_insert_value-Vector",
                   false,ContainerDetect::HasInsertValue<TestVector>())
        TEST_EQUAL("has_insert_value-Deque",
                   false,ContainerDetect::HasInsertValue<TestDeque>())
        TEST_EQUAL("has_insert_value-List",
                   false,ContainerDetect::HasInsertValue<TestList>())
        TEST_EQUAL("has_insert_value-Set",
                   true,ContainerDetect::HasInsertValue<TestSet>())
        TEST_EQUAL("has_insert_value-MultiSet",
                   true,ContainerDetect::HasInsertValue<TestMultiSet>())
        TEST_EQUAL("has_insert_value-Map",
                   true,ContainerDetect::HasInsertValue<TestMap>())
        TEST_EQUAL("has_insert_value-MultiMap",
                   true,ContainerDetect::HasInsertValue<TestMultiMap>())
        TEST_EQUAL("has_insert_value-UnorderedSet",
                   true,ContainerDetect::HasInsertValue<TestUnorderedSet>())
        TEST_EQUAL("has_insert_value-UnorderedMultiSet",
                   true,ContainerDetect::HasInsertValue<TestUnorderedMultiSet>())
        TEST_EQUAL("has_insert_value-UnorderedMap",
                   true,ContainerDetect::HasInsertValue<TestUnorderedMap>())
        TEST_EQUAL("has_insert_value-UnorderedMultiMap",
                   true,ContainerDetect::HasInsertValue<TestUnorderedMultiMap>())
        TEST_EQUAL("has_insert_value-FlatMap",
                   true,ContainerDetect::HasInsertValue<TestFlatMap>())
        TEST_EQUAL("has_insert_value-ManagedArray",
                   false,ContainerDetect::HasInsertValue<TestManagedArray>())
        TEST_EQUAL("has_insert_value-SortedManagedArray",
                   false,ContainerDetect::HasInsertValue<TestSortedManagedArray>())
        TEST_EQUAL("has_insert_value-SortedVector",
                   false,ContainerDetect::HasInsertValue<TestSortedVector>())
        TEST_EQUAL("has_insert_value-String",
                   false,ContainerDetect::HasInsertValue<TestString>())
        TEST_EQUAL("has_insert_value-SharedPtr",
                   false,ContainerDetect::HasInsertValue<TestSharedPtr>())
    }//has_insert_value

    {//has_add_value
        TEST_EQUAL("has_add_value-Array",
                   false,ContainerDetect::HasAddValue<TestArray>())
        TEST_EQUAL("has_add_value-Vector",
                   false,ContainerDetect::HasAddValue<TestVector>())
        TEST_EQUAL("has_add_value-Deque",
                   false,ContainerDetect::HasAddValue<TestDeque>())
        TEST_EQUAL("has_add_value-List",
                   false,ContainerDetect::HasAddValue<TestList>())
        TEST_EQUAL("has_add_value-Set",
                   false,ContainerDetect::HasAddValue<TestSet>())
        TEST_EQUAL("has_add_value-MultiSet",
                   false,ContainerDetect::HasAddValue<TestMultiSet>())
        TEST_EQUAL("has_add_value-Map",
                   false,ContainerDetect::HasAddValue<TestMap>())
        TEST_EQUAL("has_add_value-MultiMap",
                   false,ContainerDetect::HasAddValue<TestMultiMap>())
        TEST_EQUAL("has_add_value-UnorderedSet",
                   false,ContainerDetect::HasAddValue<TestUnorderedSet>())
        TEST_EQUAL("has_add_value-UnorderedMultiSet",
                   false,ContainerDetect::HasAddValue<TestUnorderedMultiSet>())
        TEST_EQUAL("has_add_value-UnorderedMap",
                   false,ContainerDetect::HasAddValue<TestUnorderedMap>())
        TEST_EQUAL("has_add_value-UnorderedMultiMap",
                   false,ContainerDetect::HasAddValue<TestUnorderedMultiMap>())
        TEST_EQUAL("has_add_value-FlatMap",
                   false,ContainerDetect::HasAddValue<TestFlatMap>())
        TEST_EQUAL("has_add_value-ManagedArray",
                   false,ContainerDetect::HasAddValue<TestManagedArray>())
        TEST_EQUAL("has_add_value-SortedManagedArray",
                   true,ContainerDetect::HasAddValue<TestSortedManagedArray>())
        TEST_EQUAL("has_add_value-SortedVector",
                   true,ContainerDetect::HasAddValue<TestSortedVector>())
        TEST_EQUAL("has_add_value-String",
                   false,ContainerDetect::HasAddValue<TestString>())
        TEST_EQUAL("has_add_value-SharedPtr",
                   false,ContainerDetect::HasAddValue<TestSharedPtr>())
    }//has_add_value

    {//is_stdlib_like_container
        TEST_EQUAL("is_stdlib_like_container-Array",
                   true,IsStdLibLikeContainer<TestArray>())
        TEST_EQUAL("is_stdlib_like_container-Vector",
                   true,IsStdLibLikeContainer<TestVector>())
        TEST_EQUAL("is_stdlib_like_container-Deque",
                   true,IsStdLibLikeContainer<TestDeque>())
        TEST_EQUAL("is_stdlib_like_container-List",
                   true,IsStdLibLikeContainer<TestList>())
        TEST_EQUAL("is_stdlib_like_container-Set",
                   true,IsStdLibLikeContainer<TestSet>())
        TEST_EQUAL("is_stdlib_like_container-MultiSet",
                   true,IsStdLibLikeContainer<TestMultiSet>())
        TEST_EQUAL("is_stdlib_like_container-Map",
                   true,IsStdLibLikeContainer<TestMap>())
        TEST_EQUAL("is_stdlib_like_container-MultiMap",
                   true,IsStdLibLikeContainer<TestMultiMap>())
        TEST_EQUAL("is_stdlib_like_container-UnorderedSet",
                   true,IsStdLibLikeContainer<TestUnorderedSet>())
        TEST_EQUAL("is_stdlib_like_container-UnorderedMultiSet",
                   true,IsStdLibLikeContainer<TestUnorderedMultiSet>())
        TEST_EQUAL("is_stdlib_like_container-UnorderedMap",
                   true,IsStdLibLikeContainer<TestUnorderedMap>())
        TEST_EQUAL("is_stdlib_like_container-UnorderedMultiMap",
                   true,IsStdLibLikeContainer<TestUnorderedMultiMap>())
        TEST_EQUAL("is_stdlib_like_container-FlatMap",
                   true,IsStdLibLikeContainer<TestFlatMap>())
        TEST_EQUAL("is_stdlib_like_container-ManagedArray",
                   true,IsStdLibLikeContainer<TestManagedArray>())
        TEST_EQUAL("is_stdlib_like_container-SortedManagedArray",
                   true,IsStdLibLikeContainer<TestSortedManagedArray>())
        TEST_EQUAL("is_stdlib_like_container-SortedVector",
                   true,IsStdLibLikeContainer<TestSortedVector>())
        TEST_EQUAL("is_stdlib_like_container-String",
                   true,IsStdLibLikeContainer<TestString>())
        TEST_EQUAL("is_stdlib_like_container-SharedPtr",
                   false,IsStdLibLikeContainer<TestSharedPtr>())
    }//is_stdlib_like_container
}

#endif
