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

#include "BinaryBuffer.h"
#include "Base64.h"

#include <cstring>

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    // Construction and Destruction

    BinaryBuffer::BinaryBuffer(const BinaryBuffer& Other)
    {
        this->Size = Other.Size;
        if( Other.Size && Other.Binary ) {
            this->Binary = new Byte[this->Size * sizeof(Byte)];
            memcpy(this->Binary,Other.Binary,this->Size * sizeof(Byte));
        }else{
            this->Binary = 0;
        }
    }

    BinaryBuffer::BinaryBuffer(BinaryBuffer&& Other) :
        Binary(Other.Binary),
        Size(Other.Size)
    {
        Other.Binary = nullptr;
        Other.Size = 0;
    }

    BinaryBuffer::BinaryBuffer(const SizeType PredeterminedSize) :
        Binary(new Byte[PredeterminedSize]),
        Size(PredeterminedSize)
        {  }

    BinaryBuffer::BinaryBuffer(Byte* BinaryPointer, const SizeType PredeterminedSize) :
        Binary(BinaryPointer != nullptr ? BinaryPointer : new Byte[PredeterminedSize]),
        Size(PredeterminedSize)
        {  }

    BinaryBuffer::BinaryBuffer(const String& DataString)
    {
        this->Size = DataString.size();
        this->CreateBuffer();
        memcpy(this->Binary, DataString.c_str(), this->Size * sizeof(Byte));
    }

    BinaryBuffer::~BinaryBuffer()
        { this->DeleteBuffer(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Operators

    BinaryBuffer& BinaryBuffer::operator=(const BinaryBuffer& Other)
    {
        if( Other.Binary == this->Binary ) {
            throw std::invalid_argument("Attempted a self assignment of a BinaryBuffer.");
        }
        this->DeleteBuffer(Other.Size);
        if( Other.Size && Other.Binary ) {
            this->CreateBuffer();
            memcpy(this->Binary,Other.Binary,this->Size * sizeof(Byte));
        }else{
            this->Binary = nullptr;
        }
        return *this;
    }

    BinaryBuffer& BinaryBuffer::operator=(BinaryBuffer&& Other)
    {
        if( Other.Binary == this->Binary ) {
            throw std::invalid_argument("Attempted a self assignment of a BinaryBuffer.");
        }
        this->DeleteBuffer(Other.Size);
        this->Binary = std::move(Other.Binary);
        Other.Binary = nullptr;
        Other.Size = 0;
        return *this;
    }

    BinaryBuffer& BinaryBuffer::operator+=(const BinaryBuffer& Other)
    {
        this->Concatenate(Other);
        return *this;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Element Access

    BinaryBuffer::Byte& BinaryBuffer::operator[](const SizeType Index) noexcept
        { return *(this->Binary + Index); }

    const BinaryBuffer::Byte& BinaryBuffer::operator[](const SizeType Index) const noexcept
        { return *(this->Binary + Index); }

    BinaryBuffer::Byte& BinaryBuffer::at(const SizeType Index)
    {
        if( Index >= this->Size ) {
            throw std::out_of_range("Attempted access beyond range of BinaryBuffer.");
        }
        return *(this->Binary + Index);
    }

    const BinaryBuffer::Byte& BinaryBuffer::at(const SizeType Index) const
    {
        if( Index >= this->Size ) {
            throw std::out_of_range("Attempted access beyond range of BinaryBuffer.");
        }
        return *(this->Binary + Index);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Utility

    void BinaryBuffer::CreateBuffer()
    {
        this->DeleteBuffer(this->Size);
        if( this->Size != 0 ) {
            this->Binary = new Byte[this->Size * sizeof(Byte)];
        }else{
            throw std::length_error("Cannot create zero size buffer.");
        }
    }

    void BinaryBuffer::DeleteBuffer(const SizeType NewSize)
    {
        if( this->Binary ) {
            delete[] this->Binary;
        }
        this->Binary = nullptr;
        this->Size = NewSize;
    }

    void BinaryBuffer::Concatenate(const Byte* Other, const SizeType ByteSize)
    {
        SizeType NewSize = (this->Size + ByteSize) * sizeof(Byte);
        Byte* TargetBuffer = new Byte[NewSize];

        memcpy(TargetBuffer, this->Binary, this->Size);
        memcpy(TargetBuffer + this->Size, Other, ByteSize);

        this->DeleteBuffer(NewSize);
        this->Binary = TargetBuffer;
    }

    void BinaryBuffer::Concatenate(const BinaryBuffer& Other)
        { this->Concatenate(Other.Binary, Other.Size); }

    String BinaryBuffer::ToString() const
        { return String( reinterpret_cast<char*>(this->Binary), this->Size * sizeof(Byte) ); }

    SizeType BinaryBuffer::GetSize() const noexcept
        { return this->Size; }
}//Mezzanine
