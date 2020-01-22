// © Copyright 2010 - 2019 BlackTopp Studios Inc.
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
#ifndef Mezz_Foundation_Serialization_h
#define Mezz_Foundation_Serialization_h

/// @file
/// @brief The collection of utilities that form the Serialization front-end.

#ifndef SWIG
    #include "Introspection.h"
    #include "StringTools.h"
#endif

namespace Mezzanine {
//namespace Serialization {
    /// @addtogroup Serialization
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    // ForwardDeclares

    ///////////////////////////////////////////////////////////////////////////////
    // Convenience type traits

    /// @brief Convenience type trait that decays the checked type before passing it to std::is_class.
    /// @tparam CheckType The type to decay and check if it is a class.
    template<class CheckType>
    struct is_class_decayed : std::is_class< std::remove_cv_t< std::remove_reference_t<CheckType> > >
        {  };

    /// @brief Convenience inline variable for getting just the bool of the is_class_decayed check.
    /// @tparam CheckType The type to check if it is a class type after being decayed.
    template<class CheckType>
    inline constexpr Boole is_class_decayed_v = is_class_decayed<CheckType>::value;

    /// @brief A type trait that checks to see if a type is a class that isn't a String or StringView.
    /// @tparam CheckType The type to check if it is any class type other than String or StringView.
    template<class CheckType>
    struct is_generic_serializable : std::bool_constant<!is_string_v<CheckType> && is_class_decayed_v<CheckType>>
        {  };

    /// @brief Convenience inline variable for getting just the bool of the is_non_string_class check.
    /// @tparam CheckType The type to check if it is a non-string class type.
    template<class CheckType>
    inline constexpr Boole is_generic_serializable_v = is_generic_serializable<CheckType>::value;

    ///////////////////////////////////////////////////////////////////////////////
    // Convenience detection traits

    /// @brief Convenience type for GetName method detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using GetName_t = decltype(std::declval<Class&>().GetName());
    /// @brief Convenience type for is_detected that tests for the existence of GetName on a class.
    /// @tparam Class The class that will be checked for the presence of a GetName method.
    template<typename Class>
    using HasGetName = std::is_detected<GetName_t,Class>;

    /// @brief Convenience type for GetID method detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using GetID_t = decltype(std::declval<Class&>().GetID());
    /// @brief Convenience type for is_detected that tests for the existence of GetID on a class.
    /// @tparam Class The class that will be checked for the presence of a GetID method.
    template<typename Class>
    using HasGetID = std::is_detected<GetID_t,Class>;

    /// @brief Convenience type for GetIdentification method detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using GetIdentification_t = decltype(std::declval<Class&>().GetIdentification());
    /// @brief Convenience type for is_detected that tests for the existence of GetIdentification on a class.
    /// @tparam Class The class that will be checked for the presence of a GetIdentification method.
    template<typename Class>
    using HasIdentificationName = std::is_detected<GetIdentification_t,Class>;

    ///////////////////////////////////////////////////////////////////////////////
    // Backend Interface

    class BackendBase
    {
    public:

    };//BackendBase

    ///////////////////////////////////////////////////////////////////////////////
    // Object

    class ObjectNode
    {
    public:
        StringView GetName() const = 0;

        AttributeNode* CreateAttribute(const StringView Name) = 0;
    };//Object

    ///////////////////////////////////////////////////////////////////////////////
    // Root Object
    // Can be handled by backend?  Is it needed?

    ///////////////////////////////////////////////////////////////////////////////
    // Attribute

    class AttributeNode
    {
    public:
        virtual void SetName(const StringView Name) = 0;
        virtual StringView GetName() const = 0;

        virtual void SetValue(const StringView Value) = 0;
        virtual StringView GetValue() const = 0;

        virtual void SetValue(const long double Value) = 0;
        virtual long double GetValue() const = 0;

        virtual void SetValue(const double Value) = 0;
        virtual double GetValue() const = 0;

        virtual void SetValue(const float Value) = 0;
        virtual float GetValue() const = 0;

        virtual void SetValue(const UInt64 Value) = 0;
        virtual UInt64 GetValue() const = 0;

