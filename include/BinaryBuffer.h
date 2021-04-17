// © Copyright 2010 - 2021 BlackTopp Studios Inc.
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
#ifndef Mezz_Foundation_BinaryBuffer_h
#define Mezz_Foundation_BinaryBuffer_h

#ifndef SWIG
    #include "DataTypes.h"
#endif

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief A way to store and pass raw binary buffers. For example: compiled bytecode.
    /// @details Originally intended for use with ScriptCompilable as a basic way to store and pass bytecode around.
    /// This deletes the stored buffer on destruction. To prevent this behavior set the Binary pointer to null prior
    /// to destruction.
    /// @n @n
    /// This is designed to be fairly minimalistic but passing by value causes the buffer to be copied.
    /// @n @n
    /// Where possible this class performs no speculative allocation unless explicitly requested to. In other words
    /// this should have allocated exactly as many bytes are indicated by the member Size, no more and no less.
    /// This will tend to not allocate memory unless an operation on it is specified that it does so.
    /// @n @n
    /// Whenever this needs to allocate memory it will use the Size member for determining the amount to allocate.
    /// If that is 0 an std::invalid_argument exception is thrown.
    ///////////////////////////////////////
    class MEZZ_LIB BinaryBuffer
    {
    public:
        ///////////////////////////////////////////////////////////////////////////////
        // Public Types

        /// @brief The type meant to indicate one byte in the stored array.
        typedef UInt8 Byte;
    #ifndef SWIG
        ///////////////////////////////////////////////////////////////////////////////
        // Public Data Members

        /// @brief A pointer to the actual binary data.
        Byte* Binary = nullptr;
        /// @brief The size of the buffer pointed to by @ref Binary.
        SizeType Size = 0;
    #endif
        ///////////////////////////////////////////////////////////////////////////////
        // Construction and Destruction

        /// @brief Default Constructor
        BinaryBuffer() = default;
        /// @brief Copy Constructor.
        /// @details Allocates identical amount of memory as other buffer then copies the other buffer into
        /// the allocated space. Each BinaryBuffer retains ownership of their respective buffers.
        /// @param Other The other buffer to be copied.
        BinaryBuffer(const BinaryBuffer& Other);
        /// @brief Move Constructor.
        /// @details This will perform a simple move of the Binary and Size from the Other BinaryBuffer and
        /// set it's members to their defaults (Binary:nullptr, Size:0).
        /// @param Other The other buffer to be moved.
        BinaryBuffer(BinaryBuffer&& Other);
        /// @brief Allocating Constructor,
        /// @details This sets a custom size and allocates space (uninitialized/filled with gibberish).
        /// @param PredeterminedSize The size to set on creation.
        explicit BinaryBuffer(const SizeType PredeterminedSize);
        /// @brief Verbose Constructor.
        /// @details If passed a pointer this assumes ownership of that pointer. If the pointer is null this
        /// will allocate the amount of space requested from the PredeterminedSize parameter.
        /// @param BinaryPointer A pointer to the buffer this will take ownership of, or nullptr to make a new buffer.
        /// @param PredeterminedSize The size to set on creation.
        BinaryBuffer(Byte* BinaryPointer, const SizeType PredeterminedSize);
        /// @brief String Constructor.
        /// @details Performs exactly one allocation of the amount required to store the String NOT including
        /// the null terminator.
        /// @param DataString The String to be copied into this buffer.
        explicit BinaryBuffer(const String& DataString);
        /// @brief Class Destructor
        /// @details If you want to preserve the buffer pointed to by an instance of BinaryBuffer then assign
        /// nullptr to the Binary member prior to destruction.
        ~BinaryBuffer();

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Copy Assignment Operator.
        /// @details Allocates identical amount of memory as other buffer then copies the other buffer into
        /// the allocated space. Each BinaryBuffer retains ownership of their respective buffers.
        /// @param Other The other buffer to be copied.
        /// @return Returns a reference to this.
        BinaryBuffer& operator=(const BinaryBuffer& Other);
        /// @brief Move Assignment Operator.
        /// @details This will first deallocate any buffer currently allocated in this and then perform a simple
        /// move of the Binary and Size from the Other BinaryBuffer and set it's members to their defaults
        /// (Binary:nullptr, Size:0).
        /// @param Other The other buffer to be moved.
        /// @return Returns a reference to this.
        BinaryBuffer& operator=(BinaryBuffer&& Other);
        /// @brief Addition Assignment Operator.
        /// @details This is an easier way to call @ref Concatenate(const Byte*, const SizeType)
        /// @param Other The other Buffer to copy/append.
        /// @return Returns a reference to this buffer to allow operator chaining.
        BinaryBuffer& operator+=(const BinaryBuffer& Other);

        ///////////////////////////////////////////////////////////////////////////////
        // Element Access

        /// @brief Array Access Operator.
        /// @param Index The offset from the start of the buffer to retrieve from.
        /// @return Returns a reference to the specific Byte the Index passed refers to.
        Byte& operator[](const SizeType Index) noexcept;
        /// @brief Array Access Operator.
        /// @param Index The offset from the start of the buffer to retrieve from.
        /// @return Returns a const reference to the specific Byte the Index passed refers to.
        const Byte& operator[](const SizeType Index) const noexcept;

        /// @brief Gets an element at the desired index.
        /// @exception This can throw a @ref Mezzanine::Exception::OutOfRange if the index value is higher
        /// than the Size member of this buffer.
        /// @param Index The index of the element to retrieve.
        /// @return Returns a reference to the element at the specified index.
        Byte& at(const SizeType Index);
        /// @brief Gets an element at the desired index.
        /// @exception This can throw a @ref Mezzanine::Exception::OutOfRange if the index value is higher
        /// than the Size member of this buffer.
        /// @param Index The index of the element to retrieve.
        /// @return Returns a const reference to the element at the specified index.
        const Byte& at(const SizeType Index) const;

        ///////////////////////////////////////////////////////////////////////////////
        // Utility

        /// @brief This will create a buffer with size matching the this->Size and point this->Binary to that Buffer.
        /// @exception If this buffers size is currently set at zero, a Mezzanine::Exception::LengthError exception will
        /// be thrown.
        /// @warning This will delete any existing buffer prior to creating the new one.  Use with caution.
        void CreateBuffer();
        /// @brief Deletes the existing internal buffer (if one is initialized).
        /// @param NewSize Sets the size variable after the buffer is deleted, making this ready to call CreateBuffer.
        void DeleteBuffer(const SizeType NewSize = 0);

        /// @brief Append another group of arbitrary data onto this one.
        /// @details Allocates space equal to the size of both buffers, Copies this Buffers data
        /// into it, then copies the other buffers data, then deallocates any space this buffer
        /// may have had allocated previously.
        /// @param Other A pointer to a region of memory to be copied.
        /// @param ByteSize How big in bytes is the Buffer.
        void Concatenate(const Byte* Other, const SizeType ByteSize);
        /// @brief Concatenate another buffer onto this one.
        /// @details This calls @ref Concatenate(const Byte*, SizeType)
        /// @param Other A buffer to copy and append onto this one.
        void Concatenate(const BinaryBuffer& Other);

        /// @brief Get the contents of this crudely converted to a c style string then stuff it in a string.
        /// @return A String with the value stored in binary copied into it.
        String ToString() const;
        /// @brief Gets the size of the managed buffer.
        /// @remarks Even though this class is intended to have its internals modified directly in some cases, in
        /// normal cases accessor are nice to have.
        /// @n @n
        /// This class is meant to have single byte elements so this should return the raw buffer size as well, but
        /// is written this way to help make the class future proof.
        /// @return Returns a whole representing the element count of the buffer.
        SizeType GetSize() const noexcept;
    };//BinaryBuffer
}//Mezzanine

#endif
