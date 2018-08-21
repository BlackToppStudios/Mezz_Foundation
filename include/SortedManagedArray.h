// © Copyright 2010 - 2018 BlackTopp Studios Inc.
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
#ifndef Mezz_Foundation_SortedManagedArray_h
#define Mezz_Foundation_SortedManagedArray_h

/// @file
/// @brief This file includes the declaration and definition for the SortedArray class.

#ifndef SWIG
    #include "binaryfind.h"
    #include "ManagedArray.h"
#endif

namespace Mezzanine
{
    ///////////////////////////////////////////////////////////////////////////////
    /// @brief This container uses a ManagedArray for storage, but manages the amount of used space and keeps them sorted.
    /// @tparam ElementType The type this container will store, must implement operator< for sorting with the default sorter.
    /// @tparam NumElements The number of ElementType instances this array will be allocated to store.
    ///////////////////////////////////////
    template<typename ElementType, size_t NumElements, typename Sorter = std::less<ElementType> >
    class SortedManagedArray
    {
    public:
        /// @brief Convenience type to refer to the type of this.
        using SelfType = SortedManagedArray<ElementType,NumElements,Sorter>;
        /// @brief The type used for internal storage.
        using StorageArray = ManagedArray<ElementType,NumElements>;
        /// @brief The type used when checking sizes and capacities of instances of this.
        using size_type = typename StorageArray::size_type;
        /// @brief The type of items stored.
        using value_type = typename StorageArray::value_type;
        /// @brief Type of mutable random access iterator. Invalidated on all insertions.
        using iterator = typename StorageArray::iterator;
        /// @brief Type of const random access iterator. Invalidated on all insertions.
        using const_iterator = typename StorageArray::const_iterator;
        /// @brief Type of mutable reverse iterator for random access. Invalidated on all insertions.
        using reverse_iterator = typename StorageArray::reverse_iterator;
        /// @brief Type of const reverse iterator for random access. Invalidated on all insertions.
        using const_reverse_iterator = typename StorageArray::const_reverse_iterator;
    private:
        /// @brief The internal array that does most of the interesting work.
        StorageArray InternalStorage;
    public:
        /// @brief Class constructor.
        SortedManagedArray() = default;
        /// @brief Copy constructor.
        /// @param Other The other array to be copied.
        SortedManagedArray(const SortedManagedArray& Other) = default;
        /// @brief Move constructor.
        /// @remarks Since this is all stack allocated, this is pretty much a copy.
        /// @param Other The other array to be moved.
        SortedManagedArray(SortedManagedArray&& Other) = default;
        /// @brief Initializer list constructor.
        /// @param InitList A listing of the values to initialize this array with.
        SortedManagedArray(std::initializer_list<ElementType> InitList)
            { add_range(InitList.begin(),InitList.end()); }
        /// @brief Class destructor.
        ~SortedManagedArray() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Operators

        /// @brief Copy Assignment Operator.
        /// @param Other The other array to be copied.
        /// @return Returns a reference to this.
        SelfType& operator=(const SelfType& Other) = default;
        /// @brief Move Assignment Operator.
        /// @param Other The other array to be moved.
        /// @return Returns a reference to this.
        SelfType& operator=(SelfType&& Other) = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Iterators

        /// @brief Get an iterator to the beginning of the container.
        /// @return A mutable iterator pointing to the first element.
        iterator begin()
            { return InternalStorage.begin(); }
        /// @brief Get a const iterator to the beginning of the container.
        /// @return A const iterator pointing to the first element.
        const_iterator begin() const
            { return InternalStorage.begin(); }

        /// @brief Get an iterator to one past the end of the container.
        /// @return A mutable iterator to one past the end of the container.
        iterator end()
            { return InternalStorage.end(); }
        /// @brief Get an iterator to one past the end of the container.
        /// @return A const iterator to one past the end of the container.
        const_iterator end() const
            { return InternalStorage.end(); }

        /// @brief Get an iterator to the last element of the container, going backwards.
        /// @return A mutable reverse iterator pointing to the first element at the end.
        reverse_iterator rbegin()
            { return InternalStorage.rbegin(); }
        /// @brief Get a const iterator to the last element of the container, going backwards.
        /// @return A const reverse iterator pointing to the first element at the end.
        const_reverse_iterator rbegin() const
            { return InternalStorage.rbegin(); }

        /// @brief Get an iterator to one before the first element of the container.
        /// @return A mutable reverse iterator pointing to the reverse end element at the start.
        reverse_iterator rend()
            { return InternalStorage.rend(); }
        /// @brief Get a const iterator to one before the first element of the container.
        /// @return A const reverse iterator pointing to the reverse end element at the start.
        const_reverse_iterator rend() const
            { return InternalStorage.rend(); }

        /// @brief Uses std::sort to sort this, might using something more special focus in the
        /// future.
        void sort()
            { std::sort(begin(),end(), Sorter()); }

        /// @brief How many items are stored in this?
        /// @return Some integer type, likely unsigned indicating how many items this stores.
        size_type size() const
            { return InternalStorage.size(); }

        /// @brief Since this container has no array-like concept this inserts the item were
        /// it needs to go.
        /// @details This has all the potential allocation slow downs of push_back
        /// and costs of find the required place to insert. This sorts the whole container after
        /// adding items, so calling this after for a many additions is really slow, use
        /// @ref add_range to only sort once.
        /// @param value The value to put into the array.
        /// @return Returns an iterator to the added element.
        iterator add(ElementType value)
        {
            const_iterator InsertPos = std::lower_bound(begin(),end(),value,Sorter());
            return InternalStorage.insert(InsertPos,value);
        }

