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
#ifndef Mezz_Foundation_Base64Tests_h
#define Mezz_Foundation_Base64Tests_h

/// @file
/// @brief This file tests the functionality of Base64 encoding/decoding and support functions.

#include "MezzTest.h"

#include "Base64.h"
#include "MezzException.h"

#include <iostream>

AUTOMATIC_TEST_GROUP(Base64Tests,Base64)
{
    using namespace Mezzanine;

    const String Test1String("Test");
    const String Test1Base64("VGVzdA==");

    const String Test2String("Mover");
    const String Test2Base64("TW92ZXI=");

    const String Test3String("Shaker");
    const String Test3Base64("U2hha2Vy");

    const String UnicodeString("Iｎｔèｒｎáｔìｏｎàｌïｚâｔｉòｎ");
    const String UnicodeBase64("Se+9ju+9lMOo772S772Ow6HvvZTDrO+9j++9jsOg772Mw6/vvZrDou+9lO+9icOy772O");

    const String GettysburgAddress("Four score and seven years ago our fathers brought forth on this continent"
                                   " a new nation, conceived in liberty, and dedicated to the proposition that"
                                   " all men are created equal. Now we are engaged in a great civil war, testing"
                                   " whether that nation, or any nation, so conceived and so dedicated, can long"
                                   " endure. We are met on a great battle-field of that war. We have come to"
                                   " dedicate a portion of that field, as a final resting place for those who here"
                                   " gave their lives that that nation might live. It is altogether fitting and"
                                   " proper that we should do this. But, in a larger sense, we can not dedicate,"
                                   " we can not consecrate, we can not hallow this ground. The brave men, living"
                                   " and dead, who struggled here, have consecrated it, far above our poor power"
                                   " to add or detract. The world will little note, nor long remember what we say"
                                   " here, but it can never forget what they did here. It is for us the living,"
                                   " rather, to be dedicated here to the unfinished work which they who fought"
                                   " here have thus far so nobly advanced. It is rather for us to be here dedicated"
                                   " to the great task remaining before us-that from these honored dead we take"
                                   " increased devotion to that cause for which they gave the last full measure"
                                   " of devotion-that we here highly resolve that these dead shall not have died"
                                   " in vain-that this nation, under God, shall have a new birth of freedom-and"
                                   " that government of the people, by the people, for the people, shall not perish"
                                   " from the earth.");
    const String GettysburgAddressBase64("Rm91ciBzY29yZSBhbmQgc2V2ZW4geWVhcnMgYWdvIG91ciBmYXRoZXJzIGJyb3VnaHQgZm9ydG"
                                         "ggb24gdGhpcyBjb250aW5lbnQgYSBuZXcgbmF0aW9uLCBjb25jZWl2ZWQgaW4gbGliZXJ0eSwg"
                                         "YW5kIGRlZGljYXRlZCB0byB0aGUgcHJvcG9zaXRpb24gdGhhdCBhbGwgbWVuIGFyZSBjcmVhdG"
                                         "VkIGVxdWFsLiBOb3cgd2UgYXJlIGVuZ2FnZWQgaW4gYSBncmVhdCBjaXZpbCB3YXIsIHRlc3Rp"
                                         "bmcgd2hldGhlciB0aGF0IG5hdGlvbiwgb3IgYW55IG5hdGlvbiwgc28gY29uY2VpdmVkIGFuZC"
                                         "BzbyBkZWRpY2F0ZWQsIGNhbiBsb25nIGVuZHVyZS4gV2UgYXJlIG1ldCBvbiBhIGdyZWF0IGJh"
                                         "dHRsZS1maWVsZCBvZiB0aGF0IHdhci4gV2UgaGF2ZSBjb21lIHRvIGRlZGljYXRlIGEgcG9ydG"
                                         "lvbiBvZiB0aGF0IGZpZWxkLCBhcyBhIGZpbmFsIHJlc3RpbmcgcGxhY2UgZm9yIHRob3NlIHdo"
                                         "byBoZXJlIGdhdmUgdGhlaXIgbGl2ZXMgdGhhdCB0aGF0IG5hdGlvbiBtaWdodCBsaXZlLiBJdC"
                                         "BpcyBhbHRvZ2V0aGVyIGZpdHRpbmcgYW5kIHByb3BlciB0aGF0IHdlIHNob3VsZCBkbyB0aGlz"
                                         "LiBCdXQsIGluIGEgbGFyZ2VyIHNlbnNlLCB3ZSBjYW4gbm90IGRlZGljYXRlLCB3ZSBjYW4gbm"
                                         "90IGNvbnNlY3JhdGUsIHdlIGNhbiBub3QgaGFsbG93IHRoaXMgZ3JvdW5kLiBUaGUgYnJhdmUg"
                                         "bWVuLCBsaXZpbmcgYW5kIGRlYWQsIHdobyBzdHJ1Z2dsZWQgaGVyZSwgaGF2ZSBjb25zZWNyYX"
                                         "RlZCBpdCwgZmFyIGFib3ZlIG91ciBwb29yIHBvd2VyIHRvIGFkZCBvciBkZXRyYWN0LiBUaGUg"
                                         "d29ybGQgd2lsbCBsaXR0bGUgbm90ZSwgbm9yIGxvbmcgcmVtZW1iZXIgd2hhdCB3ZSBzYXkgaG"
                                         "VyZSwgYnV0IGl0IGNhbiBuZXZlciBmb3JnZXQgd2hhdCB0aGV5IGRpZCBoZXJlLiBJdCBpcyBm"
                                         "b3IgdXMgdGhlIGxpdmluZywgcmF0aGVyLCB0byBiZSBkZWRpY2F0ZWQgaGVyZSB0byB0aGUgdW"
                                         "5maW5pc2hlZCB3b3JrIHdoaWNoIHRoZXkgd2hvIGZvdWdodCBoZXJlIGhhdmUgdGh1cyBmYXIg"
                                         "c28gbm9ibHkgYWR2YW5jZWQuIEl0IGlzIHJhdGhlciBmb3IgdXMgdG8gYmUgaGVyZSBkZWRpY2"
                                         "F0ZWQgdG8gdGhlIGdyZWF0IHRhc2sgcmVtYWluaW5nIGJlZm9yZSB1cy10aGF0IGZyb20gdGhl"
                                         "c2UgaG9ub3JlZCBkZWFkIHdlIHRha2UgaW5jcmVhc2VkIGRldm90aW9uIHRvIHRoYXQgY2F1c2"
                                         "UgZm9yIHdoaWNoIHRoZXkgZ2F2ZSB0aGUgbGFzdCBmdWxsIG1lYXN1cmUgb2YgZGV2b3Rpb24t"
                                         "dGhhdCB3ZSBoZXJlIGhpZ2hseSByZXNvbHZlIHRoYXQgdGhlc2UgZGVhZCBzaGFsbCBub3QgaG"
                                         "F2ZSBkaWVkIGluIHZhaW4tdGhhdCB0aGlzIG5hdGlvbiwgdW5kZXIgR29kLCBzaGFsbCBoYXZl"
                                         "IGEgbmV3IGJpcnRoIG9mIGZyZWVkb20tYW5kIHRoYXQgZ292ZXJubWVudCBvZiB0aGUgcGVvcG"
                                         "xlLCBieSB0aGUgcGVvcGxlLCBmb3IgdGhlIHBlb3BsZSwgc2hhbGwgbm90IHBlcmlzaCBmcm9t"
                                         "IHRoZSBlYXJ0aC4=");


    {//IsBase64
        TEST_EQUAL("IsBase64(const_Char8)-a",true,Base64::IsBase64('a'))
        TEST_EQUAL("IsBase64(const_Char8)-b",true,Base64::IsBase64('b'))
        TEST_EQUAL("IsBase64(const_Char8)-c",true,Base64::IsBase64('c'))
        TEST_EQUAL("IsBase64(const_Char8)-d",true,Base64::IsBase64('d'))
        TEST_EQUAL("IsBase64(const_Char8)-e",true,Base64::IsBase64('e'))
        TEST_EQUAL("IsBase64(const_Char8)-f",true,Base64::IsBase64('f'))
        TEST_EQUAL("IsBase64(const_Char8)-g",true,Base64::IsBase64('g'))
        TEST_EQUAL("IsBase64(const_Char8)-h",true,Base64::IsBase64('h'))
        TEST_EQUAL("IsBase64(const_Char8)-i",true,Base64::IsBase64('i'))
        TEST_EQUAL("IsBase64(const_Char8)-j",true,Base64::IsBase64('j'))
        TEST_EQUAL("IsBase64(const_Char8)-k",true,Base64::IsBase64('k'))
        TEST_EQUAL("IsBase64(const_Char8)-l",true,Base64::IsBase64('l'))
        TEST_EQUAL("IsBase64(const_Char8)-m",true,Base64::IsBase64('m'))
        TEST_EQUAL("IsBase64(const_Char8)-n",true,Base64::IsBase64('n'))
        TEST_EQUAL("IsBase64(const_Char8)-o",true,Base64::IsBase64('o'))
        TEST_EQUAL("IsBase64(const_Char8)-p",true,Base64::IsBase64('p'))
        TEST_EQUAL("IsBase64(const_Char8)-q",true,Base64::IsBase64('q'))
        TEST_EQUAL("IsBase64(const_Char8)-r",true,Base64::IsBase64('r'))
        TEST_EQUAL("IsBase64(const_Char8)-s",true,Base64::IsBase64('s'))
        TEST_EQUAL("IsBase64(const_Char8)-t",true,Base64::IsBase64('t'))
        TEST_EQUAL("IsBase64(const_Char8)-u",true,Base64::IsBase64('u'))
        TEST_EQUAL("IsBase64(const_Char8)-v",true,Base64::IsBase64('v'))
        TEST_EQUAL("IsBase64(const_Char8)-w",true,Base64::IsBase64('w'))
        TEST_EQUAL("IsBase64(const_Char8)-x",true,Base64::IsBase64('x'))
        TEST_EQUAL("IsBase64(const_Char8)-y",true,Base64::IsBase64('y'))
        TEST_EQUAL("IsBase64(const_Char8)-z",true,Base64::IsBase64('z'))
        TEST_EQUAL("IsBase64(const_Char8)-A",true,Base64::IsBase64('A'))
        TEST_EQUAL("IsBase64(const_Char8)-B",true,Base64::IsBase64('B'))
        TEST_EQUAL("IsBase64(const_Char8)-C",true,Base64::IsBase64('C'))
        TEST_EQUAL("IsBase64(const_Char8)-D",true,Base64::IsBase64('D'))
        TEST_EQUAL("IsBase64(const_Char8)-E",true,Base64::IsBase64('E'))
        TEST_EQUAL("IsBase64(const_Char8)-F",true,Base64::IsBase64('F'))
        TEST_EQUAL("IsBase64(const_Char8)-G",true,Base64::IsBase64('G'))
        TEST_EQUAL("IsBase64(const_Char8)-H",true,Base64::IsBase64('H'))
        TEST_EQUAL("IsBase64(const_Char8)-I",true,Base64::IsBase64('I'))
        TEST_EQUAL("IsBase64(const_Char8)-J",true,Base64::IsBase64('J'))
        TEST_EQUAL("IsBase64(const_Char8)-K",true,Base64::IsBase64('K'))
        TEST_EQUAL("IsBase64(const_Char8)-L",true,Base64::IsBase64('L'))
        TEST_EQUAL("IsBase64(const_Char8)-M",true,Base64::IsBase64('M'))
        TEST_EQUAL("IsBase64(const_Char8)-N",true,Base64::IsBase64('N'))
        TEST_EQUAL("IsBase64(const_Char8)-O",true,Base64::IsBase64('O'))
        TEST_EQUAL("IsBase64(const_Char8)-P",true,Base64::IsBase64('P'))
        TEST_EQUAL("IsBase64(const_Char8)-Q",true,Base64::IsBase64('Q'))
        TEST_EQUAL("IsBase64(const_Char8)-R",true,Base64::IsBase64('R'))
        TEST_EQUAL("IsBase64(const_Char8)-S",true,Base64::IsBase64('S'))
        TEST_EQUAL("IsBase64(const_Char8)-T",true,Base64::IsBase64('T'))
        TEST_EQUAL("IsBase64(const_Char8)-U",true,Base64::IsBase64('U'))
        TEST_EQUAL("IsBase64(const_Char8)-V",true,Base64::IsBase64('V'))
        TEST_EQUAL("IsBase64(const_Char8)-W",true,Base64::IsBase64('W'))
        TEST_EQUAL("IsBase64(const_Char8)-X",true,Base64::IsBase64('X'))
        TEST_EQUAL("IsBase64(const_Char8)-Y",true,Base64::IsBase64('Y'))
        TEST_EQUAL("IsBase64(const_Char8)-Z",true,Base64::IsBase64('Z'))
        TEST_EQUAL("IsBase64(const_Char8)-0",true,Base64::IsBase64('0'))
        TEST_EQUAL("IsBase64(const_Char8)-1",true,Base64::IsBase64('1'))
        TEST_EQUAL("IsBase64(const_Char8)-2",true,Base64::IsBase64('2'))
        TEST_EQUAL("IsBase64(const_Char8)-3",true,Base64::IsBase64('3'))
        TEST_EQUAL("IsBase64(const_Char8)-4",true,Base64::IsBase64('4'))
        TEST_EQUAL("IsBase64(const_Char8)-5",true,Base64::IsBase64('5'))
        TEST_EQUAL("IsBase64(const_Char8)-6",true,Base64::IsBase64('6'))
        TEST_EQUAL("IsBase64(const_Char8)-7",true,Base64::IsBase64('7'))
        TEST_EQUAL("IsBase64(const_Char8)-8",true,Base64::IsBase64('8'))
        TEST_EQUAL("IsBase64(const_Char8)-9",true,Base64::IsBase64('9'))
        TEST_EQUAL("IsBase64(const_Char8)-/",true,Base64::IsBase64('/'))
        TEST_EQUAL("IsBase64(const_Char8)-=",true,Base64::IsBase64('='))
        TEST_EQUAL("IsBase64(const_Char8)-+",true,Base64::IsBase64('+'))
        TEST_EQUAL("IsBase64(const_Char8)-NewLine",false,Base64::IsBase64('\n'))
        TEST_EQUAL("IsBase64(const_Char8)-Tab",false,Base64::IsBase64('\t'))
        TEST_EQUAL("IsBase64(const_Char8)-Space",false,Base64::IsBase64(' '))
        TEST_EQUAL("IsBase64(const_Char8)-.",false,Base64::IsBase64('.'))
        TEST_EQUAL("IsBase64(const_Char8)-^",false,Base64::IsBase64('^'))
        TEST_EQUAL("IsBase64(const_Char8)-:",false,Base64::IsBase64(':'))
        TEST_EQUAL("IsBase64(const_Char8)-@",false,Base64::IsBase64('@'))
        TEST_EQUAL("IsBase64(const_Char8)-'",false,Base64::IsBase64('\''))
        TEST_EQUAL("IsBase64(const_Char8)-[",false,Base64::IsBase64('['))
        TEST_EQUAL("IsBase64(const_Char8)-]",false,Base64::IsBase64(']'))
        TEST_EQUAL("IsBase64(const_Char8)-<",false,Base64::IsBase64('<'))
        TEST_EQUAL("IsBase64(const_Char8)->",false,Base64::IsBase64('>'))
        TEST_EQUAL("IsBase64(const_Char8)-Value0",false,Base64::IsBase64(0))
        TEST_EQUAL("IsBase64(const_Char8)-Value7",false,Base64::IsBase64(7))
        TEST_EQUAL("IsBase64(const_Char8)-Value10",false,Base64::IsBase64(10))
        TEST_EQUAL("IsBase64(const_Char8)-Value16",false,Base64::IsBase64(16))
        TEST_EQUAL("IsBase64(const_Char8)-Value27",false,Base64::IsBase64(27))
        TEST_EQUAL("IsBase64(const_Char8)-Value32",false,Base64::IsBase64(32))
        TEST_EQUAL("IsBase64(const_Char8)-Value34",false,Base64::IsBase64(34))
        TEST_EQUAL("IsBase64(const_Char8)-Value127",false,Base64::IsBase64(127))
        TEST_EQUAL("IsBase64(const_Char8)-ValueNeg127",false,Base64::IsBase64(-127))
        TEST_EQUAL("IsBase64(const_Char8)-ValueNeg34",false,Base64::IsBase64(-34))
        TEST_EQUAL("IsBase64(const_Char8)-ValueNeg27",false,Base64::IsBase64(-27))
        TEST_EQUAL("IsBase64(const_Char8)-ValueNeg7",false,Base64::IsBase64(-7))
    }//IsBase64

    {//Size Prediction
        TEST_EQUAL( "PredictBinarySize(const_String&)-First",
                    Test1String.size(),Base64::PredictBinarySize(Test1Base64.c_str(),Test1Base64.length()) )
        TEST_EQUAL( "PredictBinarySize(const_String&)-Second",
                    Test2String.size(),Base64::PredictBinarySize(Test2Base64.c_str(),Test2Base64.length()) )
        TEST_EQUAL( "PredictBinarySize(const_String&)-Third",
                    Test3String.size(),Base64::PredictBinarySize(Test3Base64.c_str(),Test3Base64.length()) )
        TEST_EQUAL( "PredictBinarySize(const_String&)-Unicode",
                    UnicodeString.size(),Base64::PredictBinarySize(UnicodeBase64) )
        TEST_EQUAL( "PredictBinarySize(const_String&)-Gettysburg",
                    GettysburgAddress.size(),Base64::PredictBinarySize(GettysburgAddressBase64) )

        const char* FailStr = "lol";
        TEST_THROW( "PredictBinarySize(const_SizeType)-Throw",
                    Mezzanine::Exception::Base64Source,
                    [FailStr](){ std::cerr << Base64::PredictBinarySize(FailStr,3); } )

        TEST_EQUAL( "PredictBase64Size(const_SizeType)-First",
                    Test1Base64.size(),Base64::PredictBase64Size(Test1String.length()) )
        TEST_EQUAL( "PredictBase64Size(const_SizeType)-Second",
                    Test2Base64.size(),Base64::PredictBase64Size(Test2String.length()) )
        TEST_EQUAL( "PredictBase64Size(const_SizeType)-Third",
                    Test3Base64.size(),Base64::PredictBase64Size(Test3String.length()) )
        TEST_EQUAL( "PredictBase64Size(const_SizeType)-Unicode",
                    UnicodeBase64.size(),Base64::PredictBase64Size(UnicodeString.length()) )
        TEST_EQUAL( "PredictBase64Size(const_SizeType)-Gettysburg",
                    GettysburgAddressBase64.size(),Base64::PredictBase64Size(GettysburgAddress.length()) )
    }//Size Prediction

    {//Encode and Decode
        SizeType BytesWritten = 0;

        BinaryBuffer TestBuffer1(Test1String);
        TEST_EQUAL( "Encode(const_BinaryBuffer&)-First",
                    Test1Base64,Base64::Encode(TestBuffer1) )
        BinaryBuffer TestBuffer2(Test2String);
        TEST_EQUAL( "Encode(const_BinaryBuffer&)-Second",
                    Test2Base64,Base64::Encode(TestBuffer2) )
        BinaryBuffer TestBuffer3(Test3String);
        TEST_EQUAL( "Encode(const_BinaryBuffer&)-Third",
                    Test3Base64,Base64::Encode(TestBuffer3) )

        const SizeType UnicodeEncodeBufSize = Base64::PredictBase64Size( UnicodeString.length() );
        Char8* UnicodeEncodeBuf = new Char8[UnicodeEncodeBufSize];
        const UInt8* UnicodeStrBuf = reinterpret_cast<const UInt8*>(UnicodeString.c_str());
        BytesWritten = Base64::EncodeRawBuffer(UnicodeStrBuf,UnicodeString.length(),
                                               UnicodeEncodeBuf,UnicodeEncodeBufSize);
        TEST_EQUAL( "EncodeRawBuffer(const_UInt8*,const_SizeType,Char8*,const_SizeType)-Unicode-BytesWritten",
                    UnicodeEncodeBufSize,BytesWritten )
        TEST_EQUAL( "EncodeRawBuffer(const_UInt8*,const_SizeType,Char8*,const_SizeType)-Unicode-Content",
                    true,String(UnicodeEncodeBuf,UnicodeEncodeBufSize) == UnicodeBase64 )

        const SizeType AddressEncodeBufSize = Base64::PredictBase64Size( GettysburgAddress.length() );
        Char8* AddressEncodeBuf = new Char8[AddressEncodeBufSize];
        const UInt8* AddressStrBuf = reinterpret_cast<const UInt8*>(GettysburgAddress.c_str());
        BytesWritten = Base64::EncodeRawBuffer(AddressStrBuf,GettysburgAddress.length(),
                                               AddressEncodeBuf,AddressEncodeBufSize);
        TEST_EQUAL( "EncodeRawBuffer(const_UInt8*,const_SizeType,Char8*,const_SizeType)-Gettysburg-BytesWritten",
                    AddressEncodeBufSize,BytesWritten )
        TEST_EQUAL( "EncodeRawBuffer(const_UInt8*,const_SizeType,Char8*,const_SizeType)-Gettysburg-ContentMatch",
                    true,String(AddressEncodeBuf,AddressEncodeBufSize) == GettysburgAddressBase64 )

        auto EncodeThrowFunct = [] () {
            BinaryBuffer ThrowBuf(10);
            std::unique_ptr<char[]> ThrowStr(new char[4]);
            std::cerr << Base64::EncodeRawBuffer(ThrowBuf.Binary,ThrowBuf.Size,ThrowStr.get(),4);
        }
        TEST_THROW( "EncodeRawBuffer(const_UInt8*,const_SizeType,Char8*,const_SizeType)-Throw-SmallDest",
                    Mezzanine::Exception::Base64Destination,
                    EncodeThrowFunct )

        BinaryBuffer TestBuffer1Base64 = Base64::Decode(Test1Base64);
        const char* TestBuffer1BinaryPtr = reinterpret_cast<char*>(TestBuffer1Base64.Binary);
        TEST_EQUAL( "Decode(const_String&)-First",
                    Test1String,String(TestBuffer1BinaryPtr,TestBuffer1Base64.Size) )
        BinaryBuffer TestBuffer2Base64 = Base64::Decode(Test2Base64);
        const char* TestBuffer2BinaryPtr = reinterpret_cast<char*>(TestBuffer2Base64.Binary);
        TEST_EQUAL( "Decode(const_String&)-Second",
                    Test2String,String(TestBuffer2BinaryPtr,TestBuffer2Base64.Size) )
        BinaryBuffer TestBuffer3Base64 = Base64::Decode(Test3Base64);
        const char* TestBuffer3BinaryPtr = reinterpret_cast<char*>(TestBuffer3Base64.Binary);
        TEST_EQUAL( "Decode(const_String&)-Third",
                    Test3String,String(TestBuffer3BinaryPtr,TestBuffer3Base64.Size) )

        const SizeType UnicodeDecodeBufSize = Base64::PredictBinarySize( UnicodeBase64 );
        UInt8* UnicodeDecodeBuf = new UInt8[UnicodeDecodeBufSize];
        BytesWritten = Base64::DecodeRawBuffer(UnicodeBase64.c_str(),UnicodeBase64.length(),
                                               UnicodeDecodeBuf,UnicodeDecodeBufSize);
        TEST_EQUAL( "DecodeRawBuffer(const_Char8*,const_SizeType,UInt8*,const_SizeType)-Unicode-BytesWritten",
                    UnicodeDecodeBufSize,BytesWritten )
        TEST_EQUAL( "DecodeRawBuffer(const_Char8*,const_SizeType,UInt8*,const_SizeType)-Unicode-Content",
                    true,String(reinterpret_cast<char*>(UnicodeDecodeBuf),UnicodeDecodeBufSize) == UnicodeString )

        const SizeType AddressDecodeBufSize = Base64::PredictBinarySize( GettysburgAddressBase64 );
        UInt8* AddressDecodeBuf = new UInt8[AddressDecodeBufSize];
        BytesWritten = Base64::DecodeRawBuffer(GettysburgAddressBase64.c_str(),GettysburgAddressBase64.length(),
                                               AddressDecodeBuf,AddressDecodeBufSize);
        TEST_EQUAL( "DecodeRawBuffer(const_Char8*,const_SizeType,UInt8*,const_SizeType)-Gettysburg-BytesWritten",
                    AddressDecodeBufSize,BytesWritten )
        TEST_EQUAL( "DecodeRawBuffer(const_Char8*,const_SizeType,UInt8*,const_SizeType)-Gettysburg-Content",
                    true,String(reinterpret_cast<char*>(AddressDecodeBuf),AddressDecodeBufSize) == GettysburgAddress )

        auto DecodeThrowOneFunct = [] () {
            BinaryBuffer ThrowBuf(2);
            String ThrowStr("Longer Than Two.");
            std::cerr << Base64::DecodeRawBuffer(ThrowStr.c_str(),ThrowStr.length(),ThrowBuf.Binary,ThrowBuf.Size);
        }
        TEST_THROW( "DecodeRawBuffer(const_Char8*,const_SizeType,UInt8*,const_SizeType)-Throw-SmallDest",
                    Mezzanine::Exception::Base64Destination,
                    DecodeThrowOneFunct )

        auto DecodeThrowTwoFunct = [] () {
            BinaryBuffer ThrowBuf(16);
            String ThrowStr("Fourteen Chars");
            std::cerr << Base64::DecodeRawBuffer(ThrowStr.c_str(),ThrowStr.length(),ThrowBuf.Binary,ThrowBuf.Size);
        }
        TEST_THROW( "DecodeRawBuffer(const_Char8*,const_SizeType,UInt8*,const_SizeType)-Throw-NotMultipleOfFour",
                    Mezzanine::Exception::Base64Source,
                    DecodeThrowTwoFunct )

        auto DecodeThrowThreeFunct = [] () {
            BinaryBuffer ThrowBuf(4);
            String ThrowStr("VG@zdA==");
            std::cerr << Base64::DecodeRawBuffer(ThrowStr.c_str(),ThrowStr.length(),ThrowBuf.Binary,ThrowBuf.Size);
        }
        TEST_THROW( "DecodeRawBuffer(const_Char8*,const_SizeType,UInt8*,const_SizeType)-Throw-NotBase64",
                    Mezzanine::Exception::Base64BadChar,
                    DecodeThrowThreeFunct )

        delete[] UnicodeEncodeBuf;
        delete[] AddressEncodeBuf;
        delete[] UnicodeDecodeBuf;
        delete[] AddressDecodeBuf;
    }//Encode and Decode

}

#endif
