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
    #include "ContainerTools.h"
    #include "Introspection.h"
    #include "StringTools.h"

    #include <unordered_map>
#endif

SAVE_WARNING_STATE
// MSVC doesn't like default arguments being included in the forward declarations, but other platforms need them
SUPPRESS_VC_WARNING(4348)
// Clang likes to complain about the inline variables not being marked inline here
SUPPRESS_CLANG_WARNING("-Wmissing-variable-declarations")

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
    void Deserialize(SerializeType& ToDeserialize,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker);
    template<typename SerializeType, typename = std::enable_if_t<std::is_pointer_v<SerializeType>>>
    void Deserialize(SerializeType ToDeserialize,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker);
    template<typename SerializeType>
    void Deserialize(std::shared_ptr<SerializeType> ToDeserialize,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker);

namespace Serialization {
    ///////////////////////////////////////////////////////////////////////////////
    // Convenience detection traits

    /// @brief Convenience type for GetName function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using GetNameFunct_t = decltype(std::declval<Class&>().GetName());
    /// @brief Type for is_detected that tests for the existence of GetName on a class.
    /// @tparam Class The class that will be checked for the presence of a GetName function.
    template<typename Class>
    using HasGetName = std::is_detected<GetNameFunct_t,Class>;
    /// @brief Convenience inline variable for the value of a HasGetName check.
    /// @tparam Class The class that will be checked for the presence of a GetName function.
    template<typename Class>
    inline constexpr Boole HasGetName_v = HasGetName<Class>::value;

    /// @brief Convenience type for GetID function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using GetIDFunct_t = decltype(std::declval<Class&>().GetID());
    /// @brief Type for is_detected that tests for the existence of GetID on a class.
    /// @tparam Class The class that will be checked for the presence of a GetID function.
    template<typename Class>
    using HasGetID = std::is_detected<GetIDFunct_t,Class>;
    /// @brief Convenience inline variable for the value of a HasGetID check.
    /// @tparam Class The class that will be checked for the presence of a GetID function.
    template<typename Class>
    inline constexpr Boole HasGetID_v = HasGetID<Class>::value;

    /// @brief Convenience type for GetIdentification function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using GetIdentificationFunct_t = decltype(std::declval<Class&>().GetIdentification());
    /// @brief Type for is_detected that tests for the existence of GetIdentification on a class.
    /// @tparam Class The class that will be checked for the presence of a GetIdentification function.
    template<typename Class>
    using HasGetIdentification = std::is_detected<GetIdentificationFunct_t,Class>;
    /// @brief Convenience inline variable for the value of a HasGetIdentification check.
    /// @tparam Class The class that will be checked for the presence of a GetIdentification function.
    template<typename Class>
    inline constexpr Boole HasGetIdentification_v = HasGetIdentification<Class>::value;

    ///////////////////////////////////////////////////////////////////////////////
    // Convenience type traits
/*
    /// @brief Convenience type trait that decays the checked type before passing it to std::is_class.
    /// @tparam CheckType The type to decay and check if it is a class.
    template<class CheckType>
    struct is_class_decayed :
        std::is_class< std::remove_cv_t< std::remove_reference_t<CheckType> > >
        {  };

    /// @brief Convenience inline variable for getting just the bool of the is_class_decayed check.
    /// @tparam CheckType The type to check if it is a class type after being decayed.
    template<class CheckType>
    inline constexpr Boole is_class_decayed_v = is_class_decayed<CheckType>::value;

    /// @brief A type trait that checks to see if a type is a class that isn't a String or StringView.
    /// @tparam CheckType The type to check if it is any class type other than String or StringView.
    template<class CheckType>
    struct is_generic_serializable :
        std::bool_constant< !StringTools::is_string_v<CheckType> && is_class_decayed_v<CheckType> >
        {  };

    /// @brief Convenience inline variable for getting just the bool of the is_generic_serializable check.
    /// @tparam CheckType The type to check if it is a non-string class type.
    template<class CheckType>
    inline constexpr Boole is_generic_serializable_v = is_generic_serializable<CheckType>::value;*/

