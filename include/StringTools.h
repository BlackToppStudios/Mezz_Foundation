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
#ifndef MEZZ_Foundation_StringTools_h
#define MEZZ_Foundation_StringTools_h

#ifndef SWIG
    #include "DataTypes.h"
#endif

/// @file
/// @brief This file includes a collection of common String manipulation utilities.

namespace Mezzanine {
namespace StringTools {
    /// @brief Convenience type used by functions in this namespace for a String iterator.
    using StrIter = String::iterator;
    /// @brief Convenience type used by functions in this namespace for a const String iterator.
    using ConstStrIter = String::const_iterator;

    ///////////////////////////////////////////////////////////////////////////////
    // Character Type Traits

    /// @brief Helper class for checking if a type is a valid character type.
    /// @tparam FailType If we're here, then this type isn't a valid character type.
    /// @remarks This is the catch-all failure type.
    template<typename FailType>
    struct is_char_helper : std::false_type
        {  };
    /// @brief Helper specialization class for checking if a type is a valid character type.
    /// @remarks This is the type used when a char is detected.
    template<>
    struct is_char_helper<char> : std::true_type
        {  };
    /// @brief Helper specialization class for checking if a type is a valid character type.
    /// @remarks This is the type used when an unsigned char is detected.
    template<>
    struct is_char_helper<unsigned char> : std::true_type
        {  };
    /// @brief Helper specialization class for checking if a type is a valid character type.
    /// @remarks This is the type used when an signed char is detected.
    template<>
    struct is_char_helper<signed char> : std::true_type
        {  };
    /// @brief Helper specialization class for checking if a type is a valid character type.
    /// @remarks This is the type used when a wchar_t is detected.
    template<>
    struct is_char_helper<wchar_t> : std::true_type
        {  };
    /// @brief Helper specialization class for checking if a type is a valid character type.
    /// @remarks This is the type used when a char16_t is detected.
    template<>
    struct is_char_helper<char16_t> : std::true_type
        {  };
    /// @brief Helper specialization class for checking if a type is a valid character type.
    /// @remarks This is the type used when a char32_t is detected.
    template<>
    struct is_char_helper<char32_t> : std::true_type
        {  };
    /// @brief A type trait for detecting a valid character type.
    /// @tparam CheckType The type to be checked.
    /// @remarks This ultimately inherits from either std::true_type or std::false_type and
    /// the bool value can be checked via the static "value" member.
    template<typename CheckType>
    struct is_char : is_char_helper< std::remove_cv_t< std::remove_reference_t<CheckType> > >
        {  };

    /// @brief Convenience function for getting just the bool of the is_char check.
    /// @tparam CheckType The type to check if it is a character type.
    /// @return Returns true if the CheckType is a character type, false otherwise.
    template<class CheckType>
    constexpr Boole IsChar()
        { return is_char<CheckType>::value; }

    ///////////////////////////////////////////////////////////////////////////////
    // String Type Traits

    /// @brief Helper class for checking if a type is a valid string type.
    /// @tparam FailType If we're here, then this type isn't a valid string type.
    /// @remarks This is the catch-all failure type.
    template<typename FailType>
    struct is_string_helper : std::false_type
        {  };
    /// @brief Helper specialization class for checking if a type is a valid string type.
    /// @remarks This is the type used when a std::string is detected.
    template<>
    struct is_string_helper<std::string> : std::true_type
        {  };
    /// @brief Helper specialization class for checking if a type is a valid string type.
    /// @remarks This is the type used when an std::string_view is detected.
    template<>
    struct is_string_helper<std::string_view> : std::true_type
        {  };
    /// @brief A type trait for detecting a valid string type.
    /// @tparam CheckType The type to be checked.
    /// @remarks This ultimately inherits from either std::true_type or std::false_type and
    /// the bool value can be checked via the static "value" member.
    /// @warning This will NOT match c-strings because they are icky.
    template<typename CheckType>
    struct is_string : is_string_helper< std::remove_cv_t< std::remove_reference_t<CheckType> > >
        {  };

    /// @brief Convenience function for getting just the bool of the is_string check.
    /// @tparam CheckType The type to check if it is a string type.
    /// @return Returns true if the CheckType is a std::string or std::string_view, false otherwise.
    template<typename CheckType>
    constexpr Boole IsString()
        { return is_string<CheckType>::value; }

    ///////////////////////////////////////////////////////////////////////////////
    // Character Checks

