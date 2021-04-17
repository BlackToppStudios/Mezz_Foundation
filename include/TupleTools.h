// © Copyright 2010 - 2021 BlackTopp Studios Inc.
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
#ifndef Mezz_Foundation_TupleTools_h
#define Mezz_Foundation_TupleTools_h

#ifndef SWIG
    #include <tuple>
    #include <type_traits>
#endif

namespace Mezzanine
{
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

    /// @brief "Loops" through the elements of a tuple to see if there are any repeats of the element at Idx.
    /// @remarks This is a support function for rebuild_tuple_unique.
    /// @tparam Idx The index of the left hand element of the comparison.
    /// @tparam Tuple The tuple type to be checked and have duplicates removed.
    /// @tparam Idxs An index sequence of all the right hand elements of the comparison.
    /// @param ToCat The tuple to check for duplicate types.
    /// @return Returns a 1 element tuple if no duplicate is found, or a 0 element tuple if a duplicate is found.
    template<size_t Idx, class Tuple, size_t... Idxs>
    constexpr auto element_as_tuple(Tuple&& ToCat, std::index_sequence<Idxs...>)
    {
        using NoRef = std::remove_reference_t<Tuple>;
        if constexpr( !( std::is_same_v<std::tuple_element_t<Idx,NoRef>,std::tuple_element_t<Idxs,NoRef>> || ... ) ) {
            return std::tuple< std::tuple_element_t<Idx,NoRef> >( std::get<Idx>( std::forward<Tuple>(ToCat) ) );
        }else{
            (void)ToCat;
            return std::make_tuple();
        }
    }
    /// @brief Removes duplicate types from a single std::tuple.
    /// @remarks This is a support function for tuple_cat_unique.
    /// @tparam Tuple The tuple type to be checked and have duplicates removed.
    /// @tparam Idxs An index sequence based on the number of elements to check.
    /// @param ToCat The tuple to check for duplicate types.
    /// @return Returns a std::tuple with duplicate elements of the parameter tuple removed.
    template<class Tuple, size_t... Idxs>
    constexpr auto rebuild_tuple_unique(Tuple&& ToCat, std::index_sequence<Idxs...>)
    {
        return std::tuple_cat(
            element_as_tuple<Idxs>( std::forward<Tuple>(ToCat), std::make_index_sequence<Idxs>{} )...
        );
    }
    /// @brief Similar to std::tuple_cat, but discards duplicate occurrences of types while concatenating.
    /// @tparam Tuples The deduced collection of tuples to concatenate while removing duplicates.
    /// @param ToCat A collection of tuples to concatenate together.
    /// @return Returns a std::tuple containing only one of each element type from the parameter tuples.
    template<typename... Tuples>
    constexpr auto tuple_cat_unique(Tuples&&... ToCat)
    {
        auto all = std::tuple_cat( std::forward<Tuples>(ToCat)... );
        return rebuild_tuple_unique( all, std::make_index_sequence<std::tuple_size_v<decltype(all)>>{} );
    }
}//Mezzanine

#endif
