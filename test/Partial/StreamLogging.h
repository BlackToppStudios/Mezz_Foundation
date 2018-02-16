// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef Mezz_Test_Boilerplate_h
#define Mezz_Test_Boilerplate_h

/// @file
/// @brief Test Some basic logging stuff.

// Add other headers you need here
#include "MezzTest.h"
#include "StreamLogging.h"

SAVE_WARNING_STATE
SUPPRESS_CLANG_WARNING("-Wweak-vtables") // We really don't care, because this will this will be recompiled each test.
SUPPRESS_CLANG_WARNING("-Wpadded") // Test classes will likely need to be padded and we don't care.

/// @brief The test suite will find all classes identified as tests in CMake, but they need to look like this.
/// @details This class's name is the filename + "Tests"
class StreamLoggingTests : public Mezzanine::Testing::UnitTestGroup
{
    public:
        /// @brief Default Constructor
        StreamLoggingTests() = default;

        /// @brief Default Deconstructor
        virtual ~StreamLoggingTests() = default;

        void RunAutomaticTests()
        {
            // The vast majority of tests will go here.

            // They should use the macros from testdatatools.h to automatically function, filename and line number.
            //TEST(true, "ExamplePassingTest");
        }
        bool HasAutomaticTests() const
            { return true; }


        //void RunSubprocessTest(const Mezzanine::String& Arg) // Add this parameter back in if you need it.
        void RunSubprocessTest(const Mezzanine::String&)
        {
            // Tests with a high risk of crashing can go here

            // If you ever need to debug stuff in here be sure to pass "debugtests" to test executable

            // To enable this test group have HasSubprocessTest() return true
        }
        bool HasSubprocessTest() const
            { return false; }


        void RunInteractiveTests()
        {
            // Tests that require the mushy and weak flesh of human (Until our machine overlords rise up).

            // To enable this test group have HasInteractiveTests() return true
        }
        bool HasInteractiveTests() const
            { return false; }


};

RESTORE_WARNING_STATE
#endif