    /// @brief Checks if a character is a space.
    /// @tparam CharType The (hopefully valid) character type to check.
    /// @param ToCheck The character to be checked.
    /// @return Returns true if the character is a space, false otherwise.
    template<typename CharType, typename = std::enable_if_t< is_char<CharType>::value > >
    constexpr Boole IsSpace(const CharType ToCheck)
        { return ( ToCheck == ' ' ); }
    /// @brief Checks if a character is a tab.
    /// @tparam CharType The (hopefully valid) character type to check.
    /// @param ToCheck The character to be checked.
    /// @return Returns true if the character is a tab, false otherwise.
    template<typename CharType, typename = std::enable_if_t< is_char<CharType>::value > >
    constexpr Boole IsTab(const CharType ToCheck)
        { return ( ToCheck == '\t' || ToCheck == '\v' ); }
    /// @brief Checks if a character is a newline.
    /// @tparam CharType The (hopefully valid) character type to check.
    /// @param ToCheck The character to be checked.
    /// @return Returns true if the character is a "\r" or "\n" newline, false otherwise.
    template<typename CharType, typename = std::enable_if_t< is_char<CharType>::value > >
    constexpr Boole IsNewline(const CharType ToCheck)
        { return ( ToCheck == '\r' || ToCheck == '\n' ); }
    /// @brief Checks if a character is a whitespace.
    /// @tparam CharType The (hopefully valid) character type to check.
    /// @param ToCheck The character to be checked.
    /// @return Returns true if the character is a whitespace, false otherwise.
    template<typename CharType, typename = std::enable_if_t< is_char<CharType>::value > >
    constexpr Boole IsWhitespace(const CharType ToCheck)
        { return ( IsSpace(ToCheck) || IsTab(ToCheck) || IsNewline(ToCheck) ); }
    /// @brief Checks if a character is a decimal digit.
    /// @tparam CharType The (hopefully valid) character type to check.
    /// @param ToCheck The character to be checked.
    /// @return Returns true if the character is a decimal digit, false otherwise.
    template<typename CharType, typename = std::enable_if_t< is_char<CharType>::value > >
    constexpr Boole IsDigit(const CharType ToCheck)
        { return ( ToCheck >= '0' && ToCheck <= '9' ); }
    /// @brief Checks if a character is a lower-case alphabetic letter.
    /// @tparam CharType The (hopefully valid) character type to check.
    /// @param ToCheck The character to be checked.
    /// @return Returns true if the character is a lower-case letter, false otherwise.
    template<typename CharType, typename = std::enable_if_t< is_char<CharType>::value > >
    constexpr Boole IsLowerAlphaLetter(const CharType ToCheck)
        { return ( ToCheck >= 'a' && ToCheck <= 'z' ); }
    /// @brief Checks if a character is a upper-case alphabetic letter.
    /// @tparam CharType The (hopefully valid) character type to check.
    /// @param ToCheck The character to be checked.
    /// @return Returns true if the character is a upper-case letter, false otherwise.
    template<typename CharType, typename = std::enable_if_t< is_char<CharType>::value > >
    constexpr Boole IsUpperAlphaLetter(const CharType ToCheck)
        { return ( ToCheck >= 'A' && ToCheck <= 'Z' ); }
    /// @brief Checks if a character is a lower or upper case letter.
    /// @tparam CharType The (hopefully valid) character type to check.
    /// @param ToCheck The character to be checked.
    /// @return Returns true if the character is a lower or upper case letter, false otherwise.
    template<typename CharType, typename = std::enable_if_t< is_char<CharType>::value > >
    constexpr Boole IsAlphaLetter(const CharType ToCheck)
        { return ( IsLowerAlphaLetter(ToCheck) || IsUpperAlphaLetter(ToCheck) ); }
    /// @brief Checks if a character is a lower-case hexadecimal letter.
    /// @tparam CharType The (hopefully valid) character type to check.
    /// @param ToCheck The character to be checked.
    /// @return Returns true if the character is a lower-case letter, false otherwise.
    template<typename CharType, typename = std::enable_if_t< is_char<CharType>::value > >
    constexpr Boole IsLowerHexLetter(const CharType ToCheck)
        { return ( ToCheck >= 'a' && ToCheck <= 'f' ); }
    /// @brief Checks if a character is a upper-case hexadecimal letter.
    /// @tparam CharType The (hopefully valid) character type to check.
    /// @param ToCheck The character to be checked.
    /// @return Returns true if the character is a upper-case letter, false otherwise.
    template<typename CharType, typename = std::enable_if_t< is_char<CharType>::value > >
    constexpr Boole IsUpperHexLetter(const CharType ToCheck)
        { return ( ToCheck >= 'A' && ToCheck <= 'F' ); }
    /// @brief Checks if a character is a hexadecimal letter.
    /// @tparam CharType The (hopefully valid) character type to check.
    /// @param ToCheck The character to be checked.
    /// @return Returns true if the character is a hexadecimal letter, false otherwise.
    template<typename CharType, typename = std::enable_if_t< is_char<CharType>::value > >
    constexpr Boole IsHexLetter(const CharType ToCheck)
        { return ( IsLowerHexLetter(ToCheck) || IsUpperHexLetter(ToCheck) ); }
    /// @brief Checks if a character is a hexadecimal digit.
    /// @tparam CharType The (hopefully valid) character type to check.
    /// @param ToCheck The character to be checked.
    /// @return Returns true if the character is a hexadecimal digit, false otherwise.
    template<typename CharType, typename = std::enable_if_t< is_char<CharType>::value > >
    constexpr Boole IsHexDigit(const CharType ToCheck)
        { return ( IsDigit(ToCheck) || IsHexLetter(ToCheck) ); }
    /// @brief Checks if a character is a letter or digit.
    /// @tparam CharType The (hopefully valid) character type to check.
    /// @param ToCheck The character to be checked.
    /// @return Returns true if the character is a letter or digit, false otherwise.
    template<typename CharType, typename = std::enable_if_t< is_char<CharType>::value > >
    constexpr Boole IsAlphanumeric(const CharType ToCheck)
        { return ( IsDigit(ToCheck) || IsAlphaLetter(ToCheck) ); }

