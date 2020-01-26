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

#ifndef Mezz_Foundation_StreamLogging_h
#define Mezz_Foundation_StreamLogging_h


// Some headers we don't want swig to see.
#ifndef SWIG

    #include "BitFieldTools.h"

    #include "CrossPlatformExport.h"
    #include "DataTypes.h"
    #include "SuppressWarnings.h"

    #include <iostream>
    #include <ostream>
    #include <string>
    #include <type_traits>

#endif

namespace Mezzanine {
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // LogLevel Declaration and Tools
    //////////////////////////////////////////////////

    /// @brief These are used with IOStreams to control when output is actually written and when it is not.
    /// @details This is a bitfield that is used the LogStream to control the output of specific messages.
    enum class LogLevel : Mezzanine::UInt8 {

        // All the individual settings.
        None        = 0,    ///< No flags are set, indicates no logging occurs. Also useful in binary math.

        Trace       = 1,    ///< This should be used for detailed output that only the craziest case requires.
        Debug       = 2,    ///< Output that is generally not useful, unless it is time to fix a bug.
        Warn        = 4,    ///< Stuff that might help a technician figure a problem out or that could indicate a deeper
                            ///< problem while not being a problem itself.
        Error       = 8,    ///< A problem has happened, but it doesn't require execution to terminate immediately.
        Fatal       = 16,   ///< A problem has happened and it cannot be recovered from, perhaps memory exhaustion or
                            ///< the GPU barfed or something like that.

        // Every log level and those more fatal
        TraceAndHigher      = Trace|Debug|Warn|Error|Fatal,     ///< All of Trace, Debug, Warn, Error and Fatal.
        DebugAndHigher      = Debug|Warn|Error|Fatal,           ///< Includes Debug, Warn, Error and Fatal.
        WarnAndHigher       = Warn|Error|Fatal,                 ///< Includes Warn, Error and Fatal.
        ErrorAndHigher      = Error|Fatal,                      ///< Includes Error and Fatal.
        FatalAndHigher      = Fatal,                            ///< Includes only Fatal.

        // Every log level and those that are more recoverable
        TraceAndLower      = Trace,                             ///< Includes Trace and thats it.
        DebugAndLower      = Trace|Debug,                       ///< Includes Trace and Debug.
        WarnAndLower       = Trace|Debug|Warn,                  ///< Includes Trace, Debug and Warn.
        ErrorAndLower      = Trace|Debug|Warn|Error,            ///< Includes Trace, Debug. Warn and Error.
        FatalAndLower      = Trace|Debug|Warn|Error|Fatal,      ///< Includes Trace, Debug. Warn and Error and Fatal.

        Lowest      = 0,    ///< Is the lowest bitvalue intended to be used for logging.
        Highest     = 16,   ///< IS the highest bitvalue intended to be used for logging.
        FlagLimit   = 31    ///< The highest value used in the most extreme case of this bitfield
    };
    ENABLE_BITMASK_OPERATORS_INSIDE_MEZZANINE(LogLevel)

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // STD Streams configuration
    //////////////////////////////////////////////////

    /// @brief The Standard Streams will filter log messages based on the value passed here.
    /// @details This defaults to Trace and higher which should include all messages.
    void MEZZ_LIB SetStandardLoggingLevel(LogLevel NewLevel);

    /// @brief Retrieve the log loglevel in use by the standard streams
    /// @details This defaults to logging everything (Trace and Higher)
    LogLevel MEZZ_LIB GetStandardLoggingLevel();


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // LogStream and Modifiers
    //////////////////////////////////////////////////

    SAVE_WARNING_STATE
    SUPPRESS_CLANG_WARNING("-Wpadded") // Don't care if this is padded, its a stream.

