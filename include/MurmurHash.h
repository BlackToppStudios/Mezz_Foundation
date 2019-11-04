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
#ifndef Mezz_Foundation_MurmurHash_h
#define Mezz_Foundation_MurmurHash_h

#include "DataTypes.h"

namespace Mezzanine {
namespace Hashing {
    /// @brief A convenience type for the return value of a x86 128-bit Hashing function.
    struct MurmurHash128_x86
    {
        UInt32 Hash[4];
    };//MurmurHash128_x86

    /// @brief A convenience type for the return value of a x64 128-bit Hashing function.
    struct MurmurHash128_x64
    {
        UInt64 Hash[2];
    };//MurmurHash128_x64

    /// @brief A Murmur hash function for 32-bit platforms that produces a 32-bit hash.
    /// @param Key A pointer to the data that will be hashed.
    /// @param Length The byte length of the Key.
    /// @param Seed
    /// @return Returns a 32-bit Hash of the Key.
    UInt32 MurmurHash3_x86_32(const void* Key, const Integer Length, const UInt32 Seed);
    /// @brief A Murmur hash function for 32-bit platforms that produces a 128-bit hash.
    /// @param Key A pointer to the data that will be hashed.
    /// @param Length The byte length of the Key.
    /// @param Seed
    /// @return Returns a 128-bit Hash of the Key.
    MurmurHash128_x86 MurmurHash3_x86_128(const void* Key, const Integer Length, const UInt32 Seed);
    /// @brief A Murmur hash function for 64-bit platforms that produces a 128-bit hash.
    /// @param Key A pointer to the data that will be hashed.
    /// @param Length The byte length of the Key.
    /// @param Seed
    /// @return Returns a 128-bit Hash of the Key.
    MurmurHash128_x64 MurmurHash3_x64_128(const void* Key, const Integer Length, const UInt32 Seed);
}//Hashing
}//Mezzanine

#endif // Mezz_Foundation_MurmurHash_h
