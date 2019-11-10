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
#ifndef Mezz_Foundation_MurmurHashTests_h
#define Mezz_Foundation_MurmurHashTests_h

/// @file
/// @brief This file tests the functionality of the Murmur Hash generating functions.

#include "MezzTest.h"

#include "MurmurHash.h"

// This ought to be replaced with "SUPPRESS_ALL_WARNINGS" when that becomes available.
// We're not responsible for this code.
SAVE_WARNING_STATE
SUPPRESS_GCC_WARNING("-Wold-style-cast")
SUPPRESS_GCC_WARNING("-Wimplicit-fallthrough")
SUPPRESS_GCC_WARNING("-Wcast-align")
SUPPRESS_CLANG_WARNING("-Wc++98-compat-pedantic")
SUPPRESS_CLANG_WARNING("-Wsign-conversion")
SUPPRESS_CLANG_WARNING("-Wcast-align")
namespace aappleby {
#if defined(_MSC_VER)

#define FORCE_INLINE	__forceinline

#include <stdlib.h>

#define ROTL32(x,y)	_rotl(x,y)
#define ROTL64(x,y)	_rotl64(x,y)

#define BIG_CONSTANT(x) (x)

// Other compilers

#else	// defined(_MSC_VER)

#define	FORCE_INLINE inline __attribute__((always_inline))

inline uint32_t rotl32 ( uint32_t x, int8_t r )
{
    return (x << r) | (x >> (32 - r));
}

inline uint64_t rotl64 ( uint64_t x, int8_t r )
{
    return (x << r) | (x >> (64 - r));
}

#define	ROTL32(x,y)	rotl32(x,y)
#define ROTL64(x,y)	rotl64(x,y)

#define BIG_CONSTANT(x) (x##LLU)

#endif // !defined(_MSC_VER)

//-----------------------------------------------------------------------------
// Block read - if your platform needs to do endian-swapping or can only
// handle aligned reads, do the conversion here

FORCE_INLINE uint32_t getblock32 ( const uint32_t * p, int i )
{
    return p[i];
}

FORCE_INLINE uint64_t getblock64 ( const uint64_t * p, int i )
{
    return p[i];
}

//-----------------------------------------------------------------------------
// Finalization mix - force all bits of a hash block to avalanche

FORCE_INLINE uint32_t fmix32 ( uint32_t h )
{
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;

    return h;
}

//----------

FORCE_INLINE uint64_t fmix64 ( uint64_t k )
{
    k ^= k >> 33;
    k *= BIG_CONSTANT(0xff51afd7ed558ccd);
    k ^= k >> 33;
    k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
    k ^= k >> 33;

    return k;
}

void MurmurHash3_x86_32  ( const void * key, int len, uint32_t seed, void * out );
void MurmurHash3_x86_32  ( const void * key, int len, uint32_t seed, void * out )
{
    const uint8_t * data = (const uint8_t*)key;
    const int nblocks = len / 4;

    uint32_t h1 = seed;

    const uint32_t c1 = 0xcc9e2d51;
    const uint32_t c2 = 0x1b873593;

    //----------
    // body

    const uint32_t * blocks = (const uint32_t *)(data + nblocks*4);

    for(int i = -nblocks; i; i++)
    {
        uint32_t k1 = getblock32(blocks,i);

        k1 *= c1;
        k1 = ROTL32(k1,15);
        k1 *= c2;

        h1 ^= k1;
        h1 = ROTL32(h1,13);
        h1 = h1*5+0xe6546b64;
    }

    //----------
    // tail

    const uint8_t * tail = (const uint8_t*)(data + nblocks*4);

    uint32_t k1 = 0;

    switch(len & 3)
    {
    case 3: k1 ^= tail[2] << 16;
    case 2: k1 ^= tail[1] << 8;
    case 1: k1 ^= tail[0];
            k1 *= c1; k1 = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= len;

    h1 = fmix32(h1);

    *(uint32_t*)out = h1;
}

void MurmurHash3_x86_128 ( const void * key, int len, uint32_t seed, void * out );
void MurmurHash3_x86_128 ( const void * key, int len, uint32_t seed, void * out )
{
    const uint8_t * data = (const uint8_t*)key;
    const int nblocks = len / 16;

    uint32_t h1 = seed;
    uint32_t h2 = seed;
    uint32_t h3 = seed;
    uint32_t h4 = seed;

    const uint32_t c1 = 0x239b961b;
    const uint32_t c2 = 0xab0e9789;
    const uint32_t c3 = 0x38b34ae5;
    const uint32_t c4 = 0xa1e38b93;

    //----------
    // body

    const uint32_t * blocks = (const uint32_t *)(data + nblocks*16);

    for(int i = -nblocks; i; i++)
    {
        uint32_t k1 = getblock32(blocks,i*4+0);
        uint32_t k2 = getblock32(blocks,i*4+1);
        uint32_t k3 = getblock32(blocks,i*4+2);
        uint32_t k4 = getblock32(blocks,i*4+3);

        k1 *= c1; k1  = ROTL32(k1,15); k1 *= c2; h1 ^= k1;

        h1 = ROTL32(h1,19); h1 += h2; h1 = h1*5+0x561ccd1b;

        k2 *= c2; k2  = ROTL32(k2,16); k2 *= c3; h2 ^= k2;

        h2 = ROTL32(h2,17); h2 += h3; h2 = h2*5+0x0bcaa747;

        k3 *= c3; k3  = ROTL32(k3,17); k3 *= c4; h3 ^= k3;

        h3 = ROTL32(h3,15); h3 += h4; h3 = h3*5+0x96cd1c35;

        k4 *= c4; k4  = ROTL32(k4,18); k4 *= c1; h4 ^= k4;

        h4 = ROTL32(h4,13); h4 += h1; h4 = h4*5+0x32ac3b17;
    }

    //----------
    // tail

    const uint8_t * tail = (const uint8_t*)(data + nblocks*16);

    uint32_t k1 = 0;
    uint32_t k2 = 0;
    uint32_t k3 = 0;
    uint32_t k4 = 0;

    switch(len & 15)
    {
    case 15: k4 ^= tail[14] << 16;
    case 14: k4 ^= tail[13] << 8;
    case 13: k4 ^= tail[12] << 0;
             k4 *= c4; k4  = ROTL32(k4,18); k4 *= c1; h4 ^= k4;

    case 12: k3 ^= tail[11] << 24;
    case 11: k3 ^= tail[10] << 16;
    case 10: k3 ^= tail[ 9] << 8;
    case  9: k3 ^= tail[ 8] << 0;
             k3 *= c3; k3  = ROTL32(k3,17); k3 *= c4; h3 ^= k3;

    case  8: k2 ^= tail[ 7] << 24;
    case  7: k2 ^= tail[ 6] << 16;
    case  6: k2 ^= tail[ 5] << 8;
    case  5: k2 ^= tail[ 4] << 0;
             k2 *= c2; k2  = ROTL32(k2,16); k2 *= c3; h2 ^= k2;

    case  4: k1 ^= tail[ 3] << 24;
    case  3: k1 ^= tail[ 2] << 16;
    case  2: k1 ^= tail[ 1] << 8;
    case  1: k1 ^= tail[ 0] << 0;
             k1 *= c1; k1  = ROTL32(k1,15); k1 *= c2; h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= len; h2 ^= len; h3 ^= len; h4 ^= len;

    h1 += h2; h1 += h3; h1 += h4;
    h2 += h1; h3 += h1; h4 += h1;

    h1 = fmix32(h1);
    h2 = fmix32(h2);
    h3 = fmix32(h3);
    h4 = fmix32(h4);

    h1 += h2; h1 += h3; h1 += h4;
    h2 += h1; h3 += h1; h4 += h1;

    ((uint32_t*)out)[0] = h1;
    ((uint32_t*)out)[1] = h2;
    ((uint32_t*)out)[2] = h3;
    ((uint32_t*)out)[3] = h4;
}

void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );
void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out )
{
    const uint8_t * data = (const uint8_t*)key;
    const int nblocks = len / 16;

    uint64_t h1 = seed;
    uint64_t h2 = seed;

    const uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
    const uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

    //----------
    // body

    const uint64_t * blocks = (const uint64_t *)(data);

    for(int i = 0; i < nblocks; i++)
    {
        uint64_t k1 = getblock64(blocks,i*2+0);
        uint64_t k2 = getblock64(blocks,i*2+1);

        k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;

        h1 = ROTL64(h1,27); h1 += h2; h1 = h1*5+0x52dce729;

        k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

        h2 = ROTL64(h2,31); h2 += h1; h2 = h2*5+0x38495ab5;
    }

    //----------
    // tail

    const uint8_t * tail = (const uint8_t*)(data + nblocks*16);

    uint64_t k1 = 0;
    uint64_t k2 = 0;

    switch(len & 15)
    {
    case 15: k2 ^= ((uint64_t)tail[14]) << 48;
    case 14: k2 ^= ((uint64_t)tail[13]) << 40;
    case 13: k2 ^= ((uint64_t)tail[12]) << 32;
    case 12: k2 ^= ((uint64_t)tail[11]) << 24;
    case 11: k2 ^= ((uint64_t)tail[10]) << 16;
    case 10: k2 ^= ((uint64_t)tail[ 9]) << 8;
    case  9: k2 ^= ((uint64_t)tail[ 8]) << 0;
             k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

    case  8: k1 ^= ((uint64_t)tail[ 7]) << 56;
    case  7: k1 ^= ((uint64_t)tail[ 6]) << 48;
    case  6: k1 ^= ((uint64_t)tail[ 5]) << 40;
    case  5: k1 ^= ((uint64_t)tail[ 4]) << 32;
    case  4: k1 ^= ((uint64_t)tail[ 3]) << 24;
    case  3: k1 ^= ((uint64_t)tail[ 2]) << 16;
    case  2: k1 ^= ((uint64_t)tail[ 1]) << 8;
    case  1: k1 ^= ((uint64_t)tail[ 0]) << 0;
             k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= len; h2 ^= len;

    h1 += h2;
    h2 += h1;

    h1 = fmix64(h1);
    h2 = fmix64(h2);

    h1 += h2;
    h2 += h1;

    ((uint64_t*)out)[0] = h1;
    ((uint64_t*)out)[1] = h2;
}

}//aappleby
RESTORE_WARNING_STATE

AUTOMATIC_TEST_GROUP(MurmurHashTests,MurmurHash)
{
    using namespace Mezzanine;
    using namespace aappleby;

    using Result_x86 = Hashing::MurmurHashResult_x86_128;
    using Result_x64 = Hashing::MurmurHashResult_x64_128;
    // These were generated randomly via an online tool.
    const UInt32 FirstSeed = 0xec42027b;
    const UInt32 SecondSeed = 0x7f615220;

    const String TinyText = "Hi";
    const String SmallText = "I have no idea what I am doing. Seriously.";
    // Excerpt from Demosthenes' Third Philippic speech, delivered on the Pnyx Hill, Athens, in 341 BC.
    const String LargeText = "It is a strange thing, perhaps, that I am about to say, but it is true. "
                             "The worst feature in the past is that in which lies our best hope for the future. "
                             "And what is this? It is that you are in your present plight because you do not do "
                             "any part of your duty, small or great; for of course, if you were doing all that you "
                             "should do, and were still in this evil case, you could not even hope for any "
                             "improvement. As it is, Philip has conquered your indolence and your indifference; but "
                             "he has not conquered Athens. You have not been vanquished, you have never even stirred.";

    const int TinyTextSize = static_cast<int>(TinyText.size());
    const int SmallTextSize = static_cast<int>(SmallText.size());
    const int LargeTextSize = static_cast<int>(LargeText.size());

    // Binary randomly generated on random.org
    const std::array<UInt8,2> TinyBinary = { 0xd0, 0x0f };
    const std::array<UInt8,32> SmallBinary = { 0x22, 0xf2, 0x51, 0x91, 0x2d, 0x04, 0x47, 0x7e,
                                               0x75, 0xd6, 0x5d, 0x95, 0xdd, 0xad, 0xba, 0xdf,
                                               0x7e, 0x40, 0x52, 0xa2, 0x17, 0x01, 0x5a, 0x93,
                                               0x73, 0x9c, 0x30, 0xe6, 0xf8, 0xb3, 0xff, 0xf2 };
    const std::array<UInt8,128> LargeBinary = { 0x0f, 0x3b, 0x0a, 0x31, 0xe9, 0x6e, 0x36, 0xa3,
                                                0x04, 0x20, 0xfd, 0xce, 0xa6, 0xcc, 0x96, 0x9d,
                                                0x3c, 0x6c, 0x59, 0x0f, 0xd5, 0x92, 0xe7, 0x92,
                                                0x91, 0xec, 0x10, 0xac, 0x59, 0x26, 0x94, 0xb5,
                                                0x23, 0xef, 0x2e, 0xdb, 0xc4, 0xd2, 0x2f, 0x63,
                                                0x2d, 0x7b, 0x3d, 0x60, 0xa4, 0x3a, 0xf3, 0x27,
                                                0xdd, 0xbb, 0x3c, 0x9f, 0xcd, 0x14, 0x5a, 0xca,
                                                0xdf, 0xfb, 0x24, 0x00, 0xc1, 0xe7, 0x7a, 0x10,
                                                0x4f, 0xd7, 0x9d, 0xcc, 0x49, 0x76, 0xaa, 0xe7,
                                                0x9f, 0x14, 0xb0, 0x85, 0x65, 0x89, 0x6e, 0xb2,
                                                0x57, 0xee, 0x06, 0x70, 0xbd, 0x4b, 0x0f, 0xdc,
                                                0x3c, 0x06, 0xd6, 0x6f, 0x86, 0xbd, 0xa3, 0x36,
                                                0x65, 0xcf, 0xd2, 0x63, 0xa3, 0x03, 0x00, 0xa6,
                                                0x6f, 0xad, 0x8d, 0x27, 0xba, 0x8f, 0xeb, 0x15,
                                                0xa0, 0x7e, 0x9d, 0xfe, 0x81, 0x6f, 0xdc, 0x51,
                                                0x9f, 0xba, 0x02, 0x24, 0x71, 0x72, 0x7c, 0xba };

    const int TinyBinarySize = static_cast<int>(TinyBinary.size());
    const int SmallBinarySize = static_cast<int>(SmallBinary.size());
    const int LargeBinarySize = static_cast<int>(LargeBinary.size());

    {// x86_32 - Text
        UInt32 AapplebyTiny{};
        MurmurHash3_x86_32(TinyText.data(),TinyTextSize,FirstSeed,&AapplebyTiny);
        UInt32 MezzTiny = Hashing::MurmurHash3_x86_32(TinyText.data(),TinyText.size(),FirstSeed);
        TEST_EQUAL("MurmurHash3_x86_32(const_void*,const_Integer,const_UInt32)-Text-Tiny",
                   AapplebyTiny,MezzTiny);

        UInt32 AapplebySmall{};
        MurmurHash3_x86_32(SmallText.data(),SmallTextSize,FirstSeed,&AapplebySmall);
        UInt32 MezzSmall = Hashing::MurmurHash3_x86_32(SmallText.data(),SmallText.size(),FirstSeed);
        TEST_EQUAL("MurmurHash3_x86_32(const_void*,const_Integer,const_UInt32)-Text-Small",
                   AapplebySmall,MezzSmall);

        UInt32 AapplebyLarge{};
        MurmurHash3_x86_32(LargeText.data(),LargeTextSize,FirstSeed,&AapplebyLarge);
        UInt32 MezzLarge = Hashing::MurmurHash3_x86_32(LargeText.data(),LargeText.size(),FirstSeed);
        TEST_EQUAL("MurmurHash3_x86_32(const_void*,const_Integer,const_UInt32)-Text-Large",
                   AapplebyLarge,MezzLarge);
    }// x86_32 - Text

    {// x86_32 - Binary
        UInt32 AapplebyTiny{};
        MurmurHash3_x86_32(TinyBinary.data(),TinyBinarySize,SecondSeed,&AapplebyTiny);
        UInt32 MezzTiny = Hashing::MurmurHash3_x86_32(TinyBinary.data(),TinyBinary.size(),SecondSeed);
        TEST_EQUAL("MurmurHash3_x86_32(const_void*,const_Integer,const_UInt32)-Binary-Tiny",
                   AapplebyTiny,MezzTiny);

        UInt32 AapplebySmall{};
        MurmurHash3_x86_32(SmallBinary.data(),SmallBinarySize,SecondSeed,&AapplebySmall);
        UInt32 MezzSmall = Hashing::MurmurHash3_x86_32(SmallBinary.data(),SmallBinary.size(),SecondSeed);
        TEST_EQUAL("MurmurHash3_x86_32(const_void*,const_Integer,const_UInt32)-Binary-Small",
                   AapplebySmall,MezzSmall);

        UInt32 AapplebyLarge{};
        MurmurHash3_x86_32(LargeBinary.data(),LargeBinarySize,SecondSeed,&AapplebyLarge);
        UInt32 MezzLarge = Hashing::MurmurHash3_x86_32(LargeBinary.data(),LargeBinary.size(),SecondSeed);
        TEST_EQUAL("MurmurHash3_x86_32(const_void*,const_Integer,const_UInt32)-Binary-Large",
                   AapplebyLarge,MezzLarge);
    }// x86_32 - Binary

    {// x86_128 - Text
        Result_x86 AapplebyTiny;
        MurmurHash3_x86_128(TinyText.data(),TinyTextSize,FirstSeed,&AapplebyTiny.Hash[0]);
        Result_x86 MezzTiny = Hashing::MurmurHash3_x86_128(TinyText.data(),TinyText.size(),FirstSeed);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Text-Tiny-Index0",
                   AapplebyTiny.Hash[0],MezzTiny.Hash[0]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Text-Tiny-Index1",
                   AapplebyTiny.Hash[1],MezzTiny.Hash[1]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Text-Tiny-Index2",
                   AapplebyTiny.Hash[2],MezzTiny.Hash[2]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Text-Tiny-Index3",
                   AapplebyTiny.Hash[3],MezzTiny.Hash[3]);

        Result_x86 AapplebySmall;
        MurmurHash3_x86_128(SmallText.data(),SmallTextSize,FirstSeed,&AapplebySmall.Hash[0]);
        Result_x86 MezzSmall = Hashing::MurmurHash3_x86_128(SmallText.data(),SmallText.size(),FirstSeed);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Text-Small-Index0",
                   AapplebySmall.Hash[0],MezzSmall.Hash[0]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Text-Small-Index1",
                   AapplebySmall.Hash[1],MezzSmall.Hash[1]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Text-Small-Index2",
                   AapplebySmall.Hash[2],MezzSmall.Hash[2]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Text-Small-Index3",
                   AapplebySmall.Hash[3],MezzSmall.Hash[3]);

