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
#endif // SWIG

namespace Mezzanine {
namespace Serialization {
    /// @addtogroup Serialization
    /// @{
/*
    namespace Impl {
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

        template<class SerializeType, class Setter, class Getter>
        void DeserializeSimpleMember(const StringView Name,
                                     SerializeType& ToDeserialize,
                                     MemberAccessor<Setter,Getter>& Accessor,
                                     Serialization::ObjectWalker& Walker)
        {
            using MemberType = decltype(Accessor)::MemberType;
            std::optional<MemberType> SimpleAttrib = Walker.Attribute<MemberType>(Name);
            if( SimpleAttrib ) {
                Accessor.SetValue(ToDeserialize,SimpleAttrib.value());
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
    }//*/

    /// @}
}//Mezzanine

#endif // Mezz_Foundation_SerializationDeserializer_h
