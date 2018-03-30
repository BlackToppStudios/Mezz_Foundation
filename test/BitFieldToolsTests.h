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
#ifndef Mezz_Test_BitFieldTools_h
#define Mezz_Test_BitFieldTools_h

/// @file
/// @brief A Simple tests for Stream logging tools

// Add other headers you need here
#include "MezzTest.h"

#include "BitFieldTools.h"

enum class TestBitField : Mezzanine::UInt8 {
    None    = 0,

    A       = 1,
    B       = 2,
    C       = 4,

    AB      = 3,
    AC      = 5,
    BC      = 6,
    ABC     = 7
};

// If your build fails here on an older version of GCC, it may be a compiler bug, please upgrade.
ENABLE_BITMASK_OPERATORS(TestBitField)

std::ostream& operator << (std::ostream& Stream, TestBitField StreamingBit);

// This is to allow  TEST_EQUAL to stream this sensibly in the case of failure
std::ostream& operator << (std::ostream& Stream, TestBitField StreamingBit)
{
    using InternalType = typename std::underlying_type<TestBitField>::type;
    if(static_cast<InternalType>(TestBitField::A) | static_cast<InternalType>(StreamingBit)) { Stream << "A"; }
    if(static_cast<InternalType>(TestBitField::B) | static_cast<InternalType>(StreamingBit)) { Stream << "B"; }
    if(static_cast<InternalType>(TestBitField::C) | static_cast<InternalType>(StreamingBit)) { Stream << "C"; }
    return Stream;
}

DEFAULT_TEST_GROUP(BitFieldToolsTests, BitFieldTools)
{
    using namespace Mezzanine;

    TEST_EQUAL("OrAB", TestBitField::AB, TestBitField::A | TestBitField::B);
    TEST_EQUAL("OrABC", TestBitField::ABC, TestBitField::A | TestBitField::B | TestBitField::C);
    TEST_EQUAL("OrABandBCisABC", TestBitField::ABC, TestBitField::AB | TestBitField::BC);

    TEST_EQUAL("AndABisNone", TestBitField::None, TestBitField::A & TestBitField::B);
    TEST_EQUAL("AndAandBCisNone", TestBitField::None, TestBitField::A & TestBitField::BC);
    TEST_EQUAL("AndABandBCisB", TestBitField::B, TestBitField::AB & TestBitField::BC);
}

#endif
