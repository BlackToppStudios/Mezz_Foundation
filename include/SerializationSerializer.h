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
#endif // SWIG

namespace Mezzanine {
namespace Serialization {
    /// @addtogroup Serialization
    /// @{

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
                    if constexpr( !IsIgnorable( Tags ) ) {
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
