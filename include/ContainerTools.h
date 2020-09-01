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

namespace Mezzanine {
namespace ContainerDetect {
    /// @brief Convenience type for begin function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using BeginFunct_t = decltype(std::declval<Class&>().begin());
    /// @brief Type for is_detected that tests for the existence of begin on a class.
    /// @tparam Class The class that will be checked for the presence of a begin function.
    template<typename Class>
    using HasBegin_t = std::is_detected<BeginFunct_t,Class>;
    /// @brief Convenience function for the value of a HasBegin check.
    /// @tparam Class The class that will be checked for the presence of a "begin()" function.
    /// @return Returns true if the provided type has a "begin()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasBegin()
        { return HasBegin_t<Class>::value; }

    /// @brief Convenience type for end function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using EndFunct_t = decltype(std::declval<Class&>().end());
    /// @brief Type for is_detected that tests for the existence of end on a class.
    /// @tparam Class The class that will be checked for the presence of a end function.
    template<typename Class>
    using HasEnd_t = std::is_detected<EndFunct_t,Class>;
    /// @brief Convenience function for the value of a HasEnd check.
    /// @tparam Class The class that will be checked for the presence of a "end()" function.
    /// @return Returns true if the provided type has a "end()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasEnd()
        { return HasEnd_t<Class>::value; }

    /// @brief Convenience type for rbegin function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using RBeginFunct_t = decltype(std::declval<Class&>().rbegin());
    /// @brief Type for is_detected that tests for the existence of rbegin on a class.
    /// @tparam Class The class that will be checked for the presence of a rbegin function.
    template<typename Class>
    using HasRBegin_t = std::is_detected<RBeginFunct_t,Class>;
    /// @brief Convenience function for the value of a HasRBegin check.
    /// @tparam Class The class that will be checked for the presence of a "rbegin()" function.
    /// @return Returns true if the provided type has a "rbegin()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasRBegin()
        { return HasRBegin_t<Class>::value; }

    /// @brief Convenience type for rend function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using REndFunct_t = decltype(std::declval<Class&>().rend());
    /// @brief Type for is_detected that tests for the existence of rend on a class.
    /// @tparam Class The class that will be checked for the presence of a rend function.
    template<typename Class>
    using HasREnd_t = std::is_detected<REndFunct_t,Class>;
    /// @brief Convenience function for the value of a HasREnd check.
    /// @tparam Class The class that will be checked for the presence of a "rend()" function.
    /// @return Returns true if the provided type has a "rend()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasREnd()
        { return HasREnd_t<Class>::value; }

    /// @brief Convenience type for cbegin function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using CBeginFunct_t = decltype(std::declval<Class&>().cbegin());
    /// @brief Type for is_detected that tests for the existence of cbegin on a class.
    /// @tparam Class The class that will be checked for the presence of a cbegin function.
    template<typename Class>
    using HasCBegin_t = std::is_detected<CBeginFunct_t,Class>;
    /// @brief Convenience function for the value of a HasCBegin check.
    /// @tparam Class The class that will be checked for the presence of a "cbegin()" function.
    /// @return Returns true if the provided type has a "cbegin()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasCBegin()
        { return HasCBegin_t<Class>::value; }

    /// @brief Convenience type for cend function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using CEndFunct_t = decltype(std::declval<Class&>().cend());
    /// @brief Type for is_detected that tests for the existence of cend on a class.
    /// @tparam Class The class that will be checked for the presence of a cend function.
    template<typename Class>
    using HasCEnd_t = std::is_detected<CEndFunct_t,Class>;
    /// @brief Convenience function for the value of a HasCEnd check.
    /// @tparam Class The class that will be checked for the presence of a "cend()" function.
    /// @return Returns true if the provided type has a "cend()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasCEnd()
        { return HasCEnd_t<Class>::value; }

