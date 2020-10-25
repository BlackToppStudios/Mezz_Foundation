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
#ifndef Mezz_Foundation_SerializationUtility_h
#define Mezz_Foundation_SerializationUtility_h

/// @file
/// @brief This file contains a set of forward declares and common utilities used by the serialization system.

#ifndef SWIG
    #include "DataTypes.h"
    #include "Introspection.h"
#endif // SWIG

namespace Mezzanine {
    /// @addtogroup Serialization
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    // Forward Declares

namespace Serialization {
    class AttributeWalker;
    class TreeWalker;
    class SerializerWalker;
    class DeserializerWalker;
}

    template<typename SerializeType, typename = std::enable_if_t<!std::is_pointer_v<SerializeType>>>
    void Serialize(const StringView Name,
                   const SerializeType& ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker);
    template<typename SerializeType, typename = std::enable_if_t<std::is_pointer_v<SerializeType>>>
    void Serialize(const StringView Name,
                   const SerializeType ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker);
    template<typename SerializeType>
    void Serialize(const StringView Name,
                   const std::shared_ptr<SerializeType> ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker);

    template<typename SerializeType, typename = std::enable_if_t<!std::is_pointer_v<SerializeType>>>
    void Serialize(const StringView Name,
                   const SerializeType& ToSerialize,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker);
    template<typename SerializeType, typename = std::enable_if_t<std::is_pointer_v<SerializeType>>>
    void Serialize(const StringView Name,
                   const SerializeType ToSerialize,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker);
    template<typename SerializeType>
    void Serialize(const StringView Name,
                   const std::shared_ptr<SerializeType> ToSerialize,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker);

    template<typename DeserializeType, typename = std::enable_if_t<!std::is_pointer_v<DeserializeType>>>
    void Deserialize(const StringView Name,
                     DeserializeType& ToDeserialize,
                     const MemberTags Tags,
                     Serialization::DeserializerWalker& Walker);
    template<typename DeserializeType, typename = std::enable_if_t<std::is_pointer_v<DeserializeType>>>
    void Deserialize(const StringView Name,
                     DeserializeType ToDeserialize,
                     const MemberTags Tags,
                     Serialization::DeserializerWalker& Walker);
    template<typename DeserializeType>
    void Deserialize(const StringView Name,
                     std::shared_ptr<DeserializeType> ToDeserialize,
                     const MemberTags Tags,
                     Serialization::DeserializerWalker& Walker);

    template<typename DeserializeType, typename = std::enable_if_t<!std::is_pointer_v<DeserializeType>>>
    void Deserialize(const StringView Name,
                     DeserializeType& ToDeserialize,
                     Serialization::DeserializerWalker& Walker);
    template<typename DeserializeType, typename = std::enable_if_t<std::is_pointer_v<DeserializeType>>>
    void Deserialize(const StringView Name,
                     DeserializeType ToDeserialize,
                     Serialization::DeserializerWalker& Walker);
    template<typename DeserializeType>
    void Deserialize(const StringView Name,
                     std::shared_ptr<DeserializeType> ToDeserialize,
                     Serialization::DeserializerWalker& Walker);

namespace Serialization {
    ///////////////////////////////////////////////////////////////////////////////
    // Serialize/Deserialize Detection

    /// @brief Convenience type for Serialize function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using SerializeFunct_t = decltype(
        std::declval<Class&>().Serialize(std::declval<Int32>(),
                                         std::declval<Serialization::SerializerWalker&>())
    );
    /// @brief Type for is_detected that tests for the existence of Serialize on a class.
    /// @tparam Class The class that will be checked for the presence of a Serialize function.
    template<typename Class>
    using HasSerialize_t = std::is_detected<SerializeFunct_t,Class>;
    /// @brief Convenience function for the value of a HasGetName check.
    /// @tparam Class The class that will be checked for the presence of a Serialize function.
    /// @return Returns true if the Class to check has a "Serialize()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasSerialize()
        { return HasSerialize_t<Class>::value; }