    ///////////////////////////////////////////////////////////////////////////////
    // String Manipulation

    /// @brief Converts all lower case characters in a character range to upper case.
    /// @param Begin An iterator to the start of the range to modify.
    /// @param End An iterator to one-passed-the-last in the range to modify.
    void MEZZ_LIB ToUpperCase(StrIter Begin, StrIter End);
    /// @brief Create a copy of the String that is upper case.
    /// @param Source The String that will be the basis for the modified String.
    /// @return Returns a copy of the String that has been modified to be in all upper case.
    String MEZZ_LIB UpperCaseCopy(String Source);
    /// @brief Converts all upper case characters in a character range to lower case.
    /// @param Begin An iterator to the start of the range to modify.
    /// @param End An iterator to one-passed-the-last in the range to modify.
    void MEZZ_LIB ToLowerCase(StrIter Begin, StrIter End);
    /// @brief Create a copy of the String that is lower case.
    /// @param Source The String that will be the basis for the modified String.
    /// @return Returns a copy of the String that has been modified to be in all lower case.
    String MEZZ_LIB LowerCaseCopy(String Source);
    /// @brief Converts the first letter of each word to upper case and all other letters to lower case.
    /// @param Begin An iterator to the start of the range to modify.
    /// @param End An iterator to one-passed-the-last in the range to modify.
    void MEZZ_LIB ToPascalCase(StrIter Begin, StrIter End);
    /// @brief Gets a copy of a String with the first letter of each word upper cased and other letters lower cased.
    /// @param Source The String that will be the basis for the modified String.
    /// @return Returns a copy of the String that has been modified to be in Pascal Case.
    String MEZZ_LIB PascalCaseCopy(String Source);

    ///////////////////////////////////////////////////////////////////////////////
    // Size Modifying String Manipulations

    /// @brief Trims all whitespace characters from both sides of a String.
    /// @param Begin An iterator to the start of the range to modify.
    /// @param End An iterator to one-passed-the-last in the range to modify.
    /// @return Returns an iterator to the new end of the provided range.
    StrIter MEZZ_LIB Trim(StrIter Begin, StrIter End);
    /// @brief Same as Trim, but in a copy of the iterator range so the original isn't modified.
    /// @param Begin An iterator to the start of the range to copy and modify.
    /// @param End An iterator to one-passed-the-last in the range to copy and modify.
    /// @return Returns a String to the copied and modified range.
    String MEZZ_LIB TrimCopy(ConstStrIter Begin, ConstStrIter End);

    /// @brief Trims all whitespace characters from the left side of a String.
    /// @param Begin An iterator to the start of the range to modify.
    /// @param End An iterator to one-passed-the-last in the range to modify.
    /// @return Returns an iterator to the new end of the provided range.
    StrIter MEZZ_LIB TrimLeft(StrIter Begin, StrIter End);
    /// @brief Same as TrimLeft, but in a copy of the iterator range so the original isn't modified.
    /// @param Begin An iterator to the start of the range to copy and modify.
    /// @param End An iterator to one-passed-the-last in the range to copy and modify.
    /// @return Returns a String to the copied and modified range.
    String MEZZ_LIB TrimLeftCopy(ConstStrIter Begin, ConstStrIter End);

