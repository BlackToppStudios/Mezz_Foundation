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
#ifndef Mezz_Foundation_CommandLine_h
#define Mezz_Foundation_CommandLine_h

/// @file
/// @brief a small set of tools for parsing command line arguments.

#ifndef SWIG
    #include "DataTypes.h"
    #include "FlatMap.h"
#endif

namespace Mezzanine
{

/// @brief Represent an ordered collection of command line arguments as strings.
using ArgVector = std::vector<String>;

/// @brief A mapping of command line switches/flags onto possible argument vectors for those flags.
using ArgMap = Mezzanine::FlatMap<String, ArgVector>;

/// @brief This token is used to prefix arguments/flags on the command line.
const char ArgToken = '-';

/// @brief Convey main args into a vector with no semantic changes
/// @param ArgCount The count of arguments as given to main.
/// @param Arguments The "Argument Vector" as given to main.
/// @return An ArgVector with a 1 to 1 conversion of the inputs without opinions in parsing or tokenizing.
ArgVector MEZZ_LIB VectorizeArgs(int ArgCount,  char** Arguments);

/// @brief An opinionated way to split grouped single letter args into multiple short arguments
/// @param DirtyArg A single arg prepended with '-' that should be split.
/// @return An array of single character args, "-zxf" would yield {"-z", "-x", "-f"}.
ArgVector MEZZ_LIB TokenizeShortArg(const String& DirtyArg);

/// @brief Break a single token into numerous tokens if they are all short args.
/// @param DirtyArg A group of short args like "-a" or "-rf"
/// @return An ArgVector with short tokens for each long token like ["-a"] or ["-r", "-f"]
ArgVector MEZZ_LIB TokenizeSingleArg(const String& DirtyArg);

/// @brief Break any amount of argument tokens into smaller argument tokens
/// @param DirtyArgs A vector of arguments that might include some arguments stuck together.
/// @return A vector containing all the tokens the passed token vector contained, but all completely individual.
ArgVector MEZZ_LIB TokenizeArgVector(const ArgVector& DirtyArgs);

/// @brief Map Arguments onto their parameters. Combine flags that accept params with those params.
/// @param DirtyArgs A vector of fully tokenized args to
/// @return A Map of flags/switchs starting with "-" onto a vector of things without these prefixes.
ArgMap MEZZ_LIB MapArgumentParameters(const ArgVector& DirtyArgs);

/// @brief A collection of arguments and possible following flags.
/// @details This is a simple container for storing for command line arguments after parsing them in a mildly
/// opinionated but common way.
/// @n @n
/// This accepts the Argument Count (commonly argc) and Argument Vector/Values(argv) in the constructor as they are
/// passed into main.
/// @code{.cpp}
/// int main(int ArgC, char** ArgV)
/// {
///      const CommandLineArguments Parsed(ArgC, ArgV);
/// @endcode
/// After this the instannce of CommandLineArguments has only readable members, every member is const and nothing is
/// mutable. This is to insure that if accidents cause compilation errors and allow confidence that uses of this that
/// compile are likely to be correct (or at least only wrong because of runtime logic errors like indexing errors).
/// @n @n
/// This tries to look a little like typical GNU command line arguments. This handles "long"
/// arguments that start with "--" like "--all" and this handles "short" arguments that start with "-" like "-a and
/// combined short forms like "-rf".
/// @n @n
/// This also handles arguments without a "-" prefix by presuming they are parameters for previous arguments. Many
/// arguments need additional information to specify some behavior. Consider a potential zip command, it might accept a
/// "--file" flag to indicate the name of the zip file to work with.
/// @n @n
/// This makes no further guarantees about preserving the order arguments are passed in. For example "-wtf" is the same
/// as "-tfw" are treated the same.
/// @n @n
/// This converts all arguments to keys in a map with a value of a vector of their parameters. Consider this command:
/// @n @n
///     foo.exe -s
/// @n @n
/// This would store "foo.exe" in the data member ExecutableCommand and turns the arg into a dictionary with one key,
/// "-s", and that key would have an empty vector for a value. As an std::initializer_list it might look like:
/// @n @n
///     { {"-s", {} }
/// @n @n
/// All of these examples have the same executable command and but would make slightly data structures and are
/// illustrated as possible initializer lists:
/// @n @n
///
/// Short arguments are broken up, and long arguments are not. Short arguments have one ArgToken ("-"), long have two
/// ArgTokens:
/// @n @n
///    foo.exe -sf --long
/// @n @n
///     {
///         { "--long", {} },
///         { "-s",     {} },
///         { "-w",     {} }
///     }
/// @n @n
/// @code{.cpp}
/// int main(int ArgC, char** ArgV)
/// {
///      const CommandLineArguments Parsed(ArgC, ArgV);
///      std::cout
///         << Parsed.ExecutableCommand // foo.exe
///         << Parsed.Arguments.at("--long").size() // 0
///         << Parsed.Arguments.contains("-s"); // true
/// @endcode
///
/// Items following arguments with not "-" are treating as parameters to the argument they follow.
/// @n @n
///    foo.exe -s --long trailing -w trailing_short
/// @n @n
///     {
///         { "-s",     {} },
///         { "--long", {"trailing"} },
///         { "-w",     {"trailing_short"} }
///     }
/// @n @n
/// @code{.cpp}
/// int main(int ArgC, char** ArgV)
/// {
///      const CommandLineArguments Parsed(ArgC, ArgV);
///      std::cout
///         << Parsed.Arguments.at("--long").at(0) // trailing
///         << Parsed.Arguments.at("-w").at(0); // trailing_short
/// @endcode
///
/// Multiple arguments of the same kind are mapped onto the same key,
/// @n @n
///    foo.exe -wtfbbq
/// @n @n
///     {
///         { "-w",     {} },
///         { "-t",     {} },
///         { "-f",     {} },
///         { "-b",     {} },
///         { "-q",     {} }
///     }
/// @n @n
///
/// If you pass multiple arguments with "-" they keeping appending to that vector in the resulting map even if that
/// argument is passed multiple times on the command line:
/// @n @n
///     foo.exe -a arg1 -pa arg2 arg3 -x arg4
/// @n @n
///     {
///         { "-a", {"arg1", "arg2", "arg3"} },
///         { "-p", {} },
///         { "-x", {"arg4"} }
///     }
/// @n @n
/// @code{.cpp}
/// int main(int ArgC, char** ArgV)
/// {
///      const CommandLineArguments Parsed(ArgC, ArgV);
///      std::cout
///         << Parsed.Arguments.at("-a").at(0)  // arg1
///         << Parsed.Arguments.at("-a").at(1)  // arg2
///         << Parsed.Arguments.at("-a").at(2)  // arg3
///         << Parsed.Arguments.at("-x").at(0)  // arg4
/// @endcode
///
/// All of together might look like:
/// @n @n
///    foo.exe -s --long trailing -wtfbbq suace --long arg2
/// @n @n
///     {
///         { "-s",     {} },
///         { "--long", {"trailing", "arg2"} },
///         { "-w",     {} },
///         { "-t",     {} },
///         { "-f",     {} },
///         { "-b",     {} },
///         { "-q",     {"sauce"} }
///     }
class MEZZ_LIB CommandLineArguments
{
public:
    /// @brief The command used to invokd the executable.
    const String ExecutableCommand;

