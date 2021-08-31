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

    /// @brief A utility class for moving about the serialization tree when serializing.
    /// @remarks This class wraps a TreeWalker and exposes serialization specific functionality from it as well
    /// as providing additional serialization specific functionality.
    class MEZZ_LIB SerializerWalker
    {
    public:
        /// @brief Convenience type for a reference to a tree walker.
        using WalkerRef = std::reference_wrapper<Serialization::TreeWalker>;
    protected:
        /// @brief The tracker used to keep track of serialized pointers.
        SerializerPointerTracker Tracker;
        /// @brief The logger that will be logged to during serialization.
        LogStream Logger;
        /// @brief A reference to the underlying tree walker navigating the serialization tree.
        WalkerRef Walker;

        /// @brief Gets the underlying tree walker.
        /// @return Returns a reference to the underlying tree walker.
        Serialization::TreeWalker& GetWalker()
            { return this->Walker; }
        /// @brief Gets the underlying tree walker.
        /// @return Returns a const reference to the underlying tree walker.
        const Serialization::TreeWalker& GetWalker() const
            { return this->Walker; }
    public:
        /// @brief Class constructor.
        /// @param ToWrap The underlying tree walker this walker will use during serialization.
        SerializerWalker(Serialization::TreeWalker& ToWrap) :
            Logger(nullptr),
            Walker(ToWrap)
            {  }
        /// @brief Logger constructor.
        /// @param Log The stream that will be logged to during serialization.
        /// @param ToWrap The underlying tree walker this walker will use during serialization.
        SerializerWalker(std::ostream& Log, Serialization::TreeWalker& ToWrap) :
            Logger(Log.rdbuf()),
            Walker(ToWrap)
            {  }
        /// @brief Class destructor.
        ~SerializerWalker() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Logging

        /// @brief Gets the logger to be used during serialization.
        /// @return Returns a reference to the serialization logger.
        [[nodiscard]]
        LogStream& GetLogger()
            { return this->Logger; }

        ///////////////////////////////////////////////////////////////////////////////
        // Pointer Tracking

        /// @brief Tracks a non-shared pointer (owned or not).
        /// @remarks The purpose of this function is to keep track of the pointers that exist in the serialization
        /// tree and how many copies exist of each throughout the serialization tree.
        /// @param ToTrack The pointer to be tracked.
        /// @param IsOwner Whether or not the pointer instance being tracked is an owning pointer.
        /// @return Returns an ObjectCounter reference that gives basic information about the tracked pointer.
        const ObjectCounter& TrackNonSharedPointer(void* ToTrack, const Boole IsOwner)
            { return this->Tracker.TrackNonSharedPointer(ToTrack,IsOwner); }
        /// @brief Tracks a shared_ptr.
        /// @remarks The purpose of this function is to keep track of the pointers that exist in the serialization
        /// tree and how many copies exist of each throughout the serialization tree.
        /// @param ToTrack The shared pointer to be tracked.
        /// @return Returns an ObjectCounter reference that gives basic information about the tracked pointer.
        const ObjectCounter& TrackSharedPointer(std::shared_ptr<void> ToTrack)
            { return this->Tracker.TrackSharedPointer(ToTrack); }

        /*
        /// @brief Tracks an object as if it were a pointer.
        /// @tparam ObjType The deduced type to track.
        /// @remarks This function exists for the edge cases where a data member on a class may have a pointer
        /// pointing to it.  In such cases the data wasn't new'd (at least directly) and didn't start it's existance
        /// as a pointer.  But if it is viewed through a pointer elsewhere then it is still an owned pointer from the
        /// serialization systems perspective.
        /// @param ToTrack The object instance to track as if it were a owned pointer.
        /// @return Returns an ObjectCounter reference that gives basic information about the tracked object/pointer.
        template<typename ObjType>
        const ObjectCounter& TrackObject(ObjType& ToTrack)
            { return this->Tracker.TrackObject(ToTrack); }//*/

        ///////////////////////////////////////////////////////////////////////////////
        // Name Operations

        /// @brief Sets the name of the Node being visited.
        /// @param Name The new name to be given to the internal Node.
        void SetName(const StringView Name)
            { this->GetWalker().SetName(Name); }
        /// @brief Gets the name of the Node being visited.
        /// @return Returns a StringVeiw containing the name of the internal Node being visited.
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
        // Serializer Specific Navigation

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
        /*template<typename AttributeType>
        [[nodiscard]]
        std::optional<AttributeType> Attribute(const StringView Name)
            { return this->GetWalker().Attribute<AttributeType>(Name); }//*/
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
        /// @param Walker The walker/visitor navigating the serialization tree.
        ScopedSerializationNode(const StringView Name, const MemberTags Tags, SerializerWalker& Walker)
        {
            if( Walker.CreateChild(Name,Tags,true) ) {
                this->Node = &Walker;
            }
        }
        /// @brief Tagless child create constructor.
        /// @remarks This constructor will attempt to create a new child with the specified name.
        /// @param Name The name of the node to be created.
        /// @param Walker The walker/visitor navigating the serialization tree.
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
        /// @brief Serializes extra metadata that can be used to validate types during deserialization.
        /// @tparam SerializeType The deduced type to be serialized.
        /// @remarks This function will not create a sub-node in the serialization tree and instead will
        /// append directly to the current node pointed to by Walker.
        /// @param ToSerialize Not used directly, only for type deduction.
        /// @param Version The version the object should be serialized as. Not all types have multiple versions.
        /// @param Walker The walker/visitor navigating the serialization tree.
        template<class SerializeType>
        void SerializeClassValidation(const SerializeType& ToSerialize,
                                      const Int32 Version,
                                      Serialization::SerializerWalker& Walker)
        {
            Walker.GetLogger() << "\nEntering \"SerializeClassValidation\".\n";
            (void)ToSerialize;
            using DecayedType = std::decay_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                Walker.Attribute("TypeName",GetRegisteredName<DecayedType>());
                Walker.Attribute("Version",Version);
            }
            Walker.GetLogger() << "\nExiting \"SerializeClassValidation\".\n";
        }
        /// @brief Serializes a String or primitive type.
        /// @tparam SerializeType The deduced type to be serialized.
        /// @remarks This function will not create a sub-node in the serialization tree and instead will
        /// append directly to the current node pointed to by Walker.
        /// @param Name The name associated with the ToSerialize parameter/instance.
        /// @param ToSerialize The String or primitive type to be serialized.
        /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
        /// @param Walker The walker/visitor navigating the serialization tree.
        template<class SerializeType>
        void SerializeSimpleMember(const StringView Name,
                                   const SerializeType& ToSerialize,
                                   const MemberTags Tags,
                                   Serialization::SerializerWalker& Walker)
        {
            Walker.Attribute(Name,Tags,ToSerialize);
        }
        /// @brief Serializes all members of a class or struct.
        /// @tparam SerializeType The deduced type to be serialized.
        /// @remarks This function will not create a sub-node in the serialization tree and instead will
        /// append directly to the current node pointed to by Walker.
        /// @param ToSerialize The type (with members) to be serialized.
        /// @param Walker The walker/visitor navigating the serialization tree.
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
        /// @brief Serializes an associative container to the current node.
        /// @tparam SerializeType The deduced type to be serialized.
        /// @remarks This function will not create a sub-node in the serialization tree and instead will
        /// append directly to the current node pointed to by Walker. If you want the serialized data to
        /// be appended to it's own node, it is expected that such a node will be created in the calling
        /// function before calling this.
        /// @param ToSerialize The container to be serialized.
        /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
        /// @param Walker The walker/visitor navigating the serialization tree.
        template<class SerializeType,
                 typename = std::enable_if_t< IsAssociativeContainer<SerializeType>() >,
                 typename = void>
        void SerializeContainer(const SerializeType& ToSerialize,
                                const MemberTags Tags,
                                Serialization::SerializerWalker& Walker)
        {
            using DecayedType = std::decay_t<SerializeType>;
            Whole Count = 0;
            Walker.Attribute("ElementCount",ToSerialize.size());
            Walker.Attribute("KeyType",GetSerializableTypeIdentity<typename DecayedType::key_type>());
            Walker.Attribute("MappedType",GetSerializableTypeIdentity<typename DecayedType::mapped_type>());
            for( auto& CurrElement : ToSerialize )
            {
                StringStream Namer;
                Namer << "Element" << Count;
                ScopedSerializationNode ElementNode(Namer.str(),Walker);
                if( ElementNode ) {
                    Mezzanine::Serialize("Key",CurrElement.first,Tags,LatestVersion,Walker);
                    Mezzanine::Serialize("Value",CurrElement.second,Tags,LatestVersion,Walker);
                    ++Count;
                }
            }
        }
        /// @brief Serializes a non-associative container to the current node.
        /// @tparam SerializeType The deduced type to be serialized.
        /// @remarks This function will not create a sub-node in the serialization tree and instead will
        /// append directly to the current node pointed to by Walker. If you want the serialized data to
        /// be appended to it's own node, it is expected that such a node will be created in the calling
        /// function before calling this.
        /// @param ToSerialize The container to be serialized.
        /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
        /// @param Walker The walker/visitor navigating the serialization tree.
        template<class SerializeType,
                 typename = std::enable_if_t< IsNonAssociativeContainer<SerializeType>() > >
        void SerializeContainer(const SerializeType& ToSerialize,
                                const MemberTags Tags,
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
                Mezzanine::Serialize(Namer.str(),CurrElement,Tags,LatestVersion,Walker);
                ++Count;
            }
        }
        /// @brief Serializes a class or struct to the current node.
        /// @tparam SerializeType The deduced type to be serialized.
        /// @remarks This function will not create a sub-node in the serialization tree and instead will
        /// append directly to the current node pointed to by Walker. If you want the serialized data to
        /// be appended to it's own node, it is expected that such a node will be created in the calling
        /// function before calling this.
        /// @param ToSerialize The class or struct to be serialized.
        /// @param Version The version the object should be serialized as. Not all types have multiple versions.
        /// @param Walker The walker/visitor navigating the serialization tree.
        template<class SerializeType>
        void SerializeGenericClass(const SerializeType& ToSerialize,
                                   const Int32 Version,
                                   Serialization::SerializerWalker& Walker)
        {
            std::cout << "\nEntering \"SerializeGenericClass\".\n";
            using DecayedType = std::decay_t<SerializeType>;
            if constexpr( HasSerialize<DecayedType>() ) {
                ToSerialize.Serialize(Version,Walker);
            }else if constexpr( IsRegistered<DecayedType>() ) {
                Serialization::Impl::SerializeClassValidation(ToSerialize,Version,Walker);
                Serialization::Impl::SerializeAllMembers(ToSerialize,Walker);
            }
            std::cout << "\nExiting \"SerializeGenericClass\".\n";
        }
        /// @brief Serializes a struct, class, or primitive behind a owned pointer.
        /// @tparam SerializeType The deduced type to be serialized.
        /// @param InstanceID A unique serializable ID for the object being pointed to.
        /// @param ToSerialize The owned pointer to be serialized.
        /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
        /// @param Version The version the object should be serialized as. Not all types have multiple versions.
        /// @param Walker The walker/visitor navigating the serialization tree.
        template<class SerializeType>
        void SerializeOwnedPointer(const uintptr_t InstanceID,
                                   const SerializeType ToSerialize,
                                   const MemberTags Tags,
                                   const Int32 Version,
                                   Serialization::SerializerWalker& Walker)
        {
            static_assert( std::is_pointer_v<SerializeType> , "SerializeType is not a pointer." );
            using DecayedType = std::remove_pointer_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                Walker.Attribute("CanDeserialize",true);
                Walker.Attribute("InstanceID",InstanceID);
                ProtoSerialize(*ToSerialize,Tags,Version,Walker);
            }
        }
        /// @brief Serializes a struct, class, or primitive behind a non-owned pointer.
        /// @tparam SerializeType The deduced type to be serialized.
        /// @param InstanceID A unique serializable ID for the object being pointed to.
        /// @param ToSerialize The non-owned pointer to be serialized.
        /// @param Walker The walker/visitor navigating the serialization tree.
        template<class SerializeType>
        void SerializeNonOwnedPointer(const uintptr_t InstanceID,
                                      const SerializeType ToSerialize,
                                      Serialization::SerializerWalker& Walker)
        {
            (void)ToSerialize;
            static_assert( std::is_pointer_v<SerializeType> , "SerializeType is not a pointer." );
            using DecayedType = std::remove_pointer_t<SerializeType>;
            if constexpr( IsRegistered<DecayedType>() ) {
                Walker.Attribute("CanDeserialize",false);
                Walker.Attribute("TypeName",GetRegisteredName<DecayedType>());
                Walker.Attribute("InstanceID",InstanceID);
            }
        }

        /// @brief Serializes a struct, class, or primitive behind a pointer without casting.
        /// @remarks This function should be called only when the pointer type matches the type behind the
        /// pointer exactly.
        /// @tparam SerializeType The deduced type to be serialized.
        /// @param ToSerialize The pointer to be serialized.
        /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
        /// @param Version The version the object should be serialized as. Not all types have multiple versions.
        /// @param Walker The walker/visitor navigating the serialization tree.
        template<class SerializeType>
        void SerializePointer(const SerializeType ToSerialize,
                              const MemberTags Tags,
                              const Int32 Version,
                              Serialization::SerializerWalker& Walker)
        {
            static_assert( std::is_pointer_v<SerializeType> , "SerializeType is not a pointer." );
            const ObjectCounter Counter = Walker.TrackNonSharedPointer(ToSerialize,IsOwned(Tags));
            if( Serialization::IsNotOwned(Tags) ) {
                Serialization::Impl::SerializeNonOwnedPointer(Counter.InstanceID,ToSerialize,Walker);
            }else{
                Serialization::Impl::SerializeOwnedPointer(Counter.InstanceID,ToSerialize,Tags,Version,Walker);
            }
        }
        /// @brief Serializes a class, struct, or primitive type behind a shared_ptr.
        /// @tparam SerializeType The deduced type to be serialized.
        /// @param ToSerialize The shared_ptr to be serialized.
        /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
        /// @param Version The version the object should be serialized as. Not all types have multiple versions.
        /// @param Walker The walker/visitor navigating the serialization tree.
        template<class SerializeType>
        void SerializeSharedPointer(const std::shared_ptr<SerializeType> ToSerialize,
                                    const MemberTags Tags,
                                    const Int32 Version,
                                    Serialization::SerializerWalker& Walker)
        {
            const ObjectCounter& CountInfo = Walker.TrackSharedPointer(ToSerialize);
            if( CountInfo.OwnerOrJoinCount > 1 ) {
                Impl::SerializeNonOwnedPointer(CountInfo.InstanceID,ToSerialize.get(),Walker);
            }else{
                Impl::SerializeOwnedPointer(CountInfo.InstanceID,ToSerialize.get(),Tags,Version,Walker);
            }
        }
        /// @brief Serializes type behind a polymorphic pointer.
        /// @tparam SerializeType The deduced type to be serialized.
        /// @param ToSerialize The pointer to be serialized.
        /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
        /// @param Version The version the object should be serialized as. Not all types have multiple versions.
        /// @param Walker The walker/visitor navigating the serialization tree.
        template<class SerializeType>
        void SerializePolymorphicPointer(const SerializeType ToSerialize,
                                         const MemberTags Tags,
                                         const Int32 Version,
                                         Serialization::SerializerWalker& Walker)
        {
            static_assert( std::is_pointer_v<SerializeType> , "SerializeType is not a pointer." );
            const std::type_info& BaseInfo = typeid(ToSerialize);
            const std::type_info& DerivedInfo = typeid(*ToSerialize);
            if( std::type_index(BaseInfo) != std::type_index(DerivedInfo) ) {
                Serialization::PolymorphicCaster* Caster = Serialization::GetPolymorphicCaster(BaseInfo,DerivedInfo);
                if( Caster != nullptr ) {
                    Caster->Serialize(ToSerialize,Tags,Version,Walker);
                }else{
                    throw std::runtime_error("No caster found for polymorphic object.");
                }
            }else{
                Impl::SerializePointer(ToSerialize,Tags,Version,Walker);
            }
        }
        /// @brief Serializes type behind a polymorphic pointer.
        /// @tparam SerializeType The deduced type to be serialized.
        /// @param ToSerialize The shared pointer to be serialized.
        /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
        /// @param Version The version the object should be serialized as. Not all types have multiple versions.
        /// @param Walker The walker/visitor navigating the serialization tree.
        template<class SerializeType>
        void SerializePolymorphicPointer(const std::shared_ptr<SerializeType> ToSerialize,
                                         const MemberTags Tags,
                                         const Int32 Version,
                                         Serialization::SerializerWalker& Walker)
        {
            static_assert( std::is_pointer_v<SerializeType> , "SerializeType is not a pointer." );
            const std::type_info& BaseInfo = typeid(ToSerialize.get());
            const std::type_info& DerivedInfo = typeid(*ToSerialize.get());
            if( std::type_index(BaseInfo) != std::type_index(DerivedInfo) ) {
                Serialization::PolymorphicCaster* Caster = Serialization::GetPolymorphicCaster(BaseInfo,DerivedInfo);
                if( Caster != nullptr ) {
                    Caster->Serialize(ToSerialize,Tags,Version,Walker);
                }else{
                    throw std::runtime_error("No caster found for polymorphic object.");
                }
            }else{
                Impl::SerializeSharedPointer(ToSerialize,Tags,Version,Walker);
            }
        }
    }//Impl
}//Serialization
    ///////////////////////////////////////////////////////////////////////////////
    // ProtoSerialize

    /// @brief Serializes a non-pointer class, struct, or primitive type.
    /// @tparam SerializeType The deduced type to be serialized.
    /// @param ToSerialize The non-pointer class, struct, or primitive to be serialized.
    /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
    /// @param Version The version the object should be serialized as. Not all types have multiple versions.
    /// @param Walker The walker/visitor navigating the serialization tree.
    template< typename SerializeType,
              typename >
    void ProtoSerialize(const SerializeType& ToSerialize,
                        const MemberTags Tags,
                        const Int32 Version,
                        Serialization::SerializerWalker& Walker)
    {
        namespace Impl = Mezzanine::Serialization::Impl;
        using DecayedType = std::decay_t<SerializeType>;
        if constexpr( IsContainer<DecayedType>() ) {
            (void)Version;
            Impl::SerializeContainer(ToSerialize,Tags,Walker);
        }else if constexpr( IsRegistered<DecayedType>() ) {
            (void)Tags;
            Impl::SerializeGenericClass(ToSerialize,Version,Walker);
        }
    }
    /// @brief Serializes a class, struct, or primitive type behind a pointer.
    /// @tparam SerializeType The deduced type to be serialized.
    /// @param ToSerialize The pointer to be serialized.
    /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
    /// @param Version The version the object should be serialized as. Not all types have multiple versions.
    /// @param Walker The walker/visitor navigating the serialization tree.
    template< typename SerializeType,
              typename >
    void ProtoSerialize(const SerializeType ToSerialize,
                        const MemberTags Tags,
                        const Int32 Version,
                        Serialization::SerializerWalker& Walker)
    {
        using DecayedType = std::remove_pointer_t< std::decay_t<SerializeType> >;
        namespace Impl = Mezzanine::Serialization::Impl;
        if constexpr( IsRegistered<DecayedType>() || IsContainer<DecayedType>() ) {
            if constexpr( std::is_polymorphic_v<DecayedType> ) {
                Impl::SerializePolymorphicPointer(ToSerialize,Tags,Version,Walker);
            }else{
                Impl::SerializePointer(ToSerialize,Tags,Version,Walker);
            }
        }
    }
    /// @brief Serializes a class, struct, or primitive type behind a unique_ptr.
    /// @tparam SerializeType The deduced type to be serialized.
    /// @param ToSerialize The pointer to be serialized.
    /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
    /// @param Version The version the object should be serialized as. Not all types have multiple versions.
    /// @param Walker The walker/visitor navigating the serialization tree.
    template< typename SerializeType >
    void ProtoSerialize(const std::unique_ptr<SerializeType>& ToSerialize,
                        const MemberTags Tags,
                        const Int32 Version,
                        Serialization::SerializerWalker& Walker)
    {
        using DecayedType = std::decay_t<SerializeType>;
        if constexpr( IsRegistered<DecayedType>() || IsContainer<DecayedType>() ) {
            // This is presuming all unique_ptrs are owned.
            const MemberTags PtrTags = Serialization::MakeOwned(Tags);
            ProtoSerialize(ToSerialize.get(),PtrTags,Version,Walker);
        }
    }
    /// @brief Serializes a class, struct, or primitive type behind a shared_ptr.
    /// @tparam SerializeType The deduced type to be serialized.
    /// @param ToSerialize The shared_ptr to be serialized.
    /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
    /// @param Version The version the object should be serialized as. Not all types have multiple versions.
    /// @param Walker The walker/visitor navigating the serialization tree.
    template< typename SerializeType >
    void ProtoSerialize(const std::shared_ptr<SerializeType> ToSerialize,
                        const MemberTags Tags,
                        const Int32 Version,
                        Serialization::SerializerWalker& Walker)
    {
        using DecayedType = std::decay_t<SerializeType>;
        namespace Impl = Mezzanine::Serialization::Impl;
        if constexpr( IsRegistered<DecayedType>() || IsContainer<DecayedType>() ) {
            const MemberTags PtrTags = Serialization::MakeOwned(Tags);
            if constexpr( std::is_polymorphic_v<DecayedType> ) {
                Impl::SerializePolymorphicPointer(ToSerialize,PtrTags,Version,Walker);
            }else{
                Impl::SerializeSharedPointer(ToSerialize,PtrTags,Version,Walker);
            }
        }
    }
    /// @brief Serializes a type inside an optional, if it is valid/exists.
    /// @tparam SerializeType The deduced type to maybe be serialized.
    /// @param ToSerialize The optional to be serialized.
    /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
    /// @param Version The version the object should be serialized as. Not all types have multiple versions.
    /// @param Walker The walker/visitor navigating the serialization tree.
    template< typename SerializeType >
    void ProtoSerialize(const std::optional<SerializeType>& ToSerialize,
                        const MemberTags Tags,
                        const Int32 Version,
                        Serialization::SerializerWalker& Walker)
    {
        using DecayedType = std::decay_t<SerializeType>;
        if constexpr( IsRegistered<DecayedType>() || IsContainer<DecayedType>() ) {
            if( ToSerialize ) {
                ProtoSerialize(ToSerialize.value(),Tags,Version,Walker);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialize

    /// @brief Serializes a non-pointer class, struct, or primitive type.
    /// @tparam SerializeType The deduced type to be serialized.
    /// @param Name The name associated with the ToSerialize parameter/instance.
    /// @param ToSerialize The non-pointer class, struct, or primitive to be serialized.
    /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
    /// @param Version The version the object should be serialized as. Not all types have multiple versions.
    /// @param Walker The walker/visitor navigating the serialization tree.
    template< typename SerializeType,
              typename >
    void Serialize(const StringView Name,
                   const SerializeType& ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker)
    {
        using DecayedType = std::decay_t<SerializeType>;
        namespace Impl = Mezzanine::Serialization::Impl;
        if constexpr( std::is_arithmetic_v<DecayedType> ) {
            (void)Version;
            Impl::SerializeSimpleMember(Name,ToSerialize,Tags,Walker);
        }else if constexpr( StringTools::is_string<DecayedType>::value ) {
            (void)Version;
            Impl::SerializeSimpleMember(Name,ToSerialize,Tags,Walker);
        }else if constexpr( IsRegistered<DecayedType>() || IsContainer<DecayedType>() ) {
            Serialization::ScopedSerializationNode Node(Name,Tags,Walker);
            if( Node ) {
                ProtoSerialize(ToSerialize,Tags,Version,Walker);
            }
        }
    }
    /// @brief Serializes a class, struct, or primitive type behind a pointer.
    /// @tparam SerializeType The deduced type to be serialized.
    /// @param Name The name associated with the ToSerialize parameter/instance.
    /// @param ToSerialize The pointer to be serialized.
    /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
    /// @param Version The version the object should be serialized as. Not all types have multiple versions.
    /// @param Walker The walker/visitor navigating the serialization tree.
    template< typename SerializeType,
              typename >
    void Serialize(const StringView Name,
                   const SerializeType ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker)
    {
        using DecayedType = std::remove_pointer_t< std::decay_t<SerializeType> >;
        if constexpr( IsRegistered<DecayedType>() || IsContainer<DecayedType>() ) {
            Serialization::ScopedSerializationNode Node(Name,Tags,Walker);
            if( Node ) {
                ProtoSerialize(ToSerialize,Tags,Version,Walker);
            }
        }
    }
    /// @brief Serializes a class, struct, or primitive type behind a unique_ptr.
    /// @tparam SerializeType The deduced type to be serialized.
    /// @param Name The name associated with the ToSerialize parameter/instance.
    /// @param ToSerialize The pointer to be serialized.
    /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
    /// @param Version The version the object should be serialized as. Not all types have multiple versions.
    /// @param Walker The walker/visitor navigating the serialization tree.
    template< typename SerializeType >
    void Serialize(const StringView Name,
                   const std::unique_ptr<SerializeType>& ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker)
    {
        using DecayedType = std::decay_t<SerializeType>;
        if constexpr( IsRegistered<DecayedType>() || IsContainer<DecayedType>() ) {
            Serialization::ScopedSerializationNode Node(Name,Tags,Walker);
            if( Node ) {
                ProtoSerialize(ToSerialize,Tags,Version,Walker);
            }
        }
    }
    /// @brief Serializes a class, struct, or primitive type behind a shared_ptr.
    /// @tparam SerializeType The deduced type to be serialized.
    /// @param Name The name associated with the ToSerialize parameter/instance.
    /// @param ToSerialize The shared_ptr to be serialized.
    /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
    /// @param Version The version the object should be serialized as. Not all types have multiple versions.
    /// @param Walker The walker/visitor navigating the serialization tree.
    template< typename SerializeType >
    void Serialize(const StringView Name,
                   const std::shared_ptr<SerializeType> ToSerialize,
                   const MemberTags Tags,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker)
    {
        using DecayedType = std::decay_t<SerializeType>;
        if constexpr( IsRegistered<DecayedType>() || IsContainer<DecayedType>() ) {
            Serialization::ScopedSerializationNode Node(Name,Tags,Walker);
            if( Node ) {
                ProtoSerialize(ToSerialize,Tags,Version,Walker);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serialize - No Tags

    /// @brief Serializes a non-pointer class, struct, or primitive type.
    /// @tparam SerializeType The deduced type to be serialized.
    /// @param Name The name associated with the ToSerialize parameter/instance.
    /// @param ToSerialize The non-pointer class, struct, or primitive to be serialized.
    /// @param Version The version the object should be serialized as. Not all types have multiple versions.
    /// @param Walker The walker/visitor navigating the serialization tree.
    template< typename SerializeType,
              typename >
    void Serialize(const StringView Name,
                   const SerializeType& ToSerialize,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker)
    {
        Mezzanine::Serialize(Name,ToSerialize,MemberTags::None,Version,Walker);
    }
    /// @brief Serializes a class, struct, or primitive type behind a pointer.
    /// @tparam SerializeType The deduced type to be serialized.
    /// @param Name The name associated with the ToSerialize parameter/instance.
    /// @param ToSerialize The pointer to be serialized.
    /// @param Version The version the object should be serialized as. Not all types have multiple versions.
    /// @param Walker The walker/visitor navigating the serialization tree.
    template< typename SerializeType,
              typename >
    void Serialize(const StringView Name,
                   const SerializeType ToSerialize,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker)
    {
        Mezzanine::Serialize(Name,ToSerialize,MemberTags::None,Version,Walker);
    }
    /// @brief Serializes a class, struct, or primitive type behind a unique_ptr.
    /// @tparam SerializeType The deduced type to be serialized.
    /// @param Name The name associated with the ToSerialize parameter/instance.
    /// @param ToSerialize The shared_ptr to be serialized.
    /// @param Version The version the object should be serialized as. Not all types have multiple versions.
    /// @param Walker The walker/visitor navigating the serialization tree.
    template< typename SerializeType >
    void Serialize(const StringView Name,
                   const std::unique_ptr<SerializeType>& ToSerialize,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker)
    {
        Mezzanine::Serialize(Name,ToSerialize,MemberTags::None,Version,Walker);
    }
    /// @brief Serializes a class, struct, or primitive type behind a shared_ptr.
    /// @tparam SerializeType The deduced type to be serialized.
    /// @param Name The name associated with the ToSerialize parameter/instance.
    /// @param ToSerialize The shared_ptr to be serialized.
    /// @param Version The version the object should be serialized as. Not all types have multiple versions.
    /// @param Walker The walker/visitor navigating the serialization tree.
    template< typename SerializeType >
    void Serialize(const StringView Name,
                   const std::shared_ptr<SerializeType> ToSerialize,
                   const Int32 Version,
                   Serialization::SerializerWalker& Walker)
    {
        Mezzanine::Serialize(Name,ToSerialize,MemberTags::None,Version,Walker);
    }
    /// @brief Serializes a class, struct, or primitive type in an std::optional.
    /// @tparam SerializeType The deduced type to be serialized.
    /// @param Name The name associated with the ToSerialize parameter/instance.
    /// @param ToSerialize The optional to (maybe) be serialized.
    /// @param Version The version the object should be serialized as. Not all types have multiple versions.
    /// @param Walker The walker/visitor navigating the serialization tree.
    template< typename SerializeType >
    void Serialize(const StringView Name,
                   const std::optional<SerializeType>& ToSerialize,
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
        //Serialization::ContextBase* Context = nullptr;
    public:
        Serializer(Serialization::BackendBase* Back) :
            Backend(Back)
            {  }


        /*Serialization::ContextBase* GetContext() const
            { return this->Context; }

        template<typename ObjectType, typename IDType>
        ObjectType* FindContextObject(const IDType ID)
        {
            if( this->Context != nullptr ) {
                void* Found = this->Context->FindContextObject(ID,std::type_index(typeid(ObjectType)));
                return reinterpret_cast<ObjectType>(Found);
            }
            return nullptr;
        }//*/
    };//Serializer

    /// @}
}//Mezzanine

#endif // Mezz_Foundation_SerializationSerializer_h
