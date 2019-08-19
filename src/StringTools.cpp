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

#include "StringTools.h"

#include <cctype>
//#include <charconv>
#include <regex>

#include <iostream>

namespace
{
    using namespace Mezzanine;
    using StrIter = StringView::const_iterator;

    // Colour conversion helper value

    /// @brief Convenience multiplier used for converting a colour value for a single channel to a scalar value.
    constexpr Real HexConversionMultiplier = Real(1.0 / 255.0);

    // Pattern Matching helper methods

    /// @brief Convenience method to compare two characters insensitive of case.
    /// @param First An iterator to the first character to be compared.
    /// @param Second An iterator to the second character to be compared.
    /// @return Returns true if the two characters are the same later, regardless of case, false otherwise.
    Boole CaseInsensitiveCompare(StrIter First, StrIter Second)
        { return ( ::tolower( *First ) == ::tolower( *Second ) ); }

    /// @brief Searches for a pattern in the source string.
    /// @param SrcIt An iterator to the current position in the source string.
    /// @param SrcEnd An iterator to the end of the source string.
    /// @param PatIt An iterator to the current position in the string containing the pattern to search for.
    /// @param PatEnd An iterator to the end of the string containing the pattern to search for.
    /// @param CaseSensitive Whether or not the pattern to be matched should be matched in the same case.
    /// @return Returns an iterator to the position in the source string the pattern begins if one is found, or SrcEnd if no pattern was found.
    StrIter SearchPattern(StrIter SrcIt, const StrIter SrcEnd, StrIter PatIt, const StrIter PatEnd, const Boole CaseSensitive)
    {
        for(  ;  ; ++SrcIt )
        {
            StrIter SrcCompIt = SrcIt;
            for( StrIter PatCompIt = PatIt ;  ; ++SrcCompIt, ++PatCompIt )
            {
                if( PatCompIt == PatEnd ) {
                    return SrcIt;
                }
                if( SrcCompIt == SrcEnd ) {
                    return SrcEnd;
                }
                if( (*PatCompIt) == '\\' ) {
                    ++PatCompIt;
                }
                if( CaseSensitive ) {
                    if( (*SrcCompIt) != (*PatCompIt) ) {
                        break;
                    }
                }else{
                    if( !CaseInsensitiveCompare(SrcCompIt,PatCompIt) ) {
                        break;
                    }
                }
            }
        }
    }

    /// @brief Checks to see if the pattern following the asterisk matches a pattern in the source string.
    /// @remarks SrcIt is expected to be pointing to a '*' character when this function is called.
    /// @param SrcIt An iterator to the current position in the source string.
    /// @param SrcEnd An iterator to the end of the source string.
    /// @param PatIt An iterator to the current position in the pattern string.
    /// @param PatEnd An iterator to the end of the pattern string.
    /// @param CaseSensitive Whether or not the pattern to be matched should be matched in the same case.
    /// @return Returns true if the pattern immediately following the asterisk matches a sequence in the source, false otherwise.
    Boole MatchAsterisk(StrIter& SrcIt, const StrIter SrcEnd, StrIter& PatIt, const StrIter PatEnd, const Boole CaseSensitive)
    {
        const StrIter PatBegin = PatIt;
        const String ControlChars("[*?");

        // Quick Check for the easy case.  An asterisk at the end of the pattern.
        if( PatBegin == ( std::prev(PatEnd) ) ) {
            SrcIt = SrcEnd;
            PatIt = PatEnd;
            return true;
        }

        // Get the position of the first non-escaped control character.
        StrIter ControlIt = PatBegin;
        do{
            ++ControlIt;
            ControlIt = std::find_first_of(ControlIt,PatEnd,ControlChars.begin(),ControlChars.end());
        }while( *( std::prev(ControlIt) ) == '\\' && ControlIt != PatEnd );

        StrIter SrcResult = SearchPattern(SrcIt,SrcEnd,std::next(PatBegin),ControlIt,CaseSensitive);
        if( SrcResult != SrcEnd ) {
            SrcIt = SrcResult;
            PatIt = ControlIt;
            return true;
        }else{
            SrcIt = SrcEnd;
            PatIt = PatEnd;
            return false;
        }
    }

