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
#ifndef Mezz_Foundation_StaticAnyTests_h
#define Mezz_Foundation_StaticAnyTests_h

/// @file
/// @brief This file tests the functionality of the StaticAny class.

#include "MezzTest.h"

#include "StaticAny.h"

DEFAULT_TEST_GROUP(StaticAnyTests,StaticAny)
{
    using namespace Mezzanine;

    {// Traits
        using AnyType = StaticAny<12,4>;
        static_assert( is_static_any<AnyType>::value, "Supposed to Pass." );
        static_assert( !is_static_any<int>::value, "Supposed to Fail." );
        static_assert( is_static_any_v<AnyType>, "Supposed to Pass." );
        static_assert( !is_static_any_v<const AnyType>, "Supposed to Fail." );
        static_assert( is_static_any_decayed<AnyType&>::value, "Supposed to Pass." );
        static_assert( !is_static_any_decayed<int>::value, "Supposed to Fail." );
        static_assert( is_static_any_decayed_v<const AnyType&>, "Supposed to Pass." );
        static_assert( !is_static_any_decayed_v<float>, "Supposed to Fail." );

        TEST_EQUAL( "is_static_any<ElementType>-Pass", true, is_static_any<AnyType>::value );
        TEST_EQUAL( "is_static_any<ElementType>-Fail", false, is_static_any<int>::value );
        TEST_EQUAL( "is_static_any_v<ElementType>-Pass", true, is_static_any_v<AnyType> );
        TEST_EQUAL( "is_static_any_v<ElementType>-Fail", false, is_static_any_v<const AnyType> );
        TEST_EQUAL( "is_static_any_decayed<ElementType>-Pass", true, is_static_any_decayed<AnyType&>::value );
        TEST_EQUAL( "is_static_any_decayed<ElementType>-Fail", false, is_static_any_decayed<int>::value );
        TEST_EQUAL( "is_static_any_decayed_v<ElementType>-Pass", true, is_static_any_decayed_v<const AnyType&> );
        TEST_EQUAL( "is_static_any_decayed_v<ElementType>-Fail", false, is_static_any_decayed_v<float> );
    }// Traits

    {// Casting
        // Originally this was setting StaticAny<32> as AnyType, but MSVC strings are too big for that.
        // So now AnyType has to be a fatty to match MSVC's fatty-ness.
        using AnyType = StaticAny<40>;

        AnyType StringAny( String("Test") );
        const AnyType ConstStringAny( String("ConstTest") );
        TEST_EQUAL( "StaticAnyCast<String>(StaticAny<>)-Ref-Pass",
                    "Test", StaticAnyCast<String>(StringAny) );
        TEST_EQUAL( "StaticAnyCast<String>(StaticAny<>)-ConstRef-Pass",
                    "ConstTest", StaticAnyCast<String>(ConstStringAny) );
        TEST_THROW( "StaticAnyCast<String>(StaticAny<>)-Ref-Fail",
                    std::bad_cast,
                    [&StringAny](){ StaticAnyCast< std::vector<char> >(StringAny); } );
        TEST_THROW( "StaticAnyCast<String>(StaticAny<>)-ConstRef-Fail",
                    std::bad_cast,
                    [&ConstStringAny](){ StaticAnyCast< std::vector<char> >(ConstStringAny); } );

        double Archimedes(3.1415926);
        const double Eulers(2.7182818);
        AnyType DoubleAny( Archimedes );
        const AnyType ConstDoubleAny( Eulers );
        TEST_EQUAL_EPSILON( "StaticAnyCast<double>(StaticAny<>)-Ptr-Pass",
                            Archimedes, *StaticAnyCast<double>(&DoubleAny) );
        TEST_EQUAL_EPSILON( "StaticAnyCast<double>(StaticAny<>)-ConstPtr-Pass",
                            Eulers, *StaticAnyCast<double>(&ConstDoubleAny) );
        TEST_THROW( "StaticAnyCast<double>(StaticAny<>)-Ptr-Fail",
                    std::bad_cast,
                    [&DoubleAny](){ StaticAnyCast<float>(&DoubleAny); } );
        TEST_THROW( "StaticAnyCast<double>(StaticAny<>)-ConstPtr-Fail",
                    std::bad_cast,
                    [&ConstDoubleAny](){ StaticAnyCast<float>(&ConstDoubleAny); } );

        AnyType SharedPtrAny( std::make_shared<long>( long(123) ) );
        const AnyType ConstSharedPtrAny( std::make_shared<short>( short(12345) ) );
        TEST_EQUAL( "StaticAnyCast<_std::shared_ptr<long>_>(StaticAny<>)-Ref-Pass",
                    long(123), *( StaticAnyCast< std::shared_ptr<long> >(SharedPtrAny) ) );
        TEST_EQUAL( "StaticAnyCast<_std::shared_ptr<short>_>(StaticAny<>)-ConstRef-Pass",
                    short(12345), *( StaticAnyCast< std::shared_ptr<short> >(ConstSharedPtrAny) ) );
        TEST_THROW( "StaticAnyCast<_std::shared_ptr<long>_>(StaticAny<>)-Ref-Fail",
                    std::bad_cast,
                    [&SharedPtrAny](){ StaticAnyCast< std::shared_ptr<char> >(SharedPtrAny); } );
        TEST_THROW( "StaticAnyCast<_std::shared_ptr<short>_>(StaticAny<>)-ConstRef-Fail",
                    std::bad_cast,
                    [&ConstSharedPtrAny](){ StaticAnyCast< std::shared_ptr<char> >(ConstSharedPtrAny); } );
    }// Casting

    {// Simple Construct / Destruct
        using AnyType = StaticAny<32,alignof(std::shared_ptr<String>)>;

        std::shared_ptr<String> StringPtr = std::make_shared<String>("Test");
        AnyType FirstAny( StringPtr );
        TEST_EQUAL( "SimpleConstructElement",
                    2, StringPtr.use_count() );
        FirstAny.clear();
        TEST_EQUAL( "SimpleDestructElement",
                    1, StringPtr.use_count() );
    }// Simple Construct / Destruct

    {// Integer
        using AnyType = StaticAny<sizeof(unsigned),alignof(unsigned)>;
        using SmallerAnyType = StaticAny<sizeof(short),alignof(short)>;

        //
        // Constructors
        //

        AnyType EmptyAny;
        TEST_EQUAL( "StaticAny()-Unsigned",
                    true, EmptyAny.empty() );

        AnyType CopyAnySource( unsigned(321) );
        AnyType CopyAnyDest(CopyAnySource);
        TEST_EQUAL( "StaticAny(const_StaticAny&)-Unsigned",
                    unsigned(321), StaticAnyCast<unsigned>(CopyAnySource) );

        AnyType MoveAnySource( unsigned(12345) );
        AnyType MoveAnyDest( std::move(MoveAnySource) );
        TEST_EQUAL( "StaticAny(StaticAny&&)-Dest-Unsigned",
                    unsigned(12345), StaticAnyCast<unsigned>(MoveAnyDest) );
        TEST_EQUAL( "StaticAny(StaticAny&&)-Source-Unsigned",
                    true, MoveAnySource.empty() );

        SmallerAnyType SmallerAny( short(4321) );
        AnyType CopySmallerAny(SmallerAny);
        TEST_EQUAL( "StaticAny(const_StaticAny<size_t>&)-Unsigned",
                    short(4321), StaticAnyCast<short>(CopySmallerAny) );

        SmallerAnyType SmallerMoveSource( short(123) );
        AnyType SmallerMoveDest( std::move(SmallerMoveSource) );
        TEST_EQUAL( "StaticAny(StaticAny<size_t>&&)-Dest-Unsigned",
                    short(123), StaticAnyCast<short>(SmallerMoveDest) );
        TEST_EQUAL( "StaticAny(StaticAny<size_t>&&)-Source-Unsigned",
                    true, SmallerMoveSource.empty() );

        AnyType AnythingAny( unsigned(1337) );
        TEST_EQUAL( "StaticAny(ElementType&&)-Unsigned",
                    unsigned(1337), StaticAnyCast<unsigned>(AnythingAny) );

        //
        // Assignment operators
        //

        AnyType CopyAssignDest;
        TEST_EQUAL( "operator=(const_StaticAny&)-Empty-Unsigned",
                    true, CopyAssignDest.empty() );
        AnyType CopyAssignSource( unsigned(144) );
        CopyAssignDest = CopyAssignSource;
        TEST_EQUAL( "operator=(const_StaticAny&)-Unsigned",
                    unsigned(144), StaticAnyCast<unsigned>(CopyAssignDest) );

        AnyType MoveAssignDest;
        TEST_EQUAL( "operator=(StaticAny&&)-Empty-Unsigned",
                    true, MoveAssignDest.empty() );
        AnyType MoveAssignSource( unsigned(288) );
        MoveAssignDest = std::move( MoveAssignSource );
        TEST_EQUAL( "operator=(StaticAny&&)-Dest-Unsigned",
                    unsigned(288), StaticAnyCast<unsigned>(MoveAssignDest) );
        TEST_EQUAL( "operator=(StaticAny&&)-Source-Unsigned",
                    true, MoveAssignSource.empty() );

        AnyType CopyAssignSmallerDest;
        TEST_EQUAL( "operator=(const_StaticAny<size_t>&)-Empty-Unsigned",
                    true, CopyAssignSmallerDest.empty() );
        SmallerAnyType CopyAssignSmallerSource( short(576) );
        CopyAssignSmallerDest = CopyAssignSmallerSource;
        TEST_EQUAL( "operator=(const_StaticAny<size_t>&)-Unsigned",
                    short(576), StaticAnyCast<short>(CopyAssignSmallerDest) );

        AnyType MoveAssignSmallerDest;
        TEST_EQUAL( "operator=(StaticAny<size_t>&&)-Empty-Unsigned",
                    true, MoveAssignSmallerDest.empty() );
        SmallerAnyType MoveAssignSmallerSource( short(1152) );
        MoveAssignSmallerDest = std::move( MoveAssignSmallerSource );
        TEST_EQUAL( "operator=(StaticAny<size_t>&&)-Dest-Unsigned",
                    short(1152), StaticAnyCast<short>(MoveAssignSmallerDest) );
        TEST_EQUAL( "operator=(StaticAny<size_t>&&)-Source-Unsigned",
                    true, MoveAssignSmallerSource.empty() );

        AnyType AnythingAnyAssignDest;
        TEST_EQUAL( "operator=(ElementType&&)-Empty-Unsigned",
                    true, AnythingAnyAssignDest.empty() );
        AnythingAnyAssignDest = unsigned(8675309);
        TEST_EQUAL( "operator=(ElementType&&)-Unsigned",
                    unsigned(8675309), StaticAnyCast<unsigned>(AnythingAnyAssignDest) );

        //
        // Utility
        //

        AnyType UtilityAny;
        TEST_EQUAL( "capacity()-Unsigned",
                    sizeof(unsigned), AnyType::capacity() );
        TEST_EQUAL( "capacity()-Short",
                    sizeof(short), SmallerAnyType::capacity() );

        TEST_EQUAL( "align()-Unsigned",
                    alignof(unsigned), AnyType::align() );
        TEST_EQUAL( "align()-Short",
                    alignof(short), SmallerAnyType::align() );

        TEST_EQUAL( "empty()_const-Unassigned-Unsigned",
                    true, UtilityAny.empty() );
        TEST_EQUAL( "size()_const-Unassigned-Unsigned",
                    size_t(0), UtilityAny.size() );
        TEST_EQUAL( "get_type()_const-Unassigned-Unsigned",
                    typeid(void).hash_code(), UtilityAny.get_type().hash_code() );

        UtilityAny.emplace<unsigned>(unsigned(2304));
        TEST_EQUAL( "emplace(ArgTypes&&...)-Unsigned",
                    unsigned(2304), StaticAnyCast<unsigned>(UtilityAny) );

        TEST_EQUAL( "empty()_const-Assigned-Unsigned",
                    false, UtilityAny.empty() );
        TEST_EQUAL( "size()_const-Assigned-Unsigned",
                    sizeof(unsigned), UtilityAny.size() );
        TEST_EQUAL( "get_type()_const-Assigned-Unsigned",
                    typeid(unsigned).hash_code(), UtilityAny.get_type().hash_code() );

        UtilityAny = char('A');
        TEST_EQUAL( "operator=(ElementType&&)-Unsigned-Char",
                    char('A'), StaticAnyCast<char>(UtilityAny) );

        TEST_EQUAL( "empty()_const-Assigned-Unsigned-Char",
                    false, UtilityAny.empty() );
        TEST_EQUAL( "size()_const-Assigned-Unsigned-Char",
                    sizeof(char), UtilityAny.size() );
        TEST_EQUAL( "get_type()_const-Assigned-Unsigned-Char",
                    typeid(char).hash_code(), UtilityAny.get_type().hash_code() );

        UtilityAny.clear();
        TEST_EQUAL( "clear()-Unsigned",
                    true, UtilityAny.empty() );
    }// Integer

    {// String
        using AnyType = StaticAny<sizeof(String),alignof(String)>;
        using SmallerAnyType = StaticAny<(sizeof(char) * 3)>;

        //
        // Constructors
        //

        AnyType EmptyAny;
        TEST_EQUAL( "StaticAny()-String",
                    true, EmptyAny.empty() );

        AnyType CopyAnySource( String("321") );
        AnyType CopyAnyDest(CopyAnySource);
        TEST_EQUAL( "StaticAny(const_StaticAny&)-String",
                    String("321"), StaticAnyCast<String>(CopyAnySource) );

        AnyType MoveAnySource( String("1234") );
        AnyType MoveAnyDest( std::move(MoveAnySource) );
        TEST_EQUAL( "StaticAny(StaticAny&&)-Dest-String",
                    String("1234"), StaticAnyCast<String>(MoveAnyDest) );
        TEST_EQUAL( "StaticAny(StaticAny&&)-Source-String",
                    true, MoveAnySource.empty() );

        SmallerAnyType SmallerAny( char('$') );
        AnyType CopySmallerAny(SmallerAny);
        TEST_EQUAL( "StaticAny(const_StaticAny<size_t>&)-String",
                    char('$'), StaticAnyCast<char>(CopySmallerAny) );

        SmallerAnyType SmallerMoveSource( char('@') );
        AnyType SmallerMoveDest( std::move(SmallerMoveSource) );
        TEST_EQUAL( "StaticAny(StaticAny<size_t>&&)-Dest-String",
                    char('@'), StaticAnyCast<char>(SmallerMoveDest) );
        TEST_EQUAL( "StaticAny(StaticAny<size_t>&&)-Source-String",
                    true, SmallerMoveSource.empty() );

        AnyType AnythingAny( String("1337") );
        TEST_EQUAL( "StaticAny(ElementType&&)-String",
                    String("1337"), StaticAnyCast<String>(AnythingAny) );

        //
        // Assignment operators
        //

        AnyType CopyAssignDest;
        TEST_EQUAL( "operator=(const_StaticAny&)-Empty-String",
                    true, CopyAssignDest.empty() );
        AnyType CopyAssignSource( String("144") );
        CopyAssignDest = CopyAssignSource;
        TEST_EQUAL( "operator=(const_StaticAny&)-String",
                    String("144"), StaticAnyCast<String>(CopyAssignDest) );

        AnyType MoveAssignDest;
        TEST_EQUAL( "operator=(StaticAny&&)-Empty-String",
                    true, MoveAssignDest.empty() );
        AnyType MoveAssignSource( String("288") );
        MoveAssignDest = std::move( MoveAssignSource );
        TEST_EQUAL( "operator=(StaticAny&&)-Dest-String",
                    String("288"), StaticAnyCast<String>(MoveAssignDest) );
        TEST_EQUAL( "operator=(StaticAny&&)-Source-String",
                    true, MoveAssignSource.empty() );

        AnyType CopyAssignSmallerDest;
        TEST_EQUAL( "operator=(const_StaticAny<size_t>&)-Empty-String",
                    true, CopyAssignSmallerDest.empty() );
        SmallerAnyType CopyAssignSmallerSource( char('$') );
        CopyAssignSmallerDest = CopyAssignSmallerSource;
        TEST_EQUAL( "operator=(const_StaticAny<size_t>&)-String",
                    char('$'), StaticAnyCast<char>(CopyAssignSmallerDest) );

        AnyType MoveAssignSmallerDest;
        TEST_EQUAL( "operator=(StaticAny<size_t>&&)-Empty-String",
                    true, MoveAssignSmallerDest.empty() );
        SmallerAnyType MoveAssignSmallerSource( char('@') );
        MoveAssignSmallerDest = std::move( MoveAssignSmallerSource );
        TEST_EQUAL( "operator=(StaticAny<size_t>&&)-Dest-String",
                    char('@'), StaticAnyCast<char>(MoveAssignSmallerDest) );
        TEST_EQUAL( "operator=(StaticAny<size_t>&&)-Source-String",
                    true, MoveAssignSmallerSource.empty() );

        AnyType AnythingAnyAssignDest;
        TEST_EQUAL( "operator=(ElementType&&)-Empty-String",
                    true, AnythingAnyAssignDest.empty() );
        AnythingAnyAssignDest = String("8675309");
        TEST_EQUAL( "operator=(ElementType&&)-String",
                    String("8675309"), StaticAnyCast<String>(AnythingAnyAssignDest) );

        //
        // Utility
        //

        AnyType UtilityAny;
        TEST_EQUAL( "capacity()-String",
                    sizeof(String), AnyType::capacity() );
        TEST_EQUAL( "capacity()-CharString",
                    (sizeof(char) * 3), SmallerAnyType::capacity() );

        TEST_EQUAL( "align()-String",
                    alignof(String), AnyType::align() );
        TEST_EQUAL( "align()-CharString",
                    size_t(4), SmallerAnyType::align() );

        TEST_EQUAL( "empty()_const-Unassigned-String",
                    true, UtilityAny.empty() );
        TEST_EQUAL( "size()_const-Unassigned-String",
                    size_t(0), UtilityAny.size() );
        TEST_EQUAL( "get_type()_const-Unassigned-String",
                    typeid(void).hash_code(),UtilityAny.get_type().hash_code() );

        UtilityAny.emplace<String>("2304");
        TEST_EQUAL( "emplace(ArgTypes&&...)-String",
                    String("2304"), StaticAnyCast<String>(UtilityAny) );

        TEST_EQUAL( "empty()_const-Assigned-String",
                    false, UtilityAny.empty() );
        TEST_EQUAL( "size()_const-Assigned-String",
                    sizeof(String), UtilityAny.size() );
        TEST_EQUAL( "get_type()_const-Assigned-String",
                    typeid(String).hash_code(),UtilityAny.get_type().hash_code() );

        UtilityAny = char('a');
        TEST_EQUAL( "operator=(ElementType&&)-String-Char",
                    char('a'), StaticAnyCast<char>(UtilityAny) );

        TEST_EQUAL( "empty()_const-Assigned-String-Char",
                    false, UtilityAny.empty() );
        TEST_EQUAL( "size()_const-Assigned-String-Char",
                    sizeof(char), UtilityAny.size() );
        TEST_EQUAL( "get_type()_const-Assigned-String-Char",
                    typeid(char).hash_code(),UtilityAny.get_type().hash_code() );

        UtilityAny.clear();
        TEST_EQUAL( "clear()-String",
                    true, UtilityAny.empty());
    }// String
}

#endif // Mezz_Foundation_StaticAnyTests_h
