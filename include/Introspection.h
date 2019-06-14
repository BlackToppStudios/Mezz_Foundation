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
#ifndef Mezz_Foundation_Introspection_h
#define Mezz_Foundation_Introspection_h

#ifndef SWIG
    #include <tuple>
    #include "DataTypes.h"
    #include "BitFieldTools.h"
#endif

#if __cplusplus <= 201703L
// A hack to allow easier exposure to std::is_detected.  Delete this when we update to C++20.
#ifndef SWIG
    #include <experimental/type_traits>
#endif
namespace std
{
#if defined(_MSC_VRE) && !defined(__INTEL_COMPILER)
    // "MSVC is a good compiler" they said.
    // "No reason to use any other" they said.
    // I fart in their general direction.
    struct nonesuch
    {
        nonesuch() = delete;
        ~nonesuch() = delete;
        nonesuch(nonesuch const&) = delete;
        void operator=(nonesuch const&) = delete;
    };

    namespace detail
    {
        template <class Default, class AlwaysVoid, template<class...> class Op, class... Args>
        struct detector
        {
            using value_t = std::false_type;
            using type = Default;
        };

        template <class Default, template<class...> class Op, class... Args>
        struct detector<Default, std::void_t<Op<Args...>>, Op, Args...>
        {
            // Note that std::void_t is a C++17 feature
            using value_t = std::true_type;
            using type = Op<Args...>;
        };
    } // namespace detail

    template <template<class...> class Op, class... Args>
    using is_detected = typename detail::detector<nonesuch, void, Op, Args...>::value_t;

    template <template<class...> class Op, class... Args>
    using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;

    template <class Default, template<class...> class Op, class... Args>
    using detected_or = detail::detector<Default, void, Op, Args...>;
#else
    template<template<typename...> class Op, typename... Args>
    using is_detected = std::experimental::is_detected<Op,Args...>;
    template<template<typename...> class Op, typename... Args>
    constexpr bool is_detected_v = std::experimental::is_detected_v<Op,Args...>;
#endif
}
#endif

namespace Mezzanine {
//namespace Introspection {
    /// @addtogroup Introspection
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    // Simple Aliases

    /// @brief A convenience type for a pointer to a member on a class.
    /// @tparam ClassType Can be any class or struct with a member.
    /// @tparam MemberType The type to point to on ClassType.
    /// @pre MemberType needs to be a type that exists on ClassType.
    template<typename ClassType, typename MemberType>
    using MemberPtrType = MemberType ClassType::*;

    /// @brief A convenience type for a setter method accepting a const reference and returning void.
    /// @tparam ClassType Can be any class or struct with a member.
    /// @tparam MemberType The type to point to on ClassType.
    /// @pre MemberType needs to be a type that is returned by the setter method that exists on the class.
    template<typename ClassType, typename MemberType>
    using RefSetterPtrType = void (ClassType::*)(const MemberType&);

    /// @brief A convenience type for a setter method accepting a move/universal reference and returning void.
    /// @tparam ClassType Can be any class or struct with a member.
    /// @tparam MemberType The type to point to on ClassType.
    /// @pre MemberType needs to be a type that is returned by the setter method that exists on the class.
    template<typename ClassType, typename MemberType>
    using MoveRefSetterPtrType = void (ClassType::*)(MemberType&&);

    /// @brief A convenience type for a setter method accepting a const value and returning void.
    /// @tparam ClassType Can be any class or struct with a member.
    /// @tparam MemberType The type to point to on ClassType.
    /// @pre MemberType needs to be a type that is returned by the setter method that exists on the class.
    template<typename ClassType, typename MemberType>
    using ValSetterPtrType = void (ClassType::*)(const MemberType);

    /// @brief A convenience type for a setter method accepting a value and returning void.
    /// @tparam ClassType Can be any class or struct with a member.
    /// @tparam MemberType The type to point to on ClassType.
    /// @pre MemberType needs to be a type that is returned by the setter method that exists on the class.
    template<typename ClassType, typename MemberType>
    using NonConstValSetterPtrType = void (ClassType::*)(MemberType);

    /// @brief A convenience type for a getter method accepting no parameters and returning by const reference.
    /// @tparam ClassType Can be any class or struct with a member.
    /// @tparam MemberType The type returned by the getter method.
    /// @pre MemberType needs to be a type that is returned by the getter method that exists on the class.
    template<typename ClassType, typename MemberType>
    using RefGetterPtrType = const MemberType& (ClassType::*)() const;

    /// @brief A convenience type for a setter method accepting no parameters and returning by reference.
    /// @tparam ClassType Can be any class or struct with a member.
    /// @tparam MemberType The type to point to on ClassType.
    /// @pre MemberType needs to be a type that is returned by the getter method that exists on the class.
    template<typename ClassType, typename MemberType>
    using NonConstRefGetterPtrType = MemberType& (ClassType::*)();

    /// @brief A convenience type for a getter method accepting no parameters and returning by value.
    /// @tparam ClassType Can be any class or struct with a member.
    /// @tparam MemberType The type to point to on ClassType.
    /// @pre MemberType needs to be a type that is returned by the getter method that exists on the class.
    template<typename ClassType, typename MemberType>
    using ValGetterPtrType = MemberType (ClassType::*)() const;

