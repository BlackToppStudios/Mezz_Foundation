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
#ifndef Mezz_Foundation_Base64Benchmarks_h
#define Mezz_Foundation_Base64Benchmarks_h

/// @file
/// @brief This contains benchmarks tests on the performance of the CountedPtr class.

#include "MezzTest.h"

#include "Base64.h"
#include "MezzException.h"
#include "RuntimeStatics.h"

// This ought to be replaced with "SUPPRESS_ALL_WARNINGS" when that becomes available.
// This is reference code from other people and we're not responsible for it, in fact
// we're benchmarking this against the code we ARE responsible for.
SAVE_WARNING_STATE
SUPPRESS_GCC_WARNING("-Wstrict-overflow")
SUPPRESS_GCC_WARNING("-Wconversion")
SUPPRESS_CLANG_WARNING("-Wglobal-constructors")
SUPPRESS_CLANG_WARNING("-Wexit-time-destructors")
SUPPRESS_VC_WARNING(4267)

// Forward declares for the functions defined in this file because GCC is picky about this.
Mezzanine::String ReneBase64Encode(Mezzanine::UInt8 const* BytesToEncode, unsigned int Length);
Mezzanine::String ReneBase64Decode(Mezzanine::String const& EncodedString);

// Code change to Match BTS naming conventions and formatting
// Copyright information in Base64.cpp
/// @brief The library of characters used in Rene's Base64 implementation
static const Mezzanine::String Base64Chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

// Code change to Match BTS naming conventions and formatting
// Copyright information in Base64.cpp
/// @brief Rene's Base64 implementation for comparison purposes.
/// @param BytesToEncode A buffer of bytes which is to be encoded. NULL bytes are encoded and not used for termination
/// @param Length The Length of the buffer pointed to by BytesToEncode
/// @return A String containing the encoded value.
Mezzanine::String ReneBase64Encode(Mezzanine::UInt8 const* BytesToEncode, unsigned int Length)
{
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (Length--)
    {
        char_array_3[i++] = *(BytesToEncode++);
        if (i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                { ret += Base64Chars[char_array_4[i]]; }
            i = 0;
        }
    }

    if (i)
    {
        for(j = i; j < 3; j++)
            { char_array_3[j] = '\0'; }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            { ret += Base64Chars[char_array_4[j]]; }

        while((i++ < 3))
        { ret += '='; }
    }

    return ret;
}
// Code change to Match BTS naming conventions and formatting
// Copyright information in Base64.cpp
/// @brief A reference implementation of Base64 decoding as provide by Rene.
/// @param EncodedString The String to be decoded as provided by ReneBase64Encode()
/// @return A string Containing the resulting binary.
Mezzanine::String ReneBase64Decode(Mezzanine::String const& EncodedString)
{
    int in_len = EncodedString.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    Mezzanine::String ret;

    while (in_len-- && ( EncodedString[in_] != '=') &&
           Mezzanine::Base64::IsBase64(EncodedString[in_]))
    {
        char_array_4[i++] = EncodedString[in_]; in_++;
        if (i ==4)
        {
            for (i = 0; i <4; i++)
                { char_array_4[i] = Base64Chars.find(char_array_4[i]); }

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                { ret += char_array_3[i]; }
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j <4; j++)
            { char_array_4[j] = 0; }

        for (j = 0; j <4; j++)
            { char_array_4[j] = Base64Chars.find(char_array_4[j]); }

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
            { ret += char_array_3[j]; }
    }

    return ret;
    //return BinaryBuffer();
}

RESTORE_WARNING_STATE


BENCHMARK_TEST_GROUP(Base64Benchmarks,Base64Benchmarks)
{
    using namespace Mezzanine;

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

    using ClockType = std::chrono::high_resolution_clock;
    using TimeType = ClockType::time_point;
    using DurationType = ClockType::duration;

    const Whole TestCount = 100000;

    String OutputS; //To prevent optimizing this whole chunk away.
    BinaryBuffer OutputB; //To prevent optimizing this whole chunk away.

    TestLog << "\nBenchmarking this for the sake of performance is dumb and I know it. "
            << "This is not performance sensitive code, I just wanted to test my ability to "
            << "gauge subtle non-optimal performance in code when I saw it.\n\n";

    TimeType Begin = ClockType::now();
    for( Whole c = 0 ; c < TestCount ; c++ )
        { OutputS = ReneBase64Decode(GettysburgAddressBase64); }
    TimeType End = ClockType::now();
    DurationType ReneDecodeTime = End - Begin;

    TestLog << "Decoding With Rene's algorithm took "
            << std::chrono::duration_cast<std::chrono::microseconds>(ReneDecodeTime).count()
            << " microseconds for " << TestCount
            << " iterations and results like: " << OutputS.substr(0,20)
            << "..." << OutputS.substr(OutputS.size() - 20,20) << "\n";

    Begin = ClockType::now();
    for( Whole c = 0 ; c < TestCount ; c++ )
    {
        OutputB = Base64::Decode(GettysburgAddressBase64);
        // Example line provided below for testing Strings.
        //OutputS = Base64Decode(GettysburgAddressBase64).ToString();
    }
    End = ClockType::now();
    DurationType BTSDecodeTime = End - Begin;
    OutputS = OutputB.ToString();
    TestLog << "Decoding With BTS algorithm took "
            << std::chrono::duration_cast<std::chrono::microseconds>(BTSDecodeTime).count()
            << " microseconds for " << TestCount
            << " iterations and results like: " << OutputS.substr(0,20)
            << "..." << OutputS.substr(OutputS.size() - 20,20) << "\n";

    TestLog << "The new decoding algorithm takes about "
            << double( ( BTSDecodeTime * 1000) / (ReneDecodeTime) ) / 10 << "% as long as the original.\n\n";

    TEST_WARN( "DecodeTime", Mezzanine::RuntimeStatic::Debug() ? true : BTSDecodeTime < ReneDecodeTime);

    //Debug on tachyon Intel 6970hq
    /*  Decoding With Rene's algorithm took 745425 microseconds for 10000 iterations and results like: Four score and seven...rish from the earth.
        Decoding With BTS algorithm took 887304 microseconds for 10000 iterations and results like: Four score and seven...rish from the earth.
        The new decoding algorithm takes about 119% as long as the original. */
    //Release on tachyon
    /*  Decoding With Rene's algorithm took 462390 microseconds for 10000 iterations and results like: Four score and seven...rish from the earth.
        Decoding With BTS algorithm took 344986 microseconds for 10000 iterations and results like: Four score and seven...rish from the earth.
        The new decoding algorithm takes about 74.6% as long as the original. */

    //Debug on NostalgiaForInfinity about a 3.73x speed up in hardware and compiler advances
    /*  Decoding With Rene's algorithm took 1998702 microseconds for 100000 iterations and results like: Four score and seven...rish from the earth.
        Decoding With BTS algorithm took 1787039 microseconds for 100000 iterations and results like: Four score and seven...rish from the earth.
        The new decoding algorithm takes about 88.4% as long as the original. */
    //Release on NostalgiaForInfinity
    /* Decoding With Rene's algorithm took 1920102 microseconds for 100000 iterations and results like: Four score and seven...rish from the earth.
       Decoding With BTS algorithm took 1635805 microseconds for 100000 iterations and results like: Four score and seven...rish from the earth.
       The new decoding algorithm takes about 78% as long as the original. */
}

#endif
