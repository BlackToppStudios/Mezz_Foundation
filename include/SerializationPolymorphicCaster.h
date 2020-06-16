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
#if 1
    template<class Base, class Derived>
    class PolymorphicCasterImpl;

    class MEZZ_LIB PolymorphicCaster
    {
    public:
        virtual ~PolymorphicCaster() = default;
        virtual StringView GetBaseTypeName() const = 0;
        virtual StringView GetDerivedTypeName() const = 0;
        virtual Boole CanUpcast(const std::type_info& BaseType, const std::type_info& DerivedType) const = 0;
        virtual Boole CanUpcast(const StringView BaseType, const StringView DerivedType) const = 0;
        virtual void Serialize(const StringView Name,
                               void* ToSerialize,
                               const MemberTags Tags,
                               const Integer Version,
                               Serialization::ObjectWalker& Walker) = 0;
        virtual void Deserialize(const StringView Name,
                                 void*& ToDeserialize,
                                 const MemberTags Tags,
                                 const Integer Version,
                                 Serialization::ObjectWalker& Walker) = 0;
    };//PolymorphicCaster

    struct MEZZ_LIB PolymorphicCasterHolder
    {
        using CasterBaseType = PolymorphicCaster;
        using StoredCasterType = std::shared_ptr<CasterBaseType>;
        using DerivedContainer = std::vector<StoredCasterType>;
        using ByTypeInfoContainer = FlatMap< std::type_index, DerivedContainer >;
        using ByTypeNameContainer = FlatMap< StringView, DerivedContainer >;

        ByTypeInfoContainer CastersByTypeInfo;
        ByTypeNameContainer CastersByTypeName;

        PolymorphicCasterHolder() = default;
        PolymorphicCasterHolder(const PolymorphicCasterHolder& Other) = delete;
        PolymorphicCasterHolder(PolymorphicCasterHolder&& Other) = delete;
        ~PolymorphicCasterHolder() = default;

        PolymorphicCasterHolder& operator=(const PolymorphicCasterHolder& Other) = delete;
        PolymorphicCasterHolder& operator=(PolymorphicCasterHolder&& Other) = delete;

        template<class Base, class Derived>
        void RegisterCaster(std::shared_ptr<PolymorphicCasterImpl<Base,Derived>> NewCaster)
        {
            this->CastersByTypeInfo[ std::type_index( typeid(Base) ) ].push_back(NewCaster);
            this->CastersByTypeName[ GetRegisteredName<Base>() ].push_back(NewCaster);
        }

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

        template<class Base, class Derived>
        StoredCasterType GetCaster() const
        {
            return this->GetCaster( typeid(Base), typeid(Derived) );
        }
    };//PolymorphicCasterHolder

SAVE_WARNING_STATE
// Yes clang, we know.  It's a singleton pattern.
SUPPRESS_CLANG_WARNING("-Wexit-time-destructors")

    PolymorphicCasterHolder& GetPolymorphicCasterHolder();
    PolymorphicCasterHolder& GetPolymorphicCasterHolder()
    {
        static PolymorphicCasterHolder Store;
        return Store;
    }

RESTORE_WARNING_STATE

    template<class Base, class Derived>
    class PolymorphicCasterImpl final : public PolymorphicCaster
    {
    public:
        static_assert( std::is_constructible_v<Derived,const Int32,ObjectWalker&> ||
                       std::is_default_constructible_v<Derived>,
                       "Derived type must be default constructable or have a constructor that accepts "
                       "an Int and ObjectWalker." );
    public:
        static_assert( std::is_base_of_v<Base,Derived>, "\"Base\" is not a base class of Derived" );

        ~PolymorphicCasterImpl() = default;
        StringView GetBaseTypeName() const
        {
            return GetRegisteredName<Base>();
        }
        StringView GetDerivedTypeName() const
        {
            return GetRegisteredName<Derived>();
        }
        Boole CanUpcast(const std::type_info& BaseType, const std::type_info& DerivedType) const
        {
            return ( BaseType == typeid(Base) && DerivedType == typeid(Derived) );
        }
        Boole CanUpcast(const StringView BaseType, const StringView DerivedType) const
        {
            return ( BaseType == GetBaseTypeName() && DerivedType == GetDerivedTypeName() );
        }
        void Serialize(const StringView Name,
                       void* ToSerialize,
                       const MemberTags Tags,
                       const Int32 Version,
                       ObjectWalker& Walker)
        {
            Derived* Casted = static_cast<Derived*>( static_cast<Base*>(ToSerialize) );
            Mezzanine::Serialize(Name,std::forward<Derived*>(Casted),Tags,Version,Walker);
        }
        void Deserialize(const StringView Name,
                         void*& ToDeserialize,
                         const MemberTags Tags,
                         const Int32 Version,
                         ObjectWalker& Walker)
        {
            Derived* UpCasted = nullptr;
            if( ToDeserialize == nullptr ) {
                if constexpr( std::is_constructible_v<Derived,decltype(Version),decltype(Walker)> ) {
                    UpCasted = new Derived(Walker);
                }else if constexpr( std::is_default_constructible_v<Derived> ) {
                    UpCasted = new Derived();
                }
                ToDeserialize = UpCasted;
            }else{
                UpCasted = static_cast<Derived*>( static_cast<Base*>(ToDeserialize) );
            }
            Mezzanine::Deserialize(Name,std::forward<Derived*>(UpCasted),Tags,Version,Walker);
        }
    };//PolymorphicCasterImpl
#else
    template<class Derived>
    class PolymorphicCaster
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
    };//PolymorphicCaster

    template<class Base>

#endif
}//Serialization
    ///////////////////////////////////////////////////////////////////////////////
    // Caster Registration

    template<class Base, class Derived>
    void RegisterCaster()
    {
        using namespace Serialization;
        GetPolymorphicCasterHolder().RegisterCaster( std::make_shared<PolymorphicCasterImpl<Base,Derived>>() );
    }

    template<class Base, class... Deriveds>
    void RegisterCasters()
    {
        ( RegisterCaster<Base,Deriveds>() , ... );
    }

    /// @}
}//Mezzanine

#endif // Mezz_Foundation_SerializationPolymorphicCaster_h