    ///////////////////////////////////////////////////////////////////////////////
    // Simple Container Aliases

    /// @brief A convenience type for a pointer to a bounded member array on a class.
    /// @tparam ClassType Can be any class or struct with a member.
    /// @tparam MemberType The type to point to on ClassType.
    /// @tparam Num The number of elements that can be stored in the array.
    /// @pre MemberType needs to be a type that exists on ClassType.
    template<class ClassType, class MemberType, size_t Num>
    using MemberArrayPtrType = MemberType(ClassType::*)[Num];

    ///////////////////////////////////////////////////////////////////////////////
    // Method Types Deduction

    /// @brief A deliberately empty setter deduction struct to catch error cases.
    /// @tparam T Any type, doesn't matter.  This will catch anything not caught by specializations.
    template<typename T>
    struct DeduceSetterTypes
        {  };

    /// @brief A DeduceSetterTypes specialization for a setter member method.
    /// @tparam Argument The setter parameter type that will be deduced.
    /// @tparam Object The object type that will be deduced.
    template<class Argument, class Object>
    struct DeduceSetterTypes<void(Object::*)(Argument)>
    {
        /// @brief The object type to which the setter method belongs.
        using ObjectType = Object;
        /// @brief The argument accepted by the setter method.
        using ArgumentType = Argument;
        /// @brief The decayed type of Argument, stripped of reference and const qualifiers.
        using DecayedType = std::decay_t<Argument>;
    };//DeduceSetterTypes

    /// @brief A DeduceSetterTypes specialization for a non-const reference setter member method.
    /// @tparam Argument The setter parameter type that will be deduced.
    /// @tparam Object The object type that will be deduced.
    template<class Argument, class Object>
    struct DeduceSetterTypes<Argument&(Object::*)()>
    {
        /// @brief The object type to which the setter method belongs.
        using ObjectType = Object;
        /// @brief The argument accepted by the setter method.
        using ArgumentType = Argument;
        /// @brief The decayed type of Argument, stripped of reference and const qualifiers.
        using DecayedType = std::decay_t<Argument>;
    };//DeduceSetterTypes

    /// @brief A DeduceSetterTypes specialization for a direct member pointer.
    /// @tparam Member The member pointer that will be deduced.
    /// @tparam Object The object type that will be deduced.
    template<class Member, class Object>
    struct DeduceSetterTypes<Member Object::*>
    {
        /// @brief The object to which the member belongs.
        using ObjectType = Object;
        /// @brief The member type being pointed to.
        using ArgumentType = Member;
        /// @brief The decayed type of Member, stripped of reference and const qualifiers.
        using DecayedType = std::decay_t<Member>;
    };//DeduceSetterTypes

    /// @brief A deliberately empty getter deduction struct to catch error cases.
    /// @tparam T Any type, doesn't matter.  This will catch anything not caught by specializations.
    template<typename T>
    struct DeduceGetterTypes
        {  };

    /// @brief A DeduceGetterTypes specialization for a reference getter member method.
    /// @tparam Return The getter return type that will be deduced.
    /// @tparam Object The object type that will be deduced.
    template<class Return, class Object>
    struct DeduceGetterTypes<Return(Object::*)() const>
    {
        /// @brief The object type to which the getter method belongs.
        using ObjectType = Object;
        /// @brief The type returned by the getter method.
        using ReturnType = Return;
        /// @brief The decayed type of Return, stripped of reference and const qualifiers.
        using DecayedType = std::decay_t<Return>;
    };//DeduceGetterTypes

    /// @brief A DeduceGetterTypes specialization for a non-const reference getter member method.
    /// @tparam Return The getter return type that will be deduced.
    /// @tparam Object The object type that will be deduced.
    template<class Return, class Object>
    struct DeduceGetterTypes<Return&(Object::*)()>
    {
        /// @brief The object type to which the getter method belongs.
        using ObjectType = Object;
        /// @brief The type returned by the getter method.
        using ReturnType = Return;
        /// @brief The decayed type of Return, stripped of reference and const qualifiers.
        using DecayedType = std::decay_t<Return>;
    };//DeduceGetterTypes

    /// @brief A DeduceGetterTypes specialization for a direct member pointer.
    /// @tparam Member The member pointer that will be deduced.
    /// @tparam Object The object type that will be deduced.
    template<class Member, class Object>
    struct DeduceGetterTypes<Member Object::*>
    {
        /// @brief The object to which the member belongs.
        using ObjectType = Object;
        /// @brief The member type being pointed to.
        using ReturnType = Member;
        /// @brief The decayed type of Member, stripped of reference and const qualifiers.
        using DecayedType = std::decay_t<Member>;
    };//DeduceGetterTypes

    ///////////////////////////////////////////////////////////////////////////////
    // Array Deduction

    /// @brief A catch-all type-trait to detect member array pointer types.
    /// @tparam T Any type.  This has no preconditions since this struct represents the failure case.
    /// @remarks This struct is meant to be caught by all types that are NOT member array pointer types.
    template<typename T>
    struct is_member_array : std::false_type
        {  };

    /// @brief A type-trait specialization for the detection of member array pointer types.
    /// @tparam Member The data member type stored in the array.
    /// @tparam Object The object on which the member array exists.
    /// @tparam Num The number of elements that can be stored in the array.
    template<class Member, class Object, size_t Num>
    struct is_member_array<Member(Object::*)[Num]> : std::true_type
        {  };

