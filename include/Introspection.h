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
#ifndef Mezz_Foundation_Introspection_h
#define Mezz_Foundation_Introspection_h

#ifndef SWIG
    #include <tuple>
    #include "DataTypes.h"
    #include "DetectionTraits.h"
    #include "BitFieldTools.h"
    #include "MezzException.h"
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
        using ArgumentType = Member&;
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
        /// @brief The type returned by the getter method when the object is const.
        using ConstReturnType = Return;
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
        using ReturnType = Return&;
        /// @brief The type returned by the getter method when the object is const.
        using ConstReturnType = const Return&;
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
        using ReturnType = Member&;
        /// @brief The type returned by the getter method when the object is const.
        using ConstReturnType = const Member&;
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
    // class members.  The idea being that there would be a third variadic template parameter for each created
    // member that contains all the tags appropriate for that member (most being empty) which would then get
    // stored in a tuple and (using) aliased on that member type.  That type could then be queried using the
    // type traits below (tuple_has_type) to generate custom behavior for specific types of data.
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
        None       = EnumBit(0), ///< Nothing special about the member.
        Ignore     = EnumBit(1), ///< Member has been explicitly requested to be ignored.
        Local      = EnumBit(2), ///< Member is only of use on the local host and should not be shared.
        Generated  = EnumBit(3), ///< Member is a cache generated from other members and can be regenerated.
        Deprecated = EnumBit(4), ///< Member is deprecated and it's use should be avoided.
        NotOwned   = EnumBit(5)  ///< Member is a Pointer or Reference to an object not owned by the parent object.
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

    SAVE_WARNING_STATE
    SUPPRESS_VC_WARNING(4371)

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

        /// @brief The type of the MemberAccessor instantiation in the immediate context.
        using SelfType = MemberAccessor<SetterMethod,GetterMethod,Tags>;
        /// @brief The class type the member being accessed belongs to.
        using ClassType = typename GetDeducer::ObjectType;
        /// @brief The member type being accessed by this.
        using MemberType = typename GetDeducer::DecayedType;
        /// @brief The type returned by the getter method.
        using ReturnType = typename GetDeducer::ReturnType;
        /// @brief The type returned by the getter method when the object is const.
        using ConstReturnType = typename GetDeducer::ConstReturnType;
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
        [[nodiscard]]
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
        [[nodiscard]]
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
        [[nodiscard]]
        static constexpr Boole CanSingleMemberInitialize()
        {
            return MemberAccessor::CanMemberPtrInitialize<ConstructFrom>() ||
                   MemberAccessor::CanRefReturnInitialize<ConstructFrom>();
        }
        /// @brief Tests where or not a type can be used to construct or assign to the member type.
        /// @tparam SetFrom The value type to test.
        /// @return Returns true if the template parameter type can be use to construct or assign to a member type.
        template<typename SetFrom>
        [[nodiscard]]
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
        /// @exception If the Setter Access pointer is nullptr, then an Mezzanine::Exception::IntrospectionNullptrCode
        /// will be thrown.
        /// @param Object The object to assign the member value to.
        /// @param Arg The value to be assigned.
        template<typename SetFrom, typename = std::enable_if_t< SelfType::CanSetFrom<SetFrom>() >>
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
                MEZZ_EXCEPTION(IntrospectionNullptrCode, "Cannot Set Member value, no setter provided.")
            }
        }
        /// @brief Gets the value of a member on an initialized object.
        /// @tparam ObjectType Expected to match ClassType on the MemberAccessor.
        /// @exception If the Getter Access pointer is nullptr, then an Mezzanine::Exception::IntrospectionNullptrCode
        /// will be thrown.
        /// @param Object The object to retrieve the member value from.
        /// @return Returns the value of the member.
        template<typename ObjectType>
        [[nodiscard]]
        ConstReturnType GetValue(const ObjectType& Object) const
        {
            if( this->HasGetter() ) {
                if constexpr( std::is_same_v<GetterAccessPtr,MemberPtrType<ClassType,MemberType>> ) {
                    return (Object.*GetterPtr);
                }else{
                    return (Object.*GetterPtr)();
                }
            }else{
                MEZZ_EXCEPTION(IntrospectionNullptrCode, "Cannot Get Member value, no getter provided.")
            }
        }
        /// @brief Gets the value of a member on an initialized object.
        /// @tparam ObjectType Expected to match ClassType on the MemberAccessor.
        /// @exception If the Getter Access pointer is nullptr, then an Mezzanine::Exception::IntrospectionNullptrCode
        /// will be thrown.
        /// @param Object The object to retrieve the member value from.
        /// @return Returns the value of the member.
        template<typename ObjectType, typename = void>
        [[nodiscard]]
        ReturnType GetValue(ObjectType& Object) const
        {
            if( this->HasGetter() ) {
                if constexpr( std::is_same_v<GetterAccessPtr,MemberPtrType<ClassType,MemberType>> ) {
                    return (Object.*GetterPtr);
                }else{
                    return (Object.*GetterPtr)();
                }
            }else{
                MEZZ_EXCEPTION(IntrospectionNullptrCode, "Cannot Get Member value, no getter provided.")
            }
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Simple Query

        /// @brief Gets the name of the member being accessed.
        /// @return Returns a C-String containing the name of the member.
        [[nodiscard]]
        constexpr StringView GetName() const
            { return this->MemberName; }
        /// @brief Checks to see if a "setter" method has been set.
        /// @return Returns true if a value can be assigned with this accessor, false otherwise.
        [[nodiscard]]
        Boole HasSetter() const
            { return this->SetterPtr != nullptr; }
        /// @brief Checks to see if a "getter" method has been set.
        /// @return Returns true if a value can be retrieved with this accessor, false otherwise.
        [[nodiscard]]
        Boole HasGetter() const
            { return this->GetterPtr != nullptr; }

        /// @brief Retrieves the metadata tags associated with this member.
        /// @return Returns a MemberTags value or mask of values describing extra details about the member.
        [[nodiscard]]
        static constexpr MemberTags GetTags()
            { return Tags; }
    };//MemberAccessor

    RESTORE_WARNING_STATE

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
    [[nodiscard]]
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
    [[nodiscard]]
    constexpr auto MakeMemberAccessor(const StringView Name, CommonMethod Common)
        { return MemberAccessor<CommonMethod,CommonMethod,Tags>(Name,Common); }
    /// @brief Convenience method to initialize a MemberAccessor from deduced types.
    /// @tparam SetterMethod The member pointer type to use for setting the member value.
    /// @tparam GetterMethod The member pointer type to use for getting the member value.
    /// @pre SetterMethod Preconditions are the same as what is stated in the @ref MemberAccessor docs.
    /// @pre GetterMethod Preconditions are the same as what is stated in the @ref MemberAccessor docs.
    /// @param Name The name of the member.
    /// @param Setter A member pointer to be used for assignment the member value.
    /// @param Getter A member pointer to be used to retrieve the member value.
    /// @return Returns a new MemberAccessor initialized with the provided values.
    template<typename SetterMethod, typename GetterMethod>
    [[nodiscard]]
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
    [[nodiscard]]
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
    constexpr auto RegisterMembers()
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
        /// @brief Convenience type for RegisterName method detection.
        /// @tparam Class The class to test.
        template<typename Class>
        using RegisterName_t = decltype(&Class::RegisterName);
        /// @brief Convenience type for is_detected that tests for the existence of RegisterName on a class.
        /// @tparam Class The class that will be checked for the presence of a RegisterName method.
        template<typename Class>
        using HasRegisterName = std::is_detected<RegisterName_t,Class>;

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

        /// @brief Convenience type for GetSerializableMembers method detection.
        /// @tparam Class The class to test.
        template<typename Class>
        using GetSerializableMembers_t = decltype(&Class::GetSerializableMembers);
        /// @brief Convenience type for is_detected that tests for the existence of GetSerializableMembers on a class.
        /// @tparam Class The class that will be checked for the presence of a GetSerializableMembers method.
        template<typename Class>
        using HasGetSerializableMembers = std::is_detected<GetSerializableMembers_t,Class>;

        /// @brief Gets the registered name of a class.
        /// @tparam Class The type to check the name for.
        /// @remarks This method exists to support different means of providing the tuple of member accessors to
        /// the introspection system.  Depending on the type and what is detected, the final method should have
        /// only have a 1 line implementation.
        /// @return Returns a String containing the registered name of the class.
        template<typename Class>
        constexpr StringView GetName()
        {
            if constexpr( HasRegisterName<Class>::value ) {
                return Class::RegisterName();
            }else if constexpr( HasGetSerializableName<Class>::value ) {
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
        constexpr decltype(auto) GetMembers()
        {
            if constexpr( HasRegisterMembers<Class>::value ) {
                return Class::RegisterMembers();
            }else if constexpr( HasGetSerializableMembers<Class>::value ) {
                return Class::GetSerializableMembers();
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

        /// @brief A unimplemented template function that will generate a reversed index_sequence type.
        /// @remarks A function was used here instead of a struct due to the ease of parameter deduction. This
        /// function simply exists to generate a type in it's return from the deduced types in it's parameter.
        /// @tparam Idxs A collection of indexes to build the index sequence with.
        /// @return Shouldn't return anything as it should never be used at runtime.
        template<size_t... Idxs>
        constexpr auto reverse_index_sequence(const std::index_sequence<Idxs...>&)
            -> decltype( std::index_sequence<sizeof...(Idxs) - 1U - Idxs...>{} );

        /// @brief A convenience type for generating a index sequence from a parameter pack and reversing it.
        /// @tparam ParamPack The parameter pack to generate a reversed sequence for.
        template<typename... ParamPack>
        using reverse_index_sequence_for = decltype(reverse_index_sequence(std::index_sequence_for<ParamPack...>{}));

        /// @brief The actual implementation for the TupleForEach.
        /// @tparam Funct Deduced type for the callback that will be called for each member of the tuple.
        /// @tparam Members Variadic template type for the members that will be passed into Funct.
        /// @tparam Idxs A collection of indexes to build the index sequence with.
        /// @param ToCall The callback to be invoked for each member of the tuple.
        /// @param Mems The tuple of (presumably) member types that will be passed into the callback individually.
        template<typename Funct, typename... Members, size_t... Idxs>
        constexpr void TupleForEachIdx(Funct&& ToCall,const std::tuple<Members...>& Mems,std::index_sequence<Idxs...>)
            { ( ToCall(std::get<Idxs>(Mems)), ... ); }

        /// @brief A ForEach implementation for the types and members of a tuple.
        /// @tparam Funct Deduced type for the callback that will be called for each member of the tuple.
        /// @tparam Members Variadic template type for the members that will be passed into Funct.
        /// @param ToCall The callback to be invoked for each member of the tuple.
        /// @param Mems The tuple of (presumably) member types that will be passed into the callback individually.
        template<typename Funct, typename... Members>
        constexpr void TupleForEach(Funct&& ToCall, const std::tuple<Members...>& Mems)
            { TupleForEachIdx(std::forward<Funct>(ToCall),Mems,std::index_sequence_for<Members...>{}); }

        /// @brief Specialized forward ForEach implementation for an empty tuple.
        /// @remarks This method is a no-op.
        /// @tparam Funct Deduced type for the callback that won't be invoked.
        /// @tparam Members Variadic template type that ought to be empty if this implementation is being used.
        /// @param ToCall The callback that won't be invoked.
        /// @param Mems An empty tuple.
        template<typename Funct, typename... Members>
        constexpr void TupleForEach(Funct&& ToCall, const std::tuple<>& Mems)
            { (void)ToCall;  (void)Mems; }

        /// @brief A reversed ForEach implementation for the types and members of a tuple.
        /// @tparam Funct Deduced type for the callback that will be called for each member of the tuple.
        /// @tparam Members Variadic template type for the members that will be passed into Funct.
        /// @param ToCall The callback to be invoked for each member of the tuple.
        /// @param Mems The tuple of (presumably) member types that will be passed into the callback individually.
        template<typename Funct, typename... Members>
        constexpr void TupleForEachReverse(Funct&& ToCall, const std::tuple<Members...>& Mems)
            { TupleForEachIdx(std::forward<Funct>(ToCall),Mems,reverse_index_sequence_for<Members...>{}); }

        /// @brief Specialized reverse ForEach implementation for an empty tuple.
        /// @remarks This method is a no-op.
        /// @tparam Funct Deduced type for the callback that won't be invoked.
        /// @tparam Members Variadic template type that ought to be empty if this implementation is being used.
        /// @param ToCall The callback that won't be invoked.
        /// @param Mems An empty tuple.
        template<typename Funct, typename... Members>
        constexpr void TupleForEachReverse(Funct&& ToCall, const std::tuple<>& Mems)
            { (void)ToCall;  (void)Mems; }
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

    /// @brief This is the same as MergeMembers, but will discard duplicate types collected.
    /// @tparam TupleTypes A collection of tuple types representing the class members to be merged.
    /// @remarks This is intended for use with 1 or more tuples of MemberAccessor instances being passed
    /// in, but no enforcement is made that that happens.  If any else is passed in something will break.
    /// @param ToMerge A list of MemberAccessor tuples to be bundled together.
    /// @return Returns a Tuple containing all the tuples merged into one bigger tuple without duplicate types.
    template<typename... TupleTypes>
    constexpr auto MergeMembersUnique(TupleTypes&&... ToMerge)
    {
        constexpr auto Comp = [](const auto Left, const auto Right) constexpr {
            return Left.GetName() == Right.GetName();
        };
        return tuple_cat_unique_comp(Comp,std::forward<TupleTypes>(ToMerge)...);
    }

    /// @brief Checks to see if a class type has members registered.
    /// @tparam Class The class type to check.
    /// @return Returns true if introspection is possible for members of the given class, false otherwise.
    template<typename Class>
    [[nodiscard]]
    constexpr Boole IsRegistered()
    {
        using TupleType = decltype(IntrospectionHelpers::GetMembers<Class>());
        return !std::is_same_v<std::tuple<>,TupleType>;
    }

    /// @brief Gets the registered name of the class type.
    /// @tparam Class The class to check.
    /// @return Returns the registered name of the class, or an empty String if no name is registered.
    template<typename Class>
    [[nodiscard]]
    constexpr StringView GetRegisteredName()
    {
        return IntrospectionHelpers::GetName<Class>();
    }

    /// @brief Gets the number of registered members for a class.
    /// @tparam Class The class to check.
    /// @return Returns the amount of members that have been registered for the class specified.
    template<typename Class>
    [[nodiscard]]
    constexpr size_t GetMemberCount()
    {
        using TupleType = decltype(IntrospectionHelpers::GetMembers<Class>());
        return std::tuple_size_v<TupleType>;
    }

    /// @brief Gets a container of all the member accessors registered for a class.
    /// @tparam Class The class to retrieve accessors for.
    /// @return Returns a const reference to a tuple of member accessor types for each registered member.
    template<typename Class>
    [[nodiscard]]
    const auto& GetRegisteredMembers()
    {
        using TupleType = decltype(IntrospectionHelpers::GetMembers<Class>());
        return IntrospectionHelpers::MemberHolder<Class,TupleType>::Members;
    }

    /// @brief Invokes a callable for each registered member on a class.
    /// @warning SFINAE will disable this function if the class is not registered.
    /// @tparam Class Provided type who's members will be looped over.
    /// @tparam Funct Deduced parameter for the callback method to invoke with each member.
    /// @param ToCall The callback that will be invoked for each member in the specified class.
    template<typename Class, typename Funct, typename = std::enable_if_t<IsRegistered<Class>()>>
    void DoForAllMembers(Funct&& ToCall)
    {
        IntrospectionHelpers::TupleForEach(std::forward<Funct>(ToCall),GetRegisteredMembers<Class>());
    }

    /// @brief Dummy implementation of DoForAllMembers that is valid for non-registered classes.
    /// @warning SFINAE will disable this function if the class is registered.
    /// @tparam Class Provided type who's members will be looped over.
    /// @tparam Funct Deduced parameter for the callback method to invoke with each member.
    /// @remarks This exists primarily to help cut down on generated template code in the error case.
    /// @param ToCall The callback that will NOT be invoked for each member in the specified class.
    template<typename Class, typename Funct, typename = std::enable_if_t<!IsRegistered<Class>()>, typename = void>
    void DoForAllMembers(Funct&& ToCall)
    {
        (void)ToCall;
        // Deliberately empty as a default dummy implementation
    }

    /// @brief Invokes a callable for each registered member on a class in reverse order.
    /// @remarks The order is determined by the order the members tuple is constructed with during registration.
    /// @warning SFINAE will disable this function if the class is not registered.
    /// @tparam Class Provided type who's members will be looped over.
    /// @tparam Funct Deduced parameter for the callback method to invoke with each member.
    /// @param ToCall The callback that will be invoked for each member in the specified class.
    template<typename Class, typename Funct, typename = std::enable_if_t<IsRegistered<Class>()>>
    void DoForAllMembersReverse(Funct&& ToCall)
    {
        IntrospectionHelpers::TupleForEachReverse(std::forward<Funct>(ToCall),GetRegisteredMembers<Class>());
    }

    /// @brief Dummy implementation of DoForAllMembersReverse that is valid for non-registered classes.
    /// @warning SFINAE will disable this function if the class is registered.
    /// @tparam Class Provided type who's members will be looped over.
    /// @tparam Funct Deduced parameter for the callback method to invoke with each member.
    /// @remarks This exists primarily to help cut down on generated template code in the error case.
    /// @param ToCall The callback that will NOT be invoked for each member in the specified class.
    template<typename Class, typename Funct, typename = std::enable_if_t<!IsRegistered<Class>()>, typename = void>
    void DoForAllMembersReverse(Funct&& ToCall)
    {
        (void)ToCall;
        // Deliberately empty as a default dummy implementation
    }

    /// @brief Invokes a callable for a specific named member on a class.
    /// @tparam Class Provided type that will be searched for the named member.
    /// @tparam MemberType The type of the MemberAccessor that is expected to be passed into the callable.
    /// @tparam Funct Deduced parameter for the callback method to invoke with the named member.
    /// @param Name The name of the member to invoke the callback with.
    /// @param ToCall The invokable that will be called for the named member in the specified class.
    template<typename Class, typename MemberType, typename Funct>
    void DoForMember(const StringView Name, Funct&& ToCall)
    {
        DoForAllMembers<Class>(
            [&](const auto& Member) {
                using CurrType = typename std::decay_t<decltype(Member)>::MemberType;
                if constexpr( std::is_same_v<CurrType,MemberType> ) {
                    if( Name == Member.GetName() ) {
                        ToCall(Member);
                    }
                }
            }
        );
    }

    /// @brief Checks to see if a class has a named member.
    /// @tparam Class Provided class type that will be checked.
    /// @param Name The name of the member to check for.
    /// @return Returns true of the specified class has a registered member with the provided name, false otherwise.
    template<typename Class>
    [[nodiscard]]
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
    [[nodiscard]]
    MemberType GetMemberValue(Class& Obj, const StringView Name)
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
            [&Obj, &Val = std::forward<ValueType>(Val)] (const auto& Member) {
                Member.SetValue(Obj,Val);
            }
        );
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Fundamental Types Name Registration

    /// @brief Name registration specialization for doubles.
    /// @return Returns a String containing "Double".
    template<>
    StringView GetRegisteredName<double>()
        { return "Double"; }
    /// @brief Name registration specialization for floats.
    /// @return Returns a String containing "Float".
    template<>
    StringView GetRegisteredName<float>()
        { return "Float"; }
    /// @brief Name registration specialization for UInt64s.
    /// @return Returns a String containing "UInt64".
    template<>
    StringView GetRegisteredName<UInt64>()
        { return "UInt64"; }
    /// @brief Name registration specialization for Int64s.
    /// @return Returns a String containing "Int64".
    template<>
    StringView GetRegisteredName<Int64>()
        { return "Int64"; }
    /// @brief Name registration specialization for UInt32s.
    /// @return Returns a String containing "UInt32".
    template<>
    StringView GetRegisteredName<UInt32>()
        { return "UInt32"; }
    /// @brief Name registration specialization for Int32s.
    /// @return Returns a String containing "Int32".
    template<>
    StringView GetRegisteredName<Int32>()
        { return "Int32"; }
    /// @brief Name registration specialization for UInt16s.
    /// @return Returns a String containing "UInt16".
    template<>
    StringView GetRegisteredName<UInt16>()
        { return "UInt16"; }
    /// @brief Name registration specialization for Int16s.
    /// @return Returns a String containing "Int16".
    template<>
    StringView GetRegisteredName<Int16>()
        { return "Int16"; }
    /// @brief Name registration specialization for UInt8s.
    /// @return Returns a String containing "UInt8".
    template<>
    StringView GetRegisteredName<UInt8>()
        { return "UInt8"; }
    /// @brief Name registration specialization for Int8s.
    /// @return Returns a String containing "Int8".
    template<>
    StringView GetRegisteredName<Int8>()
        { return "Int8"; }
    /// @brief Name registration specialization for chars.
    /// @return Returns a String containing "Char".
    template<>
    StringView GetRegisteredName<char>()
        { return "Char"; }
    /// @brief Name registration specialization for bools.
    /// @return Returns a String containing "Bool".
    template<>
    StringView GetRegisteredName<Boole>()
        { return "Bool"; }

    ///////////////////////////////////////////////////////////////////////////////
    // Standard Types Name Registration

/*
    template< template<typename...> class MaybeSV,
              typename = std::enable_if_t< std::is_same_v< MaybeSV,std::basic_string_view > > >
    constexpr StringView GetRegisteredName<MaybeSV>()
        { return "StringView"; }

    template<typename CharType,
             typename Traits = std::char_traits<CharType> >
    constexpr StringView GetRegisteredName< std::basic_string_view<CharType,Traits> >()
        { return "StringView"; }

    template<typename CharType,
             typename Traits = std::char_traits<CharType>,
             typename Allocator = std::allocator<CharType> >
    constexpr StringView GetRegisteredName< std::basic_string<CharType,Traits,Allocator> >()
        { return "String"; }

    template<typename ElementType,
             size_t Count>
    constexpr StringView GetRegisteredName< std::array<ElementType,Count> >()
        { return "Array"; }

    template<typename ElementType,
             typename Allocator = std::allocator<ElementType>>
    constexpr StringView GetRegisteredName< std::vector<ElementType,Allocator> >()
        { return "Vector"; }

    template<typename ElementType,
             typename Allocator = std::allocator<ElementType>>
    constexpr StringView GetRegisteredName< std::list<ElementType,Allocator> >()
        { return "List"; }

    template<typename ElementType,
             typename Allocator = std::allocator<ElementType>>
    constexpr StringView GetRegisteredName< std::forward_list<ElementType,Allocator> >()
        { return "ForwardList"; }

    template<typename ElementType,
             typename Allocator = std::allocator<ElementType>>
    constexpr StringView GetRegisteredName< std::deque<ElementType,Allocator> >()
        { return "Deque"; }

    template<typename KeyType,
             typename Compare = std::less<KeyType>,
             typename Allocator = std::allocator<KeyType> >
    constexpr StringView GetRegisteredName< std::set<KeyType,Compare,Allocator> >()
        { return "Set"; }

    template<typename KeyType,
             typename Compare = std::less<KeyType>,
             typename Allocator = std::allocator<KeyType> >
    constexpr StringView GetRegisteredName< std::multiset<KeyType,Compare,Allocator> >()
        { return "MultiSet"; }

    template<typename KeyType,
             typename Hash = std::hash<KeyType>,
             typename KeyEqual = std::equal_to<KeyType>,
             typename Allocator = std::allocator<KeyType> >
    constexpr StringView GetRegisteredName< std::unordered_set<KeyType,Hash,KeyEqual,Allocator> >()
        { return "UnorderedSet"; }

    template<typename KeyType,
             typename Hash = std::hash<KeyType>,
             typename KeyEqual = std::equal_to<KeyType>,
             typename Allocator = std::allocator<KeyType> >
    constexpr StringView GetRegisteredName< std::unordered_multiset<KeyType,Hash,KeyEqual,Allocator> >()
        { return "UnorderedMultiSet"; }

    template<typename KeyType,
             typename ValueType,
             typename Compare = std::less<KeyType>,
             typename Allocator = std::allocator<const KeyType, ValueType> >
    constexpr StringView GetRegisteredName< std::map<KeyType,ValueType,Compare,Allocator> >()
        { return "Map"; }

    template<typename KeyType,
             typename ValueType,
             typename Compare = std::less<KeyType>,
             typename Allocator = std::allocator<const KeyType, ValueType> >
    constexpr StringView GetRegisteredName< std::multimap<KeyType,ValueType,Compare,Allocator> >()
        { return "MultiMap"; }

    template<typename KeyType,
             typename ValueType,
             typename Hash = std::hash<KeyType>,
             typename KeyEqual = std::equal_to<KeyType>,
             typename Allocator = std::allocator<const KeyType, ValueType> >
    constexpr StringView GetRegisteredName< std::unordered_map<KeyType,ValueType,Hash,KeyEqual,Allocator> >()
        { return "UnorderedMap"; }

    template<typename KeyType,
             typename ValueType,
             typename Hash = std::hash<KeyType>,
             typename KeyEqual = std::equal_to<KeyType>,
             typename Allocator = std::allocator<const KeyType, ValueType> >
    constexpr StringView GetRegisteredName< std::unordered_multimap<KeyType,ValueType,Hash,KeyEqual,Allocator> >()
        { return "UnorderedMultiMap"; }
*/
    /// @page Mezzanine Introspection System
    /// @section IntrospectOverview Overview
    /// The Mezzanine Introspection system is a collection of Classes, Structs, and Functions that heavily
    /// leverage Template Meta-programming to allow generic code for the reading and writing of values on
    /// classes and/or structs. With the Introspection system it is possible to create as little as one
    /// function to perform the reading and/or writing of values of an arbitrary number of types, rather
    /// than creating one or more functions per type.
    /// @n @n
    /// This Introspection system could be considered a Reflection system and many people do call similar
    /// systems Reflection systems, but we have opted (perhaps incorrectly) to call this an Introspection
    /// system because we felt it was closer to the generic computer science definition given on Wikipedia.
    /// Specifically that
    /// <a href="https://en.wikipedia.org/wiki/Type_introspection">Introspection</a>
    /// involves the reading and detection of members on an object (which this system does), while
    /// <a href="https://en.wikipedia.org/wiki/Reflection_(computer_programming)">Reflection</a>
    /// involves writing to those members as well as changes to composition and behavior of an object (this
    /// system only does writing from that list). That said, this is largely a semantics issue and it's
    /// reasonable to call this system either an Introspection or Reflection system.
    /// @n @n
    /// The Introspection system utilizes many new features from c++17 and even one feature from
    /// c++20/the experimental namespace. These features are primarily std::string_view, std::is_detected,
    /// and fold expressions. If you aren't familiar with these then this system might be slightly more
    /// difficult to understand.
    /// @section IntrospectImplement Implementation
    /// In this section we'll attempt to explain the code, what it does where it's not obvious, and how
    /// the code interacts with other code where applicable. It may not be intuitive, but we'll start
    /// in the middle of the system for this explanation and move outward for this explanation.
    /// @subsection IntrospectMemAccess Member Accessor
    /// The MemberAccessor is a class that stores the means of...accessing...a member. Deeply Complicated.
    /// It actually kinda is, but only a little. The MemberAccessor contains only 3 members, two member
    /// pointers and a string view.
    /// @n @n
    /// The pointers are template parameters that are checked with static_asserts to make sure they are
    /// pointers, but they can be direct to member pointers or pointers to member functions. The only types
    /// that are not allowed are normal C-arrays. std::array is fine though. The supported function signatures
    /// vary in order to accommodate as many configurations as possible.
    /// @n @n
    /// Setter signatures supported:
    /// @code
    /// void (T::*)(const Member&)  // Set by const reference
    /// void (T::*)(const Member)   // Set by const value
    /// void (T::*)(Member&&)       // Set by rvalue/move reference
    /// void (T::*)(Member)         // Set by value
    /// @endcode
    /// Getter signatures supported:
    /// @code
    /// const Member& (T::*)() const  // Get as const reference
    /// Member (T::*)() const         // Get as value
    /// @endcode
    /// Lastly, the signatures that can be used as both Setters and Getters:
    /// @code
    /// Member& (T::*)() const  // Get mutable reference to member
    /// Member (T::*)           // Pointer directly to member (no functions)
    /// @endcode
    /// If you choose to use a direct to member pointer or a non-const reference returning function then there
    /// are SFINAE enabled constructors and support functions you can use that accept 2 parameters instead of the
    /// usual 3, allowing you to avoid passing in a duplicate of the same parameter and makes construction cleaner.
    /// @n @n
    /// An important note about the Set and Get access pointers is that they can be direct to member or pointers
    /// to member functions that set and/or get the member value. If you are concerned about memory footprint then
    /// you should strongly consider using direct to member pointers over member function pointers. We've tested
    /// GCC 9.1 and Clang 8.0 on Godbolt.org and the size of a MemberAccessor with direct to member pointers on
    /// both platforms is 32 bytes. The size of a MemberAccessor with pointers to member functions on both
    /// platforms is 48 bytes. We don't know why you'd want to, but the system supports it if you wanted to have
    /// one direct to member pointer and one member function pointer for the setter and getter then you'd get a
    /// MemberAccessor that is 40 bytes on both platforms. Curiously, we also tested MSVC 19.14 (through wine)
    /// on Godbolt.org and all 3 configurations were 32 bytes in size.
    /// @n @n
    /// After constructing, the members of the MemberAccessor instance (the name and pointers) are not meant
    /// to be changed, only queried or dereferenced. There is a pair of Set/Get MemberValue functions that
    /// are written to compile just the correct semantics for the type of pointer provided for each of the Set
    /// and Get functions provided in the template parameters. It is a feature of the system that you aren't
    /// required to provide a valid pointer to the MemberAccessor on construction, opting instead to give it a
    /// nullptr. If you do, there is a simple nullptr check that will cause an exception to be thrown when
    /// trying to Set or Get the member value.
    /// @n @n
    /// The only other functions on the MemberAccessor class are those that check if the Setter pointer has been
    /// set, the Getter pointer has been set, getting the name of the member (via a string view), and getting
    /// the MemberTags that have been set for that member (via a static function). The tags are set from a
    /// template parameter given on instantiation of the type. Bitwise OR'ing the tags is supported for some
    /// more exotic use cases. Look at the @ref MemberTags enum for more details.
    /// @n @n
    /// Specifying the Set and Get access pointer types can be verbose and tedious, so a set of functions that
    /// use type deduction were made to help make the creation of MemberAccessor instances manageable. They are
    /// 4 simple functions that just wrap the construction of a MemberAccessor.
    /// @code
    /// template<typename CommonMethod>                                             // No Tags, Common Set and Get
    /// constexpr auto MakeMemberAccessor(const StringView Name, CommonMethod Common)
    /// template<MemberTags Tags, typename CommonMethod>                            // With Tags, Common Set and Get
    /// constexpr auto MakeMemberAccessor(const StringView Name, CommonMethod Common)
    /// template<typename SetterMethod, typename GetterMethod>                      // No Tags, Separate Set and Get
    /// constexpr auto MakeMemberAccessor(const StringView Name, SetterMethod Setter, GetterMethod Getter)
    /// template<MemberTags Tags, typename SetterMethod, typename GetterMethod>     // With Tags, Separate Set and Get
    /// constexpr auto MakeMemberAccessor(const StringView Name, SetterMethod Setter, GetterMethod Getter)
    /// @endcode
    /// The access pointers are always deduced, so if you invoke a version that doesn't have MemberTags then you
    /// do not need to specify any template parameters. Otherwise if you are using MemberTags then they need to
    /// be explicitly provided as a template parameter.
    /// @code
    /// auto AccessOne = MakeMemberAccessor("One",&Foo::A);
    /// auto AccessTwo = MakeMemberAccessor<MemberTags::Local>("Two",&Foo::B);
    /// auto AccessThree = MakeMemberAccessor("Three",&Bar::SetA,&Bar::GetA);
    /// auto AccessFour = MakeMemberAccessor<MemberTags::Local | MemberTags::Generated>("Four",&Bar::SetB,&Bar::GetB);
    /// @endcode
    /// @subsection IntrospectRegister Registration
    /// So we've established there is an object that handles the accessing of member data and a convenient way
    /// to create them. But we haven't established how they, or any other custom class integrate with the
    /// Introspection system itself. The term used for this throughout the system is "Registration".
    /// @n @n
    /// There are a few ways to Register a class with the Introspection system. The simplest and most direct
    /// would be to create a static function on your class named "RegisterMembers" that returns auto and takes no
    /// arguments. Inside it you would create and return a tuple of MemberAccessor instances. Easy. Ok, fine.
    /// There is some more detail to cover.
    /// @n @n
    /// There are actually 3 functions the Introspection system will look for on your class, and if any of those
    /// exist then you'll be able to Introspect your class.
    /// @code
    /// static auto RegisterMembers();         // Static member function, definition should be visible
    /// static auto GetSerializableMembers();  // Static member function, definition should be visible
    /// template<>
    /// auto RegisterMembers<MyClass>();       // Template free function, definition should be visible
    /// @endcode
    /// Any of these functions will work, and they will be looked for by the Introspection system in the order
    /// they are listed here. So if you have both a RegisterMembers static member function AND template free
    /// function then the Introspection system will use the RegisterMembers static member function and completely
    /// ignore the template free function. This won't cause a compilation error, but results in dead code.
    /// @n @n
    /// The template free function (which is a specialization, we'll cover that later) is a major oddball here
    /// and comes with a couple of caveats. First, any attempt to use private or protected members (either
    /// functions or pointers to members) will simply fail to compile. You are limited to the public interface.
    /// Second, due to limitations imposed by Argument-Dependent Lookup(ADL) any such function needs to be
    /// declared in the same scope as the base function being specialized from. This means any such function must
    /// be located in the root Mezzanine namespace.
    /// @n @n
    /// In exchange for these caveats you get a pretty powerful feature; the ability to register a class for
    /// Introspection without altering it whatsoever. So if you have code you aren't responsible for or otherwise
    /// cannot alter you can use this approach to Introspect it without issue. You could even make a registration
    /// function for the MemberAccessor class (why? please no) for no reason!
    /// @n @n
    /// The two static member functions are essentially identical in every way other than name. RegisterMembers
    /// is the more correct/consistent name and GetSerializableMembers is more consistent (somewhat) with older
    /// legacy code in the Mezzanine engine. Otherwise they are the same. So going forward examples will use the
    /// RegisterMembers name. Speaking of, lets take a look at an implementation example.
    /// @code
    /// struct MyVector3
    /// {
    ///     float X;
    ///     float Y;
    ///     float Z;
    ///
    ///     static auto RegisterMembers()
    ///     {
    ///         return Members(
    ///             MakeMemberAccessor("X",&MyVector3::X),
    ///             MakeMemberAccessor("Y",&MyVector3::Y),
    ///             MakeMemberAccessor("Z",&MyVector3::Z)
    ///         );
    ///     }
    /// };
    /// @endcode
    /// The "Members" function is something we haven't covered before. Members is a simple wrapper around
    /// std::make_tuple but with a more appropriate name. You can safely imagine Members to instead read
    /// "std::make_tuple" and that would be correct. Inside that call, we're making a MemberAccessor for each of
    /// our 3 members; X, Y, and Z. Each MemberAccessor is made without any MemberTags and using a direct to
    /// member pointer for Set and Get access. The return type is deduced by the compiler and will be a 3 member
    /// tuple of MemberAccessor instances that can be used by the Introspection system to Introspect MyVector3.
    /// @n @n
    /// Next lets look at a slightly more complicated example.
    /// @code
    /// class MyBaseClass
    /// {
    /// protected:
    ///     int ID;
    ///     unsigned int Counter = 0;
    /// public:
    ///     MyBaseClass(int NewID) :
    ///         ID(NewID)
    ///         {  }
    ///     virtual ~MyBaseClass() = default;
    ///
    ///     void SetID(int NewID)
    ///     {
    ///         std::cout << "Assigning ID: " << ID << ".\n";
    ///         ID = NewID;
    ///     }
    ///
    ///     int GetID() const
    ///     {
    ///         std::cout << "Retrieving ID: " << ID << ".\n";
    ///         return ID;
    ///     }
    ///
    ///     virtual int Increment()
    ///     {
    ///         Counter++;
    ///     }
    ///
    ///     static StringView RegisterName()
    ///     {
    ///         return "MyBaseClass";
    ///     }
    ///
    ///     static auto RegisterMembers()
    ///     {
    ///         using Self = MyBaseClass;
    ///         return Members(
    ///             MakeMemberAccessor("ID",&Self::SetID,&Self::GetID),
    ///             MakeMemberAccessor("Counter",&Self::Counter)
    ///         );
    ///     }
    /// };
    /// @endcode
    /// A few weird things here. Lets start with the "RegisterName" function. All the same rules for *Members
    /// series of functions apply to a similar series of *Name functions. There are 3 where each is checked in
    /// order and the first one found is used and if you are using the template free function then the ADL caveat
    /// applies, but not the pointer to private members caveat, because we're not using pointers here. Each of
    /// the functions is expected to return an StringView (which is a std::string_view alias) rather than a tuple
    /// of MemberAccessors and it is intended to have the class name as a value rather than a member name. Here's
    /// what they look like.
    /// @code
    /// static StringView RegisterName();         // Static member function, definition should be visible
    /// static StringView GetSerializableName();  // Static member function, definition should be visible
    /// template<>
    /// StringView RegisterName<MyClass>();       // Template free function, definition should be visible
    /// @endcode
    /// It's important to note that the Introspection system doesn't need a name to be registered. It doesn't use
    /// class/type names at all. Our original MyVector3 example would work just fine and be able to read and write
    /// members as is. Name registration exists exclusively for the benefit of other systems that might use the
    /// Introspection system, such as Serialization. So it is strongly recommended to include a name registration
    /// function, but isn't required and can be skipped in isolated or well controlled use cases.
    /// @n @n
    /// Back to the rest of the MyBaseClass example. There is some extra logic tied with the assignment and
    /// retrieval of IDs, as is sometimes the case with data. In this case have provided member functions for the
    /// set and get access pointers to ensure that logic gets called every time that member is assigned to or
    /// retrieved through the Introspection system. If this is not desired, then a direct to member pointer would
    /// be more appropriate and used instead. This is exactly what we do with the Counter member. Also because
    /// there are no access functions for the Counter member, so...that kinda...forces us to do that. Unless, of
    /// course, we want to simply not ever expose the Counter member to the Introspection system. In which case we
    /// simply don't make a MemberAccessor for it in the RegisterMembers function. You have complete control over
    /// how your class appears to the Introspection system.
    /// @n @n
    /// If you haven't yet raised an eyebrow at the name of our last example class, now is your chance. Some of you
    /// may be thinking "This is great and all for a simple class, but what about inheritance?" To which we say
    /// "Simple! Never inherit!" Kidding, for that we got the @ref MergeMembers function, which is just a fancy
    /// wrapper around std::tuple_cat. This function will create a new tuple from two or more existing tuples with
    /// the order of elements is the order they are provided to the function.
    /// @code
    /// class MyDerivedClass : public MyBaseClass
    /// {
    /// protected:
    ///     unsigned int CounterMax = 100;
    /// public:
    ///     MyDerivedClass(int NewID) :
    ///         MyBaseClass(NewID)
    ///         {  }
    ///     virtual ~MyDerivedClass() = default;
    ///
    ///     int Increment()
    ///     {
    ///         if( Counter < CounterMax ) {
    ///             Counter++;
    ///         }else{
    ///             // Oh noes!
    ///         }
    ///     }
    ///
    ///     static StringView RegisterName()
    ///     {
    ///         return "MyDerivedClass";
    ///     }
    ///
    ///     static auto RegisterMembers()
    ///     {
    ///         using Self = MyDerivedClass;
    ///         return MergeMembers(
    ///             MyBaseClass::RegisterMembers(),
    ///             Members(
    ///                 MakeMemberAccessor("CounterMax",&Self::CounterMax)
    ///             )
    ///         );
    ///     }
    /// };
    /// @endcode
    /// Never mind that CounterMax can't be changed with this API. I SAID NEVER MIND IT! Ok, so...we have a new class
    /// that inherits from MyBaseClass, our previous example class. We now have a limit to the amount it can increment
    /// as enforced by a new Increment function and variable that is checked as a part of that enforcement. Because
    /// this is a new class we need a new RegisterName function with a new string. We also need a new RegisterMembers
    /// function.
    /// @n @n
    /// The RegisterMembers function in our new class is much like our last class with a couple of key differences.
    /// While we still have a call to Members (which wraps our class member accessors in a tuple) we also have that
    /// being called inside of MergeMembers (which combines tuples of member accessors in the order provided) with
    /// the members from our base class as the other argument. The result of this call is a new tuple with all of
    /// the base class member accessors followed immediately by the derived class member accessors, all of which can
    /// be used in all the same ways as our base class registration.
    /// @n @n
    /// Ok, if you STILL care that CounterMax can't be changed by the class API, then keep in mind that since it's
    /// registered with the Introspection system, it can be changed by the Introspection system. We didn't have to
    /// write a trivial getter and setter after all! No, calm down Bjarne (and thanks for reading!). Everything is
    /// fine.
    /// @n @n
    /// A few weirdos in the back may still be saying "Ok, you got most inheritance use cases. BUT! I hate myself.
    /// I enjoy Oatmeal Raisin cookies and mixing M&Ms with Skittles. What about diamond inheritance?" For you weirdos
    /// we have good news and bad news. The good news is this can be done. The bad news is that it'll take a little
    /// bit more work than normal inheritance.
    /// @n @n
    /// We will quickly note that non-virtual inheritance does NOT form a diamond and thus you can use all the same
    /// methods covered with normal inheritance to build a list of all the members. Everything covered here will
    /// pertain to virtual inheritance with a common base class. The problem with using the normal inheritance methods
    /// with diamond inheritance is that it will generate duplicate accessors for the common base class. This will
    /// cause @ref GetMemberCount to return bad values, and the base class members to be operated on twice with every
    /// Introspection operation. This is clearly not desired behavior.
    /// @n @n
    /// To understand the workaround, you must understand that the @ref MergeMembers function is simply a convenience
    /// function. You can simply provide all the members explicitly. This should be avoided because if the base class
    /// changes then you'll have to update all classes derived from it and this is a maintenance cost that can be
    /// avoided by using MergeMembers. Diamonds get no convenience. They should be avoided in the first place, but in
    /// the absence of convenience it can still be done by explicitly listing the members of every base class. This
    /// is the approach we took when writing the unit tests for the Introspection system.
    /// @n @n
    /// If you MUST make the code more friendly to changes and maintenance, one thing that could be done is making a
    /// separate static registration method that ONLY includes the members of the class and nothing from its base
    /// classes, which is then called upon by the class forming the diamond to assemble the accessors needed by the
    /// diamond. The Introspection system doesn't provide any utilities to make this easier, so it's on you to make
    /// it work smoothly. But doing this, diamonds can be supported.
    /// @n @n
    /// There is an important limitation to registration that should be kept in mind. As of the time of this writing
    /// in C++17 (and even C++20), partial template specialization in function declarations is expicitly not allowed.
    /// Partial template specialization is exactly how the non-intrusive registration functions operate. This means
    /// that template classes can be tricky in some cases. If you have a complete type from a template, such as a
    /// vector<int> and specialize the registration function for that type, it's fine. It works like any given
    /// non-template type. But if you want to not have to care about the template parameter, and just have common
    /// functionality for just vector while discarding the parameter you'll run into a brick wall. For other template
    /// types that you have more control over you can make the registration functions static members and that will
    /// work ok.
    /// @subsection IntrospectGuts TupleForEach
    /// So we've covered how a custom class interacts with the MemberAccessor. But, how does the MemberAccessor
    /// interact with the rest of the Introspection system? Or with custom logic given to the Introspection system?
    /// Before we go further, it should be clarified that this part of the manual covers internals that no external
    /// developer should ever have to interact with. So this is only of benefit to those that want to understand
    /// every part of the Introspection system or those that may want to make deep changes to it. If you are neither
    /// of these kinds of developers, skip to the next section.
    /// @n @n
    /// All of the functions in the Introspection system that operate on members of classes go through 2 functions
    /// (in the IntrospectionHelpers namespace) to do so generically; @ref TupleForEach and @ref TupleForEachIdx.
    /// TupleForEach is overloaded to have 2 versions, one of which is a no-op if the tuple parameter is empty. This
    /// overload exists to help ease template generation. TupleForEachIdx does the actual leg work. Here's the actual
    /// code as of the time of this writing:
    /// @code
    /// template<typename Funct, typename... Members, size_t... Idxs>
    /// constexpr void TupleForEachIdx(Funct&& ToCall,const std::tuple<Members...>& Mems,std::index_sequence<Idxs...>)
    ///     { ( ToCall(std::get<Idxs>(Mems)), ... ); }
    ///
    /// template<typename Funct, typename... Members>
    /// constexpr void TupleForEach(Funct&& ToCall, const std::tuple<Members...>& Mems)
    ///     { TupleForEachIdx(std::forward<Funct>(ToCall),Mems,std::index_sequence_for<Members...>{}); }
    ///
    /// template<typename Funct, typename... Members>
    /// constexpr void TupleForEach(Funct&& ToCall, const std::tuple<>& Mems)
    ///     { (void)ToCall;  (void)Mems; }
    /// @endcode
    /// Functions in the public part of the Introspection API that interact with MemberAccessors will call TupleForEach
    /// passing in the function (or lambda) to be invoked with the member, and a tuple of the MemberAccessors for a
    /// given class. It is expected a specific instance of the class will be referenced in the function, which we will
    /// cover in more detail later.
    /// @n @n
    /// In the event that the tuple is empty (which implies an error) then the no-op overload will be called and
    /// nothing will happen. The function will simply return. If TupleForEach were ever called with an unregistered
    /// class then the no-op overload would be selected, because the fallback RegisterMembers<T> function is always
    /// used when a specialization isn't found. If you think the Introspection system is doing nothing when it should
    /// instead be doing nothing, check to see if a class is registered.
    /// @n @n
    /// If a valid tuple is passed into TupleForEach, the top overload is selected. This method is just an indirect
    /// for generating an index sequence from the deduced tuple members. This index sequence, the tuple, and a
    /// perfectly forwarded function is then passed into TupleForEachIdx, where the real magic happens.
    /// @n @n
    /// The big, big thing in the implementation of TupleForEachIdx is the fold expression. It is one line that gets
    /// expanded to either more than one line, or one super convoluted line, depending on how you think about it. In
    /// case you aren't familiar with fold expressions, the short explanation is that they will unwrap a parameter
    /// pack and generate one call of the logic per parameter unpacked with the provided operator between them. So if
    /// we assume the index sequence has <0,1,2> as it's template parameters then the generated code would look
    /// something like this:
    /// @code
    /// // Super convoluted code
    /// ( ( ( ToCall(std::get<0>(Mems)) ) , ToCall(std::get<1>(Mems)) ) , ToCall(std::get<2>(Mems)) );
    /// //                                ^ provided operator           ^ provided operator
    /// @endcode
    /// Because of operator precedence, the std::get<0> is called first and passed into our function/lambda. The comma
    /// operator in c++ simply means that both are evaluated and then the left hand result is discarded. In this case
    /// Funct is expected to be a type without a return type anyway so we're not losing anything. But we get to use
    /// the syntax of a fold expression to get it to generate the calls we need based on the index sequence. So the
    /// first call gets it's non-existent return discarded and then std::get<1> gets called and is passed our
    /// function/lambda, then it's result is discarded and finally std::get<2> is called and passed into our
    /// function/lambda. All of this is functionally equivalent to:
    /// @code
    /// // Much more readable
    /// ( ToCall(std::get<0>) );
    /// ( ToCall(std::get<1>) );
    /// ( ToCall(std::get<2>) );
    /// @endcode
    /// All of this works with index sequences of arbitrary size, which allows us to handle classes with arbitrary
    /// numbers of members in them. This does put some limitations on how some of these lambdas are written, which
    /// we'll cover in more detail later. These limitations are reasonably manageable though.
    /// @n @n
    /// So we're not actually looping over all of the members in a given class. Since we know the members of a given
    /// class at compile time and that can't change, we can just use template meta-magic to generate us a call per
    /// member. Combined with a lambda that accepts a single "auto" parameter we can get away with and and every
    /// member a class could have. Keep in mind that the MemberAccessor for the member is being passed in here, not
    /// the member itself. We couldn't have the member itself at this stage since we lack an actual instance of the
    /// object in these functions.
    /// @subsection IntrospectQuery Checks and Querying
    /// The Introspection system contains a few utility methods for the querying of types registered. They are minor
    /// and few in number, but may prove useful in some contexts.
    /// @n @n
    /// The most important check is whether or not a class is even registered with the Introspection system. The
    /// function has this signature:
    /// @code
    /// template<typename Class>
    /// constexpr Boole IsRegistered()
    /// @endcode
    /// It's super simple. Just call it with the class you want to check and it'll return true or false. This function
    /// is useful for verifying some simple errors that may occur when using the Introspection system and it should
    /// be VERY fast, since virtually all (if not all) of the logic can be determined at compile time.
    /// @n @n
    /// Another useful check is to see whether or not a class has a specific member. The @ref HasMember function
    /// will check if a class has a member with the specified name. Here's its signature:
    /// @code
    /// template<typename Class>
    /// Boole HasMember(const StringView Name)
    /// @endcode
    /// It's important to note here that all string operations in the Introspection system are done with string views
    /// for performance. Care should be taken to ensure the string is valid during the entire lifetime of it's use in
    /// the Introspection system.
    /// @n @n
    /// A bit less useful, but may be of help in some edge cases is the ability to check how many registered members
    /// exist for a given class. For that we have the @ref GetMemberCount function. Here's the signature:
    /// @code
    /// template<typename Class>
    /// constexpr size_t GetMemberCount()
    /// @endcode
    /// Keep in mind that this will only return the number of members registered with the class, not an actual count
    /// in the class declaration. If this number seems wrong, then double check the class registration method to
    /// ensure it is emitting the members you want.
    /// @n @n
    /// The remaining functions to be covered in this section are simple functions that wrap in the internal
    /// counterparts to expose registered classes and members. @ref GetRegisteredName and @ref GetRegisteredMembers
    /// functions exist to do exactly that. They both have fall backs to return a non-initialized string view
    /// (GetRegisteredName) or an empty tuple (GetRegisteredMembers) in the event either is called with a class type
    /// that isn't registered. Here are their signatures:
    /// @code
    /// template<typename Class>
    /// constexpr StringView GetRegisteredName()
    ///
    /// template<typename Class>
    /// const auto& GetRegisteredMembers()
    /// @endcode
    /// Note that GetRegisteredMembers returns a const reference, rather than a copy. All functions on the
    /// MemberAccessor are const anyway, but in case you are going to do some template meta programming with the return
    /// types you'll have to use the class registration function directly.
    /// @subsection IntrospectDoFor Operating on Data
    /// All of this would be very limited if we weren't able to insert custom logic to run based on the registered
    /// members of a given class. There are two methods that will accept a lambda to be call for each member being
    /// worked on. @ref DoForAllMembers will invoke the lambda for every member on the class and @ref DoForMember will
    /// invoke for only the named Member(s). In general, you should make every member name unique, but the system does
    /// not check for, nor enforce, uniqueness in member names whatsoever. It is up to you to disambiguate members
    /// with the same name if that occurs.
    /// @n @n
    /// There are a couple of requirements for using both functions, mostly regarding the lambdas. Any lambda created
    /// should not have a return. It shouldn't prevent compilation in there is a return statement, just the value
    /// returned won't go anywhere and will be discarded. Additionally, it should accept only one parameter with the
    /// type "const auto&". Simply "auto" should work as well. Auto is necessary because MemberAccessor types vary
    /// greatly and making it auto will permit the compiler to generate a lambda for each MemberAccessor in a type
    /// safe way. By now this should be obvious, but I'll state it anyway. The parameter passed into the lambda is NOT
    /// the Member or a copy of the member in anyway. It is an accessor to the member and you'll have to call the
    /// appropriate function on the MemberAccessor inside the lambda to work the with actual member.
    /// @n @n
    /// DoForAllMembers and DoForAllMembersReverse are overloaded functions with two versions each and use SFINAE to
    /// ensure only one each will be visible for a given class at a time based on whether or not it is registered.
    /// DoForAllMembers will go over each member in the order provided to the members tuple during registration.
    /// DoForAllMembersReverse will go over each member in the provided tuple in reverse order. Here's the signatures:
    /// @code
    /// template<typename Class, typename Funct, typename = std::enable_if_t<IsRegistered<Class>()>>
    /// void DoForAllMembers(Funct&& ToCall)
    ///
    /// template<typename Class, typename Funct, typename = std::enable_if_t<!IsRegistered<Class>()>, typename = void>
    /// void DoForAllMembers(Funct&& ToCall)
    ///
    /// template<typename Class, typename Funct, typename = std::enable_if_t<IsRegistered<Class>()>>
    /// void DoForAllMembersReverse(Funct&& ToCall)
    ///
    /// template<typename Class, typename Funct, typename = std::enable_if_t<!IsRegistered<Class>()>, typename = void>
    /// void DoForAllMembersReverse(Funct&& ToCall)
    /// @endcode
    /// The non-registered implementations are no-ops to help reduce the amount of code generated from the template. In
    /// either case you do need to specify the class to operate on as a template parameter and provide a lambda with
    /// the operation to be performed. Following the guidelines mentioned above, it's pretty straightforward to do
    /// basic operations with this.
    /// @code
    /// MyClass MyClassInstance;
    /// DoForAllMembers<MyClass>([&](const auto& Member) {
    ///     std::cout << Member.GetName() << ": " << Member.GetValue(MyClassInstance) << "\n";
    /// });
    /// @endcode
    /// The above code will stream every member (in the sequence specified by your registration method) in MyClass to
    /// std::cout. If a stream operator doesn't exist for a member then compilation will fail. You could even make
    /// this more generic by making a template function with deduced parameters.
    /// @code
    /// template<class ObjectType>
    /// void PrintAllMembers(ObjectType&& ToPrint)
    /// {
    ///     DoForAllMembers<ObjectType>([&](const auto& Member) {
    ///         std::cout << Member.GetName() << ": " << Member.GetValue(ToPrint) << "\n";
    ///     }
    /// }
    ///
    /// void main()
    /// {
    ///     MyClass MyClassInstance;
    ///     MyOtherClass MyOtherClassInstance;
    ///
    ///     PrintAllMembers(MyClassInstance);
    ///     PrintAllMembers(MyOtherClassInstance);
    ///     return 0;
    /// }
    /// @endcode
    /// You could even take it a step further and create a lambda that captures ANOTHER lambda and will only invoke
    /// it if the name of the member in question matches the name provided. In fact, this is EXACTLY how DoForMember
    /// is implemented. DoForMember comes with an extra bonus though. You have to specify the member type, which
    /// allows the compiler to simply not generate runtime checks for mismatched types. Here's the signature:
    /// @code
    /// template<typename Class, typename MemberType, typename Funct>
    /// void DoForMember(const StringView Name, Funct&& ToCall)
    /// @endcode
    /// The function/lambda type is deduced, but the Class and MemberType parameters need to be specified. This
    /// function will invoke "ToCall" for every member whose type matches MemberType AND whose name matches Name in
    /// the class specified in the Class template parameter. In sane cases this should result in only one or zero
    /// invocations. However since classes can, and often do, have multiple members of the same type AND the
    /// Introspection system doesn't enforce unique Member names, this could result in more than one invocation.
    /// @n @n
    /// The last two functions to cover are very simple convenience functions that make use of DoForMember. The
    /// functions @ref SetMemberValue and @ref GetMemberValue take a member type as a template parameter as
    /// well as a name and object instance. Here are the signatures:
    /// @code
    /// template<typename MemberType, typename Class>
    /// MemberType GetMemberValue(Class& Obj, const StringView Name)
    ///
    /// template<typename MemberType,
    ///          typename Class,
    ///          typename ValueType,
    ///          typename = std::enable_if_t< std::is_convertible_v<ValueType,MemberType> >>
    /// void SetMemberValue(Class& Obj, const StringView Name, ValueType&& Val)
    /// @endcode
    /// The Class template parameter is deduced in both functions. As is the ValueType parameter in SetMemberValue.
    /// SetMemberValue additionally has an SFINAE switch that will make it fail if you try to provide a type that
    /// cannot convert to the specified member type.
    /// @n @n
    /// GetMemberType has a couple minor caveats to keep in mind. First, it will create a temporary that is eventually
    /// returned. This temporary is curly bracket initialized and has the type specified in MemberType. If MemberType
    /// cannot be curly bracket initialized (for whatever reason) then it will fail to compile. Second, if the
    /// specified member is not found, then the temporary is returned as is without being assigned a value. So the
    /// failure return from GetMemberType is whatever state is made with "MemberType{}". If this is a perfectly valid
    /// state for MemberType then disambiguating that valid state from an error will be a challenge. In such cases
    /// creating your own custom lambda to be passed into DoForMember should be preferred over using GetMemberValue.
    /// @section IntrospectTrouble Troubleshooting
    /// One of the largest frustrations we had while writing this system was when we attempted to debug it with
    /// GDB. The system optimizes code away so aggressively it was impossible to watch the values we needed to
    /// watch, forcing me to spread asserts and log streaming throughout the code to get some clue of what was
    /// going on. It would be difficult to describe the combination of pride and frustration we felt because
    /// of this.
    /// @n @n
    /// So troubleshooting the system when something goes wrong can be somewhat difficult. A debugger is only of very
    /// limited use. Adding some custom code that prints values will get you better results generally. Throughout
    /// this manual we mentioned error-like behaviors such as what happens when a class isn't registered and default
    /// return values because those were the primary issues we ran into while testing the system. We did consider
    /// causing compilation errors when a non-registered class is used with the system, but we consider it a feature
    /// to allow people to throw arbitrary or unknown classes at the system without it throwing a fuss. The system
    /// would lose that if we caused more errors in those situations.

    /// @}
//}// Introspection
}// Mezzanine

#endif