    /// @brief Convenience type for crbegin function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using CRBeginFunct_t = decltype(std::declval<Class&>().crbegin());
    /// @brief Type for is_detected that tests for the existence of rbegin on a class.
    /// @tparam Class The class that will be checked for the presence of a rbegin function.
    template<typename Class>
    using HasCRBegin_t = std::is_detected<CRBeginFunct_t,Class>;
    /// @brief Convenience function for the value of a HasCRBegin check.
    /// @tparam Class The class that will be checked for the presence of a "crbegin()" function.
    /// @return Returns true if the provided type has a "crbegin()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasCRBegin()
        { return HasCRBegin_t<Class>::value; }

    /// @brief Convenience type for crend function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using CREndFunct_t = decltype(std::declval<Class&>().crend());
    /// @brief Type for is_detected that tests for the existence of crend on a class.
    /// @tparam Class The class that will be checked for the presence of a crend function.
    template<typename Class>
    using HasCREnd_t = std::is_detected<CREndFunct_t,Class>;
    /// @brief Convenience function for the value of a HasCREnd check.
    /// @tparam Class The class that will be checked for the presence of a "crend()" function.
    /// @return Returns true if the provided type has a "crend()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasCREnd()
        { return HasCREnd_t<Class>::value; }

    /// @brief Convenience type for size function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using SizeFunct_t = decltype(std::declval<Class&>().size());
    /// @brief Type for is_detected that tests for the existence of size on a class.
    /// @tparam Class The class that will be checked for the presence of a size function.
    template<typename Class>
    using HasSize_t = std::is_detected<SizeFunct_t,Class>;
    /// @brief Convenience function for the value of a HasSize check.
    /// @tparam Class The class that will be checked for the presence of a "size()" function.
    /// @return Returns true if the provided type has a "size()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasSize()
        { return HasSize_t<Class>::value; }

    /// @brief Convenience type for resize function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using ResizeFunct_t = decltype(std::declval<Class&>().resize(std::declval<size_t>()));
    /// @brief Type for is_detected that tests for the existence of resize on a class.
    /// @tparam Class The class that will be checked for the presence of a resize function.
    template<typename Class>
    using HasResize_t = std::is_detected<ResizeFunct_t,Class>;
    /// @brief Convenience function for the value of a HasResize check.
    /// @tparam Class The class that will be checked for the presence of a "resize()" function.
    /// @return Returns true if the provided type has a "resize()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasResize()
        { return HasResize_t<Class>::value; }

    /// @brief Convenience type for reserve function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using ReserveFunct_t = decltype(std::declval<Class&>().reserve(std::declval<size_t>()));
    /// @brief Type for is_detected that tests for the existence of reserve on a class.
    /// @tparam Class The class that will be checked for the presence of a reserve function.
    template<typename Class>
    using HasReserve_t = std::is_detected<ReserveFunct_t,Class>;
    /// @brief Convenience function for the value of a HasReserve check.
    /// @tparam Class The class that will be checked for the presence of a "reserve()" function.
    /// @return Returns true if the provided type has a "reserve()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasReserve()
        { return HasReserve_t<Class>::value; }

    /// @brief Convenience type for clear function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using ClearFunct_t = decltype(std::declval<Class&>().clear());
    /// @brief Type for is_detected that tests for the existence of clear on a class.
    /// @tparam Class The class that will be checked for the presence of a clear function.
    template<typename Class>
    using HasClear_t = std::is_detected<ClearFunct_t,Class>;
    /// @brief Convenience function for the value of a HasClear check.
    /// @tparam Class The class that will be checked for the presence of a "clear()" function.
    /// @return Returns true if the provided type has a "clear()" member function, false otherwise.
    template<typename Class>
    constexpr Boole HasClear()
        { return HasClear_t<Class>::value; }

    /// @brief Convenience type for push_back function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using PushBackValueFunct_t = decltype(std::declval<Class&>().push_back(std::declval<typename Class::value_type>()));
    /// @brief Type for is_detected that tests for the existence of push_back on a class.
    /// @tparam Class The class that will be checked for the presence of a push_back function.
    template<typename Class>
    using PushBackValue_t = std::is_detected<PushBackValueFunct_t,Class>;
    /// @brief Convenience function for the value of a HasPushBackValue check.
    /// @tparam Class The class that will be checked for the presence of a "push_back(class::value_type)" function.
    /// @return Returns true if the provided type has a "push_back()" member function that accepts a value, false otherwise.
    template<typename Class>
    constexpr Boole HasPushBackValue()
        { return PushBackValue_t<Class>::value; }