    /// @brief A stream that tracks its own level of logging.
    /// @tparam CharType This is kind of character that the stream will work with.
    /// @tparam CharTraits The traits (move, equality, assignment, etc...) of the character type passed in
    /// @details This stream will intelligently filter messages with no messing around with the good, fail or bad bits.
    /// Instances of this stream can be configured to treat log messages differently depending on the LogLevel of the
    /// message and Level actively being logged. In other regards this is a normal output stream.
    template<class CharType = Char8, class CharTraits = std::char_traits<CharType> >
    class  BasicLogStream : public std::basic_ostream<CharType, CharTraits>
    {
        /// @brief The LogLevel for stream insertion that will be completed.
        /// @details If messages passed in come in on a level with at least one bit this matching bitfield, then the
        /// message is streamed, otherwise it is dropped silently. This can be thought of as a logging filter.
        LogLevel ActivelyLogging;

    public:
        /// @brief Buffer Stealing Constructor
        /// @details When used to create a LogStream this will use the rdbuf of another std::basic_ostream
        /// @param OutputStream This is the stream to steal the buffer from
        /// @param WillLog The starting LogLevel to capture, which Defaults to TraceAndHigher.
        BasicLogStream(std::basic_ostream<CharType, CharTraits>& OutputStream,
                       LogLevel WillLog = LogLevel::TraceAndHigher)
            : std::basic_ostream<CharType, CharTraits>(OutputStream.rdbuf()),
              ActivelyLogging(WillLog)
            { }

        /// @brief Change the level of what this Stream is logging.
        /// @param NewLevel After this returns any messages sent to this stream will be compared against this.
        void SetLoggingLevel(LogLevel NewLevel)
            { ActivelyLogging = NewLevel; }

        /// @brief What will currently be logged?
        /// @return A LogLevel that is the bitmap of what will be logged.
        LogLevel GetLoggingLevel()
            { return ActivelyLogging; }

        /// @brief Custom Manipulator inserter
        /// @param pf Accepts a pointer to a LogStream Manipulator
        /// @return Whatever stream the manipulator returns which should be *this.
        BasicLogStream<CharType, CharTraits>& operator<<
            (BasicLogStream<CharType, CharTraits>& (*pf) (BasicLogStream<CharType, CharTraits>&))
            { return pf(*this); }
    };

    RESTORE_WARNING_STATE

    /// @brief If you need a simple stream for logging use this.
    /// @details These do not provide their own backing storage of any kind so you must provide one, but this isn't
    /// hard. Simply pass any stream to this one it will steal its readbuffer. Here is a sample with a stringstream, but
    /// you can use any std::ostream:
    /// @code
    /// std::stringstream TestStealRDBuf;
    /// Mezzanine::LogStream TestingLogStream(TestStealRDBuf);
    /// @endcode
    typedef BasicLogStream<> LogStream;

    /// @brief This is a Stream modifier that sets the level of following messages to be inserted into the stream.
    /// @details This specific method is for LogStream, but it is intended to appear outwardly similar to
    /// standard stream manipulators.
    /// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
    /// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
    /// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
    /// @param CurrentLevel The new level that incoming message with be treated as.
    /// @return A reference to the stream being modified.
    template<class CharType, class CharTraits>
    BasicLogStream<CharType, CharTraits>& MessageLogLevel(BasicLogStream<CharType, CharTraits>& OutputStream,
                                                          LogLevel CurrentLevel)
    {
        if((CurrentLevel & OutputStream.GetLoggingLevel()) != LogLevel::None)
            { OutputStream.clear(std::basic_ios<CharType, CharTraits>::goodbit); }
        else
            { OutputStream.setstate(std::basic_ios<CharType, CharTraits>::failbit); }
        return OutputStream;
    }

    /// @brief The stream this is inserted into will treat this following inserted messages as traces.
    /// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
    /// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
    /// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
    /// @return A reference to the stream being modified.
    /// @note When inserted into any standard this affects all standard streams.
    /// @details Sample usage:
    /// @code
    /// LogStream Log(cout);
    /// Log << LogTrace << "Entering function int MeaningOfLifeUniverseAndEverything(42).";
    /// @endcode
    template<class CharType, class CharTraits>
    BasicLogStream<CharType, CharTraits>& LogTrace(BasicLogStream<CharType, CharTraits>& OutputStream)
        { return MessageLogLevel(OutputStream, LogLevel::Trace); }

