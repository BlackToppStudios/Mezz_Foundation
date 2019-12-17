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
#endif

namespace Mezzanine {
namespace Serialization {
    /// @addtogroup Serialization
    /// @{

    // Convenience type traits
    template<class Type>
    struct is_string_view_class :
        std::is_same<Type,std::string_view>
        {  };

    template<class Type>
    struct is_non_string_view_class :
        std::conditional<!is_string_view_class<Type> && std::is_class<Type>,std::true_type,std::false_type>
        {  };

    // Backend Interface
    class BackendBase
    {
    public:

    };//BackendBase

    // Polymorph Store
    class ObjectCasterBase
    {
    public:
        std::type_index GetType() const = 0;
        void CastAndSerialize(void* ToCast, Node* Data) = 0;
        void CastAndDeserialize(void* ToCast, Node* Data) = 0;
    };//ObjectCasterBase

    template<class Base, class Derived>
    class ObjectCaster
    {
    public:
        std::type_index GetType() const
        {
            return std::type_index<typeid(Derived)>;
        }
        void CastAndSerialize(void* ToCast, Node* Data)
        {
            Derived* Casted = static_cast<Derived>(ToCast);
            Serialize(Casted,Data);
        }
        void CastAndDeserialize(void* ToCast, Node* Data)
        {
            Derived* Casted = static_cast<Derived>(ToCast);
            Deserialize(Casted,Data);
        }
    };//ObjectCaster

    // Object
    class Object
    {
    public:
        StringView GetName() const = 0;

        template<typename Datum>
        Attribute* CreateAttribute(const StringView Name, Datum&& ToSerialize) = 0;
    };//Object

    // Root Object
    // Can be handled by backend?  Is it needed?

    // Attribute
    class Attribute
    {
    public:
        void SetName(const StringView Name) = 0;
        StringView GetName() const = 0;

        void SetValue(const StringView Value) = 0;
        StringView GetValue() const = 0;

        void SetValue(const long long double Value) = 0;
        long long double GetValue() const = 0;

        void SetValue(const long double Value) = 0;
        long double GetValue() const = 0;

        void SetValue(const double Value) = 0;
        double GetValue() const = 0;

        void SetValue(const float Value) = 0;
        float GetValue() const = 0;

        void SetValue(const UInt64 Value) = 0;
        UInt64 GetValue() const = 0;

        void SetValue(const Int64 Value) = 0;
        Int64 GetValue() const = 0;

        void SetValue(const UInt32 Value) = 0;
        UInt32 GetValue() const = 0;

        void SetValue(const Int32 Value) = 0;
        Int32 GetValue() const = 0;

        void SetValue(const UInt16 Value) = 0;
        UInt16 GetValue() const = 0;

        void SetValue(const Int16 Value) = 0;
        Int16 GetValue() const = 0;

        void SetValue(const UInt8 Value) = 0;
        UInt8 GetValue() const = 0;

        void SetValue(const Int8 Value) = 0;
        Int8 GetValue() const = 0;
    };//Attribute

    // Object Link (for handling non-owned pointers between objects)

    // SerializerContext
    class SerializerContextBase
    {
    public:
        void* GetContextObject() const = 0;
        SerializerContextBase* GetParentContext() const = 0;
    };//SerializerContextBase

    template<typename SpecialContext>
    class SerializerContext : public SerializerContextBase
        {  };

    // Serializer

    template<class SerializeType, typename = std::enable_if_t< std::is_non_string_view_class<SerializeType>::value > >
    void Serialize(const SerializeType& ToSerialize, SerializerContextBase* Context, BackendBase* Backend)
    {

    }
    template<class SerializeType, typename = std::enable_if_t< std::is_string_view_class<SerializeType>::value > >
    void Serialize(const SerializeType& ToSerialize, SerializerContextBase* Context, BackendBase* Backend)
    {

    }
    template<class SerializeType, typename = std::enable_if_t< std::is_pointer_v<SerializeType> > >
    void Serialize(const SerializeType ToSerialize, SerializerContextBase* Context, BackendBase* Back)
    {

    }
    template<class SerializeType, typename = std::enable_if_t< std::is_arithmetic_v<SerializeType> > >
    void Serialize(const SerializeType& ToSerialize, SerializerContextBase* Context, BackendBase* Back)
    {

    }
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

    class Serializer
    {
    protected:
        template<typename SpecialContext>
        void PushNewContext(SpecialContext* Object)
        {
            using ContextType = SerializerContext<SpecialContext>;
            if constexpr( !std::is_abstract_v<ContextType> ) {
                ContextType* NewContext = new ContextType(this->Context,Object);
                this->Context = NewContext;
            }
        }
        template<typename SpecialContext>
        void PopOldContext(SpecialContext* Object)
        {
            using ContextType = SerializerContext<SpecialContext>;
            if constexpr( !std::is_abstract_v<ContextType> ) {

            }
        }
    public:
        void* FindContextObject(const StringView Name, const std::type_index Type) = 0;
        void* FindContextObject(const UInt64 ID, const std::type_index Type) = 0;

        template<typename ObjectType, typename IDType>
        ObjectType* FindContextObject(const IDType ID)
        {
            void* Found = FindContextObject(ID,std::type_index(typeid(ObjectType)));
            return reinterpret_cast<ObjectType>(Found);
        }

        SerializerContext* GetContext() const = 0;
    };//Serializer

    class XMLSerializer
    {
    protected:
        SerializerContextBase* Context;

    public:
        SerializerContext* GetContext() const
            { return this->Context; }
    };

    /// @}
}//Serialization
}//Mezzanine

#endif
