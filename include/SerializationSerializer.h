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
#ifndef Mezz_Foundation_SerializationSerializer_h
#define Mezz_Foundation_SerializationSerializer_h

/// @file
/// @brief This file contains utilities specific to serializing data (not deserializing).

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

    class MEZZ_LIB SerializerWalker
    {
    public:
        using WalkerRef = std::reference_wrapper<Serialization::ObjectWalker>;
    protected:
        SerializerPointerTracker Tracker;
        LogStream Logger;
        WalkerRef Walker;

        Serialization::ObjectWalker& GetWalker()
            { return this->Walker; }
        const Serialization::ObjectWalker& GetWalker() const
            { return this->Walker; }
    public:
        SerializerWalker(Serialization::ObjectWalker& ToWrap) :
            Logger(nullptr),
            Walker(ToWrap)
            {  }
        SerializerWalker(std::ostream& Log, Serialization::ObjectWalker& ToWrap) :
            Logger(Log.rdbuf()),
            Walker(ToWrap)
            {  }
        ~SerializerWalker() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Logging

        [[nodiscard]]
        LogStream& GetLogger()
            { return this->Logger; }

        ///////////////////////////////////////////////////////////////////////////////
        // Pointer Tracking

        void TrackPointer(void* ToTrack, const Boole IsOwner)
            { this->Tracker.TrackPointer(ToTrack,IsOwner); }

        void TrackPointer(std::shared_ptr<void> ToTrack, const Boole IsOwner)
            { this->Tracker.TrackPointer(ToTrack,IsOwner); }

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
    };//SerializerWalker

    /// @brief Helper class scope-based navigation of a serialization tree during serialization.
    class MEZZ_LIB ScopedSerializationNode
    {
    protected:
        /// @brief Pointer to the walker used to navigate the serialization tree.
        SerializerWalker* Node = nullptr;
    public:
        /// @brief Child create constructor.
        /// @remarks This constructor will attempt to create a new child with the specified name.
        /// @param Name The name of the node to be created.
        /// @param Tags Member tags used by the backend to determine custom backend behavior for the node.
        /// @param Walker The walker being used to navigate the serialization tree.
        ScopedSerializationNode(const StringView Name, const MemberTags Tags, SerializerWalker& Walker)
        {
            if( Walker.CreateChild(Name,Tags,true) ) {
                this->Node = &Walker;
            }
        }
        /// @brief Tagless child create constructor.
        /// @remarks This constructor will attempt to create a new child with the specified name.
        /// @param Name The name of the node to be created.
        /// @param Walker The walker being used to navigate the serialization tree.
        ScopedSerializationNode(const StringView Name, SerializerWalker& Walker) :
            ScopedSerializationNode(Name,MemberTags::None,Walker)
            {  }
        /// @brief Class destructor.
        /// @remarks This will navigate the walker to the parent node.
        ~ScopedSerializationNode()
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
    };//ScopedSerializationNode

    namespace Impl {
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
                                      Serialization::SerializerWalker& Walker)
        {
            std::cout << "\nEntering \"SerializeClassValidation\".\n";
            (void)ToSerialize;
            using DecayedType = std::decay_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                //ScopedSerializationNode Node("Validation",Walker);
                //if( Node ) {
                    Walker.Attribute("TypeName",GetRegisteredName<DecayedType>());
                    Walker.Attribute("Version",Version);
                //}
            }
            std::cout << "\nExiting \"SerializeClassValidation\".\n";
        }
        /// @brief Serializes a String or primitive type.
        /// @tparam SerializeType
        /// @remarks This function will not create a sub-node in the serialization tree and instead will
        /// append directly to the current node pointed to by Walker.
        /// @param Name The name of the member to serialize.
        /// @param ToSerialize
        /// @param Tags
        /// @param Walker
        template<class SerializeType>
        void SerializeSimpleMember(const StringView Name,
                                   const SerializeType& ToSerialize,
                                   const MemberTags Tags,
                                   Serialization::SerializerWalker& Walker)
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
                                 Serialization::SerializerWalker& Walker)
        {
            using DecayedType = std::decay_t<SerializeType>;
            std::cout << "\nEntering \"SerializeAllMembers\" for type \"" << GetRegisteredName<DecayedType>() << "\".\n";
            if constexpr( IsRegistered<DecayedType>() ) {
                DoForAllMembers<DecayedType>([&](const auto& Member) {
                    constexpr MemberTags Tags = std::remove_reference_t<decltype(Member)>::GetTags();
                    if constexpr( !IsIgnorable( Tags ) ) {
                        Mezzanine::Serialize(Member.GetName(),Member.GetValue(ToSerialize),Tags,LatestVersion,Walker);
                    }
                });
            }
            std::cout << "\nExiting \"SerializeAllMembers\" for type \"" << GetRegisteredName<DecayedType>() << "\".\n";
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
                                Serialization::SerializerWalker& Walker)
        {
            using DecayedType = std::decay_t<SerializeType>;
            Whole Count = 0;
            Walker.Attribute("ElementCount",ToSerialize.size());
            Walker.Attribute("KeyType",GetSerializableTypeIdentity<typename DecayedType::key_type>());
            Walker.Attribute("ValueType",GetSerializableTypeIdentity<typename DecayedType::mapped_type>());
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
                                Serialization::SerializerWalker& Walker)
        {
            using DecayedType = std::decay_t<SerializeType>;
            Whole Count = 0;
            Walker.Attribute("ElementCount",ToSerialize.size());
            Walker.Attribute("ElementType",GetSerializableTypeIdentity<typename DecayedType::value_type>());
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
        /// @param Name The name of the member to serialize.
        /// @param ToSerialize
        /// @param Tags
        /// @param Walker
        template<class SerializeType,
                 typename = std::enable_if_t< IsAssociativeContainer<SerializeType>() >,
                 typename = void>
        void SerializeContainerWithNode(const StringView Name,
                                        const SerializeType& ToSerialize,
                                        const MemberTags Tags,
                                        Serialization::SerializerWalker& Walker)
        {
            ScopedSerializationNode ContainerNode(Name,Tags,Walker);
            if( ContainerNode ) {
                Serialization::Impl::SerializeContainer(ToSerialize,Walker);
            }
        }
        /// @brief
        /// @tparam SerializeType
        /// @param Name The name of the member to serialize.
        /// @param ToSerialize
        /// @param Tags
        /// @param Walker
        template<class SerializeType,
                 typename = std::enable_if_t< IsNonAssociativeContainer<SerializeType>() > >
        void SerializeContainerWithNode(const StringView Name,
                                        const SerializeType& ToSerialize,
                                        const MemberTags Tags,
                                        Serialization::SerializerWalker& Walker)
        {
            ScopedSerializationNode Node(Name,Tags,Walker);
            if( Node ) {
                Serialization::Impl::SerializeContainer(ToSerialize,Walker);
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
                                   Serialization::SerializerWalker& Walker)
        {
            std::cout << "\nEntering \"SerializeGenericClass\".\n";
            using DecayedType = std::decay_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                Serialization::Impl::SerializeClassValidation(ToSerialize,Version,Walker);
                Serialization::Impl::SerializeAllMembers(ToSerialize,Walker);
            }
            std::cout << "\nExiting \"SerializeGenericClass\".\n";
        }
        /// @brief
        /// @tparam SerializeType
        /// @param Name The name of the member to serialize.
        /// @param ToSerialize
        /// @param Tags
        /// @param Version
        /// @param Walker
        template<class SerializeType>
        void SerializeGenericClassWithNode(const StringView Name,
                                           const SerializeType& ToSerialize,
                                           const MemberTags Tags,
                                           const Int32 Version,
                                           Serialization::SerializerWalker& Walker)
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
        /// @param Name The name of the member to serialize.
        /// @param ToSerialize
        /// @param Tags
        /// @param Version
        /// @param Walker
        template<class SerializeType>
        void SerializeOwnedPointer(const StringView Name,
                                   const SerializeType ToSerialize,
                                   const MemberTags Tags,
                                   const Int32 Version,
                                   Serialization::SerializerWalker& Walker)
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
        /// @param Name The name of the member to serialize.
        /// @param ToSerialize
        /// @param Tags
        /// @param Walker
        template<class SerializeType>
        void SerializeNonOwnedPointer(const StringView Name,
                                      const SerializeType ToSerialize,
                                      const MemberTags Tags,
                                      Serialization::SerializerWalker& Walker)
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

        /// @brief
        /// @tparam SerializeType
        /// @remarks
        /// @param Name The name of the member to serialize.
        /// @param ToSerialize
        /// @param Tags
        /// @param Version
        /// @param Walker
        template<class SerializeType>
        void SerializePointer(const StringView Name,
                              const SerializeType ToSerialize,
                              const MemberTags Tags,
                              const Int32 Version,
                              Serialization::SerializerWalker& Walker)
        {
            static_assert( std::is_pointer_v<SerializeType> , "SerializeType is not a pointer." );
            if( Serialization::IsNotOwned(Tags) ) {
                Serialization::Impl::SerializeNonOwnedPointer(Name,ToSerialize,Tags,Walker);
            }else{
                Serialization::Impl::SerializeOwnedPointer(Name,ToSerialize,Tags,Version,Walker);
            }
        }
    }//Impl
}//Serialization
    ///////////////////////////////////////////////////////////////////////////////
    // Serialize

    template< typename SerializeType,
              typename >
    void Serialize(const StringView Name,
                   const SerializeType& ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker)
    {
        (void)Version;
        namespace Impl = Mezzanine::Serialization::Impl;
        using DecayedType = std::decay_t<SerializeType>;
        std::cout << "\nSerializing type: \"" << GetRegisteredName<DecayedType>() << "\".\n";
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
                   Serialization::SerializerWalker& Walker)
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
                   Serialization::SerializerWalker& Walker)
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
                   Serialization::SerializerWalker& Walker)
    {
        Mezzanine::Serialize(Name,ToSerialize,MemberTags::None,Version,Walker);
    }
    template< typename SerializeType,
              typename >
    void Serialize(const StringView Name,
                   const SerializeType ToSerialize,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker)
    {
        Mezzanine::Serialize(Name,ToSerialize,MemberTags::None,Version,Walker);
    }
    template< typename SerializeType >
    void Serialize(const StringView Name,
                   const std::shared_ptr<SerializeType> ToSerialize,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker)
    {
        Mezzanine::Serialize(Name,ToSerialize,MemberTags::None,Version,Walker);
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

#endif // Mezz_Foundation_SerializationSerializer_h
