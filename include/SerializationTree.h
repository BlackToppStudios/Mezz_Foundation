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
#ifndef Mezz_Foundation_SerializationTree_h
#define Mezz_Foundation_SerializationTree_h

/// @file
/// @brief This file contains a set of classes used to describe and navigate the serialization tree.

#ifndef SWIG
    #include "SerializationUtility.h"

    #include <typeinfo>
    #include <typeindex>
    #include <optional>
#endif // SWIG

namespace Mezzanine {
namespace Serialization {
    /// @addtogroup Serialization
    /// @{

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
        [[nodiscard]]
        virtual Boole HasNextSibling() const = 0;
        [[nodiscard]]
        virtual Boole HasPreviousSibling() const = 0;

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

    /// @}
}//Serialization
}//Mezzanine

#endif // Mezz_Foundation_SerializationTree_h
