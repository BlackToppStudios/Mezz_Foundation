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
    // Forward Declares

    namespace AttributeHelpers {
        template<typename Datum>
        void SetValue(const StringView, const Datum, TreeWalker&);
        template<typename Datum>
        std::optional<Datum> GetValue(const StringView, const TreeWalker&);
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

        virtual TreeWalker& GetWalker() = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Input and Output

        virtual void Read(std::istream& Input) = 0;

        virtual void Write(std::ostream& Output) = 0;
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
    // TreeWalker

    class MEZZ_LIB TreeWalker
    {
    protected:
        template<typename Datum>
        friend void AttributeHelpers::SetValue(const StringView, const Datum, TreeWalker&);
        template<typename Datum>
        friend std::optional<Datum> AttributeHelpers::GetValue(const StringView, const TreeWalker&);

        ///////////////////////////////////////////////////////////////////////////////
        // Attribute Access

        virtual void SetString(const StringView Name, const StringView Value) = 0;
        [[nodiscard]]
        virtual std::optional<StringView> GetString(const StringView Name) const = 0;

        virtual void SetLongDouble(const StringView Name, const long double Value) = 0;
        [[nodiscard]]
        virtual std::optional<long double> GetLongDouble(const StringView Name) const = 0;

        virtual void SetDouble(const StringView Name, const double Value) = 0;
        [[nodiscard]]
        virtual std::optional<double> GetDouble(const StringView Name) const = 0;

        virtual void SetFloat(const StringView Name, const float Value) = 0;
        [[nodiscard]]
        virtual std::optional<float> GetFloat(const StringView Name) const = 0;

        virtual void SetUInt64(const StringView Name, const UInt64 Value) = 0;
        [[nodiscard]]
        virtual std::optional<UInt64> GetUInt64(const StringView Name) const = 0;

        virtual void SetInt64(const StringView Name, const Int64 Value) = 0;
        [[nodiscard]]
        virtual std::optional<Int64> GetInt64(const StringView Name) const = 0;

        virtual void SetUInt32(const StringView Name, const UInt32 Value) = 0;
        [[nodiscard]]
        virtual std::optional<UInt32> GetUInt32(const StringView Name) const = 0;

        virtual void SetInt32(const StringView Name, const Int32 Value) = 0;
        [[nodiscard]]
        virtual std::optional<Int32> GetInt32(const StringView Name) const = 0;

        virtual void SetUInt16(const StringView Name, const UInt16 Value) = 0;
        [[nodiscard]]
        virtual std::optional<UInt16> GetUInt16(const StringView Name) const = 0;

        virtual void SetInt16(const StringView Name, const Int16 Value) = 0;
        [[nodiscard]]
        virtual std::optional<Int16> GetInt16(const StringView Name) const = 0;

        virtual void SetUInt8(const StringView Name, const UInt8 Value) = 0;
        [[nodiscard]]
        virtual std::optional<UInt8> GetUInt8(const StringView Name) const = 0;

        virtual void SetInt8(const StringView Name, const Int8 Value) = 0;
        [[nodiscard]]
        virtual std::optional<Int8> GetInt8(const StringView Name) const = 0;

        virtual void SetBool(const StringView Name, const Boole Value) = 0;
        [[nodiscard]]
        virtual std::optional<Boole> GetBool(const StringView Name) const = 0;

        template<typename SetType>
        void SetValue(const StringView Name, SetType&& ToSet);

        template<typename ReturnType>
        [[nodiscard]]
        std::optional<ReturnType> GetValue(const StringView Name) const;
    public:
        /// @brief Class destructor.
        virtual ~TreeWalker() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Name Operations

        /// @brief Sets the name of the Node being visited.
        /// @param Name The new name to be given to the internal Node.
        virtual void SetName(const StringView Name) = 0;
        /// @brief Gets the name of the Node being visited.
        /// @return Returns a StringVeiw containing the name of the internal Node being visited.
        [[nodiscard]]
        virtual StringView GetName() const = 0;

        ///////////////////////////////////////////////////////////////////////////////
        // Navigation

        [[nodiscard]]
        virtual Boole AtRoot() const = 0;
        [[nodiscard]]
        virtual Boole HasChildren() const = 0;
        [[nodiscard]]
        virtual Boole HasChild(const StringView Name) const = 0;
        [[nodiscard]]
        virtual Boole HasNextSibling() const = 0;
        [[nodiscard]]
        virtual Boole HasPreviousSibling() const = 0;

        virtual TreeWalker& Next() = 0;
        virtual TreeWalker& Previous() = 0;
        virtual TreeWalker& Parent() = 0;
        virtual TreeWalker& FirstChild() = 0;
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
        virtual Boole CreateAttribute(const StringView Name, const MemberTags Tags) = 0;

        template<typename AttributeType,
                 typename = std::enable_if_t< !std::is_same_v<std::decay_t<AttributeType>,String> > >
        void Attribute(const StringView Name, const MemberTags Tags, AttributeType&& Attrib)
        {
            if( this->CreateAttribute(Name,Tags) ) {
                this->SetValue(Name,std::forward<AttributeType>(Attrib));
            }
        }
        void Attribute(const StringView Name, const MemberTags Tags, const String& Attrib)
        {
            StringView AttribView(Attrib.data(),Attrib.size());
            this->Attribute(Name,Tags,AttribView);
        }
        template<typename AttributeType,
                 typename = std::enable_if_t< !std::is_same_v<std::decay_t<AttributeType>,String> > >
        void Attribute(const StringView Name, AttributeType&& Attrib)
        {
            this->Attribute(Name,MemberTags::None,std::forward<AttributeType>(Attrib));
        }
        void Attribute(const StringView Name, const String& Attrib)
        {
            this->Attribute(Name,MemberTags::None,Attrib);
        }
        template<typename AttributeType>
        [[nodiscard]]
        std::optional<AttributeType> Attribute(const StringView Name) const
        {
            if( this->HasAttribute(Name) ) {
                return this->GetValue<AttributeType>(Name);
            }
            return std::optional<AttributeType>();
        }
    };//TreeWalker

    namespace AttributeHelpers {
        template<typename Datum>
        void SetValue(const StringView Name, const Datum Value, TreeWalker& Walker)
            { (void)Walker;  (void)Value; }
        template<typename Datum>
        [[nodiscard]]
        std::optional<Datum> GetValue(const StringView Name, const TreeWalker& Walker)
            { (void)Walker;  return std::optional<Datum>(); }

        template<>
        void SetValue<StringView>(const StringView Name, const StringView Value, TreeWalker& Walker)
            { Walker.SetString(Name,Value); }
        template<>
        [[nodiscard]]
        std::optional<StringView> GetValue<StringView>(const StringView Name, const TreeWalker& Walker)
            { return Walker.GetString(Name); }

        template<>
        void SetValue<long double>(const StringView Name, const long double Value, TreeWalker& Walker)
            { Walker.SetLongDouble(Name,Value); }
        template<>
        [[nodiscard]]
        std::optional<long double> GetValue<long double>(const StringView Name, const TreeWalker& Walker)
            { return Walker.GetLongDouble(Name); }

        template<>
        void SetValue<double>(const StringView Name, const double Value, TreeWalker& Walker)
            { Walker.SetDouble(Name,Value); }
        template<>
        [[nodiscard]]
        std::optional<double> GetValue<double>(const StringView Name, const TreeWalker& Walker)
            { return Walker.GetDouble(Name); }

        template<>
        void SetValue<float>(const StringView Name, const float Value, TreeWalker& Walker)
            { Walker.SetFloat(Name,Value); }
        template<>
        [[nodiscard]]
        std::optional<float> GetValue<float>(const StringView Name, const TreeWalker& Walker)
            { return Walker.GetFloat(Name); }

        template<>
        void SetValue<UInt64>(const StringView Name, const UInt64 Value, TreeWalker& Walker)
            { Walker.SetUInt64(Name,Value); }
        template<>
        [[nodiscard]]
        std::optional<UInt64> GetValue<UInt64>(const StringView Name, const TreeWalker& Walker)
            { return Walker.GetUInt64(Name); }

        template<>
        void SetValue<Int64>(const StringView Name, const Int64 Value, TreeWalker& Walker)
            { Walker.SetInt64(Name,Value); }
        template<>
        [[nodiscard]]
        std::optional<Int64> GetValue<Int64>(const StringView Name, const TreeWalker& Walker)
            { return Walker.GetInt64(Name); }

        template<>
        void SetValue<UInt32>(const StringView Name, const UInt32 Value, TreeWalker& Walker)
            { Walker.SetUInt32(Name,Value); }
        template<>
        [[nodiscard]]
        std::optional<UInt32> GetValue<UInt32>(const StringView Name, const TreeWalker& Walker)
            { return Walker.GetUInt32(Name); }

        template<>
        void SetValue<Int32>(const StringView Name, const Int32 Value, TreeWalker& Walker)
            { Walker.SetInt32(Name,Value); }
        template<>
        [[nodiscard]]
        std::optional<Int32> GetValue<Int32>(const StringView Name, const TreeWalker& Walker)
            { return Walker.GetInt32(Name); }

        template<>
        void SetValue<UInt16>(const StringView Name, const UInt16 Value, TreeWalker& Walker)
            { Walker.SetUInt16(Name,Value); }
        template<>
        [[nodiscard]]
        std::optional<UInt16> GetValue<UInt16>(const StringView Name, const TreeWalker& Walker)
            { return Walker.GetUInt16(Name); }

        template<>
        void SetValue<Int16>(const StringView Name, const Int16 Value, TreeWalker& Walker)
            { Walker.SetInt16(Name,Value); }
        template<>
        [[nodiscard]]
        std::optional<Int16> GetValue<Int16>(const StringView Name, const TreeWalker& Walker)
            { return Walker.GetInt16(Name); }

        template<>
        void SetValue<UInt8>(const StringView Name, const UInt8 Value, TreeWalker& Walker)
            { Walker.SetUInt8(Name,Value); }
        template<>
        [[nodiscard]]
        std::optional<UInt8> GetValue<UInt8>(const StringView Name, const TreeWalker& Walker)
            { return Walker.GetUInt8(Name); }

        template<>
        void SetValue<Int8>(const StringView Name, const Int8 Value, TreeWalker& Walker)
            { Walker.SetInt8(Name,Value); }
        template<>
        [[nodiscard]]
        std::optional<Int8> GetValue<Int8>(const StringView Name, const TreeWalker& Walker)
            { return Walker.GetInt8(Name); }

        template<>
        void SetValue<Boole>(const StringView Name, const Boole Value, TreeWalker& Walker)
            { Walker.SetBool(Name,Value); }
        template<>
        [[nodiscard]]
        std::optional<Boole> GetValue<Boole>(const StringView Name, const TreeWalker& Walker)
            { return Walker.GetBool(Name); }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // TreeWalker Implementations

    template<typename SetType>
    void TreeWalker::SetValue(const StringView Name, SetType&& ToSet)
    {
        using DecayedType = std::decay_t<SetType>;
        AttributeHelpers::SetValue<DecayedType>(Name,std::forward<SetType>(ToSet),*this);
    }

    template<typename ReturnType>
    [[nodiscard]]
    std::optional<ReturnType> TreeWalker::GetValue(const StringView Name) const
    {
        return AttributeHelpers::GetValue<ReturnType>(Name,*this);
    }

    /// @}
}//Serialization
}//Mezzanine

#endif // Mezz_Foundation_SerializationTree_h
