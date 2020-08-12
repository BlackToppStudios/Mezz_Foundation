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
#ifndef Mezz_Foundation_SerializationDeserializer_h
#define Mezz_Foundation_SerializationDeserializer_h

/// @file
/// @brief This file contains utilities specific to deserializing data (not serializing).

#ifndef SWIG
    #include "SerializationTree.h"
    #include "SerializationPolymorphicCaster.h"
    #include "SerializationPointerTracker.h"
    #include "StringTools.h"
    #include "StreamLogging.h"
#endif // SWIG

namespace Mezzanine {
namespace Serialization {
    /// @addtogroup Serialization
    /// @{

    class MEZZ_LIB DeserializerWalker
    {
    public:
        using WalkerRef = std::reference_wrapper<Serialization::ObjectWalker>;
    protected:
        DeserializerPointerTracker Tracker;
        LogStream Logger;
        WalkerRef Walker;

        Serialization::ObjectWalker& GetWalker()
            { return this->Walker; }
        const Serialization::ObjectWalker& GetWalker() const
            { return this->Walker; }
    public:
        DeserializerWalker(Serialization::ObjectWalker& ToWrap) :
            Logger(nullptr),
            Walker(ToWrap)
            {  }
        DeserializerWalker(std::ostream& Log, Serialization::ObjectWalker& ToWrap) :
            Logger(Log.rdbuf()),
            Walker(ToWrap)
            {  }
        ~DeserializerWalker() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Logging

        [[nodiscard]]
        LogStream& GetLogger()
            { return this->Logger; }

        ///////////////////////////////////////////////////////////////////////////////
        // Pointer Tracking


        template<typename PtrType>
        uintptr_t TrackPointer(PtrType& ToTrack, const Boole IsOwner)
            { return this->Tracker.TrackPointer<PtrType>(ToTrack,IsOwner); }

        template<typename PtrType>
        uintptr_t TrackPointer(std::shared_ptr<PtrType>& ToTrack, const Boole IsOwner)
            { return this->Tracker.TrackPointer<PtrType>(ToTrack,IsOwner); }

        ///////////////////////////////////////////////////////////////////////////////
        // Name Operations

