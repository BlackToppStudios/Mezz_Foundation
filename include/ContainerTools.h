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
#ifndef Mezz_Foundation_ContainerTools_h
#define Mezz_Foundation_ContainerTools_h

#ifndef SWIG
    #include <type_traits>
    #include "DetectionTraits.h"
#endif

SAVE_WARNING_STATE
// Clang likes to complain about the inline variables not being marked inline here
SUPPRESS_CLANG_WARNING("-Wmissing-variable-declarations")
// Clang incorrectly thinks the inline variables and using aliases shouldn't have tparam docs
//SUPPRESS_CLANG_WARNING("-Wdocumentation")

namespace Mezzanine {
namespace ContainerDetect {
    /// @brief Convenience type for begin function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using BeginFunct_t = decltype(std::declval<Class&>().begin());
    /// @brief Type for is_detected that tests for the existence of begin on a class.
    /// @tparam Class The class that will be checked for the presence of a begin function.
    template<typename Class>
    using HasBegin = std::is_detected<BeginFunct_t,Class>;
    /// @brief Convenience inline variable for the value of a HasBegin check.
    /// @tparam Class The class that will be checked for the presence of a begin function.
    template<typename Class>
    inline constexpr Boole HasBegin_v = HasBegin<Class>::value;

    /// @brief Convenience type for end function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using EndFunct_t = decltype(std::declval<Class&>().end());
    /// @brief Type for is_detected that tests for the existence of end on a class.
    /// @tparam Class The class that will be checked for the presence of a end function.
    template<typename Class>
    using HasEnd = std::is_detected<EndFunct_t,Class>;
    /// @brief Convenience inline variable for the value of a HasEnd check.
    /// @tparam Class The class that will be checked for the presence of a end function.
    template<typename Class>
    inline constexpr Boole HasEnd_v = HasEnd<Class>::value;

    /// @brief Convenience type for size function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using SizeFunct_t = decltype(std::declval<Class&>().size());
    /// @brief Type for is_detected that tests for the existence of size on a class.
    /// @tparam Class The class that will be checked for the presence of a size function.
    template<typename Class>
    using HasSize = std::is_detected<SizeFunct_t,Class>;
    /// @brief Convenience inline variable for the value of a HasSize check.
    /// @tparam Class The class that will be checked for the presence of a size function.
    template<typename Class>
    inline constexpr Boole HasSize_v = HasSize<Class>::value;

    /// @brief Dummy/failure type for detecting if a class has a "value_type" defined.
    /// @tparam Class The class to be tested.
    template<typename Class, typename = void>
    struct HasValueType : std::false_type
        {  };
    /// @brief Success type for detecting if a class has a "value_type" defined.
    /// @tparam Class The class to be tested.
    template<typename Class>
    struct HasValueType<Class,std::void_t<typename Class::value_type>> : std::true_type
        {  };
    /// @brief Convenience
    template<typename Class>
    inline constexpr Boole HasValueType_v = HasValueType<Class>::value;
}//ContainerDetect

    /// @brief A type trait that checks to see if a type is/has a range of elements.
    /// @tparam CheckType The type that will be checked.
    /// @remarks This tests for the presence of begin() and end() member functions on the type.
    template<typename CheckType>
    struct is_range :
        std::bool_constant< ContainerDetect::HasBegin_v<CheckType> && ContainerDetect::HasEnd_v<CheckType> >
        {  };
    /// @brief Convenience inline variable for getting just the bool of a is_range check.
    /// @tparam CheckType The type that will be checked.
    template<typename CheckType>
    inline constexpr Boole is_range_v = is_range<CheckType>::value;

    /// @brief A type trait that checks to see if it meets the minimal requirements for a container.
    /// @tparam CheckType The type that will be checked.
    /// @remarks This tests for the presence of begin(), end(), size(), and a value_type member on the type.
    template<typename CheckType>
    struct is_container :
        std::bool_constant< is_range_v<CheckType> &&
                            ContainerDetect::HasSize_v<CheckType> &&
                            ContainerDetect::HasValueType_v<CheckType> >
        {  };
    /// @brief Convenience inline variable for getting just the bool of a is_container check.
    /// @tparam CheckType The type that will be checked.
    template<typename CheckType>
    inline constexpr Boole is_container_v = is_container<CheckType>::value;

