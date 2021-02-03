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
#ifndef Mezz_Foundation_SerializationPolymorphicCaster_h
#define Mezz_Foundation_SerializationPolymorphicCaster_h

/// @file
/// @brief This file contains a set of classes for casting runtime polymorphic types.

#ifndef SWIG
    #include "SerializationUtility.h"

    #include <type_traits>
    #include <memory>
    #include <vector>
#endif // SWIG

namespace Mezzanine {
namespace Serialization {
    /// @addtogroup Serialization
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    // PolymorphicCasterHolder

    template<class Base, class Derived>
    class PolymorphicCasterImpl;

    /// @brief Interface for classes that permit casting between two registered types.
    class MEZZ_LIB PolymorphicCaster
    {
    public:
        /// @brief Class Destructor.
        virtual ~PolymorphicCaster() = default;

        /// @brief Gets the name of the base type this caster will cast from.
        /// @return Returns a StringView containing the name of the base type the caster can cast from.
        virtual StringView GetBaseTypeName() const = 0;
        /// @brief Gets the name of the derived type this caster will cast to.
        /// @return Returns a StringView containing the name of the derived type the caster can cast to.
        virtual StringView GetDerivedTypeName() const = 0;

        /// @brief Gets whether or not this caster can perform a cast between the two specified types.
        /// @param BaseType The type_info of the base type to be casted from.
        /// @param DerivedType The type_info of the derived type to be casted to.
        /// @return Returns true if this caster can perform a cast between the two types, false otherwise.
        virtual Boole CanUpcast(const std::type_info& BaseType, const std::type_info& DerivedType) const = 0;
        /// @brief Gets whether or not this caster can perform a cast between the two specified types.
        /// @param BaseType The name of the base type to be casted from.
        /// @param DerivedType The name of the derived type to be casted to.
        /// @return Returns true if this caster can perform a cast between the two types, false otherwise.
        virtual Boole CanUpcast(const StringView BaseType, const StringView DerivedType) const = 0;

        /// @brief Gets whether ot not this caster can perform an upcast on a pointer.
        /// @tparam PtrType The type of pointer to check.
        /// @param ToCast The pointer instance to check.
        /// @return Returns true if the provided pointer can be upcased by this Caster, false otherwise.
        template<typename PtrType>
        Boole CanUpcast(PtrType ToCast)
        {
            static_assert(std::is_pointer_v<PtrType>,"Provided type is NOT a pointer.");
            using NoPtr = std::remove_pointer_t<PtrType>;

            if constexpr( std::is_polymorphic_v<NoPtr> ) {
                const std::type_info& Base = typeid(NoPtr);
                const std::type_info& MaybeDerived = typeid(*ToCast);
                return this->CanCast(Base,MaybeDerived);
            }else{
                return false;
            }
        }

        /// @brief Performs a cast to the derived type and then invokes Serialize.
        /// @param Name The name of the instance being serialized.
        /// @param ToSerialize The instance being serialized.
        /// @param Tags Descriptors associated with the data being serialized that may alter serialization behavior.
        /// @param Version The version of the class instance to serialize as.
        /// @param Walker The walker/visitor navigating the serialization tree.
        virtual void Serialize(const StringView Name,
                               void* ToSerialize,
                               const MemberTags Tags,
                               const Integer Version,
                               Serialization::SerializerWalker& Walker) = 0;
        /// @brief Performs a cast to the derived type and then invokes Deserialize.
        /// @param Name The name of the instance being deserialized.
        /// @param ToDeserialize The instance being deserialized.
        /// @param Tags Descriptors associated with the data being deserialized that may alter deserialization behavior.
        /// @param Walker The walker/visitor navigating the deserialization tree.
        virtual void Deserialize(const StringView Name,
                                 void*& ToDeserialize,
                                 const MemberTags Tags,
                                 Serialization::DeserializerWalker& Walker) = 0;
    };//PolymorphicCaster