    /// @brief The stream this is inserted into will treat this following inserted messages as debug messages.
    /// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
    /// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
    /// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
    /// @return A reference to the stream being modified.
    /// @note When inserted into any standard this affects all standard streams.
    /// @details Sample usage:
    /// @code
    /// LogStream Log(cout);
    /// Log << LogDebug << "Sample Log Message.";
    /// @endcode
    template<class CharType, class CharTraits>
    BasicLogStream<CharType, CharTraits>& LogDebug(BasicLogStream<CharType, CharTraits>& OutputStream)
        { return MessageLogLevel(OutputStream, LogLevel::Debug); }

    /// @brief The stream this is inserted into will treat this following inserted messages as warnings.
    /// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
    /// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
    /// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
    /// @return A reference to the stream being modified.
    /// @note When inserted into any standard this affects all standard streams.
    /// @details Sample usage:
    /// @code
    /// LogStream Log(cout);
    /// Log << LogWarn << "Danger Will Robinson, Danger!!!";
    /// @endcode
    template<class CharType, class CharTraits>
    BasicLogStream<CharType, CharTraits>& LogWarn(BasicLogStream<CharType, CharTraits>& OutputStream)
        { return MessageLogLevel(OutputStream, LogLevel::Warn); }

    /// @brief The stream this is inserted into will treat this following inserted messages as errors.
    /// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
    /// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
    /// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
    /// @return A reference to the stream being modified.
    /// @note When inserted into any standard this affects all standard streams.
    /// @details Sample usage:
    /// @code
    /// LogStream Log(cout);
    /// Log << LogError << "The System is dowm, the system is down, Bwoo Dee boo do do.";
    /// @endcode
    template<class CharType, class CharTraits>
    BasicLogStream<CharType, CharTraits>& LogError(BasicLogStream<CharType, CharTraits>& OutputStream)
        { return MessageLogLevel(OutputStream, LogLevel::Error); }

    /// @brief The stream this is inserted into will treat this following inserted messages as fatal.
    /// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
    /// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
    /// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
    /// @return A reference to the stream being modified.
    /// @note When inserted into any standard this affects all standard streams.
    /// @details Sample usage:
    /// @code
    /// LogStream Log(cout);
    /// Log << LogFatal << "The Tuna contains too much Flipper!!!";
    /// @endcode
    template<class CharType, class CharTraits>
    BasicLogStream<CharType, CharTraits>& LogFatal(BasicLogStream<CharType, CharTraits>& OutputStream)
        { return MessageLogLevel(OutputStream, LogLevel::Fatal); }


    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // Standard Stream Modifiers
    //////////////////////////////////////////////////


    /// @brief This is a Stream modifier that sets the level of following messages to be inserted into the stream.
    /// @details This specific method is for standard streams, but it is intended to appear outwardly similar to
    /// LogStream Versions.
    /// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
    /// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
    /// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
    /// @param CurrentLevel The new level the receiving stream should log.
    /// @return A reference to the stream being modified.
    /// @note When inserted into any standard this affects all standard streams.
    template<class CharType, class CharTraits>
    std::basic_ostream<CharType, CharTraits>& MessageLogLevel(std::basic_ostream<CharType,
                                                              CharTraits>& OutputStream,
                                                              LogLevel CurrentLevel)
    {
        if( (CurrentLevel & GetStandardLoggingLevel()) != LogLevel::None )
            { OutputStream.clear(std::basic_ios<CharType, CharTraits>::goodbit); }
        else
            { OutputStream.setstate(std::basic_ios<CharType, CharTraits>::failbit); }
        return OutputStream;
    }