    ///////////////////////////////////////////////////////////////////////////////
    // Convenience Constants

    /// @brief An integer constant to represent the value for the latest version available for serialization.
    inline constexpr Integer LatestVersion = 0;

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

    class BackendBase
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
    ContextBase* UpdateContext(const ContextBase* Current, const SpecialContext* ToUpdate)
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
    ContextBase* RevertContext(const ContextBase* Current, const SpecialContext* ToRevert)
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
            { return std::optional<Datum>(); }

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
        virtual Boole AtBegin() const = 0;
        [[nodiscard]]
        virtual Boole AtEnd() const = 0;
        [[nodiscard]]
        virtual Boole HasChildren() const = 0;
        [[nodiscard]]
        virtual Boole HasChild(const StringView Name) const = 0;

        virtual ObjectWalker& Next() = 0;
        virtual ObjectWalker& Previous() = 0;
        virtual ObjectWalker& Parent() = 0;
        virtual ObjectWalker& FirstChild() = 0;
        virtual ObjectWalker& Child(const StringView Name) = 0;

        [[nodiscard]]
        virtual Boole CreateChild(const StringView Name, const MemberTags Tags, const Boole Move) = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Attributes

        [[nodiscard]]
        virtual Boole HasAttributes() const = 0;
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
    };//ObjectWalker

    ///////////////////////////////////////////////////////////////////////////////
    // ObjectLink (for handling non-owned pointers between objects)

    struct MEZZ_LIB ObjectLink
    {
        String TypeName;
        String InstanceName;
        Boole IsConstructed = false;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // ObjectJoin (for handling shared ownership pointers between objects)

    struct MEZZ_LIB ObjectJoin
    {
        Boole IsConstructed = false;
    };

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
        ~ScopedSerializationNode()
        {
            if( this->IsValid() ) {
                this->Node->Parent();
            }
        }

        Boole IsValid() const
            { return ( this->Node != nullptr ); }
    };

    ///////////////////////////////////////////////////////////////////////////////
    // PolymorphicCasterHolder
#if 1
    template<class Base, class Derived>
    class ObjectCaster;

    class ObjectCasterBase
    {
    public:
        virtual ~ObjectCasterBase() = default;
        virtual Boole CanUpcast(const std::type_info& BaseType, const std::type_info& DerivedType) const = 0;
        virtual void Serialize(const StringView Name,
                               const MemberTags Tags,
                               void* ToCast,
                               const Integer Version,
                               Serialization::ObjectWalker& Walker) = 0;
        virtual void Deserialize(void* ToCast,
                                 const Integer Version,
                                 Serialization::ObjectWalker& Walker) = 0;
    };//ObjectCasterBase

    struct PolymorphicCasterHolder
    {
        using CasterBaseType = ObjectCasterBase;
        using DerivedContainer = std::vector<CasterBaseType*>;
        using BaseContainer = std::unordered_map< std::type_index, DerivedContainer >;

        BaseContainer Casters;

        PolymorphicCasterHolder() = default;
        PolymorphicCasterHolder(const PolymorphicCasterHolder& Other) = delete;
        PolymorphicCasterHolder(PolymorphicCasterHolder&& Other) = delete;
        ~PolymorphicCasterHolder() = default;

        PolymorphicCasterHolder& operator=(const PolymorphicCasterHolder& Other) = delete;
        PolymorphicCasterHolder& operator=(PolymorphicCasterHolder&& Other) = delete;

        template<class Base, class Derived>
        void RegisterCaster(ObjectCaster<Base,Derived>* NewCaster)
        {
            Casters[ std::type_index( typeid(Base) ) ].push_back(NewCaster);
        }

        ObjectCasterBase* GetCaster(const std::type_info& BaseInfo, const std::type_info& DerivedInfo) const
        {
            BaseContainer::const_iterator FoundBaseIt = Casters.find( std::type_index(BaseInfo) );
            if( FoundBaseIt != Casters.end() ) {
                const DerivedContainer& FoundBase = (*FoundBaseIt).second;
                DerivedContainer::const_iterator FoundDerivedIt = std::find_if(FoundBase.begin(),FoundBase.end(),
                    [&](CasterBaseType* Caster) -> Boole {
                        return Caster->CanUpcast(BaseInfo,DerivedInfo);
                    }
                );
                if( FoundDerivedIt != FoundBase.end() ) {
                    return (*FoundDerivedIt);
                }
            }
            return nullptr;
        }

        template<class Base, class Derived>
        ObjectCasterBase* GetCaster() const
        {
            return this->GetCaster( typeid(Base), typeid(Derived) );
        }
    };//PolymorphicCasterHolder

    PolymorphicCasterHolder& GetPolymorphicCasterHolder();
    PolymorphicCasterHolder& GetPolymorphicCasterHolder()
    {
        static PolymorphicCasterHolder Store;
        return Store;
    }

    template<class Base, class Derived>
    class ObjectCaster final : public ObjectCasterBase
    {
    public:
        static_assert( std::is_base_of_v<Base,Derived>, "\"Base\" is not a base class of Derived" );

        ~ObjectCaster() = default;
        Boole CanUpcast(const std::type_info& BaseType, const std::type_info& DerivedType) const
        {
            return ( BaseType == typeid(Base) && DerivedType == typeid(Derived) );
        }
        void Serialize(const StringView Name,
                       const MemberTags Tags,
                       void* ToCast,
                       const Int32 Version,
                       ObjectWalker& Walker)
        {
            Derived* Casted = static_cast<Derived*>( static_cast<Base*>(ToCast) );
            Mezzanine::Serialize(Name,std::forward<Derived*>(Casted),Tags,Version,Walker);
        }
        void Deserialize(void* ToCast,
                         const Int32 Version,
                         ObjectWalker& Walker)
        {
            Derived* Casted = static_cast<Derived*>( static_cast<Base*>(ToCast) );
            Mezzanine::Deserialize(std::forward<Derived*>(Casted),Version,Walker);
        }
    };//ObjectCaster