    /// @brief This class stores all of the Polymorphic caster implementations for deserialization.
    struct MEZZ_LIB PolymorphicCasterHolder
    {
        /// @brief Convenience type for the type the caster type.
        using CasterBaseType = PolymorphicCaster;
        /// @brief Convenience type for the caster as it is stored in the holder.
        using StoredCasterType = std::shared_ptr<CasterBaseType>;
        /// @brief Convenience type for the container of stored casters.
        using DerivedContainer = std::vector<StoredCasterType>;
        /// @brief Convenience type for storing casters sorted by typeinfo/typeindex.
        using ByTypeInfoContainer = FlatMap< std::type_index, DerivedContainer >;
        /// @brief Convenience type for storing casters sorted by typename.
        using ByTypeNameContainer = FlatMap< StringView, DerivedContainer >;

        /// @brief A container of PolymorphicCasters sorted by their typeinfo/typeindex.
        ByTypeInfoContainer CastersByTypeInfo;
        /// @brief A container of PolymorphicCasters sorted by their typename.
        ByTypeNameContainer CastersByTypeName;

        /// @brief Class constructor.
        PolymorphicCasterHolder() = default;
        /// @brief DELETED copy constructor.
        /// @param Other The other holder that will NOT be copied.
        PolymorphicCasterHolder(const PolymorphicCasterHolder& Other) = delete;
        /// @brief DELETED move constructor.
        /// @param Other The other holder that will NOT be moved.
        PolymorphicCasterHolder(PolymorphicCasterHolder&& Other) = delete;
        /// @brief Class destructor.
        ~PolymorphicCasterHolder() = default;

        /// @brief DELETED Copy Assignment Operator.
        /// @param Other The other holder that will NOT be copied.
        /// @return Returns a reference to this.
        PolymorphicCasterHolder& operator=(const PolymorphicCasterHolder& Other) = delete;
        /// @brief DELETED Move Assignment Operator.
        /// @param Other The other holder that will NOT be moved.
        /// @return Returns a reference to this.
        PolymorphicCasterHolder& operator=(PolymorphicCasterHolder&& Other) = delete;

        /// @brief Adds a new caster to the holder.
        /// @tparam Base The type the caster will cast from.
        /// @tparam Derived The type the caster will cast to.
        /// @param NewCaster The new caster to be registered/stored.
        template<class Base, class Derived>
        void RegisterCaster()
        {
            std::shared_ptr<PolymorphicCasterImpl<Base,Derived>> NewCaster = std::make_shared<Base,Derived>();
            this->CastersByTypeInfo[ std::type_index( typeid(Base) ) ].push_back(NewCaster);
            this->CastersByTypeName[ GetRegisteredName<Base>() ].push_back(NewCaster);
        }

        /// @brief Gets a caster that can upcast from base to derived.
        /// @param BaseInfo The type_info of the base type to be casted from.
        /// @param DerivedInfo The type_info of the derived type to be casted to
        /// @return Returns the caster capable of upcasting from Base to Derived, or nullptr if none is found.
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

        /// @brief Gets a caster that can upcast from base to derived.
        /// @param BaseName The typename of the base type to be casted from.
        /// @param DerivedName The typename of the derived type to be casted to.
        /// @return Returns the caster capable of upcasting from Base to Derived, or nullptr if none is found.
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

        /// @brief Gets a caster that can upcast from base to derived.
        /// @tparam Base The type the caster will cast from.
        /// @tparam Derived The type the caster will cast to.
        /// @return Returns the caster capable of upcasting from Base to Derived, or nullptr if none is found.
        template<class Base, class Derived>
        StoredCasterType GetCaster() const
        {
            return this->GetCaster( typeid(Base), typeid(Derived) );
        }
    };//PolymorphicCasterHolder

SAVE_WARNING_STATE
// Yes clang, we know.  It's a singleton pattern.
SUPPRESS_CLANG_WARNING("-Wexit-time-destructors")

    /// @brief Gets the singleton PolymorphicCasterHolder used by the serialization system.
    /// @remarks Creating your own PolymorphicCasterHolder is strongly discouraged. Use of this function
    /// should be used instead when/if needing to access the registered casters, which should be rare.
    /// @return Returns a reference to the singleton caster holder.
    PolymorphicCasterHolder& GetPolymorphicCasterHolder();
    PolymorphicCasterHolder& GetPolymorphicCasterHolder()
    {
        static PolymorphicCasterHolder Store;
        return Store;
    }

RESTORE_WARNING_STATE

