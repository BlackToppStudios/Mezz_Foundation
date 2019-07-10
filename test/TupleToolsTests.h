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
#ifndef Mezz_Foundation_TupleToolsTests_h
#define Mezz_Foundation_TupleToolsTests_h

/// @file
/// @brief This file tests the functionality of generic support functions and type-traits for working with tuples.

#include "MezzTest.h"

#include "TupleTools.h"

AUTOMATIC_TEST_GROUP(TupleToolsTests,TupleTools)
{
    using namespace Mezzanine;

    {//tuple_has_type
        using TestTuple = std::tuple<int,float,char>;

        constexpr Boole IntTest = tuple_has_type<int,TestTuple>::value;
        constexpr Boole FloatTest = tuple_has_type<float,TestTuple>::value;
        constexpr Boole CharTest = tuple_has_type<char,TestTuple>::value;
        TEST_EQUAL("tuple_has_type-int",true,IntTest);
        TEST_EQUAL("tuple_has_type-float",true,FloatTest);
        TEST_EQUAL("tuple_has_type-char",true,CharTest);

        constexpr Boole ShortTest = tuple_has_type<short,TestTuple>::value;
        constexpr Boole DoubleTest = tuple_has_type<double,TestTuple>::value;
        constexpr Boole StringTest = tuple_has_type<std::string,TestTuple>::value;
        TEST_EQUAL("tuple_has_type-short",false,ShortTest);
        TEST_EQUAL("tuple_has_type-double",false,DoubleTest);
        TEST_EQUAL("tuple_has_type-string",false,StringTest);
    }//tuple_has_type

    using FirstTupleType = std::tuple<int,float,int>;
    using SecondTupleType = std::tuple<char,short,int>;
    using ThirdTupleType = std::tuple<char,std::string>;

    {//element_as_tuple
        FirstTupleType TestTuple{10,1.0f,20};

        auto Element1 = element_as_tuple<0>(TestTuple,std::make_index_sequence<0>{});
        auto Element2 = element_as_tuple<1>(TestTuple,std::make_index_sequence<1>{});
        auto Element3 = element_as_tuple<2>(TestTuple,std::make_index_sequence<2>{});

        constexpr Boole Result1 = std::is_same_v<std::tuple<int>,decltype(Element1)>;
        constexpr Boole Result2 = std::is_same_v<std::tuple<float>,decltype(Element2)>;
        constexpr Boole Result3 = std::is_same_v<std::tuple<>,decltype(Element3)>;

        TEST_EQUAL("element_as_tuple-Element1-Type",true,Result1);
        TEST_EQUAL("element_as_tuple-Element2-Type",true,Result2);
        TEST_EQUAL("element_as_tuple-Element3-Type",true,Result3);

        TEST_EQUAL("element_as_tuple-Value1",10,std::get<0>(Element1));
        TEST_EQUAL_EPSILON("element_as_tuple-Value2",1.0f,std::get<0>(Element2));
        TEST_EQUAL("element_as_tuple-Value3",size_t(0),std::tuple_size_v<decltype(Element3)>);
    }//element_as_tuple

    {//rebuild_tuple_unique
        FirstTupleType FirstTestTuple{30,0.5f,40};
        SecondTupleType SecondTestTuple{'A',short(65),155};
        auto ThirdTestTuple = std::tuple_cat(FirstTestTuple,SecondTestTuple);

        auto Rebuild1 = rebuild_tuple_unique(FirstTestTuple,std::make_index_sequence<3>{});
        auto Rebuild2 = rebuild_tuple_unique(SecondTestTuple,std::make_index_sequence<3>{});
        auto Rebuild3 = rebuild_tuple_unique(ThirdTestTuple,std::make_index_sequence<6>{});

        constexpr Boole Result1 = std::is_same_v<std::tuple<int,float>,decltype(Rebuild1)>;
        constexpr Boole Result2 = std::is_same_v<std::tuple<char,short,int>,decltype(Rebuild2)>;
        constexpr Boole Result3 = std::is_same_v<std::tuple<int,float,char,short>,decltype(Rebuild3)>;

        TEST_EQUAL("rebuild_tuple_unique-Rebuild1-Type",true,Result1);
        TEST_EQUAL("rebuild_tuple_unique-Rebuild2-Type",true,Result2);
        TEST_EQUAL("rebuild_tuple_unique-Rebuild3-Type",true,Result3);

        TEST_EQUAL("rebuild_tuple_unique-Rebuild1-Value1",30,std::get<0>(Rebuild1));
        TEST_EQUAL_EPSILON("rebuild_tuple_unique-Rebuild1-Value2",0.5f,std::get<1>(Rebuild1));

        TEST_EQUAL("rebuild_tuple_unique-Rebuild2-Value1",'A',std::get<0>(Rebuild2));
        TEST_EQUAL("rebuild_tuple_unique-Rebuild2-Value2",short(65),std::get<1>(Rebuild2));
        TEST_EQUAL("rebuild_tuple_unique-Rebuild2-Value3",155,std::get<2>(Rebuild2));

        TEST_EQUAL("rebuild_tuple_unique-Rebuild3-Value1",30,std::get<0>(Rebuild3));
        TEST_EQUAL_EPSILON("rebuild_tuple_unique-Rebuild3-Value2",0.5f,std::get<1>(Rebuild3));
        TEST_EQUAL("rebuild_tuple_unique-Rebuild3-Value3",'A',std::get<2>(Rebuild3));
        TEST_EQUAL("rebuild_tuple_unique-Rebuild3-Value4",short(65),std::get<3>(Rebuild3));
    }//rebuild_tuple_unique

    {//tuple_cat_unique
        FirstTupleType FirstTestTuple{50,0.25f,60};
        SecondTupleType SecondTestTuple{'Z',short(255),600};
        ThirdTupleType ThirdTestTuple{'Y',std::string("Hi")};

        auto ResultTuple = tuple_cat_unique(FirstTestTuple,SecondTestTuple,ThirdTestTuple);
        constexpr Boole Result = std::is_same_v<std::tuple<int,float,char,short,std::string>,decltype(ResultTuple)>;

        TEST_EQUAL("tuple_cat_unique-Type",true,Result);

        TEST_EQUAL("tuple_cat_unique-Value1",50,std::get<0>(ResultTuple));
        TEST_EQUAL_EPSILON("tuple_cat_unique-Value2",0.25f,std::get<1>(ResultTuple));
        TEST_EQUAL("tuple_cat_unique-Value3",'Z',std::get<2>(ResultTuple));
        TEST_EQUAL("tuple_cat_unique-Value4",short(255),std::get<3>(ResultTuple));
        TEST_EQUAL("tuple_cat_unique-Value5",std::string("Hi"),std::get<4>(ResultTuple));
    }//tuple_cat_unique
}

#endif
