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
    // Result Enums

    /// @brief An enum used by Deseriailze functions to communicate non-critical results of the operation.
    enum class DeserializeResult
    {
        Success,       ///< The object was deserialized.
        NeedsDeferred  ///< The object is not ready to be deserialized and needs to be cached.
    };//DeserializeResult

    ///////////////////////////////////////////////////////////////////////////////
    // Forward Declares

namespace Serialization {
    class AttributeWalker;
    class TreeWalker;
    class SerializerWalker;
    class DeserializerWalker;
}
    template< typename SerializeType,
              typename = std::enable_if_t< !std::is_pointer_v<SerializeType> > >
    void ProtoSerialize(const SerializeType& ToSerialize,
                        const MemberTags Tags,
                        const Int32 Version,
                        Serialization::SerializerWalker& Walker);
    template< typename SerializeType,
              typename = std::enable_if_t< std::is_pointer_v<SerializeType> > >
    void ProtoSerialize(const SerializeType ToSerialize,
                        const MemberTags Tags,
                        const Int32 Version,
                        Serialization::SerializerWalker& Walker);
    template< typename SerializeType >
    void ProtoSerialize(const std::unique_ptr<SerializeType>& ToSerialize,
                        const MemberTags Tags,
                        const Int32 Version,
                        Serialization::SerializerWalker& Walker);
    template< typename SerializeType >
    void ProtoSerialize(const std::shared_ptr<SerializeType> ToSerialize,
                        const MemberTags Tags,
                        const Int32 Version,
                        Serialization::SerializerWalker& Walker);
    template< typename SerializeType >
    void ProtoSerialize(const std::optional<SerializeType>& ToSerialize,
                        const MemberTags Tags,
                        const Int32 Version,
                        Serialization::SerializerWalker& Walker);

    template< typename SerializeType,
              typename = std::enable_if_t< !std::is_pointer_v<SerializeType> > >
    void Serialize(const StringView Name,
                   const SerializeType& ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker);
    template< typename SerializeType,
              typename = std::enable_if_t< std::is_pointer_v<SerializeType> > >
    void Serialize(const StringView Name,
                   const SerializeType ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker);
    template< typename SerializeType >
    void Serialize(const StringView Name,
                   const std::unique_ptr<SerializeType>& ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker);
    template< typename SerializeType >
    void Serialize(const StringView Name,
                   const std::shared_ptr<SerializeType> ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker);

    template< typename SerializeType,
              typename = std::enable_if_t< !std::is_pointer_v<SerializeType> > >
    void Serialize(const StringView Name,
                   const SerializeType& ToSerialize,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker);
    template< typename SerializeType,
              typename = std::enable_if_t< std::is_pointer_v<SerializeType> > >
    void Serialize(const StringView Name,
                   const SerializeType ToSerialize,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker);
    template< typename SerializeType >
    void Serialize(const StringView Name,
                   const std::unique_ptr<SerializeType>& ToSerialize,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker);
    template< typename SerializeType >
    void Serialize(const StringView Name,
                   const std::shared_ptr<SerializeType> ToSerialize,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker);

    template< typename DeserializeType,
              typename = std::enable_if_t< !std::is_pointer_v<DeserializeType> >,
              typename = void >
    void ProtoDeserialize(DeserializeType& ToDeserialize,
                          const MemberTags Tags,
                          Serialization::DeserializerWalker& Walker);
    template< typename DeserializeType,
              typename = std::enable_if_t< std::is_pointer_v<DeserializeType> > >
    void ProtoDeserialize(DeserializeType& ToDeserialize,
                          const MemberTags Tags,
                          Serialization::DeserializerWalker& Walker);
    template< typename DeserializeType >
    void ProtoDeserialize(std::unique_ptr<DeserializeType>& ToDeserialize,
                          const MemberTags Tags,
                          Serialization::DeserializerWalker& Walker);
    template< typename DeserializeType >
    void ProtoDeserialize(std::shared_ptr<DeserializeType>& ToDeserialize,
                          const MemberTags Tags,
                          Serialization::DeserializerWalker& Walker);
    template< typename SerializeType >
    void ProtoDeserialize(const std::optional<SerializeType>& ToSerialize,
                          const MemberTags Tags,
                          Serialization::SerializerWalker& Walker);

    template< typename DeserializeType,
              typename = std::enable_if_t< !std::is_pointer_v<DeserializeType> >,
              typename = void >
    void Deserialize(const StringView Name,
                     DeserializeType& ToDeserialize,
                     const MemberTags Tags,
                     Serialization::DeserializerWalker& Walker);
    template< typename DeserializeType,
              typename = std::enable_if_t< std::is_pointer_v<DeserializeType> > >
    void Deserialize(const StringView Name,
                     DeserializeType& ToDeserialize,
                     const MemberTags Tags,
                     Serialization::DeserializerWalker& Walker);
    template< typename DeserializeType >
    void Deserialize(const StringView Name,
                     std::shared_ptr<DeserializeType>& ToDeserialize,
                     const MemberTags Tags,
                     Serialization::DeserializerWalker& Walker);
    template< typename DeserializeType >
    void Deserialize(const StringView Name,
                     std::unique_ptr<DeserializeType>& ToDeserialize,
                     const MemberTags Tags,
                     Serialization::DeserializerWalker& Walker);

    template< typename DeserializeType,
              typename = std::enable_if_t< !std::is_pointer_v<DeserializeType> >,
              typename = void >
    void Deserialize(const StringView Name,
                     DeserializeType& ToDeserialize,
                     Serialization::DeserializerWalker& Walker);
    template< typename DeserializeType,
              typename = std::enable_if_t< std::is_pointer_v<DeserializeType> > >
    void Deserialize(const StringView Name,
                     DeserializeType& ToDeserialize,
                     Serialization::DeserializerWalker& Walker);
    template< typename DeserializeType >
    void Deserialize(const StringView Name,
                     std::shared_ptr<DeserializeType>& ToDeserialize,
                     Serialization::DeserializerWalker& Walker);
    template< typename DeserializeType >
    void Deserialize(const StringView Name,
                     std::unique_ptr<DeserializeType>& ToDeserialize,
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
        return ( Tags & MemberTags::Ignore ) == MemberTags::Ignore;
    }

    /// @brief Convenience check for the Own tags in a MemberTags bitfield.
    /// @param Tags The bitfield to check.
    /// @return Returns true if a Own or SharedOwn tag is present in the provided bitfield, false otherwise.
    constexpr Boole IsOwned(const MemberTags Tags)
    {
        return ( Tags & MemberTags::Own ) == MemberTags::Own;
    }
    /// @brief Convenience check for the Own tags in a MemberTags bitfield.
    /// @param Tags The bitfield to check.
    /// @return Returns true if a Own or SharedOwn tag is present in the provided bitfield, false otherwise.
    constexpr Boole IsNotOwned(const MemberTags Tags)
    {
        return !IsOwned(Tags);
    }

    /// @brief Adds the Own tag to a MemberTag bitmask.
    /// @remarks If the provided bitmask alread contains the Own value, this function has no effect.
    /// @param Tags The MemberTags bitmask to (maybe) modify.
    /// @return Returns the modified MemberTags bitmask.
    constexpr MemberTags MakeOwned(const MemberTags Tags)
    {
        return ( Tags | MemberTags::Own );
    }
    /// @brief Removes all ownership tags from a MemberTag bitmask.
    /// @remarks If the provided bitmask does not contain an Own or SharedOwn value, this function has no effect.
    /// @param Tags The MemberTags bitmask to (maybe) modify.
    /// @return Returns the modified MemberTags bitmask.
    constexpr MemberTags MakeNotOwned(const MemberTags Tags)
    {
        return ( Tags & ~MemberTags::Own );
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Smart Pointer Detection

    /// @brief Type trait for the detection of a shared_ptr type.
    /// @tparam Type The type to be checked.
    /// @details This is the dummy trait for failure cases.
    template<typename Type>
    struct is_shared_ptr : std::false_type
        {  };
    /// @brief Type trait for the detection of a shared_ptr type.
    /// @tparam Type The type to be checked.
    /// @details This is a specialization type for the positive detection of a shared_ptr.
    template<typename Type>
    struct is_shared_ptr<std::shared_ptr<Type>> : std::true_type
        {  };

    /// @brief Type trait for the detection of a weak_ptr type.
    /// @tparam Type The type to be checked.
    /// @details This is the dummy trait for failure cases.
    template<typename Type>
    struct is_weak_ptr : std::false_type
        {  };
    /// @brief Type trait for the detection of a weak_ptr type.
    /// @tparam Type The type to be checked.
    /// @details This is a specialization type for the positive detection of a weak_ptr.
    template<typename Type>
    struct is_weak_ptr<std::weak_ptr<Type>> : std::true_type
        {  };

    /// @brief Type trait for the detection of a unique_ptr type.
    /// @tparam Type The type to be checked.
    /// @details This is the dummy trait for failure cases.
    template<typename Type>
    struct is_unique_ptr : std::false_type
        {  };
    /// @brief Type trait for the detection of a unique_ptr type.
    /// @tparam Type The type to be checked.
    /// @details This is a specialization type for the positive detection of a unique_ptr.
    template<typename Type>
    struct is_unique_ptr<std::unique_ptr<Type>> : std::true_type
        {  };

    ///////////////////////////////////////////////////////////////////////////////
    // Convenience Type Checks

    /// @brief Checks to see if a type is a standard array.
    /// @tparam ToCheck The type to be checked.
    /// @return Returns true if the type checked is some flavor of std::array, false otherwise.
    template<typename ToCheck>
    constexpr Boole IsArray()
        { return std::is_same_v<ToCheck,std::array>; }
    /// @brief Checks to see if a type is a raw pointer.
    /// @tparam ToCheck The type to be checked.
    /// @return Returns true if the type checked is a raw pointer, false otherwise.
    template<typename ToCheck>
    constexpr Boole IsRawPointer()
    {
        return std::is_pointer_v<ToCheck>;
    }
    /// @brief Checks to see if a type is a smart pointer with shared ownership.
    /// @tparam ToCheck The type to be checked.
    /// @return Returns true if the type checked is a shared pointer, false otherwise.
    template<typename ToCheck>
    constexpr Boole IsSharedPointer()
    {
        return is_shared_ptr<ToCheck>::value;
    }
    /// @brief Checks to see if a type is a smart pointer.
    /// @tparam ToCheck The type to be checked.
    /// @return Returns true if the type checked is a smart pointer, false otherwise.
    template<typename ToCheck>
    constexpr Boole IsSmartPointer()
    {
        return is_shared_ptr<ToCheck>::value ||
               is_weak_ptr<ToCheck>::value ||
               is_unique_ptr<ToCheck>::value;
    }
    /// @brief Checks to see if a type is a pointer.
    /// @remarks This includes raw pointers and smart pointers alike.
    /// @tparam ToCheck The type to be checked.
    /// @return Returns true if the type checked is a raw or smart pointer, false otherwise.
    template<typename ToCheck>
    constexpr Boole IsPointer()
    {
        return IsRawPointer<ToCheck>() || IsSmartPointer<ToCheck>();
    }
    /// @brief Checks to see if a type is an owned raw pointer.
    /// @remarks Keep in mind that a false return doesn't mean the pointer is not owned, as it may not
    /// even be a pointer.
    /// @tparam ToCheck The type to be checked.
    /// @return Returns true if the type checked is an owned raw or std pointer, false otherwise.
    template<typename ToCheck, MemberTags Tags>
    constexpr Boole IsOwnedRawPointer()
    {
        return ( IsRawPointer<ToCheck>() && IsOwned(Tags) );
    }
    /// @brief Checks to see if a type is a non-owned raw pointer.
    /// @remarks Keep in mind that a false return doesn't mean the pointer is owned, as it may not
    /// even be a pointer.
    /// @tparam ToCheck The type to be checked.
    /// @return Returns true if the type checked is a non-owned raw or std pointer, false otherwise.
    template<typename ToCheck, MemberTags Tags>
    constexpr Boole IsNonOwnedRawPointer()
    {
        return ( IsRawPointer<ToCheck>() && IsNotOwned(Tags) );
    }
    /// @brief Checks to see if a type is an owned pointer.
    /// @remarks This includes raw pointers and std pointers alike. @n@n
    /// Keep in mind that a false return doesn't mean the pointer is not owned, as it may not
    /// even be a pointer.
    /// @tparam ToCheck The type to be checked.
    /// @return Returns true if the type checked is an owned raw or std pointer, false otherwise.
    template<typename ToCheck, MemberTags Tags>
    constexpr Boole IsOwnedPointer()
    {
        return IsOwnedRawPointer<ToCheck,Tags>() ||
               is_shared_ptr<ToCheck>::value ||
               is_unique_ptr<ToCheck>::value;
    }
    /// @brief Checks to see if a type is a non-owned pointer.
    /// @remarks This includes raw pointers and std pointers alike. @n@n
    /// Keep in mind that a false return doesn't mean the pointer is owned, as it may not
    /// even be a pointer.
    /// @tparam ToCheck The type to be checked.
    /// @return Returns true if the type checked is a non-owned raw or std pointer, false otherwise.
    template<typename ToCheck, MemberTags Tags>
    constexpr Boole IsNonOwnedPointer()
    {
        return IsNonOwnedRawPointer<ToCheck,Tags>() ||
               is_weak_ptr<ToCheck>::value;
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

    ///////////////////////////////////////////////////////////////////////////////
    // Allocation

    /// @brief Heap allocates a new object of an arbitrary type.
    /// @pre Type must have a default constructor.
    /// @tparam Type Any type with a default constructor that may need to be created during deserialization.
    /// @remarks This function does not have a deallocate equivalent because it does not presume ownership.
    /// Anything allocated by this function will immedately be assigned to the appropriate member and deallocation
    /// is presumed to be handled by the type/instance it was placed into. @n@n
    /// This function is deliberately basic. Overrides are possible and encouraged if a more advanced allocation
    /// strategy is required.
    /// @return Returns a pointer to the created instance.
    template<typename Type>
    Type* Allocate()
    {
        return new Type();
    }

    /// @}
}//Serialization
}//Mezzanine

#endif // Mezz_Foundation_SerializationUtility_h
