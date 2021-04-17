// © Copyright 2010 - 2021 BlackTopp Studios Inc.
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

#include "StreamLogging.h"
#include "SuppressWarnings.h"

namespace Mezzanine {
    SAVE_WARNING_STATE
    SUPPRESS_CLANG_WARNING("-Wmissing-variable-declarations") // This is supposed to be well hiden and located only here.
    LogLevel StdStreamsLoggingFilter = LogLevel::TraceAndHigher;
    RESTORE_WARNING_STATE

    void SetStandardLoggingLevel(LogLevel NewLevel)
        { StdStreamsLoggingFilter = NewLevel; }

    LogLevel GetStandardLoggingLevel()
        { return StdStreamsLoggingFilter; }
} // \Mezzanine Namespace

std::ostream& operator << (std::ostream& Stream, Mezzanine::LogLevel StreamingLevel)
{
    using Mezzanine::LogLevel;

    Stream << "[LogLevel - " << static_cast<Mezzanine::Integer>(StreamingLevel) << " -";

    SAVE_WARNING_STATE
    SUPPRESS_CLANG_WARNING("-Wswitch-enum") // This is a bitfield not all cases can be explicitly covered.
    SUPPRESS_VC_WARNING(4061) // Again, this is a bitfield the extra cases are covered.
    switch(StreamingLevel)
    {
        case LogLevel::None:    Stream << " None"; break;
        case LogLevel::Trace:   Stream << " Trace"; break;
        case LogLevel::Debug:   Stream << " Debug"; break;
        case LogLevel::Warn:    Stream << " Warn"; break;
        case LogLevel::Error:   Stream << " Error"; break;
        case LogLevel::Fatal:   Stream << " Fatal"; break;
        default:
            if((LogLevel::Trace & StreamingLevel) != LogLevel::None) { Stream << " Trace"; }
            if((LogLevel::Debug & StreamingLevel) != LogLevel::None) { Stream << " Debug"; }
            if((LogLevel::Warn  & StreamingLevel) != LogLevel::None) { Stream << " Warn"; }
            if((LogLevel::Error & StreamingLevel) != LogLevel::None) { Stream << " Error"; }
            if((LogLevel::Fatal & StreamingLevel) != LogLevel::None) { Stream << " Fatal"; }
            break;
    }
    RESTORE_WARNING_STATE

    Stream << "]";
    return Stream;
}