        virtual void SetValue(const Int64 Value) = 0;
        virtual Int64 GetValue() const = 0;

        virtual void SetValue(const UInt32 Value) = 0;
        virtual UInt32 GetValue() const = 0;

        virtual void SetValue(const Int32 Value) = 0;
        virtual Int32 GetValue() const = 0;

        virtual void SetValue(const UInt16 Value) = 0;
        virtual UInt16 GetValue() const = 0;

        virtual void SetValue(const Int16 Value) = 0;
        virtual Int16 GetValue() const = 0;

        virtual void SetValue(const UInt8 Value) = 0;
        virtual UInt8 GetValue() const = 0;

        virtual void SetValue(const Int8 Value) = 0;
        virtual Int8 GetValue() const = 0;
    };//Attribute

    ///////////////////////////////////////////////////////////////////////////////
    // Object Link (for handling non-owned pointers between objects)

    ///////////////////////////////////////////////////////////////////////////////
    // Polymorphic Store
#if 1
    template<class Base, class Derived>
    class ObjectCaster;

    template<class Base>
    class ObjectCasterBase
    {
    public:
        virtual ~ObjectCasterBase() = default;
        virtual Boole CanUpcast(const std::type_info& DerivedType) const = 0;
        virtual void Serialize(const StringView Name, void* ToCast, const Integer Version, SerializerData Data) = 0;
        virtual void Deserialize(const StringView Name, void* ToCast, const Integer Version, SerializerData Data) = 0;
    };//ObjectCasterBase

    template<class Base>
    struct PolymorphicStore
    {
        using CasterBaseType = ObjectCasterBase<Base>;
        using CasterContainer = std::vector<CasterBaseType*>;

        static CasterContainer Casters;

        template<class Derived>
        static void RegisterCaster(ObjectCaster<Base,Derived>* NewCaster)
        {
            Casters.push_back(NewCaster);
        }
    };//PolymorphicStore

    template<class Base>
    typename PolymorphicStore<Base>::CasterContainer PolymorphicStore<Base>::Casters;

    template<class Base, class Derived>
    void RegisterCaster()
    {
        PolymorphicStore<Base>::RegisterCaster( new ObjectCaster<Base,Derived>() );
    }

    template<class Base, class... Deriveds>
    void RegisterCasters()
    {
        ( RegisterCaster<Base,Deriveds>() , ... );
    }

    template<class Base, class Derived>
    class ObjectCaster final : public ObjectCasterBase<Base>
    {
    public:
        static_assert( std::is_base_of_v<Base,Derived>, "\"Base\" is not a base class of Derived" );

        ~ObjectCaster() = default;
        Boole CanUpcast(const std::type_info& DerivedType) const
        {
            return ( DerivedType == typeid(Derived) );
        }
        void Serialize(const StringView Name, void* ToCast, const Integer Version, SerializerData Data)
        {
            Derived* Casted = static_cast<Derived*>( static_cast<Base*>(ToCast) );
            Mezzanine::Serialize(Name,Casted,Version,Data);
        }
        void Deserialize(const StringView Name, void* ToCast, const Integer Version, SerializerData Data)
        {
            Derived* Casted = static_cast<Derived*>( static_cast<Base*>(ToCast) );
            Mezzanine::Deserialize(Name,Casted,Version,Data);
        }
    };//ObjectCaster
#else
    template<class Derived>
    class ObjectCaster
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
    };//ObjectCaster

    template<class Base>