    /// @brief The messages following the insertion of this into a standard stream will be treated as traces.
    /// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
    /// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
    /// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
    /// @return A reference to the stream being modified.
    /// @note When inserted into any standard this affects all standard streams.
    /// @details Here is a sample of use:
    /// @code
    /// cout << LogTrace << "Entering function int MeaningOfLifeUniverseAndEverything(42).";
    /// @endcode
    template<class CharType, class CharTraits>
    std::basic_ostream<CharType, CharTraits>& LogTrace(std::basic_ostream<CharType, CharTraits>& OutputStream)
        { return MessageLogLevel(OutputStream, LogLevel::Trace); }

    /// @brief The messages following the insertion of this into a standard stream will be treated as debug messages.
    /// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
    /// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
    /// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
    /// @return A reference to the stream being modified.
    /// @note When inserted into any standard this affects all standard streams.
    /// @details Here is a sample of use:
    /// @code
    /// cout << LogDebug << "Opening file foo.txt.";
    /// @endcode
    template<class CharType, class CharTraits>
    std::basic_ostream<CharType, CharTraits>& LogDebug(std::basic_ostream<CharType, CharTraits>& OutputStream)
        { return MessageLogLevel(OutputStream, LogLevel::Debug); }

    /// @brief The messages following the insertion of this into a standard stream will be treated as warnings.
    /// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
    /// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
    /// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
    /// @return A reference to the stream being modified.
    /// @note When inserted into any standard this affects all standard streams.
    /// @details Here is a sample of use:
    /// @code
    /// cout << LogWarn << "Danger Will Robinson, Danger!!!";
    /// @endcode
    template<class CharType, class CharTraits>
    std::basic_ostream<CharType, CharTraits>& LogWarn(std::basic_ostream<CharType, CharTraits>& OutputStream)
        { return MessageLogLevel(OutputStream, LogLevel::Warn); }

    /// @brief The messages following the insertion of this into a standard stream will be treated as Errors.
    /// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
    /// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
    /// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
    /// @return A reference to the stream being modified.
    /// @note When inserted into any standard this affects all standard streams.
    /// @details Here is a sample of use:
    /// @code
    /// cout << LogError << "The System is dowm, the system is down, Bwoo Dee boo do do.";
    /// @endcode
    template<class CharType, class CharTraits>
    std::basic_ostream<CharType, CharTraits>& LogError(std::basic_ostream<CharType, CharTraits>& OutputStream)
        { return MessageLogLevel(OutputStream, LogLevel::Error); }

    /// @brief The messages following the insertion of this into a standard stream will be treated as fatal.
    /// @tparam CharType This is used as type inference to allow type safety when inserting this into a stream.
    /// @tparam CharTraits This is used as type inference to allow type safety when inserting this into a stream.
    /// @param OutputStream This is the stream to be modified and is the left hand operand of operator<<.
    /// @return A reference to the stream being modified.
    /// @note When inserted into any standard this affects all standard streams.
    /// @details Here is a sample of use:
    /// @code
    /// cout << LogFatal << "The Tuna contains too much Flipper!!!";
    /// @endcode
    template<class CharType, class CharTraits>
    std::basic_ostream<CharType, CharTraits>& LogFatal(std::basic_ostream<CharType, CharTraits>& OutputStream)
        { return MessageLogLevel(OutputStream, LogLevel::Fatal); }

} // \Mezzanine Namespace

/// @brief Display or stream a LogLevel for human viewing.
/// @param Stream The stream to send the LogLevel to.
/// @param StreamingLevel The LogLevel to stream.
/// @return A reference to the passed stream
/// @details This will produce some human readable version of the LogLevel with that attempts to contain information
/// useful for troubleshooting. Here is a sample that might make this clear:
/// @code
/// LogLevel Filter = LogLevel::DebugAndHigher;
/// std::cout << "Current LogLevel: " << Filter << std::endl;
/// @code
/// This might output something like "Current LogLevel: [LogLevel - 30 - Debug Warn Error Fatal]".
std::ostream& operator << (std::ostream& Stream, Mezzanine::LogLevel StreamingLevel);

#endif //_logging_h