    /// @brief The arguments parsed with the command GNU semantics included.
    const Mezzanine::ArgMap Arguments;


    /// @brief Constructor From main params. This will fill all values on an instance of this class.
    /// @param ArgCount The count of args from main.
    /// @param ArgValues The Argument Vector or Argument Values from Main.
    CommandLineArguments(int ArgCount,  char** ArgValues);

    /// @brief Not default constructible. because that is non-sense for a read only class.
    CommandLineArguments() = delete;
    /// @brief Not Assignable by value. because that is non-sense for a read only class.
    void operator=(CommandLineArguments&) = delete;
    /// @brief Not Assignable by move. because that is non-sense for a read only class.
    void operator=(CommandLineArguments&&) = delete;

    /// @brief Default copy constructor.
    CommandLineArguments(CommandLineArguments&) = default;
    /// @brief Default move constructor.
    CommandLineArguments(CommandLineArguments&&) = default;
    /// @brief Default virtual destructor
    virtual ~CommandLineArguments() = default;

private:
    /// @brief Get the name of the executable launched here.
    /// @param ArgCount The count of args from main.
    /// @param ArgValues The Argument Vector or Argument Values from Main.
    /// @return The Executable or an empty string.
    String ExecutableFromArgV(int ArgCount,  char** ArgValues) const;

    /// @brief Parse all the args except for the executable name
    /// @param ArgCount The count of args from main.
    /// @param ArgValues The Argument Vector or Argument Values from Main.
    /// @return A mapping of params as MapArgumentParameters would return.
    ArgMap ArgsFromArgV(int ArgCount,  char** ArgValues) const;
};

} //Mezzanine

#endif // Include guard