    /// @brief Trims all whitespace characters from the right side of a String.
    /// @param Begin An iterator to the start of the range to modify.
    /// @param End An iterator to one-passed-the-last in the range to modify.
    /// @return Returns an iterator to the new end of the provided range.
    StrIter MEZZ_LIB TrimRight(StrIter Begin, StrIter End);
    /// @brief Same as TrimRight, but in a copy of the iterator range so the original isn't modified.
    /// @param Begin An iterator to the start of the range to copy and modify.
    /// @param End An iterator to one-passed-the-last in the range to copy and modify.
    /// @return Returns a String to the copied and modified range.
    String MEZZ_LIB TrimRightCopy(ConstStrIter Begin, ConstStrIter End);

    /// @brief Replaces all occurrences of any repeating whitespace character to just one of that whitespace character.
    /// @param Begin An iterator to the start of the range to modify.
    /// @param End An iterator to one-passed-the-last in the range to modify.
    /// @return Returns an iterator to the new end of the provided range.
    StrIter MEZZ_LIB RemoveDuplicateWhitespaces(StrIter Begin, StrIter End);
    /// @brief Same as RemoveDuplicateWhitespaces, but in a copy of the iterator range so the original isn't modified.
    /// @param Begin An iterator to the start of the range to copy and modify.
    /// @param End An iterator to one-passed-the-last in the range to copy and modify.
    /// @return Returns a String to the copied and modified range.
    String MEZZ_LIB RemoveDuplicateWhitespacesCopy(ConstStrIter Begin, ConstStrIter End);

    ///////////////////////////////////////////////////////////////////////////////
    // Colour Channels

    /// @brief Converts a string containing hex to a ColourValue channel.
    /// @param Hex The Hex value to be converted. Must be a 2 character String.
    /// @return Returns a Real representing the converted Hex string.
    Real ConvertHexToColourChannel(const StringView Hex);
    /// @brief Converts a ColourValue channel to Hex.
    /// @remarks This method will produce a two character String of hexidecimal values. If needed, the "0x" prefix
    /// would have to be applied manually.
    /// @param Channel The value to be converted to Hex.
    /// @return Returns a two character string containing the hex expression for the provided channel value.
    String ConvertColourChannelToHex(const Real Channel);

    ///////////////////////////////////////////////////////////////////////////////
    // Tokenizing

    /// @brief Splits a String into multiple substrings based on the specified delimiters.
    /// @param Source The String to be split.
    /// @param Delims The characters to look for and use as split points in the source String.
    /// @param MaxSplits The maximum number of splits to perform on this String. Value of zero means unlimited splits.
    /// @return Returns a vector containing all the substrings generated from the source String.
    StringVector MEZZ_LIB Split(const StringView Source, const StringView Delims = " \t\n", const Whole MaxSplits = 0);

    /// @brief Joins multiple Strings together into one String.
    /// @param Sources A container of Strings to be Joined.
    /// @param Delims A String that will be placed in between each source String provided.
    /// @return Returns a String containing all source Strings separated by the Delim String.
    String MEZZ_LIB Join(const std::vector<StringView>& Sources, const StringView Delims = " ");

    ///////////////////////////////////////////////////////////////////////////////
    // Pattern Matching

    /// @brief Checks a String to see if it starts with a specific pattern.
    /// @param Str The String to check.
    /// @param Pattern The sequence to check for at the start of the String.
    /// @param CaseSensitive If true, the pattern will be matched exactly, otherwise differing cases will be matched.
    /// @return Returns true if the String starts with the provided pattern, false otherwise.
    Boole MEZZ_LIB StartsWith(const StringView Str, const StringView Pattern, const Boole CaseSensitive);
    /// @brief Checks a String to see if it ends with a specific pattern.
    /// @param Str The String to check.
    /// @param Pattern The sequence to check for at the end of the String.
    /// @param CaseSensitive If true, the pattern will be matched exactly, otherwise differing cases will be matched.
    /// @return Returns true if the string ends with the provided pattern, false otherwise.
    Boole MEZZ_LIB EndsWith(const StringView Str, const StringView Pattern, const Boole CaseSensitive);