    /// @brief Convenience type for insert function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using InsertValueFunct_t = decltype(std::declval<Class&>().insert(std::declval<typename Class::value_type>()));
    /// @brief Type for is_detected that tests for the existence of insert on a class.
    /// @tparam Class The class that will be checked for the presence of a insert function.
    template<typename Class>
    using InsertValue_t = std::is_detected<InsertValueFunct_t,Class>;
    /// @brief Convenience function for the value of a HasInsertValue check.
    /// @tparam Class The class that will be checked for the presence of a "insert(class::value_type)" function.
    /// @return Returns true if the provided type has a "insert()" member function that accepts a value, false otherwise.
    template<typename Class>
    constexpr Boole HasInsertValue()
        { return InsertValue_t<Class>::value; }

    /// @brief Convenience type for add(value) function detection.
    /// @tparam Class The class to test.
    template<typename Class>
    using AddValueFunct_t = decltype(std::declval<Class&>().add(std::declval<typename Class::value_type>()));
    /// @brief Type for is_detected that tests for the existence of add on a class.
    /// @tparam Class The class that will be checked for the presence of a insert function.
    template<typename Class>
    using AddValue_t = std::is_detected<AddValueFunct_t,Class>;
    /// @brief Convenience function for the value of a HasAddValue check.
    /// @tparam Class The class that will be checked for the presence of a "add(class::value_type)" function.
    /// @return Returns true if the provided type has a "add()" member function that accepts a value, false otherwise.
    template<typename Class>
    constexpr Boole HasAddValue()
        { return AddValue_t<Class>::value; }

    /// @brief Dummy/failure type for detecting if a class has a "value_type" defined.
    /// @tparam Class The class to be tested.
    template<typename Class, typename = void>
    struct DetectValueType : std::false_type
        {  };
    /// @brief Success type for detecting if a class has a "value_type" defined.
    /// @tparam Class The class to be tested.
    template<typename Class>
    struct DetectValueType<Class,std::void_t<typename Class::value_type>> : std::true_type
        {  };
    /// @brief Type for detecting the existance of a "value_type" member on a class.
    /// @tparam Class The class to be tested.
    template<typename Class>
    using HasValueType_t = DetectValueType<Class>;
    /// @brief Convenience function for the value of a HasValueType check.
    /// @tparam Class The class that will be checked for the presence of a "value_type" member.
    /// @return Returns true if the provided type has a "value_type" member, false otherwise.
    template<typename Class>
    constexpr Boole HasValueType()
        { return HasValueType_t<Class>::value; }

    /// @brief Dummy/failure type for detecting if a class has a "size_type" defined.
    /// @tparam Class The class to be tested.
    template<typename Class, typename = void>
    struct DetectSizeType : std::false_type
        {  };
    /// @brief Success type for detecting if a class has a "size_type" defined.
    /// @tparam Class The class to be tested.
    template<typename Class>
    struct DetectSizeType<Class,std::void_t<typename Class::size_type>> : std::true_type
        {  };
    /// @brief Type for detecting the existance of a "size_type" member on a class.
    /// @tparam Class The class to be tested.
    template<typename Class>
    using HasSizeType_t = DetectSizeType<Class>;
    /// @brief Convenience function for the value of a HasSizeType check.
    /// @tparam Class The class that will be checked for the presence of a "size_type" member.
    /// @return Returns true if the provided type has a "size_type" member, false otherwise.
    template<typename Class>
    constexpr Boole HasSizeType()
        { return HasSizeType_t<Class>::value; }