    /// @brief Matches any single character in the source string.
    /// @remarks SrcIt is expected to be pointing to a '?' character when this function is called.
    /// @param SrcIt An iterator to the current position in the source string.
    /// @param SrcEnd An iterator to the end of the source string.
    /// @param PatIt An iterator to the current position in the pattern string.
    /// @param PatEnd An iterator to the end of the pattern string.
    /// @return Returns true always.  Question marks are trivial to match.
    Boole MatchQuestionMark(StrIter& SrcIt, const StrIter SrcEnd, StrIter& PatIt, const StrIter PatEnd)
    {
        (void)SrcEnd;
        (void)PatEnd;
        ++PatIt;
        if( (*SrcIt) != '[' && (*SrcIt) != '*' && (*SrcIt) != '?' ) {
            ++SrcIt;
        }
        return true;
    }

    /// @brief Matches a single character in the source string with the range specified in the pattern string.
    /// @remarks SrcIt is expected to be pointing to a '-' character when this function is called.
    /// @param SrcIt An iterator to the current position in the source string.
    /// @param SrcEnd An iterator to the end of the source string.
    /// @param PatIt An iterator to the current position in the pattern string.
    /// @param PatEnd An iterator to the end of the pattern string.
    /// @return Returns true if the range is valid and the source character is within it, false otherwise.
    Boole MatchSingleRange(StrIter& SrcIt, const StrIter SrcEnd, StrIter& PatIt, const StrIter PatEnd)
    {
        (void)SrcEnd;
        if( std::distance(PatIt,PatEnd) < 2 ) {
            return false;
        }

        char LowCheck = *( std::prev(PatIt) );
        char HighCheck = *( std::next(PatIt) );
        std::advance(PatIt,2);
        if( ( StringTools::IsUpperAlphaLetter( LowCheck ) && StringTools::IsUpperAlphaLetter( HighCheck ) ) ||
            ( StringTools::IsLowerAlphaLetter( LowCheck ) && StringTools::IsLowerAlphaLetter( HighCheck ) ) ||
            ( StringTools::IsDigit( LowCheck ) && StringTools::IsDigit( HighCheck ) ) )
        {
            return ( (*SrcIt) >= LowCheck && (*SrcIt) <= HighCheck );
        }
        return false;
    }

    /// @brief Marches a single character in the source string with one or more ranges specified in the pattern string.
    /// @remarks SrcIt is expected to be pointing to a '[' character when this function is called.
    /// @param SrcIt An iterator to the current position in the source string.
    /// @param SrcEnd An iterator to the end of the source string.
    /// @param PatIt An iterator to the current position in the pattern string.
    /// @param PatEnd An iterator to the end of the pattern string.
    /// @return Returns true if the source character matches any of the valid ranges specified, false otherwise.
    Boole MatchRangeCharacter(StrIter& SrcIt, const StrIter SrcEnd, StrIter& PatIt, const StrIter PatEnd)
    {
        Boole PassRangeTest = false;
        while( PatIt != PatEnd && (*PatIt) != ']' )
        {
            if( (*PatIt) == '-' ) {
                PassRangeTest = PassRangeTest || MatchSingleRange(SrcIt,SrcEnd,PatIt,PatEnd);
            }else{
                ++PatIt;
            }
        }
        if( PassRangeTest ) {
            ++SrcIt;
        }
        if( PatIt != PatEnd ) {
            ++PatIt;
        }
        return PassRangeTest;
    }
}

namespace Mezzanine {
namespace StringTools {
    ///////////////////////////////////////////////////////////////////////////////
    // String Manipulation and Checks

    void ToUpperCase(StrIter Begin, StrIter End)
    {
        std::transform(Begin,End,Begin,[](int ToConvert) -> char {
            return static_cast<char>( ::toupper(ToConvert) );
        });
    }

    String UpperCaseCopy(String Source)
    {
        StringTools::ToUpperCase(Source.begin(),Source.end());
        return Source;
    }

    void ToLowerCase(StrIter Begin, StrIter End)
    {
        std::transform(Begin,End,Begin,[](int ToConvert) -> char {
            return static_cast<char>( ::tolower(ToConvert) );
        });
    }

    String LowerCaseCopy(String Source)
    {
        StringTools::ToLowerCase(Source.begin(),Source.end());
        return Source;
    }