    /// @brief Implementation type for classes that permit casting between two registered types.
    /// @tparam Base The type the caster will cast from.
    /// @tparam Derived The type the caster will cast to.
    template<class Base, class Derived>
    class PolymorphicCasterImpl final : public PolymorphicCaster
    {
    public:
        static_assert( std::is_base_of_v<Base,Derived>, "\"Base\" is not a base class of Derived" );
        static_assert( std::is_constructible_v<Derived,DeserializerWalker&> ||
                       std::is_default_constructible_v<Derived>,
                       "Derived type must be default constructable or have a constructor that accepts "
                       "an Int and DeserializerWalker." );

        /// @brief Class constructor.
        ~PolymorphicCasterImpl() = default;
        /// @copydoc PolymorphicCaster::GetBaseTypeName() const
        StringView GetBaseTypeName() const
            { return GetRegisteredName<Base>(); }
        /// @copydoc PolymorphicCaster::GetDerivedTypeName() const
        StringView GetDerivedTypeName() const
            { return GetRegisteredName<Derived>(); }
        /// @copydoc PolymorphicCaster::CanUpcast(const std::type_info&, const std::type_info&) const
        Boole CanUpcast(const std::type_info& BaseType, const std::type_info& DerivedType) const
            { return ( BaseType == typeid(Base) && DerivedType == typeid(Derived) ); }
        /// @copydoc PolymorphicCaster::CanUpcast(const StringView, const StringView) const
        Boole CanUpcast(const StringView BaseType, const StringView DerivedType) const
            { return ( BaseType == GetBaseTypeName() && DerivedType == GetDerivedTypeName() ); }

        /// @brief
        /// @param ToUpcast
        /// @param Funct
        void Upcast(void* ToUpcast, std::function<void(void*)> Funct)
        {

        }

        /// @copydoc PolymorphicCaster::Serialize
        void Serialize(const StringView Name,
                       void* ToSerialize,
                       const MemberTags Tags,
                       const Int32 Version,
                       Serialization::SerializerWalker& Walker)
        {
            Derived* Casted = static_cast<Derived*>( static_cast<Base*>(ToSerialize) );
            Mezzanine::Serialize(Name,std::forward<Derived*>(Casted),Tags,Version,Walker);
        }
        /// @copydoc PolymorphicCaster::Deserialize
        void Deserialize(const StringView Name,
                         void*& ToDeserialize,
                         const MemberTags Tags,
                         Serialization::DeserializerWalker& Walker)
        {
            Derived* UpCasted = nullptr;
            if( ToDeserialize == nullptr ) {
                if constexpr( std::is_constructible_v<Derived,decltype(Walker)> ) {
                    UpCasted = new Derived(Walker);
                    return;
                }else if constexpr( std::is_default_constructible_v<Derived> ) {
                    UpCasted = new Derived();
                }
                ToDeserialize = UpCasted;
            }else{
                UpCasted = static_cast<Derived*>( static_cast<Base*>(ToDeserialize) );
            }
            Mezzanine::Deserialize(Name,std::forward<Derived*>(UpCasted),Tags,Walker);
        }
    };//PolymorphicCasterImpl
}//Serialization
    ///////////////////////////////////////////////////////////////////////////////
    // Caster Registration

    /// @brief Registers a single Base/Derived relationship between two types.
    /// @tparam Base The type to be casted from. Needs to be a base of Derived.
    /// @tparam Derived The type to be casted to.
    template<class Base, class Derived>
    void RegisterCaster()
    {
        using namespace Serialization;
        GetPolymorphicCasterHolder().RegisterCaster<Base,Derived>();
    }

    /// @brief Like RegisterCaster, but registers multiple derived types to a single base type.
    /// @tparam Base The type to be casted from. Needs to be a base of types types specified in Deriveds.
    /// @tparam Deriveds A collection of types, each of which will be registered as Derived to the base type.
    template<class Base, class... Deriveds>
    void RegisterCasters()
    {
        ( RegisterCaster<Base,Deriveds>() , ... );
    }

    /// @}
}//Mezzanine

#endif // Mezz_Foundation_SerializationPolymorphicCaster_h
