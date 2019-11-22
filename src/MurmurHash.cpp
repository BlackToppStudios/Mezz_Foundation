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
//-----------------------------------------------------------------------------
// MurmurHash3 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

// Note - The x86 and x64 versions do _not_ produce the same results, as the
// algorithms are optimized for their respective platforms. You can still
// compile and run any of them on any platform, but your performance with the
// non-native version will be less than optimal.

#include "MurmurHash.h"

namespace {
    using namespace Mezzanine;
    //-----------------------------------------------------------------------------
    // Platform-specific functions and macros

    #if defined(_MSC_VER) // Microsoft Visual Studio
        #define FORCE_INLINE __forceinline

        #include <stdlib.h>

        #define ROTL32(x,y) _rotl(x,y)
        #define ROTL64(x,y) _rotl64(x,y)
    #else   // defined(_MSC_VER) aka Good Compilers
        #define FORCE_INLINE inline __attribute__((always_inline))

        inline UInt32 rotl32(UInt32 x, Int8 r)
        {
            return ( x << r ) | ( x >> ( 32 - r ) );
        }

        inline UInt64 rotl64(UInt64 x, Int8 r)
        {
            return ( x << r ) | ( x >> ( 64 - r ) );
        }

        #define ROTL32(x,y) rotl32(x,y)
        #define ROTL64(x,y) rotl64(x,y)
    #endif // !defined(_MSC_VER)

    //-----------------------------------------------------------------------------
    // Block read - if your platform needs to do endian-swapping or can only
    // handle aligned reads, do the conversion here

    FORCE_INLINE UInt32 GetBlock32(const UInt32* Blocks, const SizeType Index)
    {
        return Blocks[Index];
    }

    FORCE_INLINE UInt64 GetBlock64(const UInt64* Blocks, const SizeType Index)
    {
        return Blocks[Index];
    }

    //-----------------------------------------------------------------------------
    // Finalization mix - force all bits of a hash block to avalanche

    FORCE_INLINE UInt32 FinalMix32(UInt32 ToMix)
    {
        ToMix ^= ToMix >> 16;
        ToMix *= 0x85ebca6b;
        ToMix ^= ToMix >> 13;
        ToMix *= 0xc2b2ae35;
        ToMix ^= ToMix >> 16;

        return ToMix;
    }

    //----------

    FORCE_INLINE UInt64 FinalMix64(UInt64 ToMix)
    {
        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wc++98-compat-pedantic")
        ToMix ^= ToMix >> 33;
        ToMix *= 0xff51afd7ed558ccdllu;
        ToMix ^= ToMix >> 33;
        ToMix *= 0xc4ceb9fe1a85ec53llu;
        ToMix ^= ToMix >> 33;
        RESTORE_WARNING_STATE

        return ToMix;
    }
}

namespace Mezzanine {
namespace Hashing {
    UInt32 MurmurHash3_x86_32(const void* Key, const SizeType Length, const UInt32 Seed)
    {
        const SizeType NumBlocks = Length / 4;
        UInt32 OutputHash = Seed;

        static const UInt32 x86_32_Constant_One = 0xcc9e2d51;
        static const UInt32 x86_32_Constant_Two = 0x1b873593;

        //----------
        // Body

        const UInt32* Blocks = static_cast<const UInt32*>(Key);
        for( SizeType BlockIdx = 0 ; BlockIdx < NumBlocks ; BlockIdx++ )
        {
            UInt32 KeyBlock = GetBlock32(Blocks,BlockIdx);

            KeyBlock *= x86_32_Constant_One;
            KeyBlock = ROTL32(KeyBlock,15);
            KeyBlock *= x86_32_Constant_Two;

            OutputHash ^= KeyBlock;
            OutputHash = ROTL32(OutputHash,13);
            OutputHash = ( OutputHash * 5 ) + 0xe6546b64;
        }

        //----------
        // Tail

        const UInt8* Tail = ( static_cast<const UInt8*>(Key) + ( NumBlocks * 4 ) );
        UInt32 TailBlock = 0;
        switch( Length & 3 )
        {
            case 3: TailBlock ^= static_cast<UInt32>( Tail[2] << 16 );  [[fallthrough]];
            case 2: TailBlock ^= static_cast<UInt32>( Tail[1] << 8 );   [[fallthrough]];
            case 1: TailBlock ^= Tail[0];
                    TailBlock *= x86_32_Constant_One;
                    TailBlock = ROTL32(TailBlock,15);
                    TailBlock *= x86_32_Constant_Two;
                    OutputHash ^= TailBlock;
        };

        //----------
        // Finalization

        OutputHash ^= static_cast<UInt32>(Length);
        OutputHash = FinalMix32(OutputHash);
        return OutputHash;
    }

