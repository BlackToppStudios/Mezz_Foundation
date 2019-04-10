// © Copyright 2010 - 2019 BlackTopp Studios Inc.
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
#ifndef Mezz_Foundation_CommandLineTests_h
#define Mezz_Foundation_CommandLineTests_h

/// @file
/// @brief Tests of the CommandLineArguments. The last few serve as decent examples as well.

#include "MezzTest.h"

#include "CommandLine.h"

DEFAULT_TEST_GROUP(CommandLineTests, CommandLine)
{
    using Mezzanine::String;

    using Mezzanine::ArgVector;
    using Mezzanine::VectorizeArgs;
    using Mezzanine::TokenizeShortArg;
    using Mezzanine::TokenizeSingleArg;
    using Mezzanine::TokenizeArgVector;
    using Mezzanine::ArgMap;
    using Mezzanine::MapArgumentParameters;
    using Mezzanine::CommandLineArguments;


    {
        const char* ArgValueArray[] = {
            "foo.exe",
            "-s",
            "--long",
            "trailing",
            "-wtfbbq",
            nullptr
        };
        char** ArgV = const_cast<char**>(ArgValueArray);
        const int ArgC = 5;

        ArgVector VectorizedArgs(VectorizeArgs(ArgC, ArgV));
        TEST_EQUAL("VectorizeArgs - Count",     size_t(5),          VectorizedArgs.size());
        TEST_EQUAL("VectorizeArgs - 0",         String("foo.exe"),  VectorizedArgs.at(0));
        TEST_EQUAL("VectorizeArgs - 1",         String("-s"),       VectorizedArgs.at(1));
        TEST_EQUAL("VectorizeArgs - 2",         String("--long"),   VectorizedArgs.at(2));
        TEST_EQUAL("VectorizeArgs - 3",         String("trailing"), VectorizedArgs.at(3));
        TEST_EQUAL("VectorizeArgs - 4",         String("-wtfbbq"),  VectorizedArgs.at(4));
    }

    // Complex Short Tokenization
    {
        ArgVector ShortTokensizedArgs = TokenizeShortArg("-wtfbbq");
        TEST_EQUAL("TokenizeShortArg - Complex - Count",size_t(6),      ShortTokensizedArgs.size());
        TEST_EQUAL("TokenizeShortArg - Complex - 0",    String("-w"),   ShortTokensizedArgs.at(0));
        TEST_EQUAL("TokenizeShortArg - Complex - 1",    String("-t"),   ShortTokensizedArgs.at(1));
        TEST_EQUAL("TokenizeShortArg - Complex - 2",    String("-f"),   ShortTokensizedArgs.at(2));
        TEST_EQUAL("TokenizeShortArg - Complex - 3",    String("-b"),   ShortTokensizedArgs.at(3));
        TEST_EQUAL("TokenizeShortArg - Complex - 4",    String("-b"),   ShortTokensizedArgs.at(4));
        TEST_EQUAL("TokenizeShortArg - Complex - 5",    String("-q"),   ShortTokensizedArgs.at(5));
    }

    // Simple short tokenization
    {
        ArgVector ShortTokensizedArgs = TokenizeShortArg("-f");
        TEST_EQUAL("TokenizeShortArg - Simple - Count", size_t(1),      ShortTokensizedArgs.size());
        TEST_EQUAL("TokenizeShortArg - Simple - 0",     String("-f"),   ShortTokensizedArgs.at(0));
    }

    // Complex Single Short Tokenization
    {
        ArgVector TokenizedArgs = TokenizeSingleArg("-wtfbbq");
        TEST_EQUAL("TokenizeSingleArg - Short - Count", size_t(6),      TokenizedArgs.size());
        TEST_EQUAL("TokenizeSingleArg - Short - 0",     String("-w"),   TokenizedArgs.at(0));
        TEST_EQUAL("TokenizeSingleArg - Short - 1",     String("-t"),   TokenizedArgs.at(1));
        TEST_EQUAL("TokenizeSingleArg - Short - 2",     String("-f"),   TokenizedArgs.at(2));
        TEST_EQUAL("TokenizeSingleArg - Short - 3",     String("-b"),   TokenizedArgs.at(3));
        TEST_EQUAL("TokenizeSingleArg - Short - 4",     String("-b"),   TokenizedArgs.at(4));
        TEST_EQUAL("TokenizeSingleArg - Short - 5",     String("-q"),   TokenizedArgs.at(5));
    }

    // Complex Single Long Tokenization
    {
        ArgVector TokensizedArgs = TokenizeSingleArg("--wtfbbq");
        TEST_EQUAL("TokenizeSingleArg - Long - Count", size_t(1),           TokensizedArgs.size());
        TEST_EQUAL("TokenizeSingleArg - Long - 0",     String("--wtfbbq"),  TokensizedArgs.at(0));
    }

    // Complex Multiple Tokenization
    {
        ArgVector TokenizedArgs = TokenizeArgVector({"-lmfao", "--lmfao"});
        TEST_EQUAL("TokenizeArgVector - Count", size_t(6),          TokenizedArgs.size());
        TEST_EQUAL("TokenizeArgVector - 0",     String("-l"),       TokenizedArgs.at(0));
        TEST_EQUAL("TokenizeArgVector - 1",     String("-m"),       TokenizedArgs.at(1));
        TEST_EQUAL("TokenizeArgVector - 2",     String("-f"),       TokenizedArgs.at(2));
        TEST_EQUAL("TokenizeArgVector - 3",     String("-a"),       TokenizedArgs.at(3));
        TEST_EQUAL("TokenizeArgVector - 4",     String("-o"),       TokenizedArgs.at(4));
        TEST_EQUAL("TokenizeArgVector - 5",     String("--lmfao"),  TokenizedArgs.at(5));
    }

    {
        ArgVector TokenizedArgs({"-z", "-x", "-f", "input.tar", "-v"});
        ArgMap MappedArgs = MapArgumentParameters(TokenizedArgs);
        TEST_EQUAL("MapArgumentParameters - Count",         size_t(4),              MappedArgs.size());
        TEST_EQUAL("MapArgumentParameters - z 0",           size_t(0),              MappedArgs.at("-z").size());
        TEST_EQUAL("MapArgumentParameters - x 0",           size_t(0),              MappedArgs.at("-x").size());
        TEST_EQUAL("MapArgumentParameters - f 1",           size_t(1),              MappedArgs.at("-f").size());
        TEST_EQUAL("MapArgumentParameters - v 0",           size_t(0),              MappedArgs.at("-v").size());
        TEST_EQUAL("MapArgumentParameters - file false",    false,                  MappedArgs.contains("input.tar"));
        TEST_EQUAL("MapArgumentParameters - f with file",   String("input.tar"),    MappedArgs.at("-f").at(0));

        ArgVector DefaultArgs({"input.tar"});
        ArgMap MappedDefaultArgs = MapArgumentParameters(DefaultArgs);
        TEST_EQUAL("MapArgsDefault - Count",                size_t(1),              MappedDefaultArgs.size());
        TEST_EQUAL("MapArgsDefault - Default --",           true,                   MappedDefaultArgs.contains("--"));
        TEST_EQUAL("MapArgsDefault - filename exist",       String("input.tar"),    MappedDefaultArgs.at("--").at(0));

        ArgVector EmptyArgs;
        ArgMap MappedEmptyArgs = MapArgumentParameters(EmptyArgs);
        TEST_EQUAL("MapArgsEmpty - Count",         size_t(0),   MappedEmptyArgs.size());
    }

    // The final classes for use by end users.
    {
        const char* ArgValueArray[] = {
            "foo.exe",
            "-s",
            "--long",
            "trailing",
            "-wtfbbq",
            nullptr
        };
        char** ArgV = const_cast<char**>(ArgValueArray);
        const int ArgC = 5;

        const CommandLineArguments Parsed(ArgC, ArgV);

        TEST_EQUAL("CommandLineArguments - ExecutableCommand",      String("foo.exe"),   Parsed.ExecutableCommand);
        TEST_EQUAL("CommandLineArguments - ArgCount",   size_t(7),              Parsed.Arguments.size());
        TEST_EQUAL("CommandLineArguments - s",          size_t(0),              Parsed.Arguments.at("-s").size());
        TEST_EQUAL("CommandLineArguments - long",       size_t(1),              Parsed.Arguments.at("--long").size());
        TEST_EQUAL("CommandLineArguments - long arg",   String("trailing"),     Parsed.Arguments.at("--long").at(0));
        TEST_EQUAL("CommandLineArguments - w",          size_t(0),              Parsed.Arguments.at("-w").size());
        TEST_EQUAL("CommandLineArguments - t",          size_t(0),              Parsed.Arguments.at("-t").size());
        TEST_EQUAL("CommandLineArguments - f",          size_t(0),              Parsed.Arguments.at("-f").size());
        TEST_EQUAL("CommandLineArguments - b",          size_t(0),              Parsed.Arguments.at("-b").size());
        TEST_EQUAL("CommandLineArguments - q",          size_t(0),              Parsed.Arguments.at("-q").size());
    }

    // The final classes for use by end users.
    {
        const char* ArgValueArray[] = {
            "foo.exe",
            "-a",
            "arg1",
            "-pa",
            "arg2",
            "arg3",
            "-x",
            "arg4",
            nullptr
        };
        char** ArgV = const_cast<char**>(ArgValueArray);
        const int ArgC = 8;

        const CommandLineArguments Parsed(ArgC, ArgV);

        TEST_EQUAL("multipleParams - ExecutableCommand",    String("foo.exe"),  Parsed.ExecutableCommand);
        TEST_EQUAL("multipleParams - ArgCount",             size_t(3),          Parsed.Arguments.size());
        TEST_EQUAL("multipleParams - x",                    size_t(1),          Parsed.Arguments.at("-x").size());
        TEST_EQUAL("multipleParams - p",                    size_t(0),          Parsed.Arguments.at("-p").size());
        TEST_EQUAL("multipleParams - z",                    size_t(3),          Parsed.Arguments.at("-a").size());
        TEST_EQUAL("multipleParams - x1",                   String("arg4"),     Parsed.Arguments.at("-x").at(0));
        TEST_EQUAL("multipleParams - a1",                   String("arg1"),     Parsed.Arguments.at("-a").at(0));
        TEST_EQUAL("multipleParams - a2",                   String("arg2"),     Parsed.Arguments.at("-a").at(1));
        TEST_EQUAL("multipleParams - a3",                   String("arg3"),     Parsed.Arguments.at("-a").at(2));
    }

}

#endif
