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
#ifndef Mezz_Test_StreamLogging_h
#define Mezz_Test_StreamLogging_h

/// @file
/// @brief A Simple tests for Stream logging tools

// Add other headers you need here
#include "MezzTest.h"

#include "StreamLogging.h"

Mezzanine::Integer FindCount(const Mezzanine::String& Haystack, const Mezzanine::String& Needle);

/// @brief Counts the amount of needles is found in the haystack
/// @param Haystack A String to search through
/// @param Needle A string to find in entirety
/// @todo Use Mezzanine::String later
Mezzanine::Integer FindCount(const Mezzanine::String& Haystack, const Mezzanine::String& Needle)
{
    Mezzanine::Integer Results = 0;
    size_t LastFound = Haystack.find(Needle);
    while(LastFound != Mezzanine::String::npos)
    {
        Results++;
        LastFound = Haystack.find(Needle, LastFound+1);
    }
    return Results;
}

DEFAULT_TEST_GROUP(StreamLoggingTests, StreamLogging)
{
    // Tests should use the macros from TestMacros.h to automatically function, filename and line number.
    TEST("ExamplePassingTest", true);

    {
        std::stringstream TestStealRDBuf;
        LogStream<char> LogOut(TestStealRDBuf);
        LogOut.SetLoggingLevel(LL_DebugAndHigher);
        LogOut << "Part 1 - Logging against a LogStream backedby an std::stringstream" << std::endl;
        LogOut << "Should be displayed - Default" << std::endl;
        LogOut << LogWarn << "Should be displayed - Warning" << std::endl;
        LogOut << LogError << "Should be displayed - Error" << std::endl;
        LogOut << LogTrace << "Should be NOT displayed - Trace" << std::endl;
        LogOut << LogFatal << "Should be displayed - Fatal" << std::endl;
        Mezzanine::String Output(TestStealRDBuf.str());
        std::cout << Output << std::endl;

        TEST_EQUAL("InternalNotsDropped", 0, FindCount(Output, "NOT"));
        TEST_EQUAL("InternalPassedThrough", 4,FindCount(Output, "displayed"));
        std::cout << "You should have seen no 'NOT's and 4 messages with the word 'displayed'." << std::endl;
    }
//    {
//        std::stringstream TestRawStream;
//        SetStandardLoggingLevel( Mezzanine::MergeLogLevel(LL_Trace, LL_Debug, LL_Warn) );
//        TestRawStream << "Part 2 - Logging against any other stream" << std::endl;
//        TestRawStream << "Should be displayed - Default" << std::endl;
//        TestRawStream << LogWarn << "Should be displayed - Warning" << std::endl;
//        TestRawStream << LogError << "Should be NOT displayed - Error" << std::endl;
//        TestRawStream << LogTrace << "Should be displayed - Trace" << std::endl;
//        TestRawStream << LogFatal << "Should be NOT displayed - Fatal" << std::endl;
//        Mezzanine::String Output(TestRawStream.str());
//        std::cout << Output << std::endl;
//        TEST_EQUAL("StandardExternalNotsDropped", 0, FindCount(Output, "NOT"));
//        TEST_EQUAL("StandardExternalPassedThrough", 3, FindCount(Output, "displayed"));
//        std::cout << "You should have seen no 'NOT's and 3 messages with the word 'displayed'."  << std::endl;
//    }
}

#endif