    MurmurHashResult_x86_128 MurmurHash3_x86_128(const void* Key, const SizeType Length, const UInt32 Seed)
    {
        const SizeType NumBlocks = Length / 16;

        static const UInt32 x86_128_Constant_One = 0x239b961b;
        static const UInt32 x86_128_Constant_Two = 0xab0e9789;
        static const UInt32 x86_128_Constant_Three = 0x38b34ae5;
        static const UInt32 x86_128_Constant_Four = 0xa1e38b93;

        UInt32 HashPart1 = Seed;
        UInt32 HashPart2 = Seed;
        UInt32 HashPart3 = Seed;
        UInt32 HashPart4 = Seed;

        //----------
        // Body

        const UInt32* Blocks = static_cast<const UInt32*>(Key);

        for( SizeType BlockIdx = 0 ; BlockIdx != NumBlocks ; BlockIdx++ )
        {
            UInt32 KeyBlockPart1 = GetBlock32(Blocks,(BlockIdx * 4) + 0);
            UInt32 KeyBlockPart2 = GetBlock32(Blocks,(BlockIdx * 4) + 1);
            UInt32 KeyBlockPart3 = GetBlock32(Blocks,(BlockIdx * 4) + 2);
            UInt32 KeyBlockPart4 = GetBlock32(Blocks,(BlockIdx * 4) + 3);

            KeyBlockPart1 *= x86_128_Constant_One;
            KeyBlockPart1  = ROTL32(KeyBlockPart1,15);
            KeyBlockPart1 *= x86_128_Constant_Two;
            HashPart1 ^= KeyBlockPart1;

            HashPart1 = ROTL32(HashPart1,19);
            HashPart1 += HashPart2;
            HashPart1 = ( HashPart1 * 5 ) + 0x561ccd1b;

            KeyBlockPart2 *= x86_128_Constant_Two;
            KeyBlockPart2  = ROTL32(KeyBlockPart2,16);
            KeyBlockPart2 *= x86_128_Constant_Three;
            HashPart2 ^= KeyBlockPart2;

            HashPart2 = ROTL32(HashPart2,17);
            HashPart2 += HashPart3;
            HashPart2 = ( HashPart2 * 5 ) + 0x0bcaa747;

            KeyBlockPart3 *= x86_128_Constant_Three;
            KeyBlockPart3  = ROTL32(KeyBlockPart3,17);
            KeyBlockPart3 *= x86_128_Constant_Four;
            HashPart3 ^= KeyBlockPart3;

            HashPart3 = ROTL32(HashPart3,15);
            HashPart3 += HashPart4;
            HashPart3 = ( HashPart3 * 5 ) + 0x96cd1c35;

            KeyBlockPart4 *= x86_128_Constant_Four;
            KeyBlockPart4  = ROTL32(KeyBlockPart4,18);
            KeyBlockPart4 *= x86_128_Constant_One;
            HashPart4 ^= KeyBlockPart4;

            HashPart4 = ROTL32(HashPart4,13);
            HashPart4 += HashPart1;
            HashPart4 = ( HashPart4 * 5 ) + 0x32ac3b17;
        }

        //----------
        // Tail

        const UInt8* Tail = ( static_cast<const UInt8*>(Key) + ( NumBlocks * 16 ) );

        UInt32 TailBlockPart1 = 0;
        UInt32 TailBlockPart2 = 0;
        UInt32 TailBlockPart3 = 0;
        UInt32 TailBlockPart4 = 0;

        switch( Length & 15 )
        {
            case 15: TailBlockPart4 ^= static_cast<UInt32>( Tail[14] << 16 );  [[fallthrough]];
            case 14: TailBlockPart4 ^= static_cast<UInt32>( Tail[13] << 8 );   [[fallthrough]];
            case 13: TailBlockPart4 ^= static_cast<UInt32>( Tail[12] << 0 );
                     TailBlockPart4 *= x86_128_Constant_Four;
                     TailBlockPart4  = ROTL32(TailBlockPart4,18);
                     TailBlockPart4 *= x86_128_Constant_One;
                     HashPart4 ^= TailBlockPart4;                              [[fallthrough]];

            case 12: TailBlockPart3 ^= static_cast<UInt32>( Tail[11] << 24 );  [[fallthrough]];
            case 11: TailBlockPart3 ^= static_cast<UInt32>( Tail[10] << 16 );  [[fallthrough]];
            case 10: TailBlockPart3 ^= static_cast<UInt32>( Tail[ 9] << 8 );   [[fallthrough]];
            case  9: TailBlockPart3 ^= static_cast<UInt32>( Tail[ 8] << 0 );
                     TailBlockPart3 *= x86_128_Constant_Three;
                     TailBlockPart3  = ROTL32(TailBlockPart3,17);
                     TailBlockPart3 *= x86_128_Constant_Four;
                     HashPart3 ^= TailBlockPart3;                              [[fallthrough]];

            case  8: TailBlockPart2 ^= static_cast<UInt32>( Tail[ 7] << 24 );  [[fallthrough]];
            case  7: TailBlockPart2 ^= static_cast<UInt32>( Tail[ 6] << 16 );  [[fallthrough]];
            case  6: TailBlockPart2 ^= static_cast<UInt32>( Tail[ 5] << 8 );   [[fallthrough]];
            case  5: TailBlockPart2 ^= static_cast<UInt32>( Tail[ 4] << 0 );
                     TailBlockPart2 *= x86_128_Constant_Two;
                     TailBlockPart2  = ROTL32(TailBlockPart2,16);
                     TailBlockPart2 *= x86_128_Constant_Three;
                     HashPart2 ^= TailBlockPart2;                              [[fallthrough]];

            case  4: TailBlockPart1 ^= static_cast<UInt32>( Tail[ 3] << 24 );  [[fallthrough]];
            case  3: TailBlockPart1 ^= static_cast<UInt32>( Tail[ 2] << 16 );  [[fallthrough]];
            case  2: TailBlockPart1 ^= static_cast<UInt32>( Tail[ 1] << 8 );   [[fallthrough]];
            case  1: TailBlockPart1 ^= static_cast<UInt32>( Tail[ 0] << 0 );
                     TailBlockPart1 *= x86_128_Constant_One;
                     TailBlockPart1  = ROTL32(TailBlockPart1,15);
                     TailBlockPart1 *= x86_128_Constant_Two;
                     HashPart1 ^= TailBlockPart1;
        };

        //----------
        // Finalization

        HashPart1 ^= static_cast<UInt32>(Length);
        HashPart2 ^= static_cast<UInt32>(Length);
        HashPart3 ^= static_cast<UInt32>(Length);
        HashPart4 ^= static_cast<UInt32>(Length);

        HashPart1 += HashPart2;
        HashPart1 += HashPart3;
        HashPart1 += HashPart4;
        HashPart2 += HashPart1;
        HashPart3 += HashPart1;
        HashPart4 += HashPart1;

        HashPart1 = FinalMix32(HashPart1);
        HashPart2 = FinalMix32(HashPart2);
        HashPart3 = FinalMix32(HashPart3);
        HashPart4 = FinalMix32(HashPart4);

        HashPart1 += HashPart2;
        HashPart1 += HashPart3;
        HashPart1 += HashPart4;
        HashPart2 += HashPart1;
        HashPart3 += HashPart1;
        HashPart4 += HashPart1;

        MurmurHashResult_x86_128 Result;
        Result.Hash[0] = HashPart1;
        Result.Hash[1] = HashPart2;
        Result.Hash[2] = HashPart3;
        Result.Hash[3] = HashPart4;
        return Result;
    }