#else
    template<class Derived>
    class ObjectCaster
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
    };//ObjectCaster

    template<class Base>

#endif
    ///////////////////////////////////////////////////////////////////////////////
    // PointerTracker

    class MEZZ_LIB PointerTracker
    {
    public:
        using PtrType = void*;
        using PointerContainer = std::set<PtrType>;
        using PointerIterator = PointerContainer::const_iterator;
    protected:
        PointerContainer TrackedPointers;
    public:
        PointerTracker() = default;
        PointerTracker(const PointerTracker& Other) = delete;
        PointerTracker(PointerTracker&& Other) = delete;
        ~PointerTracker() = default;

        PointerTracker& operator=(const PointerTracker& Other) = delete;
        PointerTracker& operator=(PointerTracker&& Other) = delete;

        ///////////////////////////////////////////////////////////////////////////////
        // Tracking

        Boole TrackPointer(PtrType ToTrack)
        {
            std::pair<PointerIterator,Boole> Pair = TrackedPointers.insert(ToTrack);
            return Pair.second;
        }
        Boole IsTracked(PtrType ToCheck)
        {
            return TrackedPointers.count(ToCheck) != 0;
        }
    };//PointerTracker

    ///////////////////////////////////////////////////////////////////////////////
    // Serialization Helpers

    namespace Impl
    {
        template<typename SerializeType>
        auto GetObjectID(const SerializeType& ToID)
        {
            if constexpr( HasGetName_v<SerializeType> ) {
                return ToID.GetName();
            }else if constexpr( HasGetID_v<SerializeType> ) {
                return ToID.GetID();
            }else if constexpr( HasGetIdentification_v<SerializeType> ) {
                return ToID.GetIdentification();
            }else{
                return;
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Serialize Helpers

        template<class SerializeType>
        void SerializeSimpleMember(const StringView Name,
                                   const SerializeType& ToSerialize,
                                   const MemberTags Tags,
                                   Serialization::ObjectWalker& Walker)
        {
            Walker.Attribute(Name,Tags,ToSerialize);
        }

        template<class SerializeType>
        void SerializeOwnedPointer(const StringView Name,
                                   const SerializeType ToSerialize,
                                   const MemberTags Tags,
                                   const Int32 Version,
                                   Serialization::ObjectWalker& Walker)
        {
            Mezzanine::Serialize(Name,*ToSerialize,Tags,Version,Walker);
        }

        template<class SerializeType>
        void SerializeNonOwnedPointer(const StringView Name,
                                      const SerializeType ToSerialize,
                                      const MemberTags Tags,
                                      Serialization::ObjectWalker& Walker)
        {
            using DecayedType = std::remove_pointer_t<SerializeType>;
            using IDReturn = decltype( GetObjectID( std::declval<DecayedType&>() ) );
            if constexpr( IsRegistered<DecayedType>() && !std::is_void_v<IDReturn> ) {
                ScopedSerializationNode Node(Name,Tags,&Walker);
                if( Node.IsValid() ) {
                    Walker.Attribute("IsNotOwned",MemberTags::None,true);
                    Walker.Attribute("TypeName",MemberTags::None,GetRegisteredName<DecayedType>());
                    Walker.Attribute("InstanceID",MemberTags::None,GetObjectID(*ToSerialize));
                }
            }
        }

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

        template<class SerializeType>
        void SerializeClassValidation(const SerializeType& ToSerialize,
                                      const Int32 Version,
                                      Serialization::ObjectWalker& Walker)
        {
            (void)ToSerialize;
            using DecayedType = std::decay_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                ScopedSerializationNode Node("Validation",MemberTags::None,Walker);
                if( Node.IsValid() ) {
                    Walker.Attribute("TypeName",MemberTags::None,GetRegisteredName<DecayedType>());
                    Walker.Attribute("Version",MemberTags::None,Version);
                }
            }
        }

        template<class SerializeType,
                 typename = std::enable_if_t< is_associative_container_v<SerializeType> >,
                 typename = void>
        void SerializeContainer(const StringView Name,
                                const SerializeType& ToSerialize,
                                const MemberTags Tags,
                                Serialization::ObjectWalker& Walker)
        {
            ScopedSerializationNode ContainerNode(Name,Tags,Walker);
            if( ContainerNode.IsValid() ) {
                Whole Count = 0;
                for( auto& CurrElement : ToSerialize )
                {
                    StringStream Namer;
                    Namer << "Element" << Count;
                    ScopedSerializationNode ElementNode(Namer.str(),MemberTags::None,Walker);
                    if( ElementNode.IsValid() ) {
                        Mezzanine::Serialize("Key",CurrElement.first,MemberTags::None,LatestVersion,Walker);
                        Mezzanine::Serialize("Value",CurrElement.second,MemberTags::None,LatestVersion,Walker);
                        ++Count;
                    }
                }
            }
        }

        template<class SerializeType,
                 typename = std::enable_if_t< is_non_associative_container_v<SerializeType> > >
        void SerializeContainer(const StringView Name,
                                const SerializeType& ToSerialize,
                                const MemberTags Tags,
                                Serialization::ObjectWalker& Walker)
        {
            ScopedSerializationNode Node(Name,Tags,Walker);
            if( Node.IsValid() ) {
                Whole Count = 0;
                for( auto& CurrElement : ToSerialize )
                {
                    StringStream Namer;
                    Namer << "Element" << Count;
                    Mezzanine::Serialize(Namer.str(),CurrElement,MemberTags::None,LatestVersion,Walker);
                    ++Count;
                }
            }
        }

        template<class SerializeType>
        void SerializeGenericClass(const StringView Name,
                                   const SerializeType& ToSerialize,
                                   const MemberTags Tags,
                                   const Int32 Version,
                                   Serialization::ObjectWalker& Walker)
        {
            using DecayedType = std::decay_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                ScopedSerializationNode Node(Name,Tags,Walker);
                if( Node.IsValid() ) {
                    Serialization::Impl::SerializeClassValidation(ToSerialize,Version,Walker);
                    Serialization::Impl::SerializeAllMembers(ToSerialize,Walker);
                }
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Deserialize Helpers


    }//Impl
}//Serialization

    ///////////////////////////////////////////////////////////////////////////////
    // Caster Registration

    template<class Base, class Derived>
    void RegisterCaster()
    {
        Serialization::ObjectCaster<Base,Derived>* NewCaster = new Serialization::ObjectCaster<Base,Derived>();
        Serialization::GetPolymorphicCasterHolder().RegisterCaster(NewCaster);
    }

    template<class Base, class... Deriveds>
    void RegisterCasters()
    {
        ( RegisterCaster<Base,Deriveds>() , ... );
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialize

    template< typename SerializeType,
              typename = std::enable_if_t< !std::is_pointer_v< std::decay_t<SerializeType> > > >
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
        }else if constexpr( StringTools::is_string_v<DecayedType> ) { // Strings
            Impl::SerializeSimpleMember(Name,ToSerialize,Tags,Walker);
        }else if constexpr( Mezzanine::is_container_v<DecayedType> ) { // Generic Containers
            Impl::SerializeContainer(Name,ToSerialize,Tags,Walker);
        }else{ // Generic Class
            Impl::SerializeGenericClass(Name,ToSerialize,Tags,1,Walker);
        }
    }
    template< typename SerializeType,
              typename = std::enable_if_t< std::is_pointer_v< std::decay_t<SerializeType> > > >
    void Serialize(const StringView Name,
                   const SerializeType ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::ObjectWalker& Walker)
    {
        namespace Impl = Mezzanine::Serialization::Impl;
        if constexpr( std::is_polymorphic_v<SerializeType> ) {
            const std::type_info& BaseInfo = typeid(ToSerialize);
            const std::type_info& DerivedInfo = typeid(*ToSerialize);
            if( std::type_index(BaseInfo) != std::type_index(DerivedInfo) ) {
                Serialization::PolymorphicCasterHolder& Holder = Serialization::GetPolymorphicCasterHolder();
                Serialization::ObjectCasterBase* Caster = Holder.GetCaster(BaseInfo,DerivedInfo);
                if( Caster != nullptr ) {
                    Caster->Serialize(Name,ToSerialize,Tags,Version,Walker);
                }else{
                    throw std::runtime_error("No caster found for polymorphic object.");
                }
            }else{
                if( Serialization::IsNotOwned(Tags) ) {
                    Impl::SerializeNonOwnedPointer(Name,*ToSerialize,Tags,Walker);
                }else{
                    Mezzanine::Serialize(Name,*ToSerialize,Tags,Version,Walker);
                }
            }
        }else{
            Mezzanine::Serialize(Name,*ToSerialize,Tags,Version,Walker);
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
              typename = std::enable_if_t< !std::is_pointer_v< std::decay_t<SerializeType> > > >
    void Serialize(const StringView Name,
                   const SerializeType& ToSerialize,
                   const Int32 Version,
                   Serialization::ObjectWalker& Walker)
    {
        Mezzanine::Serialize(Name,MemberTags::None,ToSerialize,Version,Walker);
    }
    template< typename SerializeType,
              typename = std::enable_if_t< std::is_pointer_v< std::decay_t<SerializeType> > > >
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
              typename = std::enable_if_t< !std::is_pointer_v< std::decay_t<SerializeType> > > >
    void Deserialize(SerializeType& ToDeserialize,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker)
    {
        (void)ToDeserialize;
        (void)Version;
        (void)Walker;
    }
    template< typename SerializeType,
              typename = std::enable_if_t< std::is_pointer_v< std::decay_t<SerializeType> > > >
    void Deserialize(SerializeType ToDeserialize,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker)
    {
        (void)ToDeserialize;
        (void)Version;
        (void)Walker;
    }
    template< typename SerializeType >
    void Deserialize(std::shared_ptr<SerializeType> ToDeserialize,
                     const Int32 Version,
                     Serialization::ObjectWalker& Walker)
    {
        (void)ToDeserialize;
        (void)Version;
        (void)Walker;
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
}//Mezzanine

RESTORE_WARNING_STATE

#endif
