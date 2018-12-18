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

/// @file
/// @brief The implementation of some tools for making working with binary stuff easier.

#include "Base64.h"
#include "exception.h"

/*
 * Some of the following functions and 1 variable were taken from http://www.adp-gmbh.ch/cpp/common/base64.html
 * for the functions IsBase64(unsigned char c),  Base64Encode(UInt8 const* BytesToEncode, unsigned int Length),
 *     Base64Decode(String const& EncodedString) and Base64Chars and maybe a few others in this file or the
 *     tests for this file
 * with written permission as follows.


   Copyright (C) 2004-2008 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

namespace
{
    using namespace Mezzanine;
    // Code change to Match BTS naming conventions and formatting
    /// @brief A convenience constant containing all of the valid Base64 characters.
    static const String Base64Chars =
                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                 "abcdefghijklmnopqrstuvwxyz"
                 "0123456789+/";

    /// @brief A variant of the one parameter IsBase64 method used by the Base64::DecodeRawBuffer.
    /// @param First The first character to check.
    /// @param Second The second character to check.
    /// @param Third The third character to check.
    /// @param Fourth The fourth character to check.
    /// @return Returns true if any of the provided characters are not Base64, false if they are all valid Base64.
    Boole IsNotBase64(const UInt8 First, const UInt8 Second, const UInt8 Third, const UInt8 Fourth)
    {
        return !Base64::IsBase64(First) || !Base64::IsBase64(Second) ||
               !Base64::IsBase64(Third) || !Base64::IsBase64(Fourth);
    }

    /// @brief Convenience method for getting the index of a Base64 character in the main string.
    /// @param ToCheck The Base64 character to check for.
    /// @return Returns the Index in "Base64Chars" the character is at.
    UInt8 GetBase64IndexNoCheck(const UInt8 ToCheck)
    {
        return static_cast<UInt8>( Base64Chars.find(ToCheck) );
    }
}

namespace Mezzanine {
namespace Base64 {
    // Code change to Match BTS naming conventions and formatting
    Boole IsBase64(const UInt8 ToCheck)
        { return (isalnum(ToCheck) || (ToCheck == '+') || (ToCheck == '/') || (ToCheck == '=')); }

    SizeType PredictBinarySize(const String& EncodedString)
        { return PredictBinarySize(EncodedString.c_str(),EncodedString.size()); }

    SizeType PredictBinarySize(const Char8* EncodedString, const SizeType EncodedSize)
    {
        if( EncodedSize < 4 ) {
            throw std::invalid_argument("Provided String is too short to be Base64.");
            //MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION, "Provided String is too short to be Base64.")
        }

        return EncodedSize / 4 * 3 -
               ( EncodedString[ EncodedSize - 2 ] == '=' ? 1 : 0 ) -
               ( EncodedString[ EncodedSize - 1 ] == '=' ? 1 : 0 );
    }

    SizeType PredictBase64Size(const SizeType Length)
        { return ( Length + 2 ) / 3 * 4; }

    String Encode(const BinaryBuffer& Unencoded)
    {
        String Ret;
        Ret.resize(PredictBase64Size(Unencoded.Size),'\0');
        EncodeRawBuffer( Unencoded.Binary, Unencoded.Size, &Ret[0], Ret.length() );
        return Ret;
    }

    SizeType EncodeRawBuffer(const UInt8* SrcBytes, const SizeType SrcLength,
                             Char8* DestBytes, const SizeType DestLength)
    {
        if( PredictBase64Size(SrcLength) < DestLength ) {
            throw std::invalid_argument("Destination Buffer too small to hold Base64 encoding.");
          //MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Destination Buffer too small to hold Base64 encode.");
        }

        SizeType BytesWritten = 0;
        SizeType BytesRemaining = SrcLength;

        // Code change to Match BTS naming conventions and formatting
        int i = 0;
        int j = 0;
        UInt8 char_array_3[3];
        UInt8 char_array_4[4];

        while( BytesRemaining-- )
        {
            char_array_3[i++] = *(SrcBytes++);
            if( i == 3 ) {
                char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                char_array_4[1] = static_cast<UInt8>( ( (char_array_3[0] & 0x03) << 4)
                                                    + ( (char_array_3[1] & 0xf0) >> 4) );
                char_array_4[2] = static_cast<UInt8>( ( (char_array_3[1] & 0x0f) << 2)
                                                    + ( (char_array_3[2] & 0xc0) >> 6) );
                char_array_4[3] = char_array_3[2] & 0x3f;

                for( i = 0 ; i < 4 ; i++ )
                {
                    *(DestBytes + BytesWritten) = Base64Chars[char_array_4[i]];
                    BytesWritten++;
                }
                i = 0;
            }
        }

        if( i ) {
            for( j = i ; j < 3 ; j++ )
                { char_array_3[j] = '\0'; }

            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = static_cast<UInt8>( ( (char_array_3[0] & 0x03) << 4)
                                                + ( (char_array_3[1] & 0xf0) >> 4) );
            char_array_4[2] = static_cast<UInt8>( ( (char_array_3[1] & 0x0f) << 2)
                                                + ( (char_array_3[2] & 0xc0) >> 6) );
            char_array_4[3] = char_array_3[2] & 0x3f;

            SAVE_WARNING_STATE
            SUPPRESS_GCC_WARNING("-Wstrict-overflow")

            for( j = 0 ; j < i + 1 ; j++ )
            {
                *(DestBytes + BytesWritten) = Base64Chars[char_array_4[j]];
                BytesWritten++;
            }

            RESTORE_WARNING_STATE

            while( i++ < 3 )
            {
                *(DestBytes + BytesWritten) = '=';
                BytesWritten++;
            }
        }

        return BytesWritten;
    }

    BinaryBuffer Decode(const String& Encoded)
    {
        BinaryBuffer Ret( PredictBinarySize(Encoded) );
        Ret.CreateBuffer();
        DecodeRawBuffer( Encoded.c_str(),Encoded.length(), Ret.Binary, Ret.Size );
        return Ret;
    }

    SizeType DecodeRawBuffer(const Char8* SrcBytes, const SizeType SrcLength,
                             UInt8* DestBytes, const SizeType DestLength)
    {
        if( PredictBinarySize(SrcBytes,SrcLength) < DestLength ) {
            throw std::invalid_argument("Destination Buffer too small to hold decoded Base64.");
            //MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Destination Buffer too small to hold decoded Base64.");
        }

        if( ( SrcLength % 4 ) != 0 ) {
            throw std::invalid_argument("Unexpected number of characters in Base64 buffer.");
        }

        SizeType BytesWritten = 0;
        const Char8* Progress = SrcBytes;

        // Code change to Match BTS naming conventions and formatting
        UInt8 First;
        UInt8 Second;
        UInt8 Third;
        UInt8 Fourth;

        while( Progress < SrcBytes + SrcLength )
        {
            if( IsNotBase64( *(Progress + 0),*(Progress + 1),*(Progress + 2),*(Progress + 3) ) ) {
                throw std::range_error("Invalid character found during Base64 decode.");
                //MEZZ_EXCEPTION(ExceptionBase::PARAMETERS_EXCEPTION,"Invalid character found during Base64 decode.");
            }

            First = GetBase64IndexNoCheck( *(Progress + 0) );
            Second = GetBase64IndexNoCheck( *(Progress + 1) );
            Third = ( *(Progress + 2) == '=' ? 0 : GetBase64IndexNoCheck( *(Progress + 2) ) );

            *(DestBytes + BytesWritten + 0) = static_cast<UInt8>( (First << 2) + ( (Second & 0x30) >> 4 ) );
            *(DestBytes + BytesWritten + 1) = static_cast<UInt8>( ( (Second & 0xf) << 4 ) + ( (Third & 0x3c) >> 2 ) );

            if( *(Progress + 3) != '=' ) {
                Fourth = GetBase64IndexNoCheck( *(Progress + 3) );
                *(DestBytes + BytesWritten + 2) = static_cast<UInt8>( ( (Third & 0x3) << 6 ) + Fourth );
            }

            BytesWritten += 3;
            Progress += 4;
        }

        return BytesWritten;
    }
}//Base64
}//Mezzanine