    MurmurHashResult_x64_128 MurmurHash3_x64_128(const void* Key, const SizeType Length, const UInt32 Seed)
    {
        const SizeType NumBlocks = Length / 16;

        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wc++98-compat-pedantic")
        static const UInt64 x64_128_Constant_One = 0x87c37b91114253d5llu;
        static const UInt64 x64_128_Constant_Two = 0x4cf5ad432745937fllu;
        RESTORE_WARNING_STATE

        UInt64 HashPart1 = Seed;
        UInt64 HashPart2 = Seed;

        //----------
        // Body

        const UInt64* Blocks = static_cast<const UInt64*>(Key);

        for( SizeType BlockIdx = 0 ; BlockIdx < NumBlocks ; BlockIdx++ )
        {
            UInt64 KeyBlockPart1 = GetBlock64(Blocks,(BlockIdx * 2) + 0);
            UInt64 KeyBlockPart2 = GetBlock64(Blocks,(BlockIdx * 2) + 1);

            KeyBlockPart1 *= x64_128_Constant_One;
            KeyBlockPart1  = ROTL64(KeyBlockPart1,31);
            KeyBlockPart1 *= x64_128_Constant_Two;
            HashPart1 ^= KeyBlockPart1;

            HashPart1 = ROTL64(HashPart1,27);
            HashPart1 += HashPart2;
            HashPart1 = ( HashPart1 * 5 ) + 0x52dce729;

            KeyBlockPart2 *= x64_128_Constant_Two;
            KeyBlockPart2  = ROTL64(KeyBlockPart2,33);
            KeyBlockPart2 *= x64_128_Constant_One;
            HashPart2 ^= KeyBlockPart2;

            HashPart2 = ROTL64(HashPart2,31);
            HashPart2 += HashPart1;
            HashPart2 = ( HashPart2 * 5 ) + 0x38495ab5;
        }

        //----------
        // Tail

        const UInt8* Tail = ( static_cast<const UInt8*>(Key) + ( NumBlocks * 16 ) );

        UInt64 TailBlockPart1 = 0;
        UInt64 TailBlockPart2 = 0;

        switch( Length & 15 )
        {
            case 15: TailBlockPart2 ^= static_cast<UInt64>( Tail[14] ) << 48;  [[fallthrough]];
            case 14: TailBlockPart2 ^= static_cast<UInt64>( Tail[13] ) << 40;  [[fallthrough]];
            case 13: TailBlockPart2 ^= static_cast<UInt64>( Tail[12] ) << 32;  [[fallthrough]];
            case 12: TailBlockPart2 ^= static_cast<UInt64>( Tail[11] ) << 24;  [[fallthrough]];
            case 11: TailBlockPart2 ^= static_cast<UInt64>( Tail[10] ) << 16;  [[fallthrough]];
            case 10: TailBlockPart2 ^= static_cast<UInt64>( Tail[ 9] ) << 8;   [[fallthrough]];
            case  9: TailBlockPart2 ^= static_cast<UInt64>( Tail[ 8] ) << 0;
                     TailBlockPart2 *= x64_128_Constant_Two;
                     TailBlockPart2  = ROTL64(TailBlockPart2,33);
                     TailBlockPart2 *= x64_128_Constant_One;
                     HashPart2 ^= TailBlockPart2;                              [[fallthrough]];

            case  8: TailBlockPart1 ^= static_cast<UInt64>( Tail[ 7] ) << 56;  [[fallthrough]];
            case  7: TailBlockPart1 ^= static_cast<UInt64>( Tail[ 6] ) << 48;  [[fallthrough]];
            case  6: TailBlockPart1 ^= static_cast<UInt64>( Tail[ 5] ) << 40;  [[fallthrough]];
            case  5: TailBlockPart1 ^= static_cast<UInt64>( Tail[ 4] ) << 32;  [[fallthrough]];
            case  4: TailBlockPart1 ^= static_cast<UInt64>( Tail[ 3] ) << 24;  [[fallthrough]];
            case  3: TailBlockPart1 ^= static_cast<UInt64>( Tail[ 2] ) << 16;  [[fallthrough]];
            case  2: TailBlockPart1 ^= static_cast<UInt64>( Tail[ 1] ) << 8;   [[fallthrough]];
            case  1: TailBlockPart1 ^= static_cast<UInt64>( Tail[ 0] ) << 0;
                     TailBlockPart1 *= x64_128_Constant_One;
                     TailBlockPart1  = ROTL64(TailBlockPart1,31);
                     TailBlockPart1 *= x64_128_Constant_Two;
                     HashPart1 ^= TailBlockPart1;
        };

        //----------
        // Finalization

        HashPart1 ^= static_cast<UInt64>(Length);
        HashPart2 ^= static_cast<UInt64>(Length);

        HashPart1 += HashPart2;
        HashPart2 += HashPart1;

        HashPart1 = FinalMix64(HashPart1);
        HashPart2 = FinalMix64(HashPart2);

        HashPart1 += HashPart2;
        HashPart2 += HashPart1;

        MurmurHashResult_x64_128 Result;
        Result.Hash[0] = HashPart1;
        Result.Hash[1] = HashPart2;
        return Result;
    }
}//Hashing
}//Mezzanine
