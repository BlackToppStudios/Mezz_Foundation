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

#ifndef Mezz_Foundation_BitFieldTools_h
#define Mezz_Foundation_BitFieldTools_h

// Some headers we don't want swig to see.
#ifndef SWIG
    #include "DataTypes.h"
#endif

namespace Mezzanine
{
    /// @brief Convenience method for getting the value of the n-th bit.
    /// @remarks This is useful for setting values on bit-fields.  A value of 0 returns 0,
    /// a value of 1 returns 1, 2 returns 2, 3 returns 4, 4 returns 8, 5 returns 16, etc..
    /// @param Bit The n-th bit to get the value of.  Range of 0-64.
    /// @return Returns the value at the specified bit position.
    constexpr UInt64 EnumBit(const UInt32 Bit)
    {
        UInt64 BitValue = 1;
        return ( Bit == 0 ? 0 : BitValue << ( Bit - 1 ) );
    }

    /// @brief A default type traits class for all class enums.
    /// @tparam Enum A class enum of the type that this is reporting traits on.
    /// @details Defaults to not supporting the masking operators | and &.
    template<typename Enum>
    class BitMaskOperatorTraits
    {
    public:
        /// @brief Specializations that support the bit masking operators | and & should set this to true.
        static const bool Supported = false;
    };

    /// @brief Implements the bitwise OR operator for enum classes that are bitfields.
    /// @tparam Enum A class enum of the type that this the operator|.
    /// @param Left The left hand operand to OR.
    /// @param Right The right hand operand to OR.
    /// @return Another instance of the class enum with all the bits in either.
    template<typename Enum>
    constexpr std::enable_if_t<BitMaskOperatorTraits<Enum>::Supported,Enum>
        operator|(const Enum Left, const Enum Right)
    {
        using InternalType = typename std::underlying_type<Enum>::type;
        return static_cast<Enum> (
            static_cast<InternalType>(Left) |
            static_cast<InternalType>(Right)
        );
    }

    /// @brief Implements the bitwise AND operator for enum classes that are bitfields.
    /// @tparam Enum A class enum of the type that this the operator&.
    /// @param Left The left hand operand to AND.
    /// @param Right The right hand operand to AND.
    /// @return Another instance of the class enum with only the bits in both.
    template<typename Enum>
    constexpr std::enable_if_t<BitMaskOperatorTraits<Enum>::Supported,Enum>
        operator&(const Enum Left, const Enum Right)
    {
        using InternalType = typename std::underlying_type<Enum>::type;
        return static_cast<Enum> (
            static_cast<InternalType>(Left) &
            static_cast<InternalType>(Right)
        );
    }

    /// @brief Implements the bitwise Ones Compliment operator for enum classes that are bitfields.
    /// @tparam Enum A class enum of the type that this the operator~.
    /// @param Unary The operand to flip.
    /// @return Another instance of the class enum with the bits flipped/inverted.
    template<typename Enum>
    constexpr std::enable_if_t<BitMaskOperatorTraits<Enum>::Supported,Enum>
        operator~(const Enum Unary)
    {
        using InternalType = typename std::underlying_type<Enum>::type;
        return static_cast<Enum> (
            ~static_cast<InternalType>(Unary)
        );
    }

    /// @def ENABLE_BITMASK_OPERATORS
    /// @brief Use this to tag class enums OUTSIDE the Mezzanine namespace/code so to enable | and & operators.
    #define ENABLE_BITMASK_OPERATORS(x)         \
    template<>                                  \
    struct Mezzanine::BitMaskOperatorTraits<x>  \
    {                                           \
        public:                                 \
            static const bool Supported = true; \
    };

    /// @def ENABLE_BITMASK_OPERATORS_INSIDE_MEZZANINE
    /// @brief Use this to tag class enums INSIDE the Mezzanine namespace/code so to enable | and & operators.
    #define ENABLE_BITMASK_OPERATORS_INSIDE_MEZZANINE(x)    \
    template<>                                              \
    struct BitMaskOperatorTraits<x>                         \
    {                                                       \
        public:                                             \
            static const bool Supported = true;             \
    };

} // \Mezzanine Namespace

#endif //_logging_h