    /// @brief Dummy/failure type for detecting if a class is a key/value pair for associative containers.
    /// @tparam ValueType The value_type stored by the container being tested.
    /// @remarks This is used by the is_associative_container_impl struct as a failure case when testing a
    /// containers value_type member.
    template<typename ValueType>
    struct is_key_value_pair : std::false_type
        {  };
    /// @brief Success type for detecting if a class is a key/value pair for associative containers.
    /// @tparam KeyType The deduced key type of the key/value pair stored by the container.
    /// @tparam ElementType The deduced element/value type in the key/value pair stored by the container.
    /// @remarks This is used by the is_associative_container_impl struct as a success case when testing a
    /// containers value_type member.
    template<typename KeyType, typename ElementType>
    struct is_key_value_pair< std::pair<const KeyType, ElementType> > : std::true_type
        {  };
    /// @brief Convenience inline variable for getting just the bool of a is_key_value_pair check.
    /// @tparam CheckType The type that will be checked.
    template<typename CheckType>
    inline constexpr Boole is_key_value_pair_v = is_key_value_pair<CheckType>::value;

    /// @brief Dummy/failure type for detecting if a type is a container.
    /// @tparam IsContainer Whether or not the CheckType parameter is a container type.
    /// @tparam CheckType The non-container type to be tested.
    /// @remarks This is used by the is_associative_container struct as a failure case when testing
    /// if a type is a container.
    template<Boole IsContainer, typename CheckType>
    struct is_associative_container_impl : std::false_type
        {  };
    /// @brief Success type for detecting if a type is a container prior to testing its value_type member.
    /// @tparam CheckType The container type to be tested.
    /// @remarks This is used by the is_associative_container struct as a success case when testing
    /// if a type is a container. The value_type member on the CheckType is forwarded to the
    /// is_key_value_pair struct to determine if it is a key/value pair used in associative containers.
    template<typename CheckType>
    struct is_associative_container_impl<true,CheckType> :
        std::bool_constant< is_key_value_pair_v<typename CheckType::value_type> >
        {  };
    /// @brief A type trait that detects if a type is an associative container.
    /// @tparam CheckType The type to be checked if it is an associative container.
    /// @remarks This performs all the same tests as is_container, but additionally checks to see if the
    /// value_type member matches the check performed by is_key_value_pair.
    template<typename CheckType>
    struct is_associative_container : is_associative_container_impl< is_container_v<CheckType>, CheckType>
        {  };
    /// @brief Convenience inline variable for getting just the bool of an is_associative_container check.
    /// @tparam CheckType The type to be checked if it is an associative container.
    template<typename CheckType>
    inline constexpr Boole is_associative_container_v = is_associative_container<CheckType>::value;

    /// @brief Dummy/failure type for detecting if a type is a container.
    /// @tparam IsContainer Whether or not the CheckType parameter is a container type.
    /// @tparam CheckType The non-container type to be tested.
    /// @remarks This is used by the is_non_associative_container struct as a failure case when testing
    /// if a type is a container.
    template<Boole IsContainer, typename CheckType>
    struct is_non_associative_container_impl : std::false_type
        {  };
    /// @brief Success type for detecting if a type is a container prior to testing its value_type member.
    /// @tparam CheckType The container type to be tested.
    /// @remarks This is used by the is_non_associative_container struct as a success case when testing
    /// if a type is a container. The value_type member on the CheckType is forwarded to the
    /// is_key_value_pair struct to determine if it is a key/value pair used in associative containers.
    template<typename CheckType>
    struct is_non_associative_container_impl<true,CheckType> :
        std::bool_constant< !is_key_value_pair_v<typename CheckType::value_type> >
        {  };
    /// @brief A type trait that detects if a type is a non-associative container.
    /// @tparam CheckType The type to be checked if it is a non-associative container.
    /// @remarks This performs all the same tests as is_container, but additionally checks to see if the
    /// value_type member does not match the check performed by is_key_value_pair.
    template<typename CheckType>
    struct is_non_associative_container : is_non_associative_container_impl< is_container_v<CheckType>, CheckType>
        {  };
    /// @brief Convenience inline variable for getting just the bool of an is_non_associative_container check.
    /// @tparam CheckType The type to be checked if it is a non-associative container.
    template<typename CheckType>
    inline constexpr Boole is_non_associative_container_v = is_non_associative_container<CheckType>::value;
}//Mezzanine

RESTORE_WARNING_STATE

#endif // Mezz_Foundation_ContainerTools_h