    /// @brief Dummy/failure type for detecting if a class has a "difference_type" defined.
    /// @tparam Class The class to be tested.
    template<typename Class, typename = void>
    struct DetectDifferenceType : std::false_type
        {  };
    /// @brief Success type for detecting if a class has a "difference_type" defined.
    /// @tparam Class The class to be tested.
    template<typename Class>
    struct DetectDifferenceType<Class,std::void_t<typename Class::difference_type>> : std::true_type
        {  };
    /// @brief Type for detecting the existance of a "difference_type" member on a class.
    /// @tparam Class The class to be tested.
    template<typename Class>
    using HasDifferenceType_t = DetectDifferenceType<Class>;
    /// @brief Convenience function for the value of a HasDifferenceType check.
    /// @tparam Class The class that will be checked for the presence of a "difference_type" member.
    /// @return Returns true if the provided type has a "difference_type" member, false otherwise.
    template<typename Class>
    constexpr Boole HasDifferenceType()
        { return HasDifferenceType_t<Class>::value; }

    /// @brief Dummy/failure type for detecting if a class has a "key_type" defined.
    /// @tparam Class The class to be tested.
    template<typename Class, typename = void>
    struct DetectKeyType : std::false_type
        {  };
    /// @brief Success type for detecting if a class has a "key_type" defined.
    /// @tparam Class The class to be tested.
    template<typename Class>
    struct DetectKeyType<Class,std::void_t<typename Class::key_type>> : std::true_type
        {  };
    /// @brief Type for detecting the existance of a "key_type" member on a class.
    /// @tparam Class The class to be tested.
    template<typename Class>
    using HasKeyType_t = DetectKeyType<Class>;
    /// @brief Convenience function for the value of a HasKeyType check.
    /// @tparam Class The class that will be checked for the presence of a "key_type" member.
    /// @return Returns true if the provided type has a "key_type" member, false otherwise.
    template<typename Class>
    constexpr Boole HasKeyType()
        { return HasKeyType_t<Class>::value; }

    /// @brief Dummy/failure type for detecting if a class has a "mapped_type" defined.
    /// @tparam Class The class to be tested.
    template<typename Class, typename = void>
    struct DetectMappedType : std::false_type
        {  };
    /// @brief Success type for detecting if a class has a "mapped_type" defined.
    /// @tparam Class The class to be tested.
    template<typename Class>
    struct DetectMappedType<Class,std::void_t<typename Class::value_type>> : std::true_type
        {  };
    /// @brief Type for detecting the existance of a "mapped_type" member on a class.
    /// @tparam Class The class to be tested.
    template<typename Class>
    using HasMappedType_t = DetectMappedType<Class>;
    /// @brief Convenience function for the value of a HasMappedType check.
    /// @tparam Class The class that will be checked for the presence of a "mapped_type" member.
    /// @return Returns true if the provided type has a "mapped_type" member, false otherwise.
    template<typename Class>
    constexpr Boole HasMappedType()
        { return HasMappedType_t<Class>::value; }
}//ContainerDetect

    /// @brief A type trait that checks to see if a type is/has a range of elements.
    /// @tparam CheckType The type that will be checked.
    /// @remarks This tests for the presence of begin() and end() member functions on the type.
    template<typename CheckType>
    struct is_range :
        std::bool_constant< ContainerDetect::HasBegin<CheckType>() &&
                            ContainerDetect::HasEnd<CheckType>() >
        {  };
    /// @brief Convenience function for getting just the bool of a is_range check.
    /// @tparam CheckType The type that will be checked.
    /// @return Returns true if CheckType has "begin()" and "end()" member functions, false otherwise.
    template<typename CheckType>
    constexpr Boole IsRange()
        { return is_range<CheckType>::value; }

    /// @brief A type trait that checks to see if a type is/has a reverse range of elements.
    /// @tparam CheckType The type that will be checked.
    /// @remarks This tests for the presence of rbegin() and rend() member functions on the type.
    template<typename CheckType>
    struct is_reverse_range :
        std::bool_constant< ContainerDetect::HasRBegin<CheckType>() &&
                            ContainerDetect::HasREnd<CheckType>() >
        {  };
    /// @brief Convenience function for getting just the bool of a is_reverse_range check.
    /// @tparam CheckType The type that will be checked.
    /// @return Returns true if CheckType has "rbegin()" and "rend()" member functions, false otherwise.
    template<typename CheckType>
    constexpr Boole IsReverseRange()
        { return is_reverse_range<CheckType>::value; }