        void SetName(const StringView Name)
            { this->GetWalker().SetName(Name); }
        [[nodiscard]]
        StringView GetName() const
            { return this->GetWalker().GetName(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Navigation

        [[nodiscard]]
        Boole AtRoot() const
            { return this->GetWalker().AtRoot(); }
        [[nodiscard]]
        Boole IsFirstChild() const
            { return this->GetWalker().IsFirstChild(); }
        [[nodiscard]]
        Boole IsLastChild() const
            { return this->GetWalker().IsLastChild(); }
        [[nodiscard]]
        Boole HasChildren() const
            { return this->GetWalker().HasChildren(); }
        [[nodiscard]]
        Boole HasChild(const StringView Name) const
            { return this->GetWalker().HasChild(Name); }
        [[nodiscard]]
        Boole HasNextSibling() const
            { return this->GetWalker().HasNextSibling(); }
        [[nodiscard]]
        Boole HasPreviousSibling() const
            { return this->GetWalker().HasPreviousSibling(); }

        ObjectWalker& Next()
            { return this->GetWalker().Next(); }
        ObjectWalker& Previous()
            { return this->GetWalker().Previous(); }
        ObjectWalker& Parent()
            { return this->GetWalker().Parent(); }
        ObjectWalker& FirstChild()
            { return this->GetWalker().FirstChild(); }
        [[nodiscard]]
        Boole Child(const StringView Name)
            { return this->GetWalker().Child(Name); }

        [[nodiscard]]
        Boole CreateChild(const StringView Name, const MemberTags Tags, const Boole Move)
            { return this->GetWalker().CreateChild(Name,Tags,Move); }

        ///////////////////////////////////////////////////////////////////////////////
        // Attributes

        [[nodiscard]]
        Boole HasAttributes() const
            { return this->GetWalker().HasAttributes(); }
        [[nodiscard]]
        Boole HasAttribute(const StringView Name) const
            { return this->GetWalker().HasAttribute(Name); }
        [[nodiscard]]
        AttributeWalker& GetAttributes()
            { return this->GetWalker().GetAttributes(); }
        [[nodiscard]]
        AttributeWalker& GetAttribute(const StringView Name)
            { return this->GetWalker().GetAttribute(Name); }
        [[nodiscard]]
        Boole CreateAttribute(const StringView Name, const MemberTags Tags)
            { return this->GetWalker().CreateAttribute(Name,Tags); }

        template<typename AttributeType,
                 typename = std::enable_if_t< !std::is_same_v<std::decay_t<AttributeType>,String> > >
        void Attribute(const StringView Name, const MemberTags Tags, AttributeType&& Attrib)
            { this->GetWalker().Attribute(Name,Tags,std::forward<AttributeType>(Attrib)); }
        void Attribute(const StringView Name, const MemberTags Tags, const String& Attrib)
            { this->GetWalker().Attribute(Name,Tags,Attrib); }
        template<typename AttributeType,
                 typename = std::enable_if_t< !std::is_same_v<std::decay_t<AttributeType>,String> > >
        void Attribute(const StringView Name, AttributeType&& Attrib)
            { this->GetWalker().Attribute(Name,std::forward<AttributeType>(Attrib)); }
        void Attribute(const StringView Name, const String& Attrib)
            { this->GetWalker().Attribute(Name,Attrib); }
        template<typename AttributeType>
        [[nodiscard]]
        std::optional<AttributeType> Attribute(const StringView Name)
            { return this->GetWalker().Attribute<AttributeType>(Name); }
    };//DeserializerWalker

    /// @brief Helper class scope-based navigation of a serialization tree during deserialization.
    class MEZZ_LIB ScopedDeserializationNode
    {
    protected:
        /// @brief Pointer to the walker used to navigate the serialization tree.
        DeserializerWalker* Node = nullptr;
    public:
        /// @brief First Child constructor.
        /// @remarks This constructor will navigate to the first child of the current node.
        /// @param Walker The walker being used to navigate the serialization tree.
        ScopedDeserializationNode(DeserializerWalker& Walker)
        {
            if( Walker.HasChildren() ) {
                Walker.FirstChild();
                this->Node = &Walker;
            }
        }
        /// @brief Named Child constructor.
        /// @remarks This constructor will navigate to the named child of the current node.
        /// @param Name The name of the node to navigate to.
        /// @param Walker The walker being used to navigate the serialization tree.
        ScopedDeserializationNode(const StringView Name, DeserializerWalker& Walker)
        {
            if( Walker.Child(Name) ) {
                this->Node = &Walker;
            }
        }
        /// @brief Class destructor.
        /// @remarks This will navigate the walker to the parent node.
        ~ScopedDeserializationNode()
        {
            if( this->IsValid() ) {
                this->Node->Parent();
            }
        }

        /// @brief Verifies this scoped node is safe to use.
        /// @return Returns true if this scoped node has a valid reference to a walker and can be used.
        Boole IsValid() const
            { return ( this->Node != nullptr ); }

        /// @brief Bool operator validity check.
        /// @remarks This is a convenience operator that simply checks the validity of this node.
        explicit operator bool() const
            { return ( this->Node != nullptr ); }
    };//ScopedDeserializationNode
/*
    namespace Impl {
        template<class DeserializeType>
        void DeserializeClassValidation(const DeserializeType& ToDeserialize,
                                        const Int32 Version,
                                        Serialization::DeserializerWalker& Walker)
        {
            (void)ToDeserialize;
            using DecayedType = std::decay_t<DeserializeType>;
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

        template<class DeserializeType>
        void DeserializeSimpleMember(const StringView Name,
                                     DeserializeType& ToDeserialize,
                                     Serialization::DeserializerWalker& Walker)
        {
            using DecayedType = std::decay_t<DeserializeType>;
            std::optional<DecayedType> SimpleAttrib = Walker.Attribute<DecayedType>(Name);
            if( SimpleAttrib ) {
                ToDeserialize = SimpleAttrib.value();
            }
        }

        template<class DeserializeType, class Setter, class Getter>
        void DeserializeSimpleMember(const StringView Name,
                                     DeserializeType& ToDeserialize,
                                     MemberAccessor<Setter,Getter>& Accessor,
                                     Serialization::DeserializerWalker& Walker)
        {
            using MemberType = decltype(Accessor)::MemberType;
            std::optional<MemberType> SimpleAttrib = Walker.Attribute<MemberType>(Name);
            if( SimpleAttrib ) {
                Accessor.SetValue(ToDeserialize,SimpleAttrib.value());
            }
        }

        template<class DeserializeType>
        void DeserializeAllMembers(const DeserializeType& ToDeserialize,
                                   Serialization::DeserializerWalker& Walker)
        {
            using DecayedType = std::decay_t<DeserializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                DoForAllMembers<DecayedType>([&](const auto& Member) {
                    constexpr MemberTags Tags = std::remove_reference_t<decltype(Member)>::GetTags();
                    if constexpr( !IsIgnorable( Tags ) ) {
                        // The normal deserialization interface is only friendly to types passed by reference.
                        if constexpr( std::is_reference_v< decltype(Member)::ReturnType ) > ) {
                            Mezzanine::Deserialize(Member.GetName(),Member.GetValue(ToSerialize),Version,Walker);
                        }else if constexpr( std::is_arithmetic_v<DecayedType> ) {
                            Serialization::Impl::DeserializeSimpleMember(Name,ToDeserialize,Member,Walker);
                        }else if constexpr( StringTools::is_string<DecayedType>::value ) {
                            Serialization::Impl::DeserializeSimpleMember(Name,ToDeserialize,Member,Walker);
                        }else if constexpr( Mezzanine::is_container<DecayedType>::value ) {

                        }else{

                        }
                    }
                });
            }
        }

        template<class DeserializeType>
        void DeserializeAllMembers(const StringView Name,
                                   const DeserializeType& ToDeserialize,
                                   Serialization::DeserializerWalker& Walker)
        {
            (void)Name;
            (void)ToDeserialize;
            (void)Walker;
        }

        template<class DeserializeType,
                 typename = std::enable_if_t< IsAssociativeContainer<DeserializeType>() >,
                 typename = void>
        void DeserializeContainer(const StringView Name,
                                  const DeserializeType& ToDeserialize,
                                  Serialization::DeserializerWalker& Walker)
        {
            using DecayedType = std::decay_t<DeserializeType>;
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

                Walker.FirstChild();
                do{


                    Boole GoToNext = !Walker.IsLastChild();
                    if( GoToNext ) {
                        Walker.Next();
                    }
                }while( GoToNext )
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

        template<class DeserializeType,
                 typename = std::enable_if_t< IsNonAssociativeContainer<DeserializeType>() > >
        void DeserializeContainer(const StringView Name,
                                  const DeserializeType& ToDeserialize,
                                  Serialization::DeserializerWalker& Walker)
        {
            using DecayedType = std::decay_t<DeserializeType>;
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

        template<class DeserializeType>
        void DeserializeGenericClass(const StringView Name,
                                     const DeserializeType& ToDeserialize,
                                     const Int32 Version,
                                     Serialization::DeserializerWalker& Walker)
        {
            using DecayedType = std::decay_t<DeserializeType>;
            (void)Name;
            (void)ToDeserialize;
            (void)Version;
            (void)Walker;


            using DecayedType = std::decay_t<DeserializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                Serialization::Impl::SerializeClassValidation(ToSerialize,Version,Walker);
                Serialization::Impl::SerializeAllMembers(ToSerialize,Walker);
            }
        }

        template<class DeserializeType>
        void DeserializeOwnedPointer(const StringView Name,
                                     const DeserializeType ToDeserialize,
                                     const Int32 Version,
                                     Serialization::DeserializerWalker& Walker)
        {
            (void)Name;
            (void)ToDeserialize;
            (void)Version;
            (void)Walker;
        }

        template<class DeserializeType>
        void DeserializeNonOwnedPointer(const StringView Name,
                                        const DeserializeType ToDeserialize,
                                        Serialization::DeserializerWalker& Walker)
        {
            (void)Name;
            (void)ToDeserialize;
            (void)Walker;
        }

        template<class DeserializeType>
        void DeserializePointer(const StringView Name,
                                const DeserializeType ToDeserialize,
                                const MemberTags Tags,
                                const Int32 Version,
                                Serialization::DeserializerWalker& Walker)
        {
            static_assert( std::is_pointer_v<DeserializeType> , "DeserializeType is not a pointer." );
            if( Serialization::IsNotOwned(Tags) ) {
                Serialization::Impl::DeserializeNonOwnedPointer(Name,ToDeserialize,Walker);
            }else{
                Serialization::Impl::DeserializeOwnedPointer(Name,ToDeserialize,Version,Walker);
            }
        }
    }//Impl
}//Serialization
    ///////////////////////////////////////////////////////////////////////////////
    // Deserialize

    template< typename DeserializeType,
              typename >
    void Deserialize(const StringView Name,
                     DeserializeType& ToDeserialize,
                     const MemberTags Tags,
                     const Int32 Version,
                     Serialization::DeserializerWalker& Walker)
    {
        (void)Tags;
        (void)Version;
        namespace Impl = Mezzanine::Serialization::Impl;
        using DecayedType = std::decay_t<DeserializeType>;
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
    template< typename DeserializeType,
              typename >
    void Deserialize(const StringView Name,
                     DeserializeType ToDeserialize,
                     const MemberTags Tags,
                     const Int32 Version,
                     Serialization::DeserializerWalker& Walker)
    {
        static_assert( std::is_pointer_v<DeserializeType> , "DeDeserializeType is not a pointer." );
        namespace Impl = Mezzanine::Serialization::Impl;
        if constexpr( std::is_polymorphic_v<DeserializeType> ) {
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
    template< typename DeserializeType >
    void Deserialize(const StringView Name,
                     std::shared_ptr<DeserializeType> ToDeserialize,
                     const MemberTags Tags,
                     const Int32 Version,
                     Serialization::DeserializerWalker& Walker)
    {
        (void)Name;
        (void)ToDeserialize;
        (void)Tags;
        (void)Version;
        (void)Walker;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Deserialize - No Tags


    template< typename DeserializeType,
              typename >
    void Deserialize(const StringView Name,
                     DeserializeType& ToDeserialize,
                     const Int32 Version,
                     Serialization::DeserializerWalker& Walker)
    {
        Deserialize(Name,ToDeserialize,MemberTags::None,Version,Walker);
    }
    template< typename DeserializeType,
              typename >
    void Deserialize(const StringView Name,
                     DeserializeType ToDeserialize,
                     const Int32 Version,
                     Serialization::DeserializerWalker& Walker)
    {
        Deserialize(Name,ToDeserialize,MemberTags::None,Version,Walker);
    }
    template< typename DeserializeType >
    void Deserialize(const StringView Name,
                     std::shared_ptr<DeserializeType> ToDeserialize,
                     const Int32 Version,
                     Serialization::DeserializerWalker& Walker)
    {
        Deserialize(Name,ToDeserialize,MemberTags::None,Version,Walker);
    }//*/

    /// @}
}//Serialization
}//Mezzanine

#endif // Mezz_Foundation_SerializationDeserializer_h
