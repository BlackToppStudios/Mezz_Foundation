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

#include "CommandLine.h"

#include <iostream>


/// @file
/// @brief This has all the heavy lifting for the command line parsing.

namespace Mezzanine
{

ArgVector VectorizeArgs(int ArgCount, char** Arguments)
{
    ArgVector Results;
    char** EndArgument = Arguments + ArgCount;
    for(char** CurrentArg=Arguments; CurrentArg<EndArgument; CurrentArg++)
        { Results.push_back(*CurrentArg); }
    return Results;
}

ArgVector TokenizeShortArg(const String& DirtyArg)
{
    if(DirtyArg.size() < 2)
        { return  { DirtyArg }; }

    ArgVector results;
    for(String::const_iterator Iter=DirtyArg.cbegin()+1; DirtyArg.cend()!=Iter; Iter++)
        { results.push_back( String("") + ArgToken + *Iter ); }
    return results;
}

ArgVector TokenizeSingleArg(const String& DirtyArg)
{
    // Safety Checks
    if(DirtyArg.empty())
        { return {}; }
    if(DirtyArg.size() == 1)
        { return  { DirtyArg }; }

    // Opinionated Tokenizing
    if(DirtyArg[0]==ArgToken && DirtyArg[1]==ArgToken)
        { return { DirtyArg };}
    if(DirtyArg[0]==ArgToken && DirtyArg[1]!=ArgToken)
        { return TokenizeShortArg(DirtyArg); }

    // If all else fails, leave it.
    return  { DirtyArg };
}

ArgVector TokenizeArgVector(const ArgVector& DirtyArgs)
{
    ArgVector results;
    for(const String& DirtyArg : DirtyArgs)
    {
        for(const String& TokenizedArg : TokenizeSingleArg(DirtyArg))
            { results.push_back(TokenizedArg); }
    }
    return results;
}

ArgMap MapArgumentParameters(const ArgVector& DirtyArgs)
{
    ArgMap Results;
    String LastArg(2, ArgToken);
    for(const String& Arg : DirtyArgs)
    {
        if(Arg.size() && Arg[0]==ArgToken)
        {
            LastArg = Arg;
            Results[Arg];
        } else {
            Results[LastArg].push_back(Arg);
        }
    }

    return Results;
}

CommandLineArguments::CommandLineArguments(int ArgCount, char** ArgValues) :
    ExecutableCommand(ExecutableFromArgV(ArgCount,ArgValues)),
    Arguments(ArgsFromArgV(ArgCount,ArgValues))
    { }

String CommandLineArguments::ExecutableFromArgV(int ArgCount, char** ArgValues) const
    { return (ArgCount && ArgValues && ArgValues[0]) ? ArgValues[0] : ""; }

ArgMap CommandLineArguments::ArgsFromArgV(int ArgCount, char** ArgValues) const
    { return MapArgumentParameters(TokenizeArgVector(VectorizeArgs(ArgCount-1, ArgValues+1))); }

}//Mezzanine
