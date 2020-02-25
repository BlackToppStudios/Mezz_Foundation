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
#ifndef Mezz_Foundation_ExceptionTests_h
#define Mezz_Foundation_ExceptionTests_h

#include "MezzTest.h"

#include "MezzException.h"

/// @file
/// @brief This file tests basic functionality of the Exceptions and verifies the Jagati exceptions are available.

DEFAULT_TEST_GROUP(ExceptionTests,Exception)
{

    try
    {
        throw Mezzanine::Exception::Base("TestException", "TestFunction", "TestFile", 0);
    } catch (const std::exception&) {
        TEST("BaseExceptionCanBeThrownAndCaught", true);
    }

    try
    {
        MEZZ_EXCEPTION(InputOutputCode, "Test Message");
    } catch (const Mezzanine::Exception::InputOutput&) {
        TEST("BaseExceptionCanBeThro"
             "wnWithMEZZ_EXCEPTION", true);
    }

// still need to test members and add the rest of the Foundation Exceptions

    using String = Mezzanine::String;
    using namespace Mezzanine::Exception;


    TEST_EQUAL("CodeToString-Base",
               String("Base"),
               ExceptionClassNameFromCode(ExceptionCode::BaseCode));
    TEST_EQUAL("StringToCode-Base",
               ExceptionCode::BaseCode,
               ExceptionCodeFromClassname("Base"));

    TEST_EQUAL("CodeToString-InputOutput",
               String("InputOutput"),
               ExceptionClassNameFromCode(ExceptionCode::InputOutputCode));
    TEST_EQUAL("StringToCode-InputOutput",
               ExceptionCode::InputOutputCode,
               ExceptionCodeFromClassname("InputOutput"));

/*  // UB!
    TEST_EQUAL("CodeToString-Invalid",
               String("NotAnExceptionCode"),
               ExceptionClassNameFromCode(static_cast<ExceptionCode>(99999999)));*/
    TEST_EQUAL("StringToCode-Invalid",
               ExceptionCode::NotAnExceptionCode,
               ExceptionCodeFromClassname("Not a valid class name."));

}


#endif