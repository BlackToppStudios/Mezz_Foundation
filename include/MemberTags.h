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
#ifndef Mezz_Foundation_MemberTags_h
#define Mezz_Foundation_MemberTags_h

/// @file
/// @brief This file contains basic tools for conveying metadata about class/struct data members.

#ifndef SWIG
    #include "BitFieldTools.h"
#endif

namespace Mezzanine {
    /*
    // These tags are left over from a short-lived attempt at using tags as types for the metadata of individual
    // class members.  The idea being that there would be a third variadic template parameter for each created
    // member that contains all the tags appropriate for that member (most being empty) which would then get
    // stored in a tuple and (using) aliased on that member type.  That type could then be queried using the
    // type traits below (tuple_has_type) to generate custom behavior for specific types of data.
    // This system is unworkable because of the run-time polymorphic requirements of the serialization backends
    // that are planned for the system.  There is no way to pass around arbitrary and concrete types in a sane
    // manner through a virtual interface.  Furthermore, the likelihood that arbitrary tag types will be needed
    // is very low as it would require the user to write (or modify) their own serialization backends to leverage
    // the flexibility.  This is a significant coding burden that we shouldn't plan on as library maintainers.
    // For these reasons we've shelved explicit types as member tags in favor of an enum (MemberTags), despite
    // it's limitations, as we don't expect to be all that hindered by them in all but the most extreme use cases.
    // Should the run-time polymorphic requirement of the serialization system be lifted then it is possible that
    // some performance benefits could be gained by reverting to this approach to member tags.  That is why
    // this code is left commented and this documentation was written.

    struct Local_Tag
        {  };

    struct Generated_Tag
        {  };
    */

    /// @brief A collection of tags describing how member data should be treated in some cases.
    /// @remarks This enum is meant to be used to determine when special steps need to be taken when looking at
    /// member data.  For example a networked serialization backend might skip any member with the Local tag.
    enum class MemberTags
    {
        None        = EnumBit(0), ///< Nothing special about the member.
        Ignore      = EnumBit(1), ///< Member has been explicitly requested to be ignored.
        Local       = EnumBit(2), ///< Member is only of use on the local host and should not be shared.
        Generated   = EnumBit(3), ///< Member is a cache generated from other members and can be regenerated.
        Deprecated  = EnumBit(4), ///< Member is deprecated and it's use should be avoided.
        Own         = EnumBit(5), ///< Member is a Pointer or Reference to an object owned by the parent object.

        Default     = None,       ///< The default value for non-pointer members.
        DefaultPtr  = Own         ///< The default value for pointer members.
    };
    ENABLE_BITMASK_OPERATORS_INSIDE_MEZZANINE(MemberTags)

    /// @brief Stream to standard output stream operator.
    /// @param Stream The standard output to stream to.
    /// @param TagToStream The MemberTag value that will be converted and streamed.
    /// @return Returns a reference to the standard output stream.
    std::ostream& operator<<(std::ostream& Stream, const MemberTags TagToStream);//[-Werror=missing-declarations]
    std::ostream& operator<<(std::ostream& Stream, const MemberTags TagToStream)
    {
        Stream << static_cast< std::underlying_type_t<MemberTags> >(TagToStream);
        return Stream;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Default Tags

    /// @brief Base DefaultMemberTags implementation.
    /// @remarks This is used as a catch-all for non-specific types.
    /// @tparam T The type to get the default member tags of.
    template<typename T>
    struct DefaultMemberTags
    {
        static constexpr MemberTags value = MemberTags::Default;
    };
    /// @brief DefaultMemberTags implementation for pointers.
    /// @remarks This is a template specialization for pointer types.
    /// @tparam T The type behind the pointer.
    template<typename T>
    struct DefaultMemberTags<T*>
    {
        static constexpr MemberTags value = MemberTags::DefaultPtr;
    };

    /// @brief Convenience function to get the default traits for a given type.
    /// @remarks This function is intended to be used in template metaprogramming.
    /// @tparam T The type to get the default member tags of.
    /// @return Returns a MemberTags value containing the default set of tags for the provided type.
    template<typename T>
    constexpr MemberTags GetDefaultMemberTags()
        { return DefaultMemberTags< std::decay_t<T> >::value; }
}//Mezzanine

#endif