    void ToPascalCase(StrIter Begin, StrIter End)
    {
        Boole PrevCharIsWhitespace = true;
        for( StrIter CurrIt = Begin ; CurrIt != End ; ++CurrIt )
        {
            Boole CurrCharIsLowerLetter = StringTools::IsLowerAlphaLetter( *CurrIt );
            Boole CurrCharIsUpperLetter = StringTools::IsUpperAlphaLetter( *CurrIt );
            if( PrevCharIsWhitespace && CurrCharIsLowerLetter ) {
                (*CurrIt) = static_cast<Char8>( ::toupper(*CurrIt) );
            }else if( !PrevCharIsWhitespace && CurrCharIsUpperLetter ) {
                (*CurrIt) = static_cast<Char8>( ::tolower(*CurrIt) );
            }
            PrevCharIsWhitespace = StringTools::IsWhitespace( *CurrIt );
        }
    }

    String PascalCaseCopy(String Source)
    {
        StringTools::ToPascalCase(Source.begin(),Source.end());
        return Source;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Size Modifying String Manipulations

    StrIter Trim(StrIter Begin, StrIter End)
    {
        StrIter Ret = TrimLeft(Begin,End);
        Ret = TrimRight(Begin,Ret);
        return Ret;
    }

    String TrimCopy(ConstStrIter Begin, ConstStrIter End)
    {
        String Ret(Begin,End);
        ConstStrIter NewEnd = StringTools::Trim(Ret.begin(),Ret.end());
        Ret.erase( static_cast<size_t>( std::distance(Ret.cbegin(),NewEnd) ) );
        return Ret;
    }

    StrIter TrimLeft(StrIter Begin, StrIter End)
    {
        using IterDiffType = StrIter::difference_type;

        const String Delims = " \t\r";
        const IterDiffType OldSize = std::distance(Begin,End);
        IterDiffType Forward = 0;
        while( Forward < OldSize )
        {
            if( Delims.find( *std::next(Begin,Forward) ) == String::npos ) {
                break;
            }
            ++Forward;
        }

        if( Forward > 0 ) {
            return std::move( std::next(Begin,Forward), End, Begin );
        }else{
            return End;
        }
    }

    String TrimLeftCopy(ConstStrIter Begin, ConstStrIter End)
    {
        String Ret(Begin,End);
        ConstStrIter NewEnd = StringTools::TrimLeft(Ret.begin(),Ret.end());
        Ret.erase( static_cast<size_t>( std::distance(Ret.cbegin(),NewEnd) ) );
        return Ret;
    }

    StrIter TrimRight(StrIter Begin, StrIter End)
    {
        using IterDiffType = StrIter::difference_type;

        const String Delims = " \t\r";
        const IterDiffType OldSize = std::distance(Begin,End);
        IterDiffType Reverse = OldSize;
        while( Reverse > 0 )
        {
            if( Delims.find( *std::next(Begin,Reverse - 1) ) == String::npos ) {
                break;
            }
            --Reverse;
        }

        return std::next(Begin,Reverse);
    }

    String TrimRightCopy(ConstStrIter Begin, ConstStrIter End)
    {
        String Ret(Begin,End);
        ConstStrIter NewEnd = StringTools::TrimRight(Ret.begin(),Ret.end());
        Ret.erase( static_cast<size_t>( std::distance(Ret.cbegin(),NewEnd) ) );
        return Ret;
    }

    StrIter RemoveDuplicateWhitespaces(StrIter Begin, StrIter End)
    {
        using IterDiffType = StrIter::difference_type;
        // Opted for the lazy solution here. A potentially better solution would be to mark the removed spaces
        // on one loop and then have another that does the moves, so that no char is moved more than once.
        const String Delims = " \t";
        for( StrIter CurrPos = std::find_first_of(Begin,End,Delims.begin(),Delims.end()) ;
             CurrPos != End ;
             CurrPos = std::find_first_of(CurrPos,End,Delims.begin(),Delims.end()) )
        {
            StrIter WhitespaceEnd = CurrPos;
            Char8 CurrWhitespace = (*CurrPos);
            while( (*WhitespaceEnd) == CurrWhitespace && WhitespaceEnd != End )
            {
                ++WhitespaceEnd;
            }

            // Increment the CurrPos here to preserve the one whitespace char we want to keep.
            const IterDiffType RemoveCount = std::distance(++CurrPos,WhitespaceEnd);
            if( RemoveCount > 0 ) {
                std::move(WhitespaceEnd,End,CurrPos);
                End = std::prev(End,RemoveCount);
            }
        }
        return End;
    }

    String RemoveDuplicateWhitespacesCopy(ConstStrIter Begin, ConstStrIter End)
    {
        String Ret(Begin,End);
        ConstStrIter NewEnd = StringTools::RemoveDuplicateWhitespaces(Ret.begin(),Ret.end());
        Ret.erase( static_cast<size_t>( std::distance(Ret.cbegin(),NewEnd) ) );
        return Ret;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Colour Channels

    Real ConvertHexToColourChannel(const StringView Hex)
    {
        if( Hex.size() != 2 ) {
            String ExceptionString("Hex code requires 2 characters to express a Colour channel.");
            throw std::runtime_error(ExceptionString);
        }

        // <charconv> implementation
        /*unsigned int Temp = 0;
        std::from_chars_result Result = std::from_chars(Hex.data(),Hex.data() + 2,Temp,16);
        if( Result.ec != std::errc() ) {
            throw std::runtime_error("Error parsing Hex String. Does the String contain only valid Hex characters?");
        }
        Real Ret = static_cast<Real>(Temp);
        return std::min(Ret *= HexConversionMultiplier,Real(1.0));//*/

        // std::strtool implementation
        char* EndPtr = nullptr;
        Real Ret = static_cast<Real>( std::strtol(Hex.data(),&EndPtr,16) );
        if( EndPtr != std::next(Hex.data(),2) ) {
            throw std::runtime_error("Error parsing Hex String. Does the String contain only valid Hex characters?");
        }
        return std::min(Ret *= HexConversionMultiplier,Real(1.0));
    }

    String ConvertColourChannelToHex(const Real Channel)
    {
        // <charconv> implementation
        /*String Ret(2,'0');
        unsigned int Temp = static_cast<unsigned int>(Channel * Real(255.0));
        std::to_chars_result Result = std::to_chars(Ret.data(),Ret.data() + 2,Temp,16);
        if( Result.ec != std::errc() ) {
            throw std::runtime_error("Error converting Real to Hex. Is it within the range 0.0-1.0?");
        }

        if( Result.ptr == std::next(Ret.data()) ) {
            std::swap(Ret[0],Ret[1]);
        }
        StringTools::ToUpperCase(Ret.begin(),Ret.end());
        return Ret;//*/

        // StringStream implementation
        StringStream Converter;
        Converter.setf( std::ios::hex | std::ios::uppercase, std::ios::basefield | std::ios::uppercase );
        Converter.width(2);
        Converter.fill('0');
        Converter << std::min( static_cast<unsigned int>( Channel * Real(255.0) ), 255u );
        return Converter.str();
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Tokenizing

    StringVector Split(const StringView Source, const StringView Delims, const Whole MaxSplits)
    {
        StringVector Ret;

        if( Source.empty() ) {
            return Ret;
        }

        Ret.reserve( MaxSplits ? MaxSplits + 1 : 10 );
        Whole Splits = 0;

        size_t Start = 0;
        size_t Pos = 0;

        do{
            Pos = Source.find_first_of(Delims,Start);
            if( Pos == Start ) {
                Start = Pos + 1;
            }else if( Pos == String::npos || ( MaxSplits && Splits == MaxSplits ) ) {
                Ret.emplace_back(Source.substr(Start));
                break;
            }else{
                Ret.emplace_back(Source.substr(Start,Pos - Start));
                Start = Pos + 1;
                ++Splits;
            }
            Start = Source.find_first_not_of(Delims,Start);
        }while(Pos != String::npos);

        return Ret;
    }

    String Join(const std::vector<StringView>& Sources, const StringView Delims)
    {
        String Ret;
        size_t ReserveSize = 0;
        for( StringView View : Sources )
            { ReserveSize += View.size(); }
        Ret.reserve(ReserveSize);

        Ret.append( Sources[0] );
        for( unsigned int Index = 1 ; Index < Sources.size() ; ++Index )
        {
            Ret.append( Delims );
            Ret.append( Sources[Index] );
        }
        return Ret;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Pattern Matching

    Boole StartsWith(const StringView Str, const StringView Pattern, const Boole CaseSensitive)
    {
        size_t StrLen = Str.length();
        size_t PatternLen = Pattern.length();
        if( PatternLen > StrLen || PatternLen == 0 )
            return false;

        StringView Start = Str.substr(0,PatternLen);
        if( CaseSensitive ) {
            return ( Start == Pattern );
        }

        return std::equal(Start.begin(),Start.end(),Pattern.begin(),Pattern.end(),
            [](const Char8 Left, const Char8 Right){
                return ::tolower(Left) == ::tolower(Right);
            }
        );
    }

    Boole EndsWith(const StringView Str, const StringView Pattern, const Boole CaseSensitive)
    {
        size_t StrLen = Str.length();
        size_t PatternLen = Pattern.length();
        if( PatternLen > StrLen || PatternLen == 0 )
            return false;

        StringView End = Str.substr(StrLen - PatternLen,PatternLen);
        if( CaseSensitive ) {
            return ( End == Pattern );
        }

        return std::equal(End.begin(),End.end(),Pattern.begin(),Pattern.end(),
            [](const Char8 Left, const Char8 Right){
                return ::tolower(Left) == ::tolower(Right);
            }
        );
    }

    Boole PatternMatch(const StringView Source, const StringView Pattern, const Boole CaseSensitive)
    {
        StringView::const_iterator SrcIt = Source.begin();
        StringView::const_iterator PatIt = Pattern.begin();
        const StringView::const_iterator SrcEnd = Source.end();
        const StringView::const_iterator PatEnd = Pattern.end();

        Boole EscapedChar = false;
        while( PatIt != PatEnd && SrcIt != SrcEnd )
        {
            Boole MatchResult = false;
            if( EscapedChar ) {
                EscapedChar = false;
                // If we're escaped, doesn't matter what it is.  Process as normal char.
                MatchResult = ( (*PatIt) == (*SrcIt) );
                ++PatIt;
                ++SrcIt;
            }else if( (*PatIt) == '*' ) {
                EscapedChar = false;
                MatchResult = MatchAsterisk(SrcIt,SrcEnd,PatIt,PatEnd,CaseSensitive);
            }else if( (*PatIt) == '[' ) {
                EscapedChar = false;
                MatchResult = MatchRangeCharacter(SrcIt,SrcEnd,PatIt,PatEnd);
            }else if( (*PatIt) == '?' ) {
                EscapedChar = false;
                MatchResult = MatchQuestionMark(SrcIt,SrcEnd,PatIt,PatEnd);
            }else if( (*PatIt) == '\\' ) {
                EscapedChar = true;
                ++PatIt;
                continue;
            }else{
                EscapedChar = false;
                if( CaseSensitive ) {
                    MatchResult = ( (*PatIt) == (*SrcIt) );
                }else{
                    MatchResult = CaseInsensitiveCompare(PatIt,SrcIt);
                }
                ++PatIt;
                ++SrcIt;
            }

            if( !MatchResult ) {
                return false;
            }
        }
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Convert-To-Data functions

    Boole ConvertToBool(const StringView ToConvert, const Boole Default)
    {
        auto Checker = [ToConvert](const StringView Str) {
            if( Str.size() == ToConvert.size() ) {
                for( size_t Idx = 0 ; Idx < Str.size() ; ++Idx )
                {
                    if( ::tolower( Str[Idx] ) != ::tolower( ToConvert[Idx] ) ) {
                        return false;
                    }
                }
                return true;
            }
            return false;
        };

        if( Checker("true") ) return true;
        else if( Checker("yes") ) return true;
        else if( Checker("1") ) return true;
        else if( Checker("false") ) return false;
        else if( Checker("no") ) return false;
        else if( Checker("0") ) return false;
        else return Default;
    }

    template<>
    Int8 ConvertFromString<Int8>(const StringView ToConvert)
    {
        StringStream Converter;
        int Result;
        Converter << ToConvert;
        Converter >> Result;
        return static_cast<Int8>(Result);
    }

    template<>
    UInt8 ConvertFromString<UInt8>(const StringView ToConvert)
    {
        StringStream Converter;
        unsigned int Result;
        Converter << ToConvert;
        Converter >> Result;
        return static_cast<UInt8>(Result);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Convert-To-String functions

    template<>
    String ConvertToString<Boole>(const Boole& ToConvert)
    {
        if(ToConvert) return "true";
        else return "false";
    }

    template<>
    String ConvertToString<Int8>(const Int8& ToConvert)
    {
        Integer Convert = ToConvert;
        StringStream Converter;
        Converter << Convert;
        return Converter.str();
    }

    template<>
    String ConvertToString<UInt8>(const UInt8& ToConvert)
    {
        Whole Convert = ToConvert;
        StringStream Converter;
        Converter << Convert;
        return Converter.str();
    }
}//StringTools
}//Mezzanine
