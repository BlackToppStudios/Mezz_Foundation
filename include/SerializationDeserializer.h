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

    /// @brief A utility class for moving about the serialization tree when deserializing.
    class MEZZ_LIB DeserializerWalker
    {
    public:
        using WalkerRef = std::reference_wrapper<Serialization::TreeWalker>;
    protected:
        DeserializerPointerTracker Tracker;
        LogStream Logger;
        WalkerRef Walker;

        Serialization::TreeWalker& GetWalker()
            { return this->Walker; }
        const Serialization::TreeWalker& GetWalker() const
            { return this->Walker; }
    public:
        DeserializerWalker(Serialization::TreeWalker& ToWrap) :
            Logger(nullptr),
            Walker(ToWrap)
            {  }
        DeserializerWalker(std::ostream& Log, Serialization::TreeWalker& ToWrap) :
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

        TreeWalker& Next()
            { return this->GetWalker().Next(); }
        TreeWalker& Previous()
            { return this->GetWalker().Previous(); }
        TreeWalker& Parent()
            { return this->GetWalker().Parent(); }
        TreeWalker& FirstChild()
            { return this->GetWalker().FirstChild(); }
        [[nodiscard]]
        Boole Child(const StringView Name)
            { return this->GetWalker().Child(Name); }

        ///////////////////////////////////////////////////////////////////////////////
        // Deserializer Specific Navigation

        template<typename FunctType>
        void ForEachChild(FunctType&& Funct)
        {
            if( !this->HasChildren() ) {
                return;
            }

            Boole GoToNext = false;
            this->FirstChild();
            do{
                Funct( *this );

                GoToNext = this->HasNextSibling();
                if( GoToNext ) {
                    this->Next();
                }
            }while( GoToNext );
            this->Parent();
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Attributes

        [[nodiscard]]
        Boole HasAttributes() const
            { return this->GetWalker().HasAttributes(); }
        [[nodiscard]]
        Boole HasAttribute(const StringView Name) const
            { return this->GetWalker().HasAttribute(Name); }

        template<typename AttributeType>
        [[nodiscard]]
        std::optional<AttributeType> Attribute(const StringView Name) const
            { return this->GetWalker().Attribute<AttributeType>(Name); }

        ///////////////////////////////////////////////////////////////////////////////
        // Deserializer Specific Attributes

        [[nodiscard]]
        Int32 GetVersion() const
        {
            Int32 Version = Serialization::LatestVersion;
            std::optional<Int32> VersionOpt = this->Attribute<Int32>("Version");
            if( VersionOpt ) {
                Version = VersionOpt.value();
            }
            return Version;
        }
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

    namespace Impl {
        template<class DeserializeType>
        void DeserializeClassValidation(const DeserializeType& ToDeserialize,
                                        Serialization::DeserializerWalker& Walker)
        {
            (void)ToDeserialize;
            using DecayedType = std::decay_t<DeserializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                //ScopedSerializationNode Node("Validation",Walker);
                //if( Node ) {
                    std::optional<String> TypeNameAttrib = Walker.Attribute<String>("TypeName");
                    if( !TypeNameAttrib || TypeNameAttrib.value() != GetRegisteredName<DecayedType>() ) {
                        throw std::runtime_error("Invalid TypeName when Deserializing.");
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

        template<class DeserializeType>
        void DeserializeAllMembers(DeserializeType& ToDeserialize,
                                   Serialization::DeserializerWalker& Walker)
        {
            using DecayedType = std::decay_t<DeserializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                DoForAllMembers<DecayedType>([&](const auto& Member) {
                    using MemberAccessType = std::remove_reference_t<decltype(Member)>;
                    using MemberType = typename MemberAccessType::SetDeducer::DecayedType;
                    constexpr MemberTags Tags = MemberAccessType::GetTags();

                    if constexpr( !IsIgnorable( Tags ) ) {
                        MemberType ChildToDeserialize{};
                        Mezzanine::Deserialize(Member.GetName(),ChildToDeserialize,Tags,Walker.GetVersion(),Walker);
                        if constexpr( std::is_move_assignable_v<MemberType> ) {
                            Member.SetValue(ToDeserialize,std::move(ChildToDeserialize));
                        }else if constexpr( std::is_copy_assignable_v<MemberType> ) {
                            Member.SetValue(ToDeserialize,ChildToDeserialize);
                        }else{
                            throw std::runtime_error("Cannot assign to member type.");
                        }
                    }
                });
            }
        }

        template<class DeserializeType,
                 typename = std::enable_if_t< IsAssociativeContainer<DeserializeType>() >,
                 typename = void>
        void DeserializeContainer(const StringView Name,
                                  DeserializeType& ToDeserialize,
                                  Serialization::DeserializerWalker& Walker)
        {
            using DecayedType = std::decay_t<DeserializeType>;
            using KeyType = typename DecayedType::key_type;
            using MappedType = typename DecayedType::mapped_type;

            ScopedDeserializationNode Node(Name);
            if( Node && Walker.HasChildren() ) {
                if constexpr( ContainerDetect::HasReserve<DecayedType>() ) {
                    std::optional<size_t> CountAttrib = Walker.Attribute<size_t>("ElementCount");
                    if( CountAttrib ) {
                        ToDeserialize.reserve( CountAttrib.value() );
                    }
                }
                std::optional<String> KeyAttrib = Walker.Attribute<String>("KeyType");
                if( KeyAttrib && KeyAttrib.value() != GetRegisteredName<KeyType>() ) {
                    throw std::runtime_error("TypeName mismatch when deserializing container keys.");
                }
                std::optional<String> ValueAttrib = Walker.Attribute<String>("MappedType");
                if( ValueAttrib && ValueAttrib.value() != GetRegisteredName<MappedType>() ) {
                    throw std::runtime_error("TypeName mismatch when deserializing container values.");
                }

                Walker.ForEachChild( [&](Serialization::DeserializerWalker& ChildWalker) {
                    KeyType NewKey{};
                    Deserialize(ChildWalker.GetName(),NewKey,ChildWalker);
                    MappedType NewMapped{};
                    Deserialize(ChildWalker.GetName(),NewMapped,ChildWalker);

                    ToDeserialize.insert( { NewKey, NewMapped } );
                } );
            }
        }

        template<class DeserializeType,
                 typename = std::enable_if_t< IsNonAssociativeContainer<DeserializeType>() > >
        void DeserializeContainer(const StringView Name,
                                  DeserializeType& ToDeserialize,
                                  Serialization::DeserializerWalker& Walker)
        {
            using DecayedType = std::decay_t<DeserializeType>;
            using ValueType = typename DecayedType::value_type;

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

            ScopedDeserializationNode Node(Name);
            if( Node && Walker.HasChildren() ) {
                Walker.ForEachChild( [&](Serialization::DeserializerWalker& ChildWalker) {
                    ValueType NewValue{};
                    Deserialize(ChildWalker.GetName(),NewValue,ChildWalker);

                    ToDeserialize.push_back( NewValue );
                } );
            }
        }

        template<class DeserializeType>
        void DeserializeGenericClass(const StringView Name,
                                     DeserializeType& ToDeserialize,
                                     Serialization::DeserializerWalker& Walker)
        {
            (void)Name;
            using DecayedType = std::decay_t<DeserializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                Serialization::Impl::DeserializeAllMembers(ToDeserialize,Walker);
            }
        }

        template<class DeserializeType>
        void DeserializeOwnedPointer(const StringView Name,
                                     const DeserializeType ToDeserialize,
                                     Serialization::DeserializerWalker& Walker)
        {
            (void)Name;
            (void)ToDeserialize;
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
                                Serialization::DeserializerWalker& Walker)
        {
            static_assert( std::is_pointer_v<DeserializeType> , "DeserializeType is not a pointer." );
            if( Serialization::IsNotOwned(Tags) ) {
                Serialization::Impl::DeserializeNonOwnedPointer(Name,ToDeserialize,Walker);
            }else{
                Serialization::Impl::DeserializeOwnedPointer(Name,ToDeserialize,Walker);
            }
        }
    }//Impl
}//Serialization
    ///////////////////////////////////////////////////////////////////////////////
    // Deserialize

    /// @brief Deserializes a non-pointer class, struct, or primitive type.
    /// @tparam DeserializeType The deduced type to be deserialized.
    /// @param Name The name associated with the ToDeserialize parameter/instance.
    /// @param ToDeserialize The non-pointer class, struct, or primitive to be deserialized.
    /// @param Tags Descriptors associated with the data being deserialized that may alter deserialization behavior.
    /// @param Walker The walker/visitor navigating the deserialization tree.
    template< typename DeserializeType,
              typename >
    void Deserialize(const StringView Name,
                     DeserializeType& ToDeserialize,
                     const MemberTags Tags,
                     Serialization::DeserializerWalker& Walker)
    {
        (void)Tags;
        namespace Impl = Mezzanine::Serialization::Impl;
        using DecayedType = std::decay_t<DeserializeType>;
        if constexpr( std::is_arithmetic_v<DecayedType> ) { // Basic Number Types
            Impl::DeserializeSimpleMember(Name,ToDeserialize,Walker);
        }else if constexpr( StringTools::is_string<DecayedType>::value ) { // Strings
            Impl::DeserializeSimpleMember(Name,ToDeserialize,Walker);
        }else if constexpr( Mezzanine::is_container<DecayedType>::value ) { // Generic Containers
            Impl::DeserializeContainer(Name,ToDeserialize,Walker);
        }else{ // Generic Class
            Impl::DeserializeGenericClass(Name,ToDeserialize,Walker);
        }
    }
    /// @brief Deserializes a class, struct, or primitive type behind a pointer.
    /// @tparam DeserializeType The deduced type to be deserialized.
    /// @param Name The name associated with the ToDeserialize parameter/instance.
    /// @param ToDeserialize The pointer to be deserialized.
    /// @param Tags Descriptors associated with the data being deserialized that may alter deserialization behavior.
    /// @param Walker The walker/visitor navigating the deserialization tree.
    template< typename DeserializeType,
              typename >
    void Deserialize(const StringView Name,
                     DeserializeType ToDeserialize,
                     const MemberTags Tags,
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
                    Caster->Deserialize(Name,ToDeserialize,Tags,Walker);
                }else{
                    throw std::runtime_error("No caster found for polymorphic object.");
                }
            }else{
                Impl::DeserializePointer(Name,ToDeserialize,Tags,Walker);
            }
        }else{
            Impl::DeserializePointer(Name,ToDeserialize,Tags,Walker);
        }
    }
    /// @brief Deserializes a class, struct, or primitive type behind a shared_ptr.
    /// @tparam DeserializeType The deduced type to be deserialized.
    /// @param Name The name associated with the ToDeserialize parameter/instance.
    /// @param ToDeserialize The shared_ptr to be deserialized.
    /// @param Tags Descriptors associated with the data being deserialized that may alter deserialization behavior.
    /// @param Walker The walker/visitor navigating the deserialization tree.
    template< typename DeserializeType >
    void Deserialize(const StringView Name,
                     std::shared_ptr<DeserializeType> ToDeserialize,
                     const MemberTags Tags,
                     Serialization::DeserializerWalker& Walker)
    {
        (void)Name;
        (void)ToDeserialize;
        (void)Tags;
        (void)Walker;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Deserialize - No Tags

    /// @brief Deserializes a non-pointer class, struct, or primitive type.
    /// @tparam DeserializeType The deduced type to be deserialized.
    /// @param Name The name associated with the ToDeserialize parameter/instance.
    /// @param ToDeserialize The non-pointer class, struct, or primitive to be deserialized.
    /// @param Walker The walker/visitor navigating the deserialization tree.
    template< typename DeserializeType,
              typename >
    void Deserialize(const StringView Name,
                     DeserializeType& ToDeserialize,
                     Serialization::DeserializerWalker& Walker)
    {
        Deserialize(Name,ToDeserialize,MemberTags::None,Walker);
    }
    /// @brief Deserializes a class, struct, or primitive type behind a pointer.
    /// @tparam DeserializeType The deduced type to be deserialized.
    /// @param Name The name associated with the ToDeserialize parameter/instance.
    /// @param ToDeserialize The pointer to be deserialized.
    /// @param Walker The walker/visitor navigating the deserialization tree.
    template< typename DeserializeType,
              typename >
    void Deserialize(const StringView Name,
                     DeserializeType ToDeserialize,
                     Serialization::DeserializerWalker& Walker)
    {
        Deserialize(Name,ToDeserialize,MemberTags::None,Walker);
    }
    /// @brief Deserializes a class, struct, or primitive type behind a shared_ptr.
    /// @tparam DeserializeType The deduced type to be deserialized.
    /// @param Name The name associated with the ToDeserialize parameter/instance.
    /// @param ToDeserialize The shared_ptr to be deserialized.
    /// @param Walker The walker/visitor navigating the deserialization tree.
    template< typename DeserializeType >
    void Deserialize(const StringView Name,
                     std::shared_ptr<DeserializeType> ToDeserialize,
                     Serialization::DeserializerWalker& Walker)
    {
        Deserialize(Name,ToDeserialize,MemberTags::None,Walker);
    }

    /// @}
}//Mezzanine

#endif // Mezz_Foundation_SerializationDeserializer_h