        /// @brief As "add" but allows construction of the element in the container.
        /// @tparam Args A variadic template of arguments to be used to construct the element in place.
        /// @param Params List of arguments needed to construct the element.
        template<typename Compare, class... Args>
        iterator add_emplace(Compare PosFinder, Args&&... Params)
        {
            const_iterator InsertPos = std::lower_bound(begin(),end(),Params...,PosFinder);
            return InternalStorage.emplace(InsertPos,Params...);
        }

        /// @brief Add several items at once efficiently.
        /// @details Sorts only once so is much faster than add(),
        /// @tparam ForeignIterator The type of the the iterator of the other container, must
        /// be at least a forward iterator.
        /// @param OtherBegin An iterator to the start of the other range to be copied.
        /// @param OtherEnd An iterator to one past the end.
        template<class ForeignIterator>
        void add_range(ForeignIterator OtherBegin, ForeignIterator OtherEnd)
        {
            for(; OtherBegin!=OtherEnd; OtherBegin++)
                { InternalStorage.push_back(*OtherBegin); }
            sort();
        }

        /// @brief Gets the element at the specified index.
        /// @param Index The offset of the element to retrieve.
        /// @return Returns a reference to the element at the specified index.
        ElementType& operator[] (size_t Index) noexcept
            { return InternalStorage[Index]; }
        /// @brief Gets the element at the specified index.
        /// @param Index The offset of the element to retrieve.
        /// @return Returns a reference to the element at the specified index.
        const ElementType& operator[] (size_t Index) const noexcept
            { return InternalStorage[Index]; }
        /// @brief Gets the element at the specified index.
        /// @exception If the index specified is greater than the current number of stored elements,
        /// a std::out_of_range exception will be thrown.
        /// @param Index The offset of the element to retrieve.
        /// @return Returns a reference to the element at the specified index.
        ElementType& at(size_t Index)
        {
            if( Index >= size() ) {
                throw std::out_of_range("Specified array index is invalid.");
            }
            return InternalStorage[Index];
        }
        /// @brief Gets the element at the specified index.
        /// @exception If the index specified is greater than the current number of stored elements,
        /// a std::out_of_range exception will be thrown.
        /// @param Index The offset of the element to retrieve.
        /// @return Returns a reference to the element at the specified index.
        const ElementType& at(size_t Index) const
        {
            if( Index >= size() ) {
                throw std::out_of_range("Specified array index is invalid.");
            }
            return InternalStorage[Index];
        }

        /// @brief Get and iterator to a specific item, operates in fast logarithmic time.
        /// @param value the item to get the location of.
        /// @return A mutable iterator to an item, can be adjusted by random access.
        iterator find(const ElementType& value)
            { return binary_find(begin(),end(),value,Sorter()); }
        /// @brief Get and iterator to a specific item, operates in fast logarithmic time.
        /// @param value the item to get the location of.
        /// @return A const iterator to an item, can be adjusted by random access.
        const_iterator find(const ElementType& value) const
            { return binary_find(begin(),end(),value,Sorter()); }

        /// @brief A convenience method for invoking std::find_if with all the elements of this container.
        /// @tparam UnaryPredicate The type of invokable that will perform the checks.
        /// @param Pred An instance of the predicate that will takes one of this containers elements and returns true or false.
        /// @return Returns an iterator to the element that made the predicate return true, or the end iterator if they all returned false.
        template<class UnaryPredicate>
        iterator find_if(UnaryPredicate Pred)
            { return std::find_if(begin(),end(),Pred); }
        /// @brief A convenience method for invoking std::find_if with all the elements of this container.
        /// @tparam UnaryPredicate The type of invokable that will perform the checks.
        /// @param Pred An instance of the predicate that will takes one of this containers elements and returns true or false.
        /// @return Returns a const iterator to the element that made the predicate return true, or the end iterator if they all returned false.
        template<class UnaryPredicate>
        const_iterator find_if(UnaryPredicate Pred) const
            { return std::find_if(begin(),end(),Pred); }

        /// @brief Does the item exist in this array?
        /// @param value The item in question.
        /// @return True if present false otherwise.
        Boole contains(const ElementType& value) const
            { return std::binary_search(begin(),end(),value); }

        /// @brief Empty the array discarding all data.
        void clear()
            { InternalStorage.clear(); }
        /// @brief How many items can this store before requiring an allocation.
        /// @return Returns the total number of elements this array has room for.
        size_type capacity() const
            { return InternalStorage.capacity(); }
        /// @brief Gets whether or not this array is empty.
        /// @return Returns true if this array is not storing any elements, false otherwise.
        Boole empty() const noexcept
            { return size() == 0;  }

        /// @brief Remove an item indicated by the iterator.
        /// @param position An iterator pointing to the item to remove.
        /// @return An iterator to one after the erased item.
        iterator erase( iterator position )
            { return InternalStorage.erase(position); }
        /// @brief Remove items indicated by an iterator range.
        /// @param first An iterator pointing to the first item to be erased.
        /// @param last An iterator pointer to one past the last item to be erased.
        /// @return An iterator to one after the erased item.
        iterator erase( iterator first, iterator last )
            { return InternalStorage.erase(first, last); }
    };//SortedManagedArray
} // Mezzanine

#endif // Include guard