#endif
    ///////////////////////////////////////////////////////////////////////////////
    // SerializerContext

    class SerializerContextBase
    {
    public:
        virtual SerializerContextBase* GetParentContext() const = 0;

        virtual Boole IsSame(void* ObjectPtr, const std::type_info& Info) const = 0;

        virtual void* FindContextObject(const StringView Name, const std::type_index Type) const = 0;
        virtual void* FindContextObject(const UInt64 ID, const std::type_index Type) const = 0;
    };//SerializerContextBase

    template<typename SpecialContext>
    class SerializerContext : public SerializerContextBase
        {  };

    template<typename SpecialContext>
    SerializerContextBase* UpdateContext(const SerializerContextBase* Current, const SpecialContext* ToUpdate)
    {
        using ContextType = SerializerContext<SpecialContext>;
        if constexpr( !std::is_abstract_v<ContextType> ) {
            static_assert( std::is_constructible_v<ContextType,SerializerContextBase*,SpecialContext*> ,
                           "SerializerContext type lacks well-formed constructor." );
            if( Current != nullptr && !Current->IsSame(ToUpdate,typeid(SpecialContext)) ) {
                return new ContextType(Current,ToUpdate);
            }
        }
        return Current;
    }

    template<typename SpecialContext>
    SerializerContextBase* RevertContext(const SerializerContextBase* Current, const SpecialContext* ToRevert)
    {
        using ContextType = SerializerContext<SpecialContext>;
        if constexpr( !std::is_abstract_v<ContextType> ) {
            if( Current != nullptr && Current->IsSame(ToRevert,typeid(SpecialContext)) ) {
                SerializerContextBase* Reverted = Current->GetParentContext();
                delete Current;
                return Reverted;
            }
        }
        return Current;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // SerializerData

    struct MEZZ_LIB SerializerData
    {
        BackendBase* Backend = nullptr;
        SerializerContextBase* Context = nullptr;

        SerializerData() = default;
        SerializerData(BackendBase* Back) :
            Backend(Back)
            {  }
    };

    ///////////////////////////////////////////////////////////////////////////////
    // Serialize

    template<class SerializeType, typename = std::enable_if_t< is_generic_serializable_v<SerializeType> > >
    void Serialize(const StringView Name, const SerializeType& ToSerialize, const Integer Version, SerializerData Data)
    {
        SerializeAllMembers(ToSerialize,Version,Data);
    }
    template<class SerializeType, typename = std::enable_if_t< std::is_pointer_v<SerializeType> > >
    void Serialize(const StringView Name, const SerializeType ToSerialize, const Integer Version, SerializerData Data)
    {
        if constexpr( std::is_polymorphic_v< SerializeType > ) {

        }else{

        }
    }
    template<class SerializeType, typename = std::enable_if_t< is_string_v<SerializeType> > >
    void Serialize(const StringView Name, const SerializeType& ToSerialize, const Integer Version, SerializerData Data)
    {

    }
    template<class SerializeType, typename = std::enable_if_t< std::is_arithmetic_v<SerializeType> > >
    void Serialize(const StringView Name, const SerializeType& ToSerialize, const Integer Version, SerializerData Data)
    {

    }
    template<class SerializeType>
    void SerializeAllMembers(const SerializeType& ToSerialize, const Integer Version, SerializerData Data)
    {
        if constexpr( IsRegistered<SerializeType>() ) {
            DoForAllMembers([=](const auto& Member){
                Serialize( Member.GetName(), Member.GetValue(), Version, Data );
            });
        }
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Deserialize

    template<class SerializeType>
    void Deserialize(SerializeType& ToSerialize, SerializerContextBase* Context, BackendBase* Backend)
    {

    }
    template<class SerializeType>
    void Deserialize(SerializeType* ToSerialize, SerializerContextBase* Context, BackendBase* Back)
    {

    }
    template<class SerializeType>
    void Deserialize(SerializeType* ToSerialize, SerializerContextBase* Context, BackendBase* Back)
    {

    }

    ///////////////////////////////////////////////////////////////////////////////
    // Serializer

    class Serializer
    {
    protected:
        SerializerData Data;
    public:
        Serializer(BackendBase* Backend) :
            Data(Backend)
            {  }


        SerializerContextBase* GetContext() const
            { return this->Data.Context; }

        template<typename ObjectType, typename IDType>
        ObjectType* FindContextObject(const IDType ID)
        {
            if( this->Data.Context != nullptr ) {
                void* Found = this->Data.Context->FindContextObject(ID,std::type_index(typeid(ObjectType)));
                return reinterpret_cast<ObjectType>(Found);
            }
            return nullptr;
        }
    };//Serializer

    /// @}
//}//Serialization
}//Mezzanine

#endif
