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
#ifndef Mezz_Foundation_Serialization_h
#define Mezz_Foundation_Serialization_h

/// @file
/// @brief The collection of utilities that form the Serialization front-end.

#ifndef SWIG
    #include "SerializationUtility.h"
    #include "SerializationTree.h"
    #include "SerializationPolymorphicCaster.h"
    #include "SerializationPointerTracker.h"
    #include "SerializationSerializer.h"
    #include "SerializationDeserializer.h"
#endif
/*
namespace Mezzanine {
    /// @addtogroup Serialization
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    // Forward Declares

namespace Serialization {
    class AttributeWalker;
    class ObjectWalker;
}

    template<typename SerializeType, typename = std::enable_if_t<!std::is_pointer_v<SerializeType>>>
    void Serialize(const StringView Name,
                   const SerializeType& ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::ObjectWalker& Walker);
    template<typename SerializeType, typename = std::enable_if_t<std::is_pointer_v<SerializeType>>>
    void Serialize(const StringView Name,
                   const SerializeType ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::ObjectWalker& Walker);
    template<typename SerializeType>
    void Serialize(const StringView Name,
                   const std::shared_ptr<SerializeType> ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::ObjectWalker& Walker);

    template<typename SerializeType, typename = std::enable_if_t<!std::is_pointer_v<SerializeType>>>
    void Serialize(const StringView Name,
                   const SerializeType& ToSerialize,
                   const Int32 Version,
                   Serialization::ObjectWalker& Walker);
    template<typename SerializeType, typename = std::enable_if_t<std::is_pointer_v<SerializeType>>>
    void Serialize(const StringView Name,
                   const SerializeType ToSerialize,
                   const Int32 Version,
                   Serialization::ObjectWalker& Walker);
    template<typename SerializeType>
    void Serialize(const StringView Name,
                   const std::shared_ptr<SerializeType> ToSerialize,
                   const Int32 Version,
                   Serialization::ObjectWalker& Walker);

    template<typename SerializeType, typename = std::enable_if_t<!std::is_pointer_v<SerializeType>>>
    void Deserialize(const StringView Name,
                     SerializeType& ToDeserialize,
                     const MemberTags Tags,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker);
    template<typename SerializeType, typename = std::enable_if_t<std::is_pointer_v<SerializeType>>>
    void Deserialize(const StringView Name,
                     SerializeType ToDeserialize,
                     const MemberTags Tags,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker);
    template<typename SerializeType>
    void Deserialize(const StringView Name,
                     std::shared_ptr<SerializeType> ToDeserialize,
                     const MemberTags Tags,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker);

    template<typename SerializeType, typename = std::enable_if_t<!std::is_pointer_v<SerializeType>>>
    void Deserialize(const StringView Name,
                     SerializeType& ToDeserialize,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker);
    template<typename SerializeType, typename = std::enable_if_t<std::is_pointer_v<SerializeType>>>
    void Deserialize(const StringView Name,
                     SerializeType ToDeserialize,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker);
    template<typename SerializeType>
    void Deserialize(const StringView Name,
                     std::shared_ptr<SerializeType> ToDeserialize,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker);

    template<typename SerializeFunctType>
    void Deserialize(const StringView Name,
                     const MemberTags Tags,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker,
                     SerializeFunctType Funct);

    //template<typename SerializeFunctType>
    //void Deserialize(const StringView Name,
    //                 const Int32 Version,
    //                 Serialization::ObjectWalker& Walker,
    //                 SerializeFunctType Funct);

namespace Serialization {
    ///////////////////////////////////////////////////////////////////////////////
    // Serialize/Deserialize Detection

    /// @brief Convenience type for Serialize function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using SerializeFunct_t = decltype(
        std::declval<Class&>().Serialize(std::declval<const StringView>(),
                                         std::declval<const MemberTags>(),
                                         std::declval<Int32>(),
                                         std::declval<Serialization::ObjectWalker&>())
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
                                           std::declval<Serialization::ObjectWalker&>())
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
        return ( Tags & MemberTags::Ignore ) == MemberTags::None;
    }
    /// @brief Convenience check for the NotOwned tag in a MemberTags bitfield.
    /// @param Tags The bitfield to check.
    /// @return Returns true if the NotOwned tag is present in the provided bitfield, false otherwise.
    constexpr Boole IsNotOwned(const MemberTags Tags)
    {
        return ( Tags & MemberTags::NotOwned ) == MemberTags::None;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Backend Interface

    // Needs ObjectNode forward declare

    class MEZZ_LIB BackendBase
    {
    public:
        /// @brief Class destructor.
        virtual ~BackendBase() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        virtual StringView GetImplementationName() const = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Root Object

        virtual ObjectWalker& GetWalker() const = 0;
    };//BackendBase

    ///////////////////////////////////////////////////////////////////////////////
    // SerializerContext

    class ContextBase
    {
    public:
        /// @brief Class destructor.
        virtual ~ContextBase() = default;

        virtual ContextBase* GetParentContext() const = 0;

        virtual Boole IsSame(void* ObjectPtr, const std::type_info& Info) const = 0;

        virtual void* FindContextObject(const StringView Name, const std::type_index Type) const = 0;
        virtual void* FindContextObject(const UInt64 ID, const std::type_index Type) const = 0;
    };//SerializerContextBase

    template<typename SpecialContext>
    class Context : public ContextBase
        {  };

    template<typename SpecialContext>
    const ContextBase* UpdateContext(const ContextBase* Current, const SpecialContext* ToUpdate)
    {
        using ContextType = Context<SpecialContext>;
        if constexpr( !std::is_abstract_v<ContextType> ) {
            static_assert( std::is_constructible_v<ContextType,ContextBase*,SpecialContext*>,
                           "SerializerContext type lacks well-formed constructor." );
            if( Current != nullptr && !Current->IsSame(ToUpdate,typeid(SpecialContext)) ) {
                return new ContextType(Current,ToUpdate);
            }
        }
        return Current;
    }

    template<typename SpecialContext>
    const ContextBase* RevertContext(const ContextBase* Current, const SpecialContext* ToRevert)
    {
        using ContextType = Context<SpecialContext>;
        if constexpr( !std::is_abstract_v<ContextType> ) {
            if( Current != nullptr && Current->IsSame(ToRevert,typeid(SpecialContext)) ) {
                ContextBase* Reverted = Current->GetParentContext();
                delete Current;
                return Reverted;
            }
        }
        return Current;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Tree Walkers

    ///////////////////////////////////////////////////////////////////////////////
    // AttributeWalker

    class MEZZ_LIB AttributeWalker
    {
    public:
        /// @brief Class destructor.
        virtual ~AttributeWalker() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Name Operations

        virtual void SetName(const StringView Name) = 0;
        [[nodiscard]]
        virtual StringView GetName() const = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Value Operations

        virtual void SetString(const StringView Value) = 0;
        [[nodiscard]]
        virtual std::optional<StringView> GetString() const = 0;

        virtual void SetLongDouble(const long double Value) = 0;
        [[nodiscard]]
        virtual std::optional<long double> GetLongDouble() const = 0;

        virtual void SetDouble(const double Value) = 0;
        [[nodiscard]]
        virtual std::optional<double> GetDouble() const = 0;

        virtual void SetFloat(const float Value) = 0;
        [[nodiscard]]
        virtual std::optional<float> GetFloat() const = 0;

        virtual void SetUInt64(const UInt64 Value) = 0;
        [[nodiscard]]
        virtual std::optional<UInt64> GetUInt64() const = 0;

        virtual void SetInt64(const Int64 Value) = 0;
        [[nodiscard]]
        virtual std::optional<Int64> GetInt64() const = 0;

        virtual void SetUInt32(const UInt32 Value) = 0;
        [[nodiscard]]
        virtual std::optional<UInt32> GetUInt32() const = 0;

        virtual void SetInt32(const Int32 Value) = 0;
        [[nodiscard]]
        virtual std::optional<Int32> GetInt32() const = 0;

        virtual void SetUInt16(const UInt16 Value) = 0;
        [[nodiscard]]
        virtual std::optional<UInt16> GetUInt16() const = 0;

        virtual void SetInt16(const Int16 Value) = 0;
        [[nodiscard]]
        virtual std::optional<Int16> GetInt16() const = 0;

        virtual void SetUInt8(const UInt8 Value) = 0;
        [[nodiscard]]
        virtual std::optional<UInt8> GetUInt8() const = 0;

        virtual void SetInt8(const Int8 Value) = 0;
        [[nodiscard]]
        virtual std::optional<Int8> GetInt8() const = 0;

        template<typename SetType>
        void SetValue(SetType&& ToSet);

        template<typename ReturnType>
        [[nodiscard]]
        ReturnType GetValue() const;

        ///////////////////////////////////////////////////////////////////////////////
        // Navigation

        virtual AttributeWalker& Next() = 0;
        virtual AttributeWalker& Previous() = 0;
        [[nodiscard]]
        virtual Boole AtBegin() const = 0;
        [[nodiscard]]
        virtual Boole AtEnd() const = 0;
    };//AttributeWalker

    namespace AttributeHelpers {
        template<typename Datum>
        void SetValue(AttributeWalker& Walker, const Datum Value)
            { (void)Walker;  (void)Value; }
        template<typename Datum>
        [[nodiscard]]
        std::optional<Datum> GetValue(const AttributeWalker& Walker)
            { (void)Walker; return std::optional<Datum>(); }

        template<>
        void SetValue<StringView>(AttributeWalker& Walker, const StringView Value)
            { Walker.SetString(Value); }
        template<>
        [[nodiscard]]
        std::optional<StringView> GetValue<StringView>(const AttributeWalker& Walker)
            { return Walker.GetString(); }

        template<>
        void SetValue<long double>(AttributeWalker& Walker, const long double Value)
            { Walker.SetLongDouble(Value); }
        template<>
        [[nodiscard]]
        std::optional<long double> GetValue<long double>(const AttributeWalker& Walker)
            { return Walker.GetLongDouble(); }

        template<>
        void SetValue<double>(AttributeWalker& Walker, const double Value)
            { Walker.SetDouble(Value); }
        template<>
        [[nodiscard]]
        std::optional<double> GetValue<double>(const AttributeWalker& Walker)
            { return Walker.GetDouble(); }

        template<>
        void SetValue<float>(AttributeWalker& Walker, const float Value)
            { Walker.SetFloat(Value); }
        template<>
        [[nodiscard]]
        std::optional<float> GetValue<float>(const AttributeWalker& Walker)
            { return Walker.GetFloat(); }

        template<>
        void SetValue<UInt64>(AttributeWalker& Walker, const UInt64 Value)
            { Walker.SetUInt64(Value); }
        template<>
        [[nodiscard]]
        std::optional<UInt64> GetValue<UInt64>(const AttributeWalker& Walker)
            { return Walker.GetUInt64(); }

        template<>
        void SetValue<Int64>(AttributeWalker& Walker, const Int64 Value)
            { Walker.SetInt64(Value); }
        template<>
        [[nodiscard]]
        std::optional<Int64> GetValue<Int64>(const AttributeWalker& Walker)
            { return Walker.GetInt64(); }

        template<>
        void SetValue<UInt32>(AttributeWalker& Walker, const UInt32 Value)
            { Walker.SetUInt32(Value); }
        template<>
        [[nodiscard]]
        std::optional<UInt32> GetValue<UInt32>(const AttributeWalker& Walker)
            { return Walker.GetUInt32(); }

        template<>
        void SetValue<Int32>(AttributeWalker& Walker, const Int32 Value)
            { Walker.SetInt32(Value); }
        template<>
        [[nodiscard]]
        std::optional<Int32> GetValue<Int32>(const AttributeWalker& Walker)
            { return Walker.GetInt32(); }

        template<>
        void SetValue<UInt16>(AttributeWalker& Walker, const UInt16 Value)
            { Walker.SetUInt16(Value); }
        template<>
        [[nodiscard]]
        std::optional<UInt16> GetValue<UInt16>(const AttributeWalker& Walker)
            { return Walker.GetUInt16(); }

        template<>
        void SetValue<Int16>(AttributeWalker& Walker, const Int16 Value)
            { Walker.SetInt16(Value); }
        template<>
        [[nodiscard]]
        std::optional<Int16> GetValue<Int16>(const AttributeWalker& Walker)
            { return Walker.GetInt16(); }

        template<>
        void SetValue<UInt8>(AttributeWalker& Walker, const UInt8 Value)
            { Walker.SetUInt8(Value); }
        template<>
        [[nodiscard]]
        std::optional<UInt8> GetValue<UInt8>(const AttributeWalker& Walker)
            { return Walker.GetUInt8(); }

        template<>
        void SetValue<Int8>(AttributeWalker& Walker, const Int8 Value)
            { Walker.SetInt8(Value); }
        template<>
        [[nodiscard]]
        std::optional<Int8> GetValue<Int8>(const AttributeWalker& Walker)
            { return Walker.GetInt8(); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // AttributeWalker Implementations

    template<typename SetType>
    void AttributeWalker::SetValue(SetType&& ToSet)
    {
        AttributeHelpers::SetValue<SetType>(*this,std::forward<SetType>(ToSet));
    }

    template<typename ReturnType>
    [[nodiscard]]
    ReturnType AttributeWalker::GetValue() const
    {
        return AttributeHelpers::GetValue<ReturnType>(*this);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // ObjectWalker

    class MEZZ_LIB ObjectWalker
    {
    public:
        /// @brief Class destructor.
        virtual ~ObjectWalker() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Object Operations

        virtual void SetName(const StringView Name) = 0;
        [[nodiscard]]
        virtual StringView GetName() const = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Object Navigation

        [[nodiscard]]
        virtual Boole AtRoot() const = 0;
        [[nodiscard]]
        virtual Boole IsFirstChild() const = 0;
        [[nodiscard]]
        virtual Boole IsLastChild() const = 0;
        [[nodiscard]]
        virtual Boole HasChildren() const = 0;
        [[nodiscard]]
        virtual Boole HasChild(const StringView Name) const = 0;

        virtual ObjectWalker& Next() = 0;
        virtual ObjectWalker& Previous() = 0;
        virtual ObjectWalker& Parent() = 0;
        virtual ObjectWalker& FirstChild() = 0;
        [[nodiscard]]
        virtual Boole Child(const StringView Name) = 0;

        [[nodiscard]]
        virtual Boole CreateChild(const StringView Name, const MemberTags Tags, const Boole Move) = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Attributes

        [[nodiscard]]
        virtual Boole HasAttributes() const = 0;
        [[nodiscard]]
        virtual Boole HasAttribute(const StringView Name) const = 0;
        [[nodiscard]]
        virtual AttributeWalker& GetAttributes() const = 0;
        [[nodiscard]]
        virtual AttributeWalker& GetAttribute(const StringView Name) const = 0;
        [[nodiscard]]
        virtual Boole CreateAttribute(const StringView Name, const MemberTags Tags) = 0;

        template<typename AttributeType>
        void Attribute(const StringView Name, const MemberTags Tags, AttributeType&& Attrib)
        {
            if( this->CreateAttribute(Name,Tags) ) {
                AttributeWalker& Walker = this->GetAttribute(Name);
                Walker.SetValue(std::forward<AttributeType>(Attrib));
            }
        }
        template<typename AttributeType>
        void Attribute(const StringView Name, AttributeType&& Attrib)
        {
            this->Attribute(Name,MemberTags::None,std::forward<AttributeType>(Attrib));
        }
        template<typename AttributeType>
        std::optional<AttributeType> Attribute(const StringView Name)
        {
            if( this->HasAttribute(Name) ) {
                AttributeWalker& Walker = this->GetAttribute(Name);
                return Walker.GetValue< std::decay<AttributeType> >();
            }
            return std::optional<AttributeType>();
        }
    };//ObjectWalker

    class MEZZ_LIB TreeWalker
    {
    public:

    };//TreeWalker

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization Tree Navigation Helpers

    class MEZZ_LIB ScopedSerializationNode
    {
    protected:
        Serialization::ObjectWalker* Node = nullptr;
    public:
        ScopedSerializationNode(const StringView Name, const MemberTags Tags, Serialization::ObjectWalker& Walker)
        {
            if( Walker.CreateChild(Name,Tags,true) ) {
                this->Node = &Walker;
            }
        }
        ScopedSerializationNode(const StringView Name, Serialization::ObjectWalker& Walker) :
            ScopedSerializationNode(Name,MemberTags::None,Walker)
            {  }
        ~ScopedSerializationNode()
        {
            if( this->IsValid() ) {
                this->Node->Parent();
            }
        }

        Boole IsValid() const
            { return ( this->Node != nullptr ); }

        explicit operator bool() const
            { return ( this->Node != nullptr ); }
    };

    class MEZZ_LIB ScopedDeserializationNode
    {
    protected:
        Serialization::ObjectWalker* Node = nullptr;
    public:
        ScopedDeserializationNode(Serialization::ObjectWalker& Walker)
        {
            if( Walker.HasChildren() ) {
                Walker.FirstChild();
                this->Node = &Walker;
            }
        }
        ScopedDeserializationNode(const StringView Name, Serialization::ObjectWalker& Walker)
        {
            if( Walker.Child(Name) ) {
                this->Node = &Walker;
            }
        }
        ~ScopedDeserializationNode()
        {
            if( this->IsValid() ) {
                this->Node->Parent();
            }
        }

        Boole IsValid() const
            { return ( this->Node != nullptr ); }

        explicit operator bool() const
            { return ( this->Node != nullptr ); }
    };

    ///////////////////////////////////////////////////////////////////////////////
    // PolymorphicCasterHolder
#if 1
    template<class Base, class Derived>
    class PolymorphicCasterImpl;

    class MEZZ_LIB PolymorphicCaster
    {
    public:
        virtual ~PolymorphicCaster() = default;
        virtual StringView GetBaseTypeName() const = 0;
        virtual StringView GetDerivedTypeName() const = 0;
        virtual Boole CanUpcast(const std::type_info& BaseType, const std::type_info& DerivedType) const = 0;
        virtual Boole CanUpcast(const StringView BaseType, const StringView DerivedType) const = 0;
        virtual void Serialize(const StringView Name,
                               void* ToSerialize,
                               const MemberTags Tags,
                               const Integer Version,
                               Serialization::ObjectWalker& Walker) = 0;
        virtual void Deserialize(const StringView Name,
                                 void*& ToDeserialize,
                                 const MemberTags Tags,
                                 const Integer Version,
                                 Serialization::ObjectWalker& Walker) = 0;
    };//PolymorphicCaster

    struct MEZZ_LIB PolymorphicCasterHolder
    {
        using CasterBaseType = PolymorphicCaster;
        using StoredCasterType = std::shared_ptr<CasterBaseType>;
        using DerivedContainer = std::vector<StoredCasterType>;
        using ByTypeInfoContainer = FlatMap< std::type_index, DerivedContainer >;
        using ByTypeNameContainer = FlatMap< StringView, DerivedContainer >;

        ByTypeInfoContainer CastersByTypeInfo;
        ByTypeNameContainer CastersByTypeName;

        PolymorphicCasterHolder() = default;
        PolymorphicCasterHolder(const PolymorphicCasterHolder& Other) = delete;
        PolymorphicCasterHolder(PolymorphicCasterHolder&& Other) = delete;
        ~PolymorphicCasterHolder() = default;

        PolymorphicCasterHolder& operator=(const PolymorphicCasterHolder& Other) = delete;
        PolymorphicCasterHolder& operator=(PolymorphicCasterHolder&& Other) = delete;

        template<class Base, class Derived>
        void RegisterCaster(std::shared_ptr<PolymorphicCasterImpl<Base,Derived>> NewCaster)
        {
            this->CastersByTypeInfo[ std::type_index( typeid(Base) ) ].push_back(NewCaster);
            this->CastersByTypeName[ GetRegisteredName<Base>() ].push_back(NewCaster);
        }

        StoredCasterType GetCaster(const std::type_info& BaseInfo, const std::type_info& DerivedInfo) const
        {
            ByTypeInfoContainer::const_iterator FoundBaseIt = this->CastersByTypeInfo.find(std::type_index(BaseInfo));
            if( FoundBaseIt != this->CastersByTypeInfo.end() ) {
                const DerivedContainer& FoundBase = (*FoundBaseIt).second;
                DerivedContainer::const_iterator FoundDerivedIt = std::find_if(FoundBase.begin(),FoundBase.end(),
                    [&](StoredCasterType Caster) -> Boole {
                        return Caster->CanUpcast(BaseInfo,DerivedInfo);
                    }
                );
                if( FoundDerivedIt != FoundBase.end() ) {
                    return (*FoundDerivedIt);
                }
            }
            return nullptr;
        }

        StoredCasterType GetCaster(const StringView BaseName, const StringView DerivedName) const
        {
            ByTypeNameContainer::const_iterator FoundBaseIt = this->CastersByTypeName.find(BaseName);
            if( FoundBaseIt != this->CastersByTypeName.end() ) {
                const DerivedContainer& FoundBase = (*FoundBaseIt).second;
                DerivedContainer::const_iterator FoundDerivedIt = std::find_if(FoundBase.begin(),FoundBase.end(),
                    [&](StoredCasterType Caster) -> Boole {
                        return Caster->CanUpcast(BaseName,DerivedName);
                    }
                );
                if( FoundDerivedIt != FoundBase.end() ) {
                    return (*FoundDerivedIt);
                }
            }
            return nullptr;
        }

        template<class Base, class Derived>
        StoredCasterType GetCaster() const
        {
            return this->GetCaster( typeid(Base), typeid(Derived) );
        }
    };//PolymorphicCasterHolder

SAVE_WARNING_STATE
// Yes clang, we know.  It's a singleton pattern.
SUPPRESS_CLANG_WARNING("-Wexit-time-destructors")

    PolymorphicCasterHolder& GetPolymorphicCasterHolder();
    PolymorphicCasterHolder& GetPolymorphicCasterHolder()
    {
        static PolymorphicCasterHolder Store;
        return Store;
    }

RESTORE_WARNING_STATE

    template<class Base, class Derived>
    class PolymorphicCasterImpl final : public PolymorphicCaster
    {
    public:
        static_assert( std::is_constructible_v<Derived,const Int32,ObjectWalker&> ||
                       std::is_default_constructible_v<Derived>,
                       "Derived type must be default constructable or have a constructor that accepts "
                       "an Int and ObjectWalker." );
    public:
        static_assert( std::is_base_of_v<Base,Derived>, "\"Base\" is not a base class of Derived" );

        ~PolymorphicCasterImpl() = default;
        StringView GetBaseTypeName() const
        {
            return GetRegisteredName<Base>();
        }
        StringView GetDerivedTypeName() const
        {
            return GetRegisteredName<Derived>();
        }
        Boole CanUpcast(const std::type_info& BaseType, const std::type_info& DerivedType) const
        {
            return ( BaseType == typeid(Base) && DerivedType == typeid(Derived) );
        }
        Boole CanUpcast(const StringView BaseType, const StringView DerivedType) const
        {
            return ( BaseType == GetBaseTypeName() && DerivedType == GetDerivedTypeName() );
        }
        void Serialize(const StringView Name,
                       void* ToSerialize,
                       const MemberTags Tags,
                       const Int32 Version,
                       ObjectWalker& Walker)
        {
            Derived* Casted = static_cast<Derived*>( static_cast<Base*>(ToSerialize) );
            Mezzanine::Serialize(Name,std::forward<Derived*>(Casted),Tags,Version,Walker);
        }
        void Deserialize(const StringView Name,
                         void*& ToDeserialize,
                         const MemberTags Tags,
                         const Int32 Version,
                         ObjectWalker& Walker)
        {
            Derived* UpCasted = nullptr;
            if( ToDeserialize == nullptr ) {
                if constexpr( std::is_constructible_v<Derived,decltype(Version),decltype(Walker)> ) {
                    UpCasted = new Derived(Walker);
                }else if constexpr( std::is_default_constructible_v<Derived> ) {
                    UpCasted = new Derived();
                }
                ToDeserialize = UpCasted;
            }else{
                UpCasted = static_cast<Derived*>( static_cast<Base*>(ToDeserialize) );
            }
            Mezzanine::Deserialize(Name,std::forward<Derived*>(UpCasted),Tags,Version,Walker);
        }
    };//PolymorphicCasterImpl
#else
    template<class Derived>
    class PolymorphicCaster
    {
    public:
        static constexpr const std::type_info& GetType()
        {
            return typeid(Derived);
        }
        static constexpr std::type_index GetTypeIndex()
        {
            return std::type_index( GetType() );
        }
        template< class Base, typename = std::enable_if_t< std::is_base_of<Base,Derived> > >
        static constexpr Derived* Upcast(Base* ToUpcast)
        {
            return static_cast<Derived*>(ToUpcast);
        }
    };//PolymorphicCaster

    template<class Base>

#endif
    ///////////////////////////////////////////////////////////////////////////////
    // ObjectLink (for handling non-owned pointers between objects)

    struct MEZZ_LIB ObjectLink
    {
        std::vector< std::reference_wrapper<void*> > UnresolvedLinks;
        StringView TypeName;
        uintptr_t InstanceID;
        void* Instance = nullptr;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // ObjectJoin (for handling shared ownership pointers between objects)

    struct MEZZ_LIB ObjectJoin
    {
        std::vector< std::reference_wrapper< std::shared_ptr<void> > > UnresolvedJoins;
        StringView TypeName;
        uintptr_t InstanceID;
        std::shared_ptr<void> Instance = nullptr;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // Pointer ID Generator

    uintptr_t GetIDOfPtr(void* Ptr);
    uintptr_t GetIDOfPtr(void* Ptr)
    {
        return reinterpret_cast<uintptr_t>(Ptr);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Tracker Predicate

    template<typename TrackerMeta>
    struct TrackerPredicate
    {
        using is_transparent = void;
        Boole operator()(const TrackerMeta& First, const TrackerMeta& Second)
            { return First.InstanceID < Second.InstanceID; }
        Boole operator()(const TrackerMeta& Link, const uintptr_t ID)
            { return Link.InstanceID < ID; }
        Boole operator()(const uintptr_t ID, const TrackerMeta& Link)
            { return ID < Link.InstanceID; }
    };

    ///////////////////////////////////////////////////////////////////////////////
    // PointerTracker

    class MEZZ_LIB SerializerPointerTracker
    {
    public:
        using RawPtrContainer = SortedVector<void*>;
        using RawPtrIterator = RawPtrContainer::const_iterator;
        using SharedPtrContainer = SortedVector<std::shared_ptr<void>>;
        using SharedPtrIterator = SharedPtrContainer::const_iterator;
    protected:
        RawPtrContainer RawPtrs;
        SharedPtrContainer SharedPtrs;
    public:
        SerializerPointerTracker() = default;
        SerializerPointerTracker(const SerializerPointerTracker& Other) = delete;
        SerializerPointerTracker(SerializerPointerTracker&& Other) = delete;
        ~SerializerPointerTracker() = default;

        SerializerPointerTracker& operator=(const SerializerPointerTracker& Other) = delete;
        SerializerPointerTracker& operator=(SerializerPointerTracker&& Other) = delete;

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        //Serialization
        // Detect if the object pointed to has already been serialized.
        //  Error when a second owner is detected.
        //  Signal for the system to create a link to the original otherwise.

        void TrackPointer(void* ToTrack, const Boole IsOwner)
        {
            if( IsOwner ) {
                RawPtrIterator FoundIt = this->RawPtrs.find(ToTrack);
                if( FoundIt == this->RawPtrs.end() ) {
                    this->RawPtrs.add(ToTrack);
                }else{
                    throw std::runtime_error("Detected multiple owners to pointer.");
                }
            }
        }
        void TrackPointer(std::shared_ptr<void> ToTrack, const Boole IsOwner)
        {
            if( IsOwner ) {
                SharedPtrIterator FoundIt = this->SharedPtrs.find(ToTrack);
                if( FoundIt == this->SharedPtrs.end() ) {
                    this->SharedPtrs.add(ToTrack);
                }else{
                    throw std::runtime_error("Detected multiple owners to pointer.");
                }
            }
        }

    };//SerializerPointerTracker

    class MEZZ_LIB DeserializerPointerTracker
    {
    public:
        using LinkContainer = SortedVector<ObjectLink,TrackerPredicate<ObjectLink>>;
        using LinkIterator = LinkContainer::const_iterator;
        using JoinContainer = SortedVector<ObjectJoin,TrackerPredicate<ObjectJoin>>;
        using JoinIterator = JoinContainer::const_iterator;
    protected:
        LinkContainer TrackedLinks;
        JoinContainer TrackedJoins;

        template<typename PtrType>
        ObjectLink& GetOrCreateLink(uintptr_t PtrID)
        {
            LinkIterator TrackIt = TrackedLinks.find(PtrID);
            if( TrackIt == TrackedLinks.end() ) {
                ObjectLink NewLink;
                NewLink.TypeName = GetRegisteredName<std::remove_pointer_t<PtrType>>();
                NewLink.InstanceID = PtrID;
                NewLink.Instance = nullptr;
                return *(TrackedLinks.add(NewLink));
            }
            return *TrackIt;
        }
        template<typename PtrType>
        ObjectLink& GetOrCreateJoin(uintptr_t PtrID)
        {
            JoinIterator TrackIt = TrackedJoins.find(PtrID);
            if( TrackIt == TrackedJoins.end() ) {
                ObjectJoin NewJoin;
                NewJoin.TypeName = GetRegisteredName<std::remove_pointer_t<PtrType>>();
                NewJoin.InstanceID = PtrID;
                NewJoin.Instance = nullptr;
                return *(TrackedJoins.add(NewJoin));
            }
            return *TrackIt;
        }
    public:
        DeserializerPointerTracker() = default;
        DeserializerPointerTracker(const DeserializerPointerTracker& Other) = delete;
        DeserializerPointerTracker(DeserializerPointerTracker&& Other) = delete;
        ~DeserializerPointerTracker() = default;

        DeserializerPointerTracker& operator=(const DeserializerPointerTracker& Other) = delete;
        DeserializerPointerTracker& operator=(DeserializerPointerTracker&& Other) = delete;

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        //Deserialization
        // Detect if the authoritive (non-link) copy has been deserialized/created.
        //  Queue a link resolution if it hasn't.
        //  Assign the value of the authoritive copy if it has.
        //   Additionally, resolve any pending links if the owned copy exists.

        template<typename PtrType>
        uintptr_t TrackPointer(PtrType& ToTrack, const Boole IsOwner)
        {
            static_assert(std::is_pointer_v<PtrType>,"Provided type is NOT a pointer.");

            PtrType NoRefPtr = ToTrack;
            uintptr_t PtrID = GetIDOfPtr(NoRefPtr);
            ObjectLink& FoundLink = this->GetOrCreateLink<PtrType>(PtrID);
            if( IsOwner ) {
                if( FoundLink.Instance ) {
                    throw std::runtime_error("Detected multiple owners to pointer.");
                }
                FoundLink.Instance = NoRefPtr;
                if( !FoundLink.UnresolvedLinks.empty() ) {
                    for( void*& Pending : FoundLink.UnresolvedLinks )
                        { Pending = NoRefPtr; }
                }
            }else{
                if( FoundLink.Instance ) {
                    ToTrack = FoundLink.Instance;
                }else{
                    FoundLink.UnresolvedLinks.push_back( std::ref(ToTrack) );
                }
            }
            return PtrID;
        }
        template<typename PtrType>
        Boole TrackPointer(std::shared_ptr<PtrType>& ToTrack, const Boole IsOwner)
        {
            static_assert(std::is_pointer_v<PtrType>,"Provided type is NOT a pointer.");

            uintptr_t PtrID = GetIDOfPtr(ToTrack.get());
            ObjectJoin& FoundJoin = this->GetOrCreateJoin<PtrType>(PtrID);
            if( IsOwner ) {
                if( FoundJoin.Instance ) {
                    throw std::runtime_error("Detected multiple owners to pointer.");
                }
                FoundJoin.Instance = ToTrack;
                if( !FoundJoin.UnresolvedJoins.empty() ) {
                    for( std::shared_ptr<void>& Pending : FoundJoin.UnresolvedJoins )
                        { Pending = ToTrack; }
                }
            }else{
                if( FoundJoin.Instance ) {
                    ToTrack = FoundJoin.Instance;
                }else{
                    FoundJoin.UnresolvedJoins.push_back( std::ref(ToTrack) );
                }
            }
            return PtrID;
        }
    };//DeserializerPointerTracker

    class MEZZ_LIB SerializerTrackerHolder
    {
    protected:
        std::vector<SerializerPointerTracker*> SerializerTrackers;

        void RemoveAllTrackers()
        {
            for( SerializerPointerTracker* Ptr : this->SerializerTrackers )
                { delete Ptr; }
            this->SerializerTrackers.clear();
        }
    public:
        SerializerTrackerHolder() = default;
        SerializerTrackerHolder(const SerializerTrackerHolder& Other) = delete;
        SerializerTrackerHolder(SerializerTrackerHolder&& Other) = delete;

        SerializerTrackerHolder& operator=(const SerializerTrackerHolder& Other) = delete;
        SerializerTrackerHolder& operator=(SerializerTrackerHolder&& Other) = delete;

        ~SerializerTrackerHolder()
            { this->RemoveAllTrackers(); }
    };//SerializerTrackerHolder

    class MEZZ_LIB DeserializerTrackerHolder
    {
    protected:
        std::vector<DeserializerPointerTracker*> DeserializerTrackers;

        void RemoveAllTrackers()
        {
            for( DeserializerPointerTracker* Ptr : this->DeserializerTrackers )
                { delete Ptr; }
            this->DeserializerTrackers.clear();
        }
    public:
        DeserializerTrackerHolder() = default;
        DeserializerTrackerHolder(const DeserializerTrackerHolder& Other) = delete;
        DeserializerTrackerHolder(DeserializerTrackerHolder&& Other) = delete;

        DeserializerTrackerHolder& operator=(const DeserializerTrackerHolder& Other) = delete;
        DeserializerTrackerHolder& operator=(DeserializerTrackerHolder&& Other) = delete;

        ~DeserializerTrackerHolder()
            { this->RemoveAllTrackers(); }
    };//DeserializerTrackerHolder

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization Helpers

    namespace Impl
    {
        /// @brief
        /// @tparam SerializeType
        /// @remarks This function will not create a sub-node in the serialization tree and instead will
        /// append directly to the current node pointed to by Walker.
        /// @param ToSerialize
        /// @param Version
        /// @param Walker
        template<class SerializeType>
        void SerializeClassValidation(const SerializeType& ToSerialize,
                                      const Int32 Version,
                                      Serialization::ObjectWalker& Walker)
        {
            (void)ToSerialize;
            using DecayedType = std::decay_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                //ScopedSerializationNode Node("Validation",Walker);
                //if( Node ) {
                    Walker.Attribute("TypeName",GetRegisteredName<DecayedType>());
                    Walker.Attribute("Version",Version);
                //}
            }
        }
        /// @brief
        /// @tparam SerializeType
        /// @remarks This function will not create a sub-node in the serialization tree and instead will
        /// append directly to the current node pointed to by Walker.
        /// @param Name
        /// @param ToSerialize
        /// @param Tags
        /// @param Walker
        template<class SerializeType>
        void SerializeSimpleMember(const StringView Name,
                                   const SerializeType& ToSerialize,
                                   const MemberTags Tags,
                                   Serialization::ObjectWalker& Walker)
        {
            Walker.Attribute(Name,Tags,ToSerialize);
        }
        /// @brief
        /// @tparam SerializeType
        /// @remarks This function will not create a sub-node in the serialization tree and instead will
        /// append directly to the current node pointed to by Walker.
        /// @param ToSerialize
        /// @param Walker
        template<class SerializeType>
        void SerializeAllMembers(const SerializeType& ToSerialize,
                                 Serialization::ObjectWalker& Walker)
        {
            using DecayedType = std::decay_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                DoForAllMembers<DecayedType>([&](const auto& Member) {
                    constexpr MemberTags Tags = std::remove_reference_t<decltype(Member)>::GetTags();
                    if( !IsIgnorable( Tags ) ) {
                        Mezzanine::Serialize(Member.GetName(),Member.GetValue(ToSerialize),Tags,LatestVersion,Walker);
                    }
                });
            }
        }
        /// @brief
        /// @tparam SerializeType
        /// @remarks This function will not create a sub-node in the serialization tree and instead will
        /// append directly to the current node pointed to by Walker. If you want the serialized data to
        /// be appended to it's own node, it is expected that such a node will be created in the calling
        /// function before calling this.
        /// @param ToSerialize
        /// @param Walker
        template<class SerializeType,
                 typename = std::enable_if_t< IsAssociativeContainer<SerializeType>() >,
                 typename = void>
        void SerializeContainer(const SerializeType& ToSerialize,
                                Serialization::ObjectWalker& Walker)
        {
            using DecayedType = std::decay_t<SerializeType>;
            Whole Count = 0;
            Walker.Attribute("ElementCount",ToSerialize.size());
            Walker.Attribute("KeyType",GetRegisteredName<DecayedType::key_type>());
            Walker.Attribute("ValueType",GetRegisteredName<DecayedType::mapped_type>());
            for( auto& CurrElement : ToSerialize )
            {
                StringStream Namer;
                Namer << "Element" << Count;
                ScopedSerializationNode ElementNode(Namer.str(),Walker);
                if( ElementNode ) {
                    Mezzanine::Serialize("Key",CurrElement.first,LatestVersion,Walker);
                    Mezzanine::Serialize("Value",CurrElement.second,LatestVersion,Walker);
                    ++Count;
                }
            }
        }
        /// @brief
        /// @tparam SerializeType
        /// @remarks This function will not create a sub-node in the serialization tree and instead will
        /// append directly to the current node pointed to by Walker. If you want the serialized data to
        /// be appended to it's own node, it is expected that such a node will be created in the calling
        /// function before calling this.
        /// @param ToSerialize
        /// @param Walker
        template<class SerializeType,
                 typename = std::enable_if_t< IsNonAssociativeContainer<SerializeType>() > >
        void SerializeContainer(const SerializeType& ToSerialize,
                                Serialization::ObjectWalker& Walker)
        {
            using DecayedType = std::decay_t<SerializeType>;
            Whole Count = 0;
            Walker.Attribute("ElementCount",ToSerialize.size());
            Walker.Attribute("ElementType",GetRegisteredName<DecayedType::value_type>());
            for( auto& CurrElement : ToSerialize )
            {
                StringStream Namer;
                Namer << "Element" << Count;
                Mezzanine::Serialize(Namer.str(),CurrElement,LatestVersion,Walker);
                ++Count;
            }
        }
        /// @brief
        /// @tparam SerializeType
        /// @remarks
        /// @param Name
        /// @param ToSerialize
        /// @param Tags
        /// @param Walker
        template<class SerializeType,
                 typename = std::enable_if_t< IsAssociativeContainer<SerializeType>() >,
                 typename = void>
        void SerializeContainerWithNode(const StringView Name,
                                        const SerializeType& ToSerialize,
                                        const MemberTags Tags,
                                        Serialization::ObjectWalker& Walker)
        {
            ScopedSerializationNode ContainerNode(Name,Tags,Walker);
            if( ContainerNode ) {
                Serialization::Impl::SerializeContainer(Name,ToSerialize,Tags,Walker);
            }
        }
        /// @brief
        /// @tparam SerializeType
        /// @param Name
        /// @param ToSerialize
        /// @param Tags
        /// @param Walker
        template<class SerializeType,
                 typename = std::enable_if_t< IsNonAssociativeContainer<SerializeType>() > >
        void SerializeContainerWithNode(const StringView Name,
                                        const SerializeType& ToSerialize,
                                        const MemberTags Tags,
                                        Serialization::ObjectWalker& Walker)
        {
            ScopedSerializationNode Node(Name,Tags,Walker);
            if( Node ) {
                Serialization::Impl::SerializeContainer(Name,ToSerialize,Tags,Walker);
            }
        }
        /// @brief
        /// @tparam SerializeType
        /// @remarks This function will not create a sub-node in the serialization tree and instead will
        /// append directly to the current node pointed to by Walker. If you want the serialized data to
        /// be appended to it's own node, it is expected that such a node will be created in the calling
        /// function before calling this.
        /// @param ToSerialize
        /// @param Version
        /// @param Walker
        template<class SerializeType>
        void SerializeGenericClass(const SerializeType& ToSerialize,
                                   const Int32 Version,
                                   Serialization::ObjectWalker& Walker)
        {
            using DecayedType = std::decay_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                Serialization::Impl::SerializeClassValidation(ToSerialize,Version,Walker);
                Serialization::Impl::SerializeAllMembers(ToSerialize,Walker);
            }
        }
        /// @brief
        /// @tparam SerializeType
        /// @param Name
        /// @param ToSerialize
        /// @param Tags
        /// @param Version
        /// @param Walker
        template<class SerializeType>
        void SerializeGenericClassWithNode(const StringView Name,
                                           const SerializeType& ToSerialize,
                                           const MemberTags Tags,
                                           const Int32 Version,
                                           Serialization::ObjectWalker& Walker)
        {
            using DecayedType = std::decay_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                ScopedSerializationNode Node(Name,Tags,Walker);
                if( Node ) {
                    Serialization::Impl::SerializeGenericClass(ToSerialize,Version,Walker);
                }
            }
        }
        /// @brief
        /// @tparam SerializeType
        /// @remarks
        /// @param Name
        /// @param ToSerialize
        /// @param Tags
        /// @param Version
        /// @param Walker
        template<class SerializeType>
        void SerializeOwnedPointer(const StringView Name,
                                   const SerializeType ToSerialize,
                                   const MemberTags Tags,
                                   const Int32 Version,
                                   Serialization::ObjectWalker& Walker)
        {
            (void)Version;
            static_assert( std::is_pointer_v<SerializeType> , "SerializeType is not a pointer." );
            using DecayedType = std::remove_pointer_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                ScopedSerializationNode Node(Name,Tags,Walker);
                if( Node ) {
                    Walker.Attribute("IsOwned",true);
                    if constexpr( std::is_arithmetic_v<DecayedType> ) { // Basic Number Types
                        Serialization::Impl::SerializeSimpleMember(Name,*ToSerialize,Tags,Walker);
                    }else if constexpr( StringTools::is_string<DecayedType>::value ) { // Strings
                        Serialization::Impl::SerializeSimpleMember(Name,*ToSerialize,Tags,Walker);
                    }else if constexpr( Mezzanine::is_container<DecayedType>::value ) { // Generic Containers
                        Serialization::Impl::SerializeContainer(*ToSerialize,Walker);
                    }else{ // Generic Class
                        Serialization::Impl::SerializeGenericClass(*ToSerialize,1,Walker);
                    }
                }
            }
        }
        /// @brief
        /// @tparam SerializeType
        /// @remarks
        /// @param Name
        /// @param ToSerialize
        /// @param Tags
        /// @param Walker
        template<class SerializeType>
        void SerializeNonOwnedPointer(const StringView Name,
                                      const SerializeType ToSerialize,
                                      const MemberTags Tags,
                                      Serialization::ObjectWalker& Walker)
        {
            static_assert( std::is_pointer_v<SerializeType> , "SerializeType is not a pointer." );
            using DecayedType = std::remove_pointer_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                ScopedSerializationNode Node(Name,Tags,Walker);
                if( Node ) {
                    Walker.Attribute("IsOwned",false);
                    Walker.Attribute("TypeName",GetRegisteredName<DecayedType>());
                    Walker.Attribute("InstanceID",reinterpret_cast<uintptr_t>(ToSerialize));
                }
            }
        }

        template<class SerializeType>
        void SerializePointer(const StringView Name,
                              const SerializeType ToSerialize,
                              const MemberTags Tags,
                              const Int32 Version,
                              Serialization::ObjectWalker& Walker)
        {
            static_assert( std::is_pointer_v<SerializeType> , "SerializeType is not a pointer." );
            if( Serialization::IsNotOwned(Tags) ) {
                Serialization::Impl::SerializeNonOwnedPointer(Name,ToSerialize,Tags,Walker);
            }else{
                Serialization::Impl::SerializeOwnedPointer(Name,ToSerialize,Tags,Version,Walker);
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Deserialize Helpers

        template<class SerializeType>
        void DeserializeClassValidation(const SerializeType& ToDeserialize,
                                        const Int32 Version,
                                        Serialization::ObjectWalker& Walker)
        {
            (void)ToDeserialize;
            using DecayedType = std::decay_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                //ScopedSerializationNode Node("Validation",Walker);
                //if( Node ) {
                    std::optional<String> TypeNameAttrib = Walker.Attribute<String>("TypeName");
                    if( !( TypeNameAttrib && TypeNameAttrib.value() == GetRegisteredName<DecayedType>() ) ) {
                        throw std::runtime_error("Invalid TypeName when Deserializing.");
                    }
                    std::optional<int> VersionAttrib = Walker.Attribute<int>("Version");
                    if( !( VersionAttrib && VersionAttrib.value() == Version ) ) {
                        throw std::runtime_error("Invalid Version when Deserializing.");
                    }
                //}
            }
        }

        template<class SerializeType>
        void DeserializeSimpleMember(const StringView Name,
                                     SerializeType& ToDeserialize,
                                     Serialization::ObjectWalker& Walker)
        {
            using DecayedType = std::decay_t<SerializeType>;
            std::optional<DecayedType> SimpleAttrib = Walker.Attribute<DecayedType>(Name);
            if( SimpleAttrib ) {
                ToDeserialize = SimpleAttrib.value();
            }
        }

        template<class SerializeType>
        void DeserializeAllMembers(const SerializeType& ToDeserialize,
                                   Serialization::ObjectWalker& Walker)
        {
            using DecayedType = std::decay_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                DoForAllMembers<DecayedType>([&](const auto& Member) {
                    constexpr MemberTags Tags = std::remove_reference_t<decltype(Member)>::GetTags();
                    if( !IsIgnorable( Tags ) ) {
                        Mezzanine::Deserialize(Member.GetName(), )

                        Mezzanine::Serialize(Member.GetName(),Member.GetValue(ToSerialize),Tags,LatestVersion,Walker);
                    }
                });
            }
        }

        template<class SerializeType>
        void DeserializeAllMembers(const StringView Name,
                                   const SerializeType& ToDeserialize,
                                   Serialization::ObjectWalker& Walker)
        {
            (void)Name;
            (void)ToDeserialize;
            (void)Walker;
        }

        template<class SerializeType,
                 typename = std::enable_if_t< IsAssociativeContainer<SerializeType>() >,
                 typename = void>
        void DeserializeContainer(const StringView Name,
                                  const SerializeType& ToDeserialize,
                                  Serialization::ObjectWalker& Walker)
        {
            using DecayedType = std::decay_t<SerializeType>;
            ScopedDeserializationNode Node(Name);
            if( Node && Walker.HasChildren() ) {
                if constexpr( ContainerDetect::HasReserve<DecayedType>() ) {
                    std::optional<size_t> CountAttrib = Walker.Attribute<size_t>("ElementCount");
                    if( CountAttrib ) {
                        ToDeserialize.reserve( CountAttrib.value() );
                    }
                }
                std::optional<String> KeyAttrib = Walker.Attribute<String>("KeyType");
                if( KeyAttrib && KeyAttrib.value() != GetRegisteredName<DecayedType::key_type>() ) {
                    throw std::runtime_error("TypeName mismatch when deserializing container keys.");
                }
                std::optional<String> ValueAttrib = Walker.Attribute<String>("ValueType");
                if( ValueAttrib && ValueAttrib.value() != GetRegisteredName<DecayedType::mapped_type>() ) {
                    throw std::runtime_error("TypeName mismatch when deserializing container values.");
                }

                size_t ElementCount = ;
                size_t ElementsVisited = 0;
                for( size_t ElementsVisited = 0 ; ElementsVisited < ElementCount ; ++ElementsVisited )
                {
                    StringStream Namer;
                    Namer << "Element" << ElementsVisited;
                    Mezzanine::Deserialize(Namer.str(),,,Walker);
                }


                {
                    ScopedDeserializationNode ChildNode(Walker);
                    while(  )
                    {

                    }
                }

                for( Walker.FirstChild() ; !Walker.IsLastChild() ; Walker.Next() )
                {

                }

                Walker.FirstChild();
                do{

                }while( !Walker.IsLastChild() )
                Walker.Parent();
            }

            for( auto& CurrElement : ToSerialize )
            {
                StringStream Namer;
                Namer << "Element" << Count;
                ScopedSerializationNode ElementNode(Namer.str(),Walker);
                if( ElementNode ) {
                    Mezzanine::Serialize("Key",CurrElement.first,LatestVersion,Walker);
                    Mezzanine::Serialize("Value",CurrElement.second,LatestVersion,Walker);
                    ++Count;
                }
            }
        }

        template<class SerializeType,
                 typename = std::enable_if_t< IsNonAssociativeContainer<SerializeType>() > >
        void DeserializeContainer(const StringView Name,
                                  const SerializeType& ToDeserialize,
                                  Serialization::ObjectWalker& Walker)
        {
            using DecayedType = std::decay_t<SerializeType>;
            if constexpr( ContainerDetect::HasReserve<DecayedType>() ) {
                std::optional<size_t> CountAttrib = Walker.Attribute<size_t>("ElementCount");
                if( CountAttrib ) {
                    ToDeserialize.reserve( CountAttrib.value() );
                }
            }
            std::optional<String> ElementAttrib = Walker.Attribute<String>("ElementType");
            if( ElementAttrib && ElementAttrib.value() != GetRegisteredName<DecayedType::value_type>() ) {
                throw std::runtime_error("TypeName mismatch when deserializing container elements.");
            }

            (void)Name;

            for( auto& CurrElement : ToSerialize )
            {
                StringStream Namer;
                Namer << "Element" << Count;
                Mezzanine::Serialize(Namer.str(),CurrElement,LatestVersion,Walker);
                ++Count;
            }
        }

        template<class SerializeType>
        void DeserializeGenericClass(const StringView Name,
                                     const SerializeType& ToDeserialize,
                                     const Int32 Version,
                                     Serialization::ObjectWalker& Walker)
        {
            using DecayedType = std::decay_t<SerializeType>;
            (void)Name;
            (void)ToDeserialize;
            (void)Version;
            (void)Walker;


            using DecayedType = std::decay_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                Serialization::Impl::SerializeClassValidation(ToSerialize,Version,Walker);
                Serialization::Impl::SerializeAllMembers(ToSerialize,Walker);
            }
        }

        template<class SerializeType>
        void DeserializeOwnedPointer(const StringView Name,
                                     const SerializeType ToDeserialize,
                                     const Int32 Version,
                                     Serialization::ObjectWalker& Walker)
        {
            (void)Name;
            (void)ToDeserialize;
            (void)Version;
            (void)Walker;
        }

        template<class SerializeType>
        void DeserializeNonOwnedPointer(const StringView Name,
                                        const SerializeType ToDeserialize,
                                        Serialization::ObjectWalker& Walker)
        {
            (void)Name;
            (void)ToDeserialize;
            (void)Walker;
        }

        template<class SerializeType>
        void DeserializePointer(const StringView Name,
                                const SerializeType ToDeserialize,
                                const MemberTags Tags,
                                const Int32 Version,
                                Serialization::ObjectWalker& Walker)
        {
            static_assert( std::is_pointer_v<SerializeType> , "SerializeType is not a pointer." );
            if( Serialization::IsNotOwned(Tags) ) {
                Serialization::Impl::DeserializeNonOwnedPointer(Name,ToDeserialize,Walker);
            }else{
                Serialization::Impl::DeserializeOwnedPointer(Name,ToDeserialize,Version,Walker);
            }
        }
    }//Impl
}//Serialization

    ///////////////////////////////////////////////////////////////////////////////
    // Caster Registration

    template<class Base, class Derived>
    void RegisterCaster()
    {
        using namespace Serialization;
        GetPolymorphicCasterHolder().RegisterCaster( std::make_shared<PolymorphicCasterImpl<Base,Derived>>() );
    }

    template<class Base, class... Deriveds>
    void RegisterCasters()
    {
        ( RegisterCaster<Base,Deriveds>() , ... );
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialize

    template< typename SerializeType,
              typename >
    void Serialize(const StringView Name,
                   const SerializeType& ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::ObjectWalker& Walker)
    {
        (void)Version;
        namespace Impl = Mezzanine::Serialization::Impl;
        using DecayedType = std::decay_t<SerializeType>;
        if constexpr( std::is_arithmetic_v<DecayedType> ) { // Basic Number Types
            Impl::SerializeSimpleMember(Name,ToSerialize,Tags,Walker);
        }else if constexpr( StringTools::is_string<DecayedType>::value ) { // Strings
            Impl::SerializeSimpleMember(Name,ToSerialize,Tags,Walker);
        }else if constexpr( Mezzanine::is_container<DecayedType>::value ) { // Generic Containers
            Impl::SerializeContainerWithNode(Name,ToSerialize,Tags,Walker);
        }else{ // Generic Class
            Impl::SerializeGenericClassWithNode(Name,ToSerialize,Tags,1,Walker);
        }
    }
    template< typename SerializeType,
              typename >
    void Serialize(const StringView Name,
                   const SerializeType ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::ObjectWalker& Walker)
    {
        static_assert( std::is_pointer_v<SerializeType> , "SerializeType is not a pointer." );
        namespace Impl = Mezzanine::Serialization::Impl;
        if constexpr( std::is_polymorphic_v<SerializeType> ) {
            const std::type_info& BaseInfo = typeid(ToSerialize);
            const std::type_info& DerivedInfo = typeid(*ToSerialize);
            if( std::type_index(BaseInfo) != std::type_index(DerivedInfo) ) {
                Serialization::PolymorphicCasterHolder& Holder = Serialization::GetPolymorphicCasterHolder();
                Serialization::PolymorphicCaster* Caster = Holder.GetCaster(BaseInfo,DerivedInfo);
                if( Caster != nullptr ) {
                    Caster->Serialize(Name,ToSerialize,Tags,Version,Walker);
                }else{
                    throw std::runtime_error("No caster found for polymorphic object.");
                }
            }else{
                Impl::SerializePointer(Name,ToSerialize,Tags,Version,Walker);
            }
        }else{
            Impl::SerializePointer(Name,ToSerialize,Tags,Version,Walker);
        }
    }
    template< typename SerializeType >
    void Serialize(const StringView Name,
                   const std::shared_ptr<SerializeType> ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::ObjectWalker& Walker)
    {
        (void)Name;
        (void)Tags;
        (void)ToSerialize;
        (void)Version;
        (void)Walker;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialize - No Tags

    template< typename SerializeType,
              typename >
    void Serialize(const StringView Name,
                   const SerializeType& ToSerialize,
                   const Int32 Version,
                   Serialization::ObjectWalker& Walker)
    {
        Mezzanine::Serialize(Name,MemberTags::None,ToSerialize,Version,Walker);
    }
    template< typename SerializeType,
              typename >
    void Serialize(const StringView Name,
                   const SerializeType ToSerialize,
                   const Int32 Version,
                   Serialization::ObjectWalker& Walker)
    {
        Mezzanine::Serialize(Name,MemberTags::None,ToSerialize,Version,Walker);
    }
    template< typename SerializeType >
    void Serialize(const StringView Name,
                   const std::shared_ptr<SerializeType> ToSerialize,
                   const Int32 Version,
                   Serialization::ObjectWalker& Walker)
    {
        Mezzanine::Serialize(Name,MemberTags::None,ToSerialize,Version,Walker);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Deserialize

    template< typename SerializeType,
              typename >
    void Deserialize(const StringView Name,
                     SerializeType& ToDeserialize,
                     const MemberTags Tags,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker)
    {
        (void)Tags;
        (void)Version;
        namespace Impl = Mezzanine::Serialization::Impl;
        using DecayedType = std::decay_t<SerializeType>;
        if constexpr( std::is_arithmetic_v<DecayedType> ) { // Basic Number Types
            Impl::DeserializeSimpleMember(Name,ToDeserialize,Walker);
        }else if constexpr( StringTools::is_string<DecayedType>::value ) { // Strings
            Impl::DeserializeSimpleMember(Name,ToDeserialize,Walker);
        }else if constexpr( Mezzanine::is_container<DecayedType>::value ) { // Generic Containers
            Impl::DeserializeContainer(Name,ToDeserialize,Walker);
        }else{ // Generic Class
            Impl::DeserializeGenericClass(Name,ToDeserialize,1,Walker);
        }
    }
    template< typename SerializeType,
              typename >
    void Deserialize(const StringView Name,
                     SerializeType ToDeserialize,
                     const MemberTags Tags,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker)
    {
        static_assert( std::is_pointer_v<SerializeType> , "DeserializeType is not a pointer." );
        namespace Impl = Mezzanine::Serialization::Impl;
        if constexpr( std::is_polymorphic_v<SerializeType> ) {
            const std::type_info& BaseInfo = typeid(ToDeserialize);
            const std::type_info& DerivedInfo = typeid(*ToDeserialize);
            if( std::type_index(BaseInfo) != std::type_index(DerivedInfo) ) {
                Serialization::PolymorphicCasterHolder& Holder = Serialization::GetPolymorphicCasterHolder();
                Serialization::PolymorphicCaster* Caster = Holder.GetCaster(BaseInfo,DerivedInfo);
                if( Caster != nullptr ) {
                    Caster->Deserialize(Name,ToDeserialize,Tags,Version,Walker);
                }else{
                    throw std::runtime_error("No caster found for polymorphic object.");
                }
            }else{
                Impl::DeserializePointer(Name,ToDeserialize,Tags,Version,Walker);
            }
        }else{
            Impl::DeserializePointer(Name,ToDeserialize,Tags,Version,Walker);
        }
    }
    template< typename SerializeType >
    void Deserialize(const StringView Name,
                     std::shared_ptr<SerializeType> ToDeserialize,
                     const MemberTags Tags,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker)
    {
        (void)Name;
        (void)ToDeserialize;
        (void)Tags;
        (void)Version;
        (void)Walker;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Deserialize - No Tags


    template< typename SerializeType,
              typename >
    void Deserialize(const StringView Name,
                     SerializeType& ToDeserialize,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker)
    {
        Deserialize(Name,ToDeserialize,MemberTags::None,Version,Walker);
    }
    template< typename SerializeType,
              typename >
    void Deserialize(const StringView Name,
                     SerializeType ToDeserialize,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker)
    {
        Deserialize(Name,ToDeserialize,MemberTags::None,Version,Walker);
    }
    template< typename SerializeType >
    void Deserialize(const StringView Name,
                     std::shared_ptr<SerializeType> ToDeserialize,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker)
    {
        Deserialize(Name,ToDeserialize,MemberTags::None,Version,Walker);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serializer

    class Serializer
    {
    protected:
        Serialization::BackendBase* Backend = nullptr;
        Serialization::ContextBase* Context = nullptr;
    public:
        Serializer(Serialization::BackendBase* Back) :
            Backend(Back)
            {  }


        Serialization::ContextBase* GetContext() const
            { return this->Context; }

        template<typename ObjectType, typename IDType>
        ObjectType* FindContextObject(const IDType ID)
        {
            if( this->Context != nullptr ) {
                void* Found = this->Context->FindContextObject(ID,std::type_index(typeid(ObjectType)));
                return reinterpret_cast<ObjectType>(Found);
            }
            return nullptr;
        }
    };//Serializer

    /// @}
}//Mezzanine //*/

// - Investigate the need to support c-string serialization.
// + Track serialized pointers to prevent loops/infinite recurrsion during serialization.
// + Investigate way to register pointer types to better check if pointers to different types in the inheritance heirarchy is pointing to the same object. (Pointer math with the RTTI pointer?)
// + Track and store objects with shared ownership.
// + Implement a way to generate/construct child objects for deserialization. ("Create" MemberTag value?)(Require default constructors for serialized types?)
// + Consider better methods of storing unique IDs of linked objects and methods of getting at them. (Maybe encourage more designs that only store the ID and fetch pointers per invocation)
// + Investigate templatizing the backend for a full template solution (instead of polymorphic backends).
// + Investigate the possibility of having a single versioned save/load function that calls the Serialize/Deserialize template specializations, allowing overloads at different levels.

#endif
