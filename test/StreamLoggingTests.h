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
#ifndef Mezz_Foundation_StreamLoggingTests_h
#define Mezz_Foundation_StreamLoggingTests_h

/// @file
/// @brief A Simple tests for Stream logging tools

// Add other headers you need here
#include "MezzTest.h"

#include "StreamLogging.h"

// Function declatations
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
    using namespace Mezzanine;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Test Mezzanine::LogStream
    {
        // Test a simple use of normal log streams
        std::stringstream TestStealRDBuf;
        Mezzanine::LogStream TestingLogStream(TestStealRDBuf); // Creating our own logstream

        LogLevel Filter = LogLevel::DebugAndHigher;
        TestingLogStream.SetLoggingLevel(Filter);

        TestingLogStream << "Part 1 - Logging against a LogStream backed by an std::stringstream" << std::endl;
        TestingLogStream << "Current LogLevel: " << Filter << std::endl;
        TestingLogStream << "Should be displayed - Default" << std::endl;
        TestingLogStream << LogTrace << "Should be NOT displayed - Trace" << std::endl;
        TestingLogStream << LogDebug << "Should be displayed - Debug" << std::endl;
        TestingLogStream << LogWarn  << "Should be displayed - Warning" << std::endl;
        TestingLogStream << LogError << "Should be displayed - Error" << std::endl;
        TestingLogStream << LogFatal << "Should be displayed - Fatal" << std::endl;

        Mezzanine::Integer Expected = 5;
        Mezzanine::String Output(TestStealRDBuf.str());
        std::cout << Output
                  << "You should have seen no 'NOT's and " << Expected << " messages with the word 'displayed'. \n"
                  << std::endl;

        TEST_EQUAL("InternalStreamSingleNotsDropped", 0, FindCount(Output, "NOT"));
        TEST_EQUAL("InternalStreamSinglePassedThrough", Expected, FindCount(Output, "Should be displayed"));
    }

    {
        // Tests the OR(|) operator on Log streams

        std::stringstream TestStealRDBuf;
        Mezzanine::LogStream TestingLogStream(TestStealRDBuf); // Creating our own logstream

        LogLevel Filter = LogLevel::Trace | LogLevel::Warn;
        TestingLogStream.SetLoggingLevel(Filter);

        TestingLogStream << "Current LogLevel: " << Filter << std::endl;
        TestingLogStream << "Part 2 - Logging against any other stream" << std::endl;

        TestingLogStream << "Should be displayed - Default" << std::endl;
        TestingLogStream << LogTrace << "Should be displayed - Trace" << std::endl;
        TestingLogStream << LogDebug << "Should NOT be displayed - Debug" << std::endl;
        TestingLogStream << LogWarn  << "Should be displayed - Warning" << std::endl;
        TestingLogStream << LogError << "Should NOT be displayed - Error" << std::endl;
        TestingLogStream << LogFatal << "Should NOT be displayed - Fatal" << std::endl;

        Mezzanine::Integer Expected = 3;
        Mezzanine::String Output(TestStealRDBuf.str());
        std::cout << Output
                  << "You should have seen no 'NOT's and " << Expected << " messages with the word 'displayed'. \n"
                  << std::endl;

        TEST_EQUAL("InternalStreamORdNotsDropped", 0, FindCount(Output, "NOT"));
        TEST_EQUAL("InternalStreamORdPassedThrough", Expected, FindCount(Output, "Should be displayed"));
    }

    {
        // Tests the AND(&) operator on log streams

        std::stringstream TestStealRDBuf;
        Mezzanine::LogStream TestingLogStream(TestStealRDBuf); // Creating our own logstream

        LogLevel Filter = LogLevel::DebugAndHigher & LogLevel::ErrorAndLower;
        TestingLogStream.SetLoggingLevel(Filter);

        SetStandardLoggingLevel( LogLevel::DebugAndHigher & LogLevel::ErrorAndLower);
        TestingLogStream << "Current LogLevel: " << Filter << std::endl;
        TestingLogStream << "Part 3 - Logging against any other stream" << std::endl;

        TestingLogStream << "Should be displayed - Default" << std::endl;
        TestingLogStream << LogTrace << "Should NOT be displayed - Trace" << std::endl;
        TestingLogStream << LogDebug << "Should be displayed - Debug" << std::endl;
        TestingLogStream << LogWarn  << "Should be displayed - Warning" << std::endl;
        TestingLogStream << LogError << "Should be displayed - Error" << std::endl;
        TestingLogStream << LogFatal << "Should NOT be displayed - Fatal" << std::endl;

        Mezzanine::Integer Expected = 4;
        Mezzanine::String Output(TestStealRDBuf.str());
        std::cout << Output
                  << "You should have seen no 'NOT's and " << Expected << " messages with the word 'displayed'. \n"
                  << std::endl;

        TEST_EQUAL("InternalStreamANDedNotsDropped", 0, FindCount(Output, "NOT"));
        TEST_EQUAL("InternalStreamANDedPassedThrough", Expected, FindCount(Output, "Should be displayed"));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Using the logging tools with iostreams
    {
        // Test a simple use of normal iostreams

        std::stringstream TestingLogStream;
        // Not creating log stream, setting globally
        LogLevel Filter = LogLevel::DebugAndHigher;
        SetStandardLoggingLevel(Filter);

        TestingLogStream << "Part 4 - Logging against a LogStream backed by an std::stringstream" << std::endl;
        TestingLogStream << "Current LogLevel: " << Filter << std::endl;
        TestingLogStream << "Should be displayed - Default" << std::endl;
        TestingLogStream << LogTrace << "Should be NOT displayed - Trace" << std::endl;
        TestingLogStream << LogDebug << "Should be displayed - Debug" << std::endl;
        TestingLogStream << LogWarn  << "Should be displayed - Warning" << std::endl;
        TestingLogStream << LogError << "Should be displayed - Error" << std::endl;
        TestingLogStream << LogFatal << "Should be displayed - Fatal" << std::endl;

        Mezzanine::Integer Expected = 5;
        Mezzanine::String Output(TestingLogStream.str());
        std::cout << Output
                  << "You should have seen no 'NOT's and " << Expected << " messages with the word 'displayed'. \n"
                  << std::endl;

        TEST_EQUAL("StandardlStreamSingleNotsDropped", 0, FindCount(Output, "NOT"));
        TEST_EQUAL("StandardStreamSinglePassedThrough", Expected, FindCount(Output, "Should be displayed"));
    }

    {
        // Tests the OR(|) operator globally in std iostreams

        std::stringstream TestingLogStream;
        // Not creating log stream, setting globally
        LogLevel Filter = LogLevel::Trace | LogLevel::Warn;
        SetStandardLoggingLevel(Filter);

        TestingLogStream << "Current LogLevel: " << Filter << std::endl;
        TestingLogStream << "Part 5 - Logging against any other stream" << std::endl;

        TestingLogStream << "Should be displayed - Default" << std::endl;
        TestingLogStream << LogTrace << "Should be displayed - Trace" << std::endl;
        TestingLogStream << LogDebug << "Should NOT be displayed - Debug" << std::endl;
        TestingLogStream << LogWarn  << "Should be displayed - Warning" << std::endl;
        TestingLogStream << LogError << "Should NOT be displayed - Error" << std::endl;
        TestingLogStream << LogFatal << "Should NOT be displayed - Fatal" << std::endl;

        Mezzanine::Integer Expected = 3;
        Mezzanine::String Output(TestingLogStream.str());
        std::cout << Output
                  << "You should have seen no 'NOT's and " << Expected << " messages with the word 'displayed'. \n"
                  << std::endl;

        TEST_EQUAL("StandardStreamORdNotsDropped", 0, FindCount(Output, "NOT"));
        TEST_EQUAL("StandardStreamORdPassedThrough", Expected, FindCount(Output, "Should be displayed"));
    }

    {
        // Tests the AND(&) operator globally in std iostreams

        std::stringstream TestingLogStream;
        // Not creating log stream, setting globally

        LogLevel Filter = LogLevel::DebugAndHigher & LogLevel::ErrorAndLower;
        SetStandardLoggingLevel(Filter);

        TestingLogStream << "Current LogLevel: " << Filter << std::endl;
        TestingLogStream << "Part 6 - Logging against any other stream" << std::endl;

        TestingLogStream << "Should be displayed - Default" << std::endl;
        TestingLogStream << LogTrace << "Should NOT be displayed - Trace" << std::endl;
        TestingLogStream << LogDebug << "Should be displayed - Debug" << std::endl;
        TestingLogStream << LogWarn  << "Should be displayed - Warning" << std::endl;
        TestingLogStream << LogError << "Should be displayed - Error" << std::endl;
        TestingLogStream << LogFatal << "Should NOT be displayed - Fatal" << std::endl;

        Mezzanine::Integer Expected = 4;
        Mezzanine::String Output(TestingLogStream.str());
        std::cout << Output
                  << "You should have seen no 'NOT's and " << Expected << " messages with the word 'displayed'. \n"
                  << std::endl;

        TEST_EQUAL("StandardStreamANDedNotsDropped", 0, FindCount(Output, "NOT"));
        TEST_EQUAL("StandardStreamANDedPassedThrough", Expected, FindCount(Output, "Should be displayed"));
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Streaming operator tests
    {
        // This is just a few checks that the name of each level appears at least once for each LogLevel

        std::stringstream Converter;

        Converter << LogLevel::None;
        TEST("LogLevelNoneStreams", 0 < FindCount(Converter.str(), "None"));
        Converter.str("");

        Converter << LogLevel::Trace;
        TEST("LogLevelTraceStreams", 0 < FindCount(Converter.str(), "Trace"));
        Converter.str("");

        Converter << LogLevel::Debug;
        TEST("LogLevelDebugStreams", 0 < FindCount(Converter.str(), "Debug"));
        Converter.str("");

        Converter << LogLevel::Warn;
        TEST("LogLevelWarnStreams", 0 < FindCount(Converter.str(), "Warn"));
        Converter.str("");

        Converter << LogLevel::Error;
        TEST("LogLevelErrorStreams", 0 < FindCount(Converter.str(), "Error"));
        Converter.str("");

        Converter << LogLevel::Fatal;
        TEST("LogLevelFatalStreams", 0 < FindCount(Converter.str(), "Fatal"));
        Converter.str("");
    }
}

#endif
