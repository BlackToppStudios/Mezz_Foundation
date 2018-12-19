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
#ifndef Mezz_Foundation_Base64_h
#define Mezz_Foundation_Base64_h

#ifndef SWIG
    #include "DataTypes.h"
    #include "BinaryBuffer.h"
#endif

namespace Mezzanine {
namespace Base64 {
    /// @brief Checks to see if a character a valid Base64 character.
    /// @param ToCheck The character to check if it is valid Base64.
    /// @return Returns true if the character could be part of Base64 text, false otherwise.
    Boole MEZZ_LIB IsBase64(const UInt8 ToCheck);

    /// @brief From an encoded string get the exact size of the decode binary in bytes.
    /// @remarks The binary is about 3/4 of the base64 size, but that can be off by just enough to
    /// make memory allocation an issue if not calculated carefully.
    /// @param EncodedString The base64 encoded string.
    /// @return Returns the exact length of the result once it is decoded.
    SizeType MEZZ_LIB PredictBinarySize(const String& EncodedString);
    /// @brief From an encoded string get the exact size of the decode binary in bytes.
    /// @remarks The binary is about 3/4 of the base64 size, but that can be off by just enough to
    /// make memory allocation an issue if not calculated carefully.
    /// @param EncodedString The base64 encoded string.
    /// @param EncodedSize The size of the base64 encoded string.
    /// @return Returns the exact length of the result once it is decoded.
    SizeType MEZZ_LIB PredictBinarySize(const Char8* EncodedString, const SizeType EncodedSize);
    /// @brief From the size of a binary get the exact Base64 size in bytes.
    /// @remarks The Base64 is about 133% of the binary size, but that can be off by just enough
    /// to make memory allocation an issue if not calculated carefully.
    /// @param Length The byte length of the item before Base64 encoding.
    /// @return Returns the exact length of the result once it is encoded.
    SizeType MEZZ_LIB PredictBase64Size(const SizeType Length);

    /// @brief Converts the contents of a String to Base64.
    /// @param Unencoded A BinaryBuffer to be encoded to Base64.
    /// @return Returns a String containing the Base64 encoded result of the unencoded string.
    String MEZZ_LIB Encode(const BinaryBuffer& Unencoded);
    /// @brief Converts the contents of a raw buffer to Base64.
    /// @param SrcBytes A pointer to the src(source) buffer to convert.
    /// @param SrcLength The length of the src(source) buffer in bytes.
    /// @param DestBytes A pointer to the dest(destination) buffer to place the result of the conversion into.
    /// @param DestLength The length of the dest(destination) buffer in bytes.
    /// @return Returns the number of actual bytes written to the destination buffer.
    SizeType MEZZ_LIB EncodeRawBuffer(const UInt8* SrcBytes, const SizeType SrcLength,
                                      Char8* DestBytes, const SizeType DestLength);

    /// @brief Converts the contents of a BinaryBuffer that is Base64 encoded to "normal" encoding.
    /// @param Encoded A String to be decoded.
    /// @return Returns a BinaryBuffer containing the decoded result of the encoded BinaryBuffer.
    BinaryBuffer MEZZ_LIB Decode(const String& Encoded);
    /// @brief Converts the contents of a raw buffer that is Base64 encoded to "normal" encoding.
    /// @param SrcBytes A pointer to the src(source) buffer to convert.
    /// @param SrcLength The length of the src(source) buffer in bytes.
    /// @param DestBytes A pointer to the dest(destination) buffer to place the result of the conversion into.
    /// @param DestLength The length of the dest(destination) buffer in bytes.
    /// @return Returns the number of actual bytes written to the destination buffer.
    SizeType MEZZ_LIB DecodeRawBuffer(const Char8* SrcBytes, const SizeType SrcLength,
                                      UInt8* DestBytes, const SizeType DestLength);
}//Base64
}//Mezzanine

#endif