    ///////////////////////////////////////////////////////////////////////////////
    // Member Tags

    /*
    // These tags are left over from a short-lived attempt at using tags as types for the metadata of individual
    // class members.  The idea being that there be a third variadic template parameter for each created member
    // that contains all the tags appropriate for that member (most being empty) which would then get stored in
    // a tuple and (using) aliased on that member type.  That type could then be queried using the type traits
    // below (tuple_has_type) to generate custom behavior for specific types of data.
    // This system is unworkable because of the run-time polymorphic requirements of the serialization backends
    // that are planned for the system.  There is no way to pass around arbitrary and concrete types in a sane
    // manner through a virtual interface.  Furthermore, the likelihood that arbitrary tag types will be needed
    // is very low as it would require the user to write (or modify) their own serialization backends to leverage
    // the flexibility.  This is a significant coding burden that we shouldn't plan on as library maintainers.
    // For these reasons we've shelved explicit types as member tags in favor of an enum (MemberTags), despite
    // it's limitations, as we don't expect to be all that hindered by them in all but the most extreme use cases.
    // Should the run-time polymorphic requirement of the serialization system be lifted then it is possible that
    // some performance benefits could be gained by reverting to this approach to member tags.  That is why
    // this code is left commented and this documentation was written.

    struct Local_Tag
        {  };

    struct Generated_Tag
        {  };
    */

    /// @brief A collection of tags describing how member data should be treated in some cases.
    /// @remarks This enum is meant to be used to determine when special steps need to be taken when looking at
    /// member data.  For example a networked serialization backend might skip any member with the Local tag.
    enum class MemberTags
    {
        None      = EnumBit(0), ///< Nothing special about the member.
        Local     = EnumBit(1), ///< Member is only of use on the local host and should not be shared.
        Generated = EnumBit(2)  ///< Member is a cache generated from other members and can be regenerated.
    };
    ENABLE_BITMASK_OPERATORS_INSIDE_MEZZANINE(MemberTags)