    /// @brief Checks to see if a string matches a specific pattern.
    /// @details This function supports a number of features similar to globbing/wildcard matching implementations
    /// that exist elsewhere. @n
    /// A "[" with a matching "]" will encase one or more ranges that can be checked against a single character.
    /// Ranges defined within square brackets must have a hyphen "-" separating the two extents of the range. They
    /// must also be the same category of character. For example "[A-Z]" is valid as both characters are upper case,
    /// but "[A-z]" is not. Upper case, lower case, or numbers can be defined in a range, but cannot be mixed when
    /// defining one. However multiple ranges can be defined back to back. For example "[A-Za-z]" is valid. Defining
    /// multiple ranges will only expand the valid matches for a single character though. Defining multiple ranges
    /// will not cause this to match multiple characters. @n
    /// A "*" (asterisk) will match 0 or more characters of any value. @n
    /// A "?" (question mark) will match 0 or 1 character of any value.
    /// @param Source The string to check.
    /// @param Pattern The pattern to try and match to the source string.
    /// @param CaseSensitive Whether or not the pattern should match case exactly.
    /// @return Returns true if the source string matches the specified pattern.
    Boole MEZZ_LIB PatternMatch(const StringView Source, const StringView Pattern, const Boole CaseSensitive);

    ///////////////////////////////////////////////////////////////////////////////
    // Convert-From-String Functions

    /// @brief Converts a String to any.
    /// @tparam Return The type the String will be converted to.
    /// @remarks This function should work for any type with an input streaming operator.
    /// @param ToConvert The String that will be converted.
    /// @return Returns the value from the converted String.
    template<typename Return>
    Return ConvertFromString(const StringView ToConvert)
    {
        Return Converted{};
        StringStream Converter;
        if constexpr( std::is_same_v<Return,double> ) {
            Converter.precision(15);
        }
        Converter << ToConvert;
        Converter >> Converted;
        return Converted;
    }

    /// @brief Converts a String into a Boole.
    /// @param ToConvert The String to be converted to a Boole.
    /// @return Returns a Boole with the converted value.
    Boole MEZZ_LIB ConvertToBool(const StringView ToConvert, const Boole Default = false);
    /// @brief Converts a String to an Int8.
    /// @remarks This requires its own specialization because otherwise a Int8 will use the char overload.
    /// @param ToConvert The String to be converted to an Int8.
    /// @return Returns an Int8 with the converted value.
    template<>
    Int8 ConvertFromString<Int8>(const StringView ToConvert);
    /// @brief Converts a String to a UInt8.
    /// @remarks This requires its own specialization because otherwise a UInt8 will implicitly covert to a char
    /// and use that overload.
    /// @param ToConvert The String to be converted to a UInt8.
    /// @return Returns a UInt8 with the converted value.
    template<>
    UInt8 ConvertFromString<UInt8>(const StringView ToConvert);

    ///////////////////////////////////////////////////////////////////////////////
    // Convert-To-String Functions

    /// @brief Converts any into a String.
    /// @tparam T The deduced type that will be converted to a String.
    /// @remarks This function should work for any type with an output streaming operator.
    /// @param ToConvert Streamable type instance to be converted.
    /// @return Returns a String containing the lexicographically converted data.
    template<typename T>
    String ConvertToString(const T& ToConvert)
    {
        StringStream Converter;
        if constexpr( std::is_same_v<T,double> ) {
            Converter.precision(15);
        }
        Converter << ToConvert;
        return Converter.str();
    }

    /// @brief Converts a Boole into a String.
    /// @remarks This requires its own specialization because otherwise a Boole will implicitly covert to an int
    /// and use that overload.
    /// @param ToConvert The Boole to be converted.
    /// @return Returns "true" if true, or "false" if false.
    template<>
    String MEZZ_LIB ConvertToString<Boole>(const Boole& ToConvert);
    /// @brief Converts a Int8 into a String.
    /// @remarks This requires its own specialization because otherwise a Int8 will use the char overload.
    /// @param ToConvert The Int8 to be converted.
    /// @return Returns the converted Int8 as a String.
    template<>
    String MEZZ_LIB ConvertToString<Int8>(const Int8& ToConvert);
    /// @brief Converts a UInt8 into a String.
    /// @remarks This requires its own specialization because otherwise a UInt8 will implicitly covert to a char
    /// and use that overload.
    /// @param ToConvert The UInt8 to be converted.
    /// @return Returns the converted UInt8 as a String.
    template<>
    String MEZZ_LIB ConvertToString<UInt8>(const UInt8& ToConvert);
}//StringTools
}//Mezzanine

#endif