    /// @brief A type trait that checks to see if a type is/has a const range of elements.
    /// @tparam CheckType The type that will be checked.
    /// @remarks This tests for the presence of cbegin() and cend() member functions on the type.
    template<typename CheckType>
    struct is_const_range :
        std::bool_constant< ContainerDetect::HasCBegin<CheckType>() &&
                            ContainerDetect::HasCEnd<CheckType>() >
        {  };
    /// @brief Convenience function for getting just the bool of a is_const_range check.
    /// @tparam CheckType The type that will be checked.
    /// @return Returns true if CheckType has "cbegin()" and "cend()" member functions, false otherwise.
    template<typename CheckType>
    constexpr Boole IsConstRange()
        { return is_const_range<CheckType>::value; }

    /// @brief A type trait that checks to see if a type is/has a const reverse range of elements.
    /// @tparam CheckType The type that will be checked.
    /// @remarks This tests for the presence of crbegin() and crend() member functions on the type.
    template<typename CheckType>
    struct is_const_reverse_range :
        std::bool_constant< ContainerDetect::HasCRBegin<CheckType>() &&
                            ContainerDetect::HasCREnd<CheckType>() >
        {  };
    /// @brief Convenience function for getting just the bool of a is_const_reverse_range check.
    /// @tparam CheckType The type that will be checked.
    /// @return Returns true if CheckType has "crbegin()" and "crend()" member functions, false otherwise.
    template<typename CheckType>
    constexpr Boole IsConstReverseRange()
        { return is_const_reverse_range<CheckType>::value; }

    /// @brief A type trait that checks to see if a type has contiguous internal storage that can be resized.
    /// @tparam CheckType The type that will be checked.
    /// @remarks This tests for the presence of resize() and reserve() member functions on the type.
    template<typename CheckType>
    struct is_resizable_contiguous :
        std::bool_constant< ContainerDetect::HasResize<CheckType>() &&
                            ContainerDetect::HasReserve<CheckType>() >
        {  };
    /// @brief Convenience function for getting just the bool of a is_resizable_contiguous check.
    /// @tparam CheckType The type that will be checked.
    /// @return Returns true if CheckType has "resize()" and "reserve()" member functions, false otherwise.
    template<typename CheckType>
    constexpr Boole IsResizableContiguous()
        { return is_resizable_contiguous<CheckType>::value; }

    /// @brief A type trait that checks to see if a type has noncontiguous internal storage that can be resized.
    /// @tparam CheckType The type that will be checked.
    /// @remarks This tests for the presence of a resize() member on the type, but not reserve().
    template<typename CheckType>
    struct is_resizable_noncontiguous :
        std::bool_constant< ContainerDetect::HasResize<CheckType>() &&
                            !ContainerDetect::HasReserve<CheckType>() >
        {  };
    /// @brief Convenience function for getting just the bool of a is_resizable_noncontiguous.
    /// @tparam CheckType The type that will be checked.
    /// @return Returns true if CheckType has "resize()" and "reserve()" member functions, false otherwise.
    template<typename CheckType>
    constexpr Boole IsResizableNonContiguous()
        { return is_resizable_noncontiguous<CheckType>::value; }

    /// @brief A type trait that checks to see if it meets the minimal requirements for a container.
    /// @tparam CheckType The type that will be checked.
    /// @remarks This tests for the presence of begin(), end(), size(), and a value_type member on the type.
    template<typename CheckType>
    struct is_container :
        std::bool_constant< IsRange<CheckType>() &&
                            ContainerDetect::HasSize<CheckType>() &&
                            ContainerDetect::HasValueType<CheckType>() >
        {  };
    /// @brief Convenience function for getting just the bool of a is_container check.
    /// @tparam CheckType The type that will be checked.
    /// @return Returns true if CheckType is a range and has "size()" and "value_type" members, false otherwise.
    template<typename CheckType>
    constexpr Boole IsContainer()
        { return is_container<CheckType>::value; }

