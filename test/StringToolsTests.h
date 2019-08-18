// � Copyright 2010 - 2019 BlackTopp Studios Inc.
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
#ifndef Mezz_Foundation_StringToolsTests_h
#define Mezz_Foundation_StringToolsTests_h

/// @file
/// @brief This file tests the common String manipulation utilities of the Mezzanine.

#include "MezzTest.h"

#include "StringTools.h"

DEFAULT_TEST_GROUP(StringToolsTests,StringTools)
{
    using namespace Mezzanine;

    {//Character Checks
        TEST_EQUAL("IsSpace(const_CharType)-Pass",true,StringTools::IsSpace(' '));
        TEST_EQUAL("IsSpace(const_CharType)-Fail",false,StringTools::IsSpace('\t'));
        TEST_EQUAL("IsTab(const_CharType)-Pass1",true,StringTools::IsTab('\t'));
        TEST_EQUAL("IsTab(const_CharType)-Pass2",true,StringTools::IsTab('\v'));
        TEST_EQUAL("IsTab(const_CharType)-Fail",false,StringTools::IsTab('\r'));
        TEST_EQUAL("IsNewline(const_CharType)-Pass1",true,StringTools::IsNewline('\r'));
        TEST_EQUAL("IsNewline(const_CharType)-Pass2",true,StringTools::IsNewline('\n'));
        TEST_EQUAL("IsNewline(const_CharType)-Fail",false,StringTools::IsNewline('!'));
        TEST_EQUAL("IsWhitespace(const_CharType)-Pass1",true,StringTools::IsWhitespace(' '));
        TEST_EQUAL("IsWhitespace(const_CharType)-Pass2",true,StringTools::IsWhitespace('\t'));
        TEST_EQUAL("IsWhitespace(const_CharType)-Pass3",true,StringTools::IsWhitespace('\n'));
        TEST_EQUAL("IsWhitespace(const_CharType)-Fail",false,StringTools::IsWhitespace('&'));
        TEST_EQUAL("IsDigit(const_CharType)-Pass",true,StringTools::IsDigit('5'));
        TEST_EQUAL("IsDigit(const_CharType)-Fail",false,StringTools::IsDigit('L'));
        TEST_EQUAL("IsLowerAlphaLetter(const_CharType)-Pass",true,StringTools::IsLowerAlphaLetter('a'));
        TEST_EQUAL("IsLowerAlphaLetter(const_CharType)-Fail",false,StringTools::IsLowerAlphaLetter('A'));
        TEST_EQUAL("IsUpperAlphaLetter(const_CharType)-Pass",true,StringTools::IsUpperAlphaLetter('B'));
        TEST_EQUAL("IsUpperAlphaLetter(const_CharType)-Fail",false,StringTools::IsUpperAlphaLetter('b'));
        TEST_EQUAL("IsAlphaLetter(const_CharType)-Pass",true,StringTools::IsAlphaLetter('y'));
        TEST_EQUAL("IsAlphaLetter(const_CharType)-Fail",false,StringTools::IsAlphaLetter('9'));
        TEST_EQUAL("IsLowerHexLetter(const_CharType)-Pass",true,StringTools::IsLowerHexLetter('e'));
        TEST_EQUAL("IsLowerHexLetter(const_CharType)-Fail",false,StringTools::IsLowerHexLetter('g'));
        TEST_EQUAL("IsUpperHexLetter(const_CharType)-Pass",true,StringTools::IsUpperHexLetter('F'));
        TEST_EQUAL("IsUpperHexLetter(const_CharType)-Fail",false,StringTools::IsUpperHexLetter('1'));
        TEST_EQUAL("IsHexLetter(const_CharType)-Pass",true,StringTools::IsHexLetter('B'));
        TEST_EQUAL("IsHexLetter(const_CharType)-Fail",false,StringTools::IsHexLetter('z'));
        TEST_EQUAL("IsHexDigit(const_CharType)-Pass",true,StringTools::IsHexDigit('8'));
        TEST_EQUAL("IsHexDigit(const_CharType)-Fail",false,StringTools::IsHexDigit('t'));
        TEST_EQUAL("IsAlphanumeric(const_CharType)-Pass",true,StringTools::IsAlphanumeric('R'));
        TEST_EQUAL("IsAlphanumeric(const_CharType)-Fail",false,StringTools::IsAlphanumeric(' '));

        unsigned char UCharPass = ' ';
        unsigned char UCharFail = '\t';
        TEST_EQUAL("IsSpace(const_CharType)-unsigned_char-Pass",true,StringTools::IsSpace(UCharPass));
        TEST_EQUAL("IsSpace(const_CharType)-unsigned_char-Fail",false,StringTools::IsSpace(UCharFail));
        TEST_EQUAL("IsSpace(const_CharType)-wchar_t-Pass",true,StringTools::IsSpace(wchar_t(' ')));
        TEST_EQUAL("IsSpace(const_CharType)-wchar_t-Fail",false,StringTools::IsSpace(wchar_t('\t')));
        TEST_EQUAL("IsSpace(const_CharType)-char16_t-Pass",true,StringTools::IsSpace(char16_t(' ')));
        TEST_EQUAL("IsSpace(const_CharType)-char16_t-Fail",false,StringTools::IsSpace(char16_t('\t')));
        TEST_EQUAL("IsSpace(const_CharType)-char32_t-Pass",true,StringTools::IsSpace(char32_t(' ')));
        TEST_EQUAL("IsSpace(const_CharType)-char32_t-Fail",false,StringTools::IsSpace(char32_t('\t')));
    }//Character Manipulation and Checks

    {//String Manipulation
        String ToUpperString("This is typed in a reasonable, soft-spoken tone.");
        const String ToUpperResult("THIS IS TYPED IN A REASONABLE, SOFT-SPOKEN TONE.");
        StringTools::ToUpperCase(ToUpperString.begin(),ToUpperString.end());
        TEST_EQUAL("ToUpperCase(StrIter,StrIter)",ToUpperResult,ToUpperString);

        String UpperCopyString("This Is An Odd Sentence Typed In Pascal-Case; With An Unnecessary Semi-Colon.");
        const String UpperCopyResult("THIS IS AN ODD SENTENCE TYPED IN PASCAL-CASE; WITH AN UNNECESSARY SEMI-COLON.");
        TEST_EQUAL("UpperCaseCopy(String)",UpperCopyResult,StringTools::UpperCaseCopy(UpperCopyString));

        String ToLowerString("CAPS LOCK IS CRUISE CONTROL FOR COOL!");
        const String ToLowerResult("caps lock is cruise control for cool!");
        StringTools::ToLowerCase(ToLowerString.begin(),ToLowerString.end());
        TEST_EQUAL("ToLowerCase(StrIter,StrIter)",ToLowerResult,ToLowerString);

        String LowerCopyString("I HAVE A VERY GOOD BRAIN AND HAVE SAID MANY THINGS.");
        const String LowerCopyResult("i have a very good brain and have said many things.");
        TEST_EQUAL("LowerCaseCopy(String)",LowerCopyResult,StringTools::LowerCaseCopy(LowerCopyString));

        String ToPascalCaseString("Test string, do not reply.");
        const String ToPascalCaseResult("Test String, Do Not Reply.");
        StringTools::ToPascalCase(ToPascalCaseString.begin(),ToPascalCaseString.end());
        TEST_EQUAL("ToPascalCase(StrIter,StrIter)",ToPascalCaseResult,ToPascalCaseString);

        String PascalCaseCopyString("I'm so meta, even this acronym.");
        const String PascalCaseCopyResult("I'm So Meta, Even This Acronym.");
        TEST_EQUAL("PascalCaseCopy(String)",PascalCaseCopyResult,StringTools::PascalCaseCopy(PascalCaseCopyString));
    }//String Manipulation

    {//Size Modifying String Manipulation
        const String TrimSourceString("    This is a trim test. \t\t");
        String TrimPassString1("    This is a trim test. \t\t");
        String TrimPassString2("    This is a trim test. \t\t");
        String TrimPassString3("    This is a trim test. \t\t");
        StringTools::StrIter Pass1Iter = StringTools::Trim(TrimPassString1.begin(),TrimPassString1.end());
        TrimPassString1.erase( size_t(Pass1Iter - TrimPassString1.begin()) );// Manual Cleanup
        StringTools::StrIter Pass2Iter = StringTools::TrimRight(TrimPassString2.begin(),TrimPassString2.end());
        TrimPassString2.erase( size_t(Pass2Iter - TrimPassString2.begin()) );// Manual Cleanup
        StringTools::StrIter Pass3Iter = StringTools::TrimLeft(TrimPassString3.begin(),TrimPassString3.end());
        TrimPassString3.erase( size_t(Pass3Iter - TrimPassString3.begin()) );// Manual Cleanup
        TEST_EQUAL("Trim(String&,const_Boole,const_Boole)-StringVal",
                   String("This is a trim test."),TrimPassString1);
        TEST_EQUAL("Trim(String&,const_Boole,const_Boole)-EndPos",
                   size_t(20),size_t(Pass1Iter - TrimPassString1.begin()));
        TEST_EQUAL("TrimRight(String&,const_Boole,const_Boole)-StringVal",
                   String("    This is a trim test."),TrimPassString2);
        TEST_EQUAL("TrimRight(String&,const_Boole,const_Boole)-EndPos",
                   size_t(24),size_t(Pass2Iter - TrimPassString2.begin()));
        TEST_EQUAL("TrimLeft(String&,const_Boole,const_Boole)-StringVal",
                   String("This is a trim test. \t\t"),TrimPassString3);
        TEST_EQUAL("TrimLeft(String&,const_Boole,const_Boole)-EndPos",
                   size_t(23),size_t(Pass3Iter - TrimPassString3.begin()));

        const String TrimCopySourceString("\tThis is an indented trim test.  ");
        TEST_EQUAL("TrimCopy(String,const_Boole,const_Boole)-Pass",
                   String("This is an indented trim test."),
                   StringTools::TrimCopy(TrimCopySourceString.begin(),TrimCopySourceString.end()));
        TEST_EQUAL("TrimRightCopy(String,const_Boole,const_Boole)-Pass",
                   String("\tThis is an indented trim test."),
                   StringTools::TrimRightCopy(TrimCopySourceString.begin(),TrimCopySourceString.end()));
        TEST_EQUAL("TrimLeftCopy(String,const_Boole,const_Boole)-Pass",
                   String("This is an indented trim test.  "),
                   StringTools::TrimLeftCopy(TrimCopySourceString.begin(),TrimCopySourceString.end()));

        String DupWhiteSourceString("This  is  a needlessly  wide sentence.   ");
        String DupWhiteResultString("This is a needlessly wide sentence. ");
        StringTools::StrIter DupWhiteIter = StringTools::RemoveDuplicateWhitespaces(DupWhiteSourceString.begin(),
                                                                                    DupWhiteSourceString.end());
        DupWhiteSourceString.erase( size_t(DupWhiteIter - DupWhiteSourceString.begin()) );// Manual Cleanup
        TEST_EQUAL("RemoveDuplicateWhitespaces(String&)",DupWhiteResultString,DupWhiteSourceString);

        String DupWhiteCopySourceString("\t\tThis indented  sentence   is    stretching     really      far.");
        const String DupWhiteCopyResultString("\tThis indented sentence is stretching really far.");
        //TEST_EQUAL("RemoveDuplicateWhitespacesCopy(String)",
        //           DupWhiteCopyResultString,
        //           StringTools::RemoveDuplicateWhitespacesCopy(DupWhiteCopySourceString.begin(),
        //                                                       DupWhiteSourceString.end()));
    }//Size Modifying String Manipulation

    {//Colour Channels
        StringView HexStr1("71");
        StringView HexStr2("A2");
        StringView HexStr3("3F");
        StringView HexStr4("06");
        TEST_EQUAL_EPSILON("ConvertHexToColourChannel(const_StringView)-First",
                           Real(113.0 / 255.0),StringTools::ConvertHexToColourChannel(HexStr1));
        TEST_EQUAL_EPSILON("ConvertHexToColourChannel(const_StringView)-Second",
                           Real(162.0 / 255.0),StringTools::ConvertHexToColourChannel(HexStr2));
        TEST_EQUAL_EPSILON("ConvertHexToColourChannel(const_StringView)-Third",
                           Real(63.0 / 255.0),StringTools::ConvertHexToColourChannel(HexStr3));
        TEST_EQUAL_EPSILON("ConvertHexToColourChannel(const_StringView)-Fourth",
                           Real(6.0 / 255.0),StringTools::ConvertHexToColourChannel(HexStr4));

        Real ChannelVal1 = 0.8675f;//221.2125
        Real ChannelVal2 = 0.1234f;//31.467
        Real ChannelVal3 = 0.9274f;//236.487
        Real ChannelVal4 = 0.0475f;//12.1125
        TEST_EQUAL("ConvertColourChannelToHex(const_Real)-First",
                   String("DD"),StringTools::ConvertColourChannelToHex(ChannelVal1));
        TEST_EQUAL("ConvertColourChannelToHex(const_Real)-Second",
                   String("1F"),StringTools::ConvertColourChannelToHex(ChannelVal2));
        TEST_EQUAL("ConvertColourChannelToHex(const_Real)-Third",
                   String("EC"),StringTools::ConvertColourChannelToHex(ChannelVal3));
        TEST_EQUAL("ConvertColourChannelToHex(const_Real)-Fourth",
                   String("0C"),StringTools::ConvertColourChannelToHex(ChannelVal4));
    }//Colour Channels

    {//Tokenizing
        const String SplitSourceString("This is a string that will be split into 11 parts.");
        StringVector DefaultSplitResult = StringTools::Split(SplitSourceString);
        StringVector ISplitResult = StringTools::Split(SplitSourceString,"i",5);// Should normally split 7 times.
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-DefaultCount",
                   size_t(11),DefaultSplitResult.size());
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-DefaultElement1",
                   String("This"),DefaultSplitResult[0]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-DefaultElement2",
                   String("is"),DefaultSplitResult[1]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-DefaultElement3",
                   String("a"),DefaultSplitResult[2]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-DefaultElement4",
                   String("string"),DefaultSplitResult[3]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-DefaultElement5",
                   String("that"),DefaultSplitResult[4]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-DefaultElement6",
                   String("will"),DefaultSplitResult[5]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-DefaultElement7",
                   String("be"),DefaultSplitResult[6]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-DefaultElement8",
                   String("split"),DefaultSplitResult[7]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-DefaultElement9",
                   String("into"),DefaultSplitResult[8]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-DefaultElement10",
                   String("11"),DefaultSplitResult[9]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-DefaultElement11",
                   String("parts."),DefaultSplitResult[10]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-ISplitCount",
                   size_t(6),ISplitResult.size());
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-ISplitElement1",
                   String("Th"),ISplitResult[0]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-ISplitElement2",
                   String("s "),ISplitResult[1]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-ISplitElement3",
                   String("s a str"),ISplitResult[2]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-ISplitElement4",
                   String("ng that w"),ISplitResult[3]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-ISplitElement5",
                   String("ll be spl"),ISplitResult[4]);
        TEST_EQUAL("Split(const_String&,const_String&,const_Whole)-ISplitElement6",
                   String("t into 11 parts."),ISplitResult[5]);
    }//Tokenizing

    {//Pattern Matching
        const String StartsWithSourceString("Autocannibalism");
        TEST_EQUAL("StartsWith(const_StringView,const_StringView,const_Boole)-Pass",
                   true,StringTools::StartsWith(StartsWithSourceString,"auto",false));
        TEST_EQUAL("StartsWith(const_StringView,const_StringView,const_Boole)-CasePass",
                   true,StringTools::StartsWith(StartsWithSourceString,"Auto",true));
        TEST_EQUAL("StartsWith(const_StringView,const_StringView,const_Boole)-Fail",
                   false,StringTools::StartsWith(StartsWithSourceString,"Self",false));
        TEST_EQUAL("StartsWith(const_StringView,const_StringView,const_Boole)-CaseFail",
                   false,StringTools::StartsWith(StartsWithSourceString,"AUTO",true));

        const String EndsWithSourceString("ThatsTheJoke.jpg");
        TEST_EQUAL("EndsWith(const_StringView,const_StringView,const_Boole)-Pass",
                   true,StringTools::EndsWith(EndsWithSourceString,".JPG",false));
        TEST_EQUAL("EndsWith(const_StringView,const_StringView,const_Boole)-CasePass",
                   true,StringTools::EndsWith(EndsWithSourceString,".jpg",true));
        TEST_EQUAL("EndsWith(const_StringView,const_StringView,const_Boole)-Fail",
                   false,StringTools::EndsWith(EndsWithSourceString,".png",false));
        TEST_EQUAL("EndsWith(const_StringView,const_StringView,const_Boole)-CaseFail",
                   false,StringTools::EndsWith(EndsWithSourceString,".JPG",true));

        String FirstSource("Test/Data/Results.txt");
        String SecondSource("WS_29_06_18.dat");
        String ThirdSource("Test\\Data\\Results.txt");
        String FourthSource("*Star*");

        String FirstPattern("Test/Data/*.txt");
        String SecondPattern("*.???");
        String ThirdPattern("*.dat");
        String FourthPattern("ws_[0-9][0-9]_??_*.dat");
        String FifthPattern("Test\\\\Data\\\\*.txt");
        String SixthPattern("\\**\\*");

        TEST_EQUAL("PatternMatch(const_StringView,const_StringView,const_Boole)-Pass1",
                   true,StringTools::PatternMatch(FirstSource,FirstPattern,false));
        TEST_EQUAL("PatternMatch(const_StringView,const_StringView,const_Boole)-Pass2",
                   true,StringTools::PatternMatch(FirstSource,SecondPattern,false));
        TEST_EQUAL("PatternMatch(const_StringView,const_StringView,const_Boole)-Pass3",
                   true,StringTools::PatternMatch(SecondSource,SecondPattern,true));
        TEST_EQUAL("PatternMatch(const_StringView,const_StringView,const_Boole)-Pass4",
                   true,StringTools::PatternMatch(SecondSource,FourthPattern,false));
        TEST_EQUAL("PatternMatch(const_StringView,const_StringView,const_Boole)-Pass5",
                   true,StringTools::PatternMatch(ThirdSource,FifthPattern,false));
        TEST_EQUAL("PatternMatch(const_StringView,const_StringView,const_Boole)-Pass6",
                   true,StringTools::PatternMatch(FourthSource,SixthPattern,true));

        TEST_EQUAL("PatternMatch(const_StringView,const_StringView,const_Boole)-Fail1",
                   false,StringTools::PatternMatch(FirstSource,ThirdPattern,false));
        TEST_EQUAL("PatternMatch(const_StringView,const_StringView,const_Boole)-Fail2",
                   false,StringTools::PatternMatch(SecondSource,FourthPattern,true));
        TEST_EQUAL("PatternMatch(const_StringView,const_StringView,const_Boole)-Fail3",
                   false,StringTools::PatternMatch(FirstSource,ThirdPattern,false));
        TEST_EQUAL("PatternMatch(const_StringView,const_StringView,const_Boole)-Fail4",
                   false,StringTools::PatternMatch(ThirdSource,SixthPattern,true));
    }//Pattern Matching

    {//Convert-From-String Functions
        TEST_EQUAL_EPSILON("ConvertFromString(const_StringView)-Real",
                   Real(3.1415926),StringTools::ConvertFromString<Real>("3.1415926"));
        TEST_EQUAL("ConvertFromString(const_StringView)-Integer",
                   Integer(-7272),StringTools::ConvertFromString<Integer>("-7272"));
        TEST_EQUAL("ConvertFromString(const_StringView)-Whole",
                   Whole(8383),StringTools::ConvertFromString<Whole>("8383"));
        TEST_EQUAL("ConvertFromString(const_StringView)-Int8",
                   static_cast<Int8>(-45),StringTools::ConvertFromString<Int8>("-45"));
        TEST_EQUAL("ConvertFromString(const_StringView)-UInt8",
                   static_cast<UInt8>(66),StringTools::ConvertFromString<UInt8>("66"));
        TEST_EQUAL("ConvertFromString(const_StringView)-Int16",
                   Int16(-30100),StringTools::ConvertFromString<Int16>("-30100"));
        TEST_EQUAL("ConvertFromString(const_StringView)-UInt16",
                   UInt16(27015),StringTools::ConvertFromString<UInt16>("27015"));
        TEST_EQUAL("ConvertFromString(const_StringView)-Int32",
                   Int32(-1256844),StringTools::ConvertFromString<Int32>("-1256844"));
        TEST_EQUAL("ConvertFromString(const_StringView)-UInt32",
                   UInt32(3872423),StringTools::ConvertFromString<UInt32>("3872423"));

        TEST_EQUAL("ConvertToBool(const_StringView,const_Boole)-Yes",
                   true,StringTools::ConvertToBool("Yes",false));
        TEST_EQUAL("ConvertToBool(const_StringView,const_Boole)-No",
                   false,StringTools::ConvertToBool("No",true));
        TEST_EQUAL("ConvertToBool(const_StringView,const_Boole)-1",
                   true,StringTools::ConvertToBool("1",false));
        TEST_EQUAL("ConvertToBool(const_StringView,const_Boole)-0",
                   false,StringTools::ConvertToBool("0",true));
        TEST_EQUAL("ConvertToBool(const_StringView,const_Boole)-True",
                   true,StringTools::ConvertToBool("True",false));
        TEST_EQUAL("ConvertToBool(const_StringView,const_Boole)-False",
                   false,StringTools::ConvertToBool("False",true));
        TEST_EQUAL("ConvertToBool(const_StringView,const_Boole)-Default",
                   true,StringTools::ConvertToBool("Yes"));
        TEST_EQUAL("ConvertToBool(const_StringView,const_Boole)-Gibberish",
                   false,StringTools::ConvertToBool("Gibberish"));
    }//Convert-From-String Functions

    {//Convert-To-String Functions
        TEST_EQUAL("ConvertToString(const_T&)-BooleTrue",String("true"),StringTools::ConvertToString(Boole(true)));
        TEST_EQUAL("ConvertToString(const_T&)-BooleFalse",String("false"),StringTools::ConvertToString(Boole(false)));
        TEST_EQUAL("ConvertToString(const_T&)-Real",String("3.14"),StringTools::ConvertToString(Real(3.14)));
        TEST_EQUAL("ConvertToString(const_T&)-Integer",String("-7272"),StringTools::ConvertToString(Integer(-7272)));
        TEST_EQUAL("ConvertToString(const_T&)-Whole",String("8383"),StringTools::ConvertToString(Whole(8383)));
        TEST_EQUAL("ConvertToString(const_T&)-Int8",String("-45"),StringTools::ConvertToString(Int8(-45)));
        TEST_EQUAL("ConvertToString(const_T&)-UInt8",String("66"),StringTools::ConvertToString(UInt8(66)));
        TEST_EQUAL("ConvertToString(const_T&)-Int16",String("-30100"),StringTools::ConvertToString(Int16(-30100)));
        TEST_EQUAL("ConvertToString(const_T&)-UInt16",String("27015"),StringTools::ConvertToString(UInt16(27015)));
        TEST_EQUAL("ConvertToString(const_T&)-Int32",String("-1256844"),StringTools::ConvertToString(Int32(-1256844)));
        TEST_EQUAL("ConvertToString(const_T&)-UInt32",String("3872423"),StringTools::ConvertToString(UInt32(3872423)));
    }//Convert-To-String Functions
}

#endif