        Result_x86 AapplebyLarge;
        MurmurHash3_x86_128(LargeText.data(),LargeTextSize,FirstSeed,&AapplebyLarge.Hash[0]);
        Result_x86 MezzLarge = Hashing::MurmurHash3_x86_128(LargeText.data(),LargeText.size(),FirstSeed);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Text-Large-Index0",
                   AapplebyLarge.Hash[0],MezzLarge.Hash[0]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Text-Large-Index1",
                   AapplebyLarge.Hash[1],MezzLarge.Hash[1]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Text-Large-Index2",
                   AapplebyLarge.Hash[2],MezzLarge.Hash[2]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Text-Large-Index3",
                   AapplebyLarge.Hash[3],MezzLarge.Hash[3]);
    }// x86_128 - Text

    {// x86_128 - Binary
        Result_x86 AapplebyTiny;
        MurmurHash3_x86_128(TinyBinary.data(),TinyBinarySize,SecondSeed,&AapplebyTiny.Hash[0]);
        Result_x86 MezzTiny = Hashing::MurmurHash3_x86_128(TinyBinary.data(),TinyBinary.size(),SecondSeed);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Binary-Tiny-Index0",
                   AapplebyTiny.Hash[0],MezzTiny.Hash[0]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Binary-Tiny-Index1",
                   AapplebyTiny.Hash[1],MezzTiny.Hash[1]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Binary-Tiny-Index2",
                   AapplebyTiny.Hash[2],MezzTiny.Hash[2]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Binary-Tiny-Index3",
                   AapplebyTiny.Hash[3],MezzTiny.Hash[3]);

        Result_x86 AapplebySmall;
        MurmurHash3_x86_128(SmallBinary.data(),SmallBinarySize,SecondSeed,&AapplebySmall.Hash[0]);
        Result_x86 MezzSmall = Hashing::MurmurHash3_x86_128(SmallBinary.data(),SmallBinary.size(),SecondSeed);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Binary-Small-Index0",
                   AapplebySmall.Hash[0],MezzSmall.Hash[0]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Binary-Small-Index1",
                   AapplebySmall.Hash[1],MezzSmall.Hash[1]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Binary-Small-Index2",
                   AapplebySmall.Hash[2],MezzSmall.Hash[2]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Binary-Small-Index3",
                   AapplebySmall.Hash[3],MezzSmall.Hash[3]);

        Result_x86 AapplebyLarge;
        MurmurHash3_x86_128(LargeBinary.data(),LargeBinarySize,SecondSeed,&AapplebyLarge.Hash[0]);
        Result_x86 MezzLarge = Hashing::MurmurHash3_x86_128(LargeBinary.data(),LargeBinary.size(),SecondSeed);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Binary-Large-Index0",
                   AapplebyLarge.Hash[0],MezzLarge.Hash[0]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Binary-Large-Index1",
                   AapplebyLarge.Hash[1],MezzLarge.Hash[1]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Binary-Large-Index2",
                   AapplebyLarge.Hash[2],MezzLarge.Hash[2]);
        TEST_EQUAL("MurmurHash3_x86_128(const_void*,const_Integer,const_UInt32)-Binary-Large-Index3",
                   AapplebyLarge.Hash[3],MezzLarge.Hash[3]);
    }// x86_128 - Binary

    {// x64_128 - Text
        Result_x64 AapplebyTiny;
        MurmurHash3_x64_128(TinyText.data(),TinyTextSize,FirstSeed,&AapplebyTiny.Hash[0]);
        Result_x64 MezzTiny = Hashing::MurmurHash3_x64_128(TinyText.data(),TinyText.size(),FirstSeed);
        TEST_EQUAL("MurmurHash3_x64_128(const_void*,const_Integer,const_UInt32)-Text-Tiny-Index0",
                   AapplebyTiny.Hash[0],MezzTiny.Hash[0]);
        TEST_EQUAL("MurmurHash3_x64_128(const_void*,const_Integer,const_UInt32)-Text-Tiny-Index1",
                   AapplebyTiny.Hash[1],MezzTiny.Hash[1]);

        Result_x64 AapplebySmall;
        MurmurHash3_x64_128(SmallText.data(),SmallTextSize,FirstSeed,&AapplebySmall.Hash[0]);
        Result_x64 MezzSmall = Hashing::MurmurHash3_x64_128(SmallText.data(),SmallText.size(),FirstSeed);
        TEST_EQUAL("MurmurHash3_x64_128(const_void*,const_Integer,const_UInt32)-Text-Small-Index0",
                   AapplebySmall.Hash[0],MezzSmall.Hash[0]);
        TEST_EQUAL("MurmurHash3_x64_128(const_void*,const_Integer,const_UInt32)-Text-Small-Index1",
                   AapplebySmall.Hash[1],MezzSmall.Hash[1]);

        Result_x64 AapplebyLarge;
        MurmurHash3_x64_128(LargeText.data(),LargeTextSize,FirstSeed,&AapplebyLarge.Hash[0]);
        Result_x64 MezzLarge = Hashing::MurmurHash3_x64_128(LargeText.data(),LargeText.size(),FirstSeed);
        TEST_EQUAL("MurmurHash3_x64_128(const_void*,const_Integer,const_UInt32)-Text-Large-Index0",
                   AapplebyLarge.Hash[0],MezzLarge.Hash[0]);
        TEST_EQUAL("MurmurHash3_x64_128(const_void*,const_Integer,const_UInt32)-Text-Large-Index1",
                   AapplebyLarge.Hash[1],MezzLarge.Hash[1]);
    }// x64_128 - Text

    {// x64_128 - Binary
        Result_x64 AapplebyTiny;
        MurmurHash3_x64_128(TinyBinary.data(),TinyBinarySize,SecondSeed,&AapplebyTiny.Hash[0]);
        Result_x64 MezzTiny = Hashing::MurmurHash3_x64_128(TinyBinary.data(),TinyBinary.size(),SecondSeed);
        TEST_EQUAL("MurmurHash3_x64_128(const_void*,const_Integer,const_UInt32)-Binary-Tiny-Index0",
                   AapplebyTiny.Hash[0],MezzTiny.Hash[0]);
        TEST_EQUAL("MurmurHash3_x64_128(const_void*,const_Integer,const_UInt32)-Binary-Tiny-Index1",
                   AapplebyTiny.Hash[1],MezzTiny.Hash[1]);

        Result_x64 AapplebySmall;
        MurmurHash3_x64_128(SmallBinary.data(),SmallBinarySize,SecondSeed,&AapplebySmall.Hash[0]);
        Result_x64 MezzSmall = Hashing::MurmurHash3_x64_128(SmallBinary.data(),SmallBinary.size(),SecondSeed);
        TEST_EQUAL("MurmurHash3_x64_128(const_void*,const_Integer,const_UInt32)-Binary-Small-Index0",
                   AapplebySmall.Hash[0],MezzSmall.Hash[0]);
        TEST_EQUAL("MurmurHash3_x64_128(const_void*,const_Integer,const_UInt32)-Binary-Small-Index1",
                   AapplebySmall.Hash[1],MezzSmall.Hash[1]);

        Result_x64 AapplebyLarge;
        MurmurHash3_x64_128(LargeBinary.data(),LargeBinarySize,SecondSeed,&AapplebyLarge.Hash[0]);
        Result_x64 MezzLarge = Hashing::MurmurHash3_x64_128(LargeBinary.data(),LargeBinary.size(),SecondSeed);
        TEST_EQUAL("MurmurHash3_x64_128(const_void*,const_Integer,const_UInt32)-Binary-Large-Index0",
                   AapplebyLarge.Hash[0],MezzLarge.Hash[0]);
        TEST_EQUAL("MurmurHash3_x64_128(const_void*,const_Integer,const_UInt32)-Binary-Large-Index1",
                   AapplebyLarge.Hash[1],MezzLarge.Hash[1]);
    }// x64_128 - Binary
}

#endif // Mezz_Foundation_MurmurHashTests_h