    /// @brief A type trait that checks to see if it meets stricter requirements like most StdLib containers.
    /// @tparam CheckType The type that will be checked.
    /// @remarks This tests for the presence of begin(), end(), cbegin(), cend(), size(), size_type, difference_type,
    /// and a value_type member on the type.
    template<typename CheckType>
    struct is_stdlib_like_container :
        std::bool_constant< IsRange<CheckType>() &&
                            IsConstRange<CheckType>() &&
                            ContainerDetect::HasSize<CheckType>() &&
                            ContainerDetect::HasValueType<CheckType>() &&
                            ContainerDetect::HasDifferenceType<CheckType>() >
        {  };
    /// @brief Convenience function for getting just the bool of a is_container check.
    /// @tparam CheckType The type that will be checked.
    /// @return Returns true if CheckType is very similar to most std library containers., false otherwise.
    template<typename CheckType>
    constexpr Boole IsStdLibLikeContainer()
        { return is_stdlib_like_container<CheckType>::value; }

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
    /// @brief Convenience function for getting just the bool of a is_key_value_pair check.
    /// @tparam CheckType The type that will be checked.
    /// @return Returns true if CheckType is an std::pair with the first type being const, false otherwise.
    template<typename CheckType>
    constexpr Boole IsKeyValuePair()
        { return is_key_value_pair<CheckType>::value; }

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
        std::bool_constant< ContainerDetect::HasKeyType<CheckType>() &&
                            ContainerDetect::HasMappedType<CheckType>() &&
                            IsKeyValuePair<typename CheckType::value_type>() >
        {  };
    /// @brief A type trait that detects if a type is an associative container.
    /// @tparam CheckType The type to be checked if it is an associative container.
    /// @remarks This performs all the same tests as is_container, but additionally checks to see if the
    /// value_type member matches the check performed by is_key_value_pair.
    template<typename CheckType>
    struct is_associative_container : is_associative_container_impl< IsContainer<CheckType>(), CheckType>
        {  };
    /// @brief Convenience function for getting just the bool of an is_associative_container check.
    /// @tparam CheckType The type to be checked if it is an associative container.
    /// @return Returns true if CheckType is a container and it's value type is a key-value pair, false otherwise.
    template<typename CheckType>
    constexpr Boole IsAssociativeContainer()
        { return is_associative_container<CheckType>::value; }

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
        std::bool_constant< !IsKeyValuePair<typename CheckType::value_type>() >
        {  };
    /// @brief A type trait that detects if a type is a non-associative container.
    /// @tparam CheckType The type to be checked if it is a non-associative container.
    /// @remarks This performs all the same tests as is_container, but additionally checks to see if the
    /// value_type member does not match the check performed by is_key_value_pair.
    template<typename CheckType>
    struct is_non_associative_container : is_non_associative_container_impl< IsContainer<CheckType>(), CheckType>
        {  };
    /// @brief Convenience function for getting just the bool of an is_non_associative_container check.
    /// @tparam CheckType The type to be checked if it is a non-associative container.
    /// @return Returns true if CheckType is a container and it's value type is NOT a key-value pair, false otherwise.
    template<typename CheckType>
    constexpr Boole IsNonAssociativeContainer()
        { return is_non_associative_container<CheckType>::value; }

    /// @brief Type trait for detecting of a comparator is transparent.
    /// @tparam Comp The comparator functor to check.
    /// @tparam KeyType The key being used.
    /// @details This is the dummy trait for failure cases.
    template<typename Comp, typename KeyType, typename = std::void_t<>>
    struct comp_is_transparent : std::false_type
        {  };
    /// @brief Type trait specialization for detecting of a comparator is transparent.
    /// @tparam Comp The comparator functor to check.
    /// @tparam KeyType The key being used.
    template<typename Comp, typename KeyType>
    struct comp_is_transparent<Comp,KeyType,std::void_t<typename Comp::is_transparent>> : std::true_type
        {  };

}//Mezzanine

#endif // Mezz_Foundation_ContainerTools_h