    /// @brief Stream to standard output stream operator.
    /// @param Stream The standard output to stream to.
    /// @param TagToStream The MemberTag value that will be converted and streamed.
    /// @return Returns a reference to the standard output stream.
    std::ostream& operator<<(std::ostream& Stream, const MemberTags TagToStream);//[-Werror=missing-declarations]
    std::ostream& operator<<(std::ostream& Stream, const MemberTags TagToStream)
    {
        Stream << static_cast< std::underlying_type_t<MemberTags> >(TagToStream);
        return Stream;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Custom Tuple Type Traits and Utilities

    /// @brief A (dummy) type-trait for detecting if a tuple contains a given type.
    /// @tparam Type The type to check for.
    /// @tparam NotTuple See remarks.
    /// @remarks Because this is the dummy failure implementation, anything not a tuple will end up here.
    template<typename Type, typename NotTuple>
    struct tuple_has_type : std::false_type
        {  };
    /// @brief A (empty tuple specialization) type-trait for detecting if a tuple contains a given type.
    /// @tparam Type The type to check for.
    /// @remarks Empty tuples will end up using this specialization.
    template<typename Type>
    struct tuple_has_type<Type,std::tuple<>> : std::false_type
        {  };
    /// @brief A type-trait for detecting if a tuple contains a given type.
    /// @tparam Type The type to check for.
    /// @tparam TupleTypes A variadic template of types that will be deduced from a tuple.
    /// @remarks Actual tuples with types will end up in this specialization and be checked.
    template<typename Type, typename... TupleTypes>
    struct tuple_has_type<Type,std::tuple<TupleTypes...>> : std::disjunction<std::is_same<Type,TupleTypes>...>
        {  };

    template<size_t Idx, class Tuple, size_t... Idxs>
    constexpr auto element_as_tuple(Tuple&& ToCat, std::index_sequence<Idxs...>)
    {
        using T = std::remove_reference_t<Tuple>;
        if constexpr( !( std::is_same_v<std::tuple_element_t<Idx,T>,std::tuple_element_t<Idxs,T>> || ... ) ) {
            return std::forward_as_tuple( std::get<Idx>( std::forward<Tuple>(ToCat) ) );
        }else{
            return std::make_tuple();
        }
    }

    template<class Tuple, size_t... Idxs>
    constexpr auto tuple_cat_unique(Tuple&& ToCat, std::index_sequence<Idxs...>)
    {
        return std::tuple_cat(
            element_as_tuple<Idxs>( std::forward<Tuple>(ToCat), std::make_index_sequence<Idxs>{} )...
        );
    }

    template<typename... Tuples>
    auto tuple_cat_unique(Tuples&&... ToCat)
    {
        auto all = std::tuple_cat(std::forward<Tuples>(ToCat)...);
        return tuple_cat_unique( std::move(all), std::make_index_sequence<std::tuple_size_v<decltype(all)>>{} );
    }//*/

    ///////////////////////////////////////////////////////////////////////////////
    // MemberAccessor

    /// @brief A simple accessor class for assigning and retrieving a data member on a class.
    /// @tparam SetterMethod The member pointer type to use for setting the member value.
    /// @tparam GetterMethod The member pointer type to use for getting the member value.
    /// @tparam Tags A value or mask of values from the MemberTags enum describing extra details about the member.
    /// @pre SetterMethod This is expected to be a member pointer with both the object type and decayed member type
    /// matching that of GetterMethod.
    /// @pre GetterMethod This is expected to be a member pointer with both the object type and decayed member type
    /// matching that of SetterMethod.
    /// @details The template parameters of this class aren't meant to be provided explicitly.  The normal use case
    /// is for them to be deduced by a call to MakeMemberAccessor.  If they need to be specified without invoking
    /// MakeMemberAccessor then it's best to decltype the member pointers intended for the MemberAccessor
    /// constructor.
    /// @n @n
    /// There are a few important notes about constructing a MemberAccessor.  First, for convenience, it's
    /// possible to provide only a single common setter/getter parameter to the constructor which will be used as
    /// both a setter and a getter.  This feature is only available if the parameter is a direct to member
    /// pointer, or a pointer to a non-const reference returning method.  For anything else attempting to provide
    /// a common setter/getter will fail. @n
    /// Second, it is possible (although not recommended) to supply a nullptr if you want to disable setting or
    /// retrieving values of a member via a MemberAccessor.  Method types still need to be supplied, but the value
    /// passed into the constructor may be a nullptr. @n
    /// Lastly, due to unnecessary complexity that would be required to support them, direct pointers to member
    /// array types are not supported.  Arrays can still be used by classes using the introspection system, however
    /// they require the use of a MemberAccessor per array position that is initialized with the corresponding
    /// set/get methods for that array position.
    template<typename SetterMethod, typename GetterMethod, MemberTags Tags>
    class MemberAccessor
    {
    public:
        /// @brief Convenience type for deducing the various types belonging to the setter method.
        using SetDeducer = DeduceSetterTypes<SetterMethod>;
        /// @brief Convenience type for deducing the various types belonging to the getter method.
        using GetDeducer = DeduceGetterTypes<GetterMethod>;

        static_assert( std::is_member_pointer_v<SetterMethod>,
                       "Setter method is not a member of a class!" );
        static_assert( std::is_member_pointer_v<GetterMethod>,
                       "Getter method is not a member of a class!" );
        static_assert( std::is_same_v<typename SetDeducer::ObjectType,typename GetDeducer::ObjectType>,
                       "Object types don't match!");
        static_assert( std::is_same_v<typename SetDeducer::DecayedType,typename GetDeducer::DecayedType>,
                       "Member types don't match!");
        static_assert( !is_member_array<SetterMethod>::value,
                       "Member Array types aren't supported!  Use get/set methods instead." );
        static_assert( !is_member_array<GetterMethod>::value,
                       "Member Array types aren't supported!  Use get/set methods instead." );

        /// @brief The class type the member being accessed belongs to.
        using ClassType = typename GetDeducer::ObjectType;
        /// @brief The member type being accessed by this.
        using MemberType = typename GetDeducer::DecayedType;
        /// @brief The type returned by the getter method.
        using ReturnType = typename GetDeducer::ReturnType;
        /// @brief The member pointer type of the setter method.
        using SetterAccessPtr = SetterMethod;
        /// @brief The member pointer type of the getter method.
        using GetterAccessPtr = GetterMethod;

        ///////////////////////////////////////////////////////////////////////////////
        // Init Checks

        /// @brief Checks to see if a type is MemberPtrType and can be used to set and get member values.
        /// @tparam ConstructFrom The setter or getter method type to be tested.
        /// @return Returns true if ConstructFrom can be used as a setter and getter method, false otherwise.
        template<typename ConstructFrom>
        static constexpr Boole CanMemberPtrInitialize()
        {
            return std::is_same_v<ConstructFrom,MemberPtrType<ClassType,MemberType>> &&
                   std::is_same_v<ConstructFrom,SetterAccessPtr> &&
                   std::is_same_v<ConstructFrom,GetterAccessPtr>;
        }
        /// @brief Checks to see if a type is NonConstRefGetterPtrType and can be used to set and get member values.
        /// @tparam ConstructFrom The setter or getter method type to be tested.
        /// @return Returns true if ConstructFrom can be used as a setter and getter method, false otherwise.
        template<typename ConstructFrom>
        static constexpr Boole CanRefReturnInitialize()
        {
            return std::is_same_v<ConstructFrom,NonConstRefGetterPtrType<ClassType,MemberType>> &&
                   std::is_same_v<ConstructFrom,SetterAccessPtr> &&
                   std::is_same_v<ConstructFrom,GetterAccessPtr>;
        }
        /// @brief Gets whether or not an access method can be used to set and get member values.
        /// @tparam ConstructFrom The setter or getter method type to be tested.
        /// @return Returns true if ConstructFrom can be used as a setter and getter method, false otherwise.
        template<typename ConstructFrom>
        static constexpr Boole CanSingleMemberInitialize()
        {
            return MemberAccessor::CanMemberPtrInitialize<ConstructFrom>() ||
                   MemberAccessor::CanRefReturnInitialize<ConstructFrom>();
        }
        /// @brief Tests where or not a type can be used to construct or assign to the member type.
        /// @tparam SetFrom The value type to test.
        /// @return Returns true if the template parameter type can be use to construct or assign to a member type.
        template<typename SetFrom>
        static constexpr Boole CanSetFrom()
        {
            return std::is_convertible_v<SetFrom,MemberType>;
        }
    private:
        /// @brief A pointer to the function setting the member or to the member itself.
        SetterAccessPtr SetterPtr;
        /// @brief A pointer to the function getting the member or to the member itself.
        GetterAccessPtr GetterPtr;
        /// @brief The name of the member being accessed.
        StringView MemberName;
    public:
        /// @brief Deleted parameterless constructor.
        constexpr MemberAccessor() = delete;
        /// @brief Copy Constructor.
        /// @param Other The other MemberAccessor to be copied.
        constexpr MemberAccessor(const MemberAccessor& Other) = default;
        /// @brief Move Constructor.
        /// @param Other The other MemberAccessor to be moved.
        constexpr MemberAccessor(MemberAccessor&& Other) = default;
        /// @brief Common Method Constructor.
        /// @tparam ConstructFrom The deduced type to be used as both the setter and getter method.
        /// @remarks This method uses SFINAE to disable use of this method if unsupported types are attempted
        /// with this method.
        /// @param Name The name of the member.
        /// @param Ptr The method to use for assigning AND retrieving the value of the member.
        template<typename ConstructFrom, typename = std::enable_if_t< CanSingleMemberInitialize<ConstructFrom>() >>
        constexpr MemberAccessor(const StringView Name, ConstructFrom Ptr) :
            SetterPtr(Ptr),
            GetterPtr(Ptr),
            MemberName(Name)
            {  }
        /// @brief Descriptive Constructor.
        /// @param Name The name of the member.
        /// @param Setter The method to use for assigning values to the member.
        /// @param Getter The method to use for retrieving the value of the member.
        constexpr MemberAccessor(const StringView Name, SetterAccessPtr Setter, GetterAccessPtr Getter) :
            SetterPtr(Setter),
            GetterPtr(Getter),
            MemberName(Name)
            {  }

        ///////////////////////////////////////////////////////////////////////////////
        // Set and Get Value

        /// @brief Sets the value of a member on an initialized object.
        /// @tparam SetFrom The deduced type to be used to assign the new value for the member.
        /// @remarks This method uses SFINAE to disable use of this method if the provided value cannot be
        /// assigned to the member type.
        /// @param Object The object to assign the member value to.
        /// @param Arg The value to be assigned.
        template<typename SetFrom, typename = std::enable_if_t< CanSetFrom<SetFrom>() >>
        void SetValue(ClassType& Object, SetFrom&& Arg) const
        {
            if( this->HasSetter() ) {
                if constexpr( std::is_same_v<SetterAccessPtr,MemberPtrType<ClassType,MemberType>> ) {
                    (Object.*SetterPtr) = Arg;
                }else if constexpr( std::is_same_v<SetterAccessPtr,NonConstRefGetterPtrType<ClassType,MemberType>> ) {
                    (Object.*SetterPtr)() = Arg;
                }else{
                    (Object.*SetterPtr)(Arg);
                }
            }else{
                throw std::runtime_error("Cannot Set Member value, no setter provided.");
            }
        }
        /// @brief Gets the value of a member on an initialized object.
        /// @param Object The object to retrieve the member value from.
        /// @return Returns the value of the member.
        ReturnType GetValue(const ClassType& Object) const
        {
            if( this->HasGetter() ) {
                if constexpr( std::is_same_v<GetterAccessPtr,MemberPtrType<ClassType,MemberType>> ) {
                    return (Object.*GetterPtr);
                }else{
                    return (Object.*GetterPtr)();
                }
            }
            throw std::runtime_error("Cannot Get Member value, no getter provided.");
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Simple Query

        /// @brief Gets the name of the member being accessed.
        /// @return Returns a C-String containing the name of the member.
        constexpr StringView GetName() const
            { return this->MemberName; }
        /// @brief Checks to see if a "setter" method has been set.
        /// @return Returns true if a value can be assigned with this accessor, false otherwise.
        Boole HasSetter() const
            { return this->SetterPtr != nullptr; }
        /// @brief Checks to see if a "getter" method has been set.
        /// @return Returns true if a value can be retrieved with this accessor, false otherwise.
        Boole HasGetter() const
            { return this->GetterPtr != nullptr; }

        /// @brief Retrieves the metadata tags associated with this member.
        /// @return Returns a MemberTags value or mask of values describing extra details about the member.
        static constexpr MemberTags GetTags()
            { return Tags; }
    };//MemberAccessor

    ///////////////////////////////////////////////////////////////////////////////
    // Making Member Accessors

    /// @brief Convenience method to initialize a MemberAccessor from deduced types.
    /// @tparam CommonMethod The deduced type to use as both the setter and getter method of the MemberAccessor.
    /// @pre CommonMethod should be either a direct to member pointer to a pointer to a member function accepting
    /// no arguments and returning a non-const reference to the member.  Other types will fail.
    /// @param Name The name of the member.
    /// @param Common A member pointer to the common set/get method for the member.
    /// @return Returns a new MemberAccessor initialized with the provided values.
    template<typename CommonMethod>
    constexpr auto MakeMemberAccessor(const StringView Name, CommonMethod Common)
        { return MemberAccessor<CommonMethod,CommonMethod,MemberTags::None>(Name,Common); }
    /// @brief Convenience method to initialize a MemberAccessor from deduced types.
    /// @tparam Tags A MemberTags bitmask to describe the member.
    /// @tparam CommonMethod The deduced type to use as both the setter and getter method of the MemberAccessor.
    /// @pre CommonMethod should be either a direct to member pointer to a pointer to a member function accepting
    /// no arguments and returning a non-const reference to the member.  Other types will fail.
    /// @param Name The name of the member.
    /// @param Common A member pointer to the common set/get method for the member.
    /// @return Returns a new MemberAccessor initialized with the provided values.
    template<MemberTags Tags, typename CommonMethod>
    constexpr auto MakeMemberAccessor(const StringView Name, CommonMethod Common)
        { return MemberAccessor<CommonMethod,CommonMethod,Tags>(Name,Common); }
    /// @brief Convenience method to initialize a MemberAccessor from deduced types.
    /// @tparam SetterMethod The member pointer type to use for setting the member value.
    /// @tParam GetterMethod The member pointer type to use for getting the member value.
    /// @pre SetterMethod Preconditions are the same as what is stated in the @ref MemberAccessor docs.
    /// @pre GetterMethod Preconditions are the same as what is stated in the @ref MemberAccessor docs.
    /// @param Name The name of the member.
    /// @param Setter A member pointer to be used for assignment the member value.
    /// @param Getter A member pointer to be used to retrieve the member value.
    /// @return Returns a new MemberAccessor initialized with the provided values.
    template<typename SetterMethod, typename GetterMethod>
    constexpr auto MakeMemberAccessor(const StringView Name, SetterMethod Setter, GetterMethod Getter)
        { return MemberAccessor<SetterMethod,GetterMethod,MemberTags::None>(Name,Setter,Getter); }
    /// @brief Convenience method to initialize a MemberAccessor from deduced types.
    /// @tparam Tags A MemberTags bitmask to describe the member.
    /// @tparam SetterMethod The member pointer type to use for setting the member value.
    /// @tParam GetterMethod The member pointer type to use for getting the member value.
    /// @pre SetterMethod Preconditions are the same as what is stated in the @ref MemberAccessor docs.
    /// @pre GetterMethod Preconditions are the same as what is stated in the @ref MemberAccessor docs.
    /// @param Name The name of the member.
    /// @param Setter A member pointer to be used for assignment the member value.
    /// @param Getter A member pointer to be used to retrieve the member value.
    /// @return Returns a new MemberAccessor initialized with the provided values.
    template<MemberTags Tags, typename SetterMethod, typename GetterMethod>
    constexpr auto MakeMemberAccessor(const StringView Name, SetterMethod Setter, GetterMethod Getter)
        { return MemberAccessor<SetterMethod,GetterMethod,Tags>(Name,Setter,Getter); }

    ///////////////////////////////////////////////////////////////////////////////
    // (Dummy) Registration

    /// @brief Dummy implementation for registering class members.
    /// @tparam Class The class whose data members will be registered.
    /// @remarks This method must be implemented for every class that wants to interact with the introspection
    /// system, except with actual MemberAccessor instances being passed into a call to Members.
    /// @return Returns a tuple of all the MemberAccessors needed to access the class being registered.
    template<typename Class>
    inline auto RegisterMembers()
        { return std::make_tuple(); }

    /// @brief Dummy implementation for registering class names.
    /// @tparam Class The class whose name will be registered.
    /// @remarks This method must be implemented for every class that wants to interact with the introspection
    /// system, except with an actual name to be associated with the class.
    /// @return Returns a CString containing the name of the class being registered.
    template<typename Class>
    constexpr StringView RegisterName()
        { return StringView(); }

    ///////////////////////////////////////////////////////////////////////////////
    // Introspection Internals

    namespace IntrospectionHelpers
    {
        /// @brief Convenience type for GetSerializableName method detection.
        /// @tparam Class The class to test.
        template<typename Class>
        using GetSerializableName_t = decltype(&Class::GetSerializableName);
        /// @brief Convenience type for is_detected that tests for the existence of GetSerializableName on a class.
        /// @tparam Class The class that will be checked for the presence of a GetSerializableName method.
        template<typename Class>
        using HasGetSerializableName = std::is_detected<GetSerializableName_t,Class>;

        /// @brief Convenience type for RegisterMembers method detection.
        /// @tparam Class The class to test.
        template<typename Class>
        using RegisterMembers_t = decltype(&Class::RegisterMembers);
        /// @brief Convenience type for is_detected that tests for the existence of RegisterMembers on a class.
        /// @tparam Class The class that will be checked for the presence of a RegisterMembers method.
        template<typename Class>
        using HasRegisterMembers = std::is_detected<RegisterMembers_t,Class>;

        /// @brief Gets the registered name of a class.
        /// @tparam Class The type to check the name for.
        /// @remarks This method exists to support different means of providing the tuple of member accessors to
        /// the introspection system.  Depending on the type and what is detected, the final method should have
        /// only have a 1 line implementation.
        /// @return Returns a String containing the registered name of the class.
        template<typename Class>
        constexpr StringView GetName()
        {
            if constexpr( HasGetSerializableName<Class>::value ) {
                return Class::GetSerializableName();
            }else{
                return RegisterName<Class>();
            }
        }

        /// @brief Gets the registered member accessors of a class.
        /// @tparam Class The type to get the member accessors of.
        /// @remarks This method exists to support different means of providing the tuple of member accessors to
        /// the introspection system.  Depending on the type and what is detected, the final method should have
        /// only have a 1 line implementation. @n
        /// Because of the MemberHolder class implementation, this method should only need to be called once per
        /// run of a given application.
        /// @return Returns a tuple of member accessors registered with a given class type.
        template<typename Class>
        decltype(auto) GetMembers()
        {
            if constexpr( HasRegisterMembers<Class>::value ) {
                return Class::RegisterMembers();
            }else{
                return RegisterMembers<Class>();
            }
        }

        /// @brief A convenience storage class for the tuple of registered member accessors of a given class.
        /// @tparam Class The class who's registered members will be stored.
        /// @tparam Tuple The type of tuple containing member accessors generated for a class.
        /// @pre Class Should be a class, no preconditions otherwise.
        /// @pre Tuple This must be the tuple type returned by the Class's RegisterMembers method.
        template<typename Class, typename Tuple>
        struct MemberHolder
        {
            /// @brief Convenience type of the tuple used that can be retrieved.
            using TupleType = Tuple;

            /// @brief A tuple container of the registered members for a given class type.
            static TupleType Members;
        };//MemberHolder

        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wglobal-constructors")

        // Static Initialization for MemberHolder static member.
        template<typename Class, typename Tuple>
        Tuple MemberHolder<Class,Tuple>::Members = GetMembers<Class>();//*/

        RESTORE_WARNING_STATE

        /// @brief The actual implementation for the TupleForEach.
        /// @tparam Funct Deduced type for the callback that will be called for each member of the tuple.
        /// @tparam ArgTypes Variadic template type for the members that will be passed into Funct.
        /// @tparam Index A collection of indexes to build the index sequence with.
        /// @param Callable The callback to be invoked for each member of the tuple.
        /// @param Args The tuple of (presumably) member types that will be passed into the callback individually.
        template<typename Funct, typename... ArgTypes, size_t... Index>
        void TupleForEachIndex(Funct&& Callable, const std::tuple<ArgTypes...>& Args, std::index_sequence<Index...>)
            { ( Callable(std::get<Index>(Args)), ... ); }

        /// @brief A ForEach implementation for the types and members of a tuple.
        /// @tparam Funct Deduced type for the callback that will be called for each member of the tuple.
        /// @tparam ArgTypes Variadic template type for the members that will be passed into Funct.
        /// @param Callable The callback to be invoked for each member of the tuple.
        /// @param Args The tuple of (presumably) member types that will be passed into the callback individually.
        template<typename Funct, typename... ArgTypes>
        void TupleForEach(Funct&& Callable, const std::tuple<ArgTypes...>& Args)
            { TupleForEachIndex(std::forward<Funct>(Callable),Args,std::index_sequence_for<ArgTypes...>{}); }

        /// @brief Specialization implementation for an empty tuple.
        /// @remarks This method is a no-op.
        /// @tparam Funct Deduced type for the callback that won't be invoked.
        /// @tparam ArgTypes Variadic template type that ought to be empty if this implementation is being used.
        /// @param Callable The callback that won't be invoked.
        /// @param Args An empty tuple.
        template<typename Funct, typename... ArgTypes>
        void TupleForEach(Funct&& Callable, const std::tuple<>& Args)
            { (void)Callable;  (void)Args; }
    }//IntrospectionHelpers

    ///////////////////////////////////////////////////////////////////////////////
    // Introspect Checks and Operations

    /// @brief Generates a tuple of class members from what is passed in.
    /// @tparam ArgTypes A collection of types representing the members of a class.
    /// @remarks This is intended for use with 1 or more MemberAccessor instances being passed in, but no
    /// enforcement is made that that happens.  If any else is passed in something will break.
    /// @param Args A list of members to be bundled together.
    /// @return Returns a Tuple containing all the parameters passed in.
    template<typename... ArgTypes>
    constexpr auto Members(ArgTypes&&... Args)
        { return std::make_tuple(std::forward<ArgTypes>(Args)...); }

    /// @brief Generates a tuple of class members from two or more classes.
    /// @tparam TupleTypes A collection of tuple types representing the class members to be merged.
    /// @remarks This is intended for use with 1 or more tuples of MemberAccessor instances being passed
    /// in, but no enforcement is made that that happens.  If any else is passed in something will break.
    /// @param ToMerge A list of MemberAccessor tuples to be bundled together.
    /// @return Returns a Tuple containing all the tuples merged into one bigger tuple.
    template<typename... TupleTypes>
    constexpr auto MergeMembers(TupleTypes&&... ToMerge)
        { return std::tuple_cat(std::forward<TupleTypes>(ToMerge)...); }

    template<typename... TupleTypes>
    constexpr auto MergeMembersUnique(TupleTypes&&... ToMerge)
        { return tuple_cat_unique(std::forward<TupleTypes>(ToMerge)...); }

    /// @brief Checks to see if a class type has members registered.
    /// @tparam Class The class type to check.
    /// @return Returns true if introspection is possible for members of the given class, false otherwise.
    template<typename Class>
    constexpr Boole IsRegistered()
    {
        using TupleType = decltype(IntrospectionHelpers::GetMembers<Class>());
        return !std::is_same_v<std::tuple<>,TupleType>;
    }

    /// @brief Gets the registered name of the class type.
    /// @tparam Class The class to check.
    /// @return Returns the registered name of the class, or an empty String if no name is registered.
    template<typename Class>
    constexpr StringView GetRegisteredName()
    {
        return IntrospectionHelpers::GetName<Class>();
    }

    /// @brief Gets the number of registered members for a class.
    /// @tparam Class The class to check.
    /// @return Returns the amount of members that have been registered for the class specified.
    template<typename Class>
    constexpr size_t GetMemberCount()
    {
        using TupleType = decltype(IntrospectionHelpers::GetMembers<Class>());
        return std::tuple_size_v<TupleType>;
    }

    /// @brief Gets a container of all the member accessors registered for a class.
    /// @tparam Class The class to retrieve accessors for.
    /// @return Returns a const reference to a tuple of member accessor types for each registered member.
    template<typename Class>
    const auto& GetMembers()
    {
        using TupleType = decltype(IntrospectionHelpers::GetMembers<Class>());
        return IntrospectionHelpers::MemberHolder<Class,TupleType>::Members;
    }

    /// @brief Invokes a callable for each registered member on a class.
    /// @warning SFINAE will disable this function if the class is not registered.
    /// @tparam Class Provided type who's members will be looped over.
    /// @tparam Funct Deduced parameter for the callback method to invoke with each member.
    /// @param Callable The callback that will be invoked for each member in the specified class.
    template<typename Class, typename Funct, typename = std::enable_if_t<IsRegistered<Class>()>>
    void DoForAllMembers(Funct&& Callable)
    {
        IntrospectionHelpers::TupleForEach(std::forward<Funct>(Callable),GetMembers<Class>());
    }

    /// @brief Dummy implementation of DoForAllMembers that is valid for non-registered classes.
    /// @warning SFINAE will disable this function if the class is registered.
    /// @tparam Class Provided type who's members will be looped over.
    /// @tparam Funct Deduced parameter for the callback method to invoke with each member.
    /// @remarks This exists primarily to help cut down on generated template code in the error case.
    /// @param Callable The callback that will NOT be invoked for each member in the specified class.
    template<typename Class, typename Funct, typename = std::enable_if_t<!IsRegistered<Class>()>, typename = void>
    void DoForAllMembers(Funct&& Callable)
    {
        (void)Callable;
        // Deliberately empty as a default dummy implementation
    }

    /// @brief Invokes a callable for a specific named member on a class.
    /// @tparam Class Provided type that will be searched for the named member.
    /// @tparam MemberType The type of the MemberAccessor that is expected to be passed into the callable.
    /// @tparam Funct Deduced parameter for the callback method to invoke with the named member.
    /// @param Name The name of the member to invoke the callback with.
    /// @param Callable The invokable that will be called for the named member in the specified class.
    template<typename Class, typename MemberType, typename Funct>
    void DoForMember(const StringView Name, Funct&& Callable)
    {
        DoForAllMembers<Class>(
            [&](const auto& Member) {
                if( Name == Member.GetName() ) {
                    using CurrType = typename std::decay_t<decltype(Member)>::MemberType;
                    static_assert( std::is_same_v<CurrType,MemberType>, "Member type isn't what was expected!" );
                    Callable(Member);
                }
            }
        );
    }

    /// @brief Checks to see if a class has a named member.
    /// @tparam Class Provided class type that will be checked.
    /// @param Name The name of the member to check for.
    /// @return Returns true of the specified class has a registered member with the provided name, false otherwise.
    template<typename Class>
    Boole HasMember(const StringView Name)
    {
        Boole Found = false;
        DoForAllMembers<Class>(
            [&](const auto& Member) {
                if( Name == Member.GetName() ) {
                    Found = true;
                }
            }
        );
        return Found;
    }

    /// @brief Convenience method for getting a specific named member on an object.
    /// @tparam MemberType The type of the member that will be returned.
    /// @tparam Class The class type to get the member value of.
    /// @param Obj The object instance to get the value from.
    /// @param Name The name of the member to retrieve on the object.
    template<typename MemberType, typename Class>
    MemberType GetMemberValue(const Class& Obj, const StringView Name)
    {
        MemberType Value{};
        DoForMember<Class,MemberType>(Name,
            [&](const auto& Member) {
                Value = Member.GetValue(Obj);
            }
        );
        return Value;
    }

    /// @brief Convenience method for setting the value of a specific named member on an object.
    /// @tparam MemberType The type of the member that will be returned.
    /// @tparam Class The class type to set the member value of.
    /// @tparam ValueType The type of the value to be converted to the named member type.
    /// @param Obj The object instance to set the value of.
    /// @param Name The name of the member to set on the object.
    /// @param Val The value that will be used to set the named member.
    template<typename MemberType,
             typename Class,
             typename ValueType,
             typename = std::enable_if_t< std::is_convertible_v<ValueType,MemberType> >>
    void SetMemberValue(Class& Obj, const StringView Name, ValueType&& Val)
    {
        DoForMember<Class,MemberType>(Name,
            [&Obj, Val = std::forward<ValueType>(Val)](const auto& Member) {
                Member.SetValue(Obj,Val);
            }
        );
    }

    /// @}
//}// Introspection
}// Mezzanine

#endif