    /// @brief Convenience type for Deserialize function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using DeserializeFunct_t = decltype(
        std::declval<Class&>().Deserialize(std::declval<Int32>(),
                                           std::declval<Serialization::DeserializerWalker&>())
    );
    /// @brief Type for is_detected that tests for the existence of Deserialize on a class.
    /// @tparam Class The class that will be checked for the presence of a Deserialize function.
    template<typename Class>
    using HasDeserialize_t = std::is_detected<DeserializeFunct_t,Class>;
    /// @brief Convenience function for the value of a HasGetName check.
    /// @tparam Class The class that will be checked for the presence of a Deserialize function.
    /// @return Returns true if the Class to check has a "Deserialize()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasDeserialize()
        { return HasDeserialize_t<Class>::value; }

    ///////////////////////////////////////////////////////////////////////////////
    // Name/ID Detection and Retrieval

    /// @brief Convenience type for GetName function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using GetNameFunct_t = decltype(std::declval<Class&>().GetName());
    /// @brief Type for is_detected that tests for the existence of GetName on a class.
    /// @tparam Class The class that will be checked for the presence of a GetName function.
    template<typename Class>
    using HasGetName_t = std::is_detected<GetNameFunct_t,Class>;
    /// @brief Convenience function for the value of a HasGetName check.
    /// @tparam Class The class that will be checked for the presence of a GetName function.
    /// @return Returns true if the Class to check has a "GetName()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasGetName()
        { return HasGetName_t<Class>::value; }

    /// @brief Convenience type for GetID function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using GetIDFunct_t = decltype(std::declval<Class&>().GetID());
    /// @brief Type for is_detected that tests for the existence of GetID on a class.
    /// @tparam Class The class that will be checked for the presence of a GetID function.
    template<typename Class>
    using HasGetID_t = std::is_detected<GetIDFunct_t,Class>;
    /// @brief Convenience function for the value of a HasGetID check.
    /// @tparam Class The class that will be checked for the presence of a GetID function.
    /// @return Returns true if the Class to check has a "GetID()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasGetID()
        { return HasGetID_t<Class>::value; }

    /// @brief Convenience type for GetIdentification function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using GetIdentificationFunct_t = decltype(std::declval<Class&>().GetIdentification());
    /// @brief Type for is_detected that tests for the existence of GetIdentification on a class.
    /// @tparam Class The class that will be checked for the presence of a GetIdentification function.
    template<typename Class>
    using HasGetIdentification_t = std::is_detected<GetIdentificationFunct_t,Class>;
    /// @brief Convenience function for the value of a HasGetIdentification check.
    /// @tparam Class The class that will be checked for the presence of a GetIdentification function.
    /// @return Returns true if the Class to check has a "GetIdentification()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasGetIdentification()
        { return HasGetIdentification_t<Class>::value; }

    /// @brief Gets the ID of an object instance, if such an ID can be retrieved.
    /// @tparam SerializeType The (deduced) type of object to attemp to get the ID of.
    /// @remarks This function will detect for the presence of a member function with the signature of
    /// "GetName()", "GetID()", or "GetIdentification()" (in that order).  If found, the return type of this
    /// function will match that of the detected member function (or void if none are found).  Invoking this
    /// function will also invoke that member function, or be a no-op in the case that no such function is found.
    /// @param ToID The instance to invoke the ID retrieval function on.
    /// @return Returns the ID (Often a String or Int, but depends on SerializeType) of the object instance.
    template<typename SerializeType>
    auto GetObjectID(const SerializeType& ToID)
    {
        if constexpr( HasGetName<SerializeType>() ) {
            return ToID.GetName();
        }else if constexpr( HasGetID<SerializeType>() ) {
            return ToID.GetID();
        }else if constexpr( HasGetIdentification<SerializeType>() ) {
            return ToID.GetIdentification();
        }else{
            return;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Base Class Detection

    template<typename Return, typename Object>
    Object BaseReturner(Return(Object::*));

    ///////////////////////////////////////////////////////////////////////////////
    // Convenience Constants

SAVE_WARNING_STATE
SUPPRESS_CLANG_WARNING("-Wmissing-variable-declarations")

    /// @brief An integer constant to represent the value for the latest version available for serialization.
    inline constexpr Integer LatestVersion = 0;

RESTORE_WARNING_STATE

    ///////////////////////////////////////////////////////////////////////////////
    // MemberTag Checks

    /// @brief Convenience check for the Ignore tag in a MemberTags bitfield.
    /// @param Tags The bitfield to check.
    /// @return Returns true if the Ignore tag is present in the provided bitfield, false otherwise.
    constexpr Boole IsIgnorable(const MemberTags Tags)
    {
        return ( Tags & MemberTags::Ignore ) != MemberTags::None;
    }
    /// @brief Convenience check for the NotOwned tag in a MemberTags bitfield.
    /// @param Tags The bitfield to check.
    /// @return Returns true if the NotOwned tag is present in the provided bitfield, false otherwise.
    constexpr Boole IsNotOwned(const MemberTags Tags)
    {
        return ( Tags & MemberTags::NotOwned ) != MemberTags::None;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Identity Functions

    /// @brief Gets a serialization friendly identity of a type.
    /// @remarks This function operates a lot like @ref GetRegisteredName, except it can handle
    /// pointer and reference types and will add them to the type name string as necessary.
    /// @tparam TypeToName The type that will be identified.
    /// @return Returns a String containing the identity of the specified type.
    template<typename TypeToName>
    constexpr String GetSerializableTypeIdentity()
    {
        using DecayedType = std::decay_t< TypeToName >;
        String ReturnName{ GetRegisteredName< std::remove_pointer_t<DecayedType> >() };
        if constexpr( std::is_pointer_v<DecayedType> ) {
            ReturnName.push_back('*');
        }
        if constexpr( std::is_reference_v<TypeToName> ) {
            ReturnName.push_back('&');
        }
        return ReturnName;
    }

    /// @}
}//Serialization
}//Mezzanine

#endif // Mezz_Foundation_SerializationUtility_h
