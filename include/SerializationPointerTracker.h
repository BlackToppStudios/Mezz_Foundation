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
#ifndef Mezz_Foundation_SerializationPointerTracker_h
#define Mezz_Foundation_SerializationPointerTracker_h

/// @file
/// @brief This file contains a set of classes for tracking pointers that have been serialized/deserialized.

#ifndef SWIG
    #include "SerializationUtility.h"
    #include "SortedVector.h"

    #include <type_traits>
    #include <vector>
#endif // SWIG

namespace Mezzanine {
namespace Serialization {
    /// @addtogroup Serialization
    /// @{

    ///////////////////////////////////////////////////////////////////////////////
    // ObjectCounter (for counting the number of references to a given object)

    struct MEZZ_LIB ObjectCounter
    {
        uintptr_t InstanceID = 0;
        UInt32 LinkCount = 0;
        UInt32 OwnerOrJoinCount = 0;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // ObjectLink (for handling non-owned pointers between objects)

    struct MEZZ_LIB ObjectLink
    {
        std::vector< std::reference_wrapper<void*> > UnresolvedLinks;
        StringView TypeName;
        uintptr_t InstanceID = 0;
        void* Instance = nullptr;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // ObjectJoin (for handling shared ownership pointers between objects)

    struct MEZZ_LIB ObjectJoin
    {
        std::vector< std::reference_wrapper< std::shared_ptr<void> > > UnresolvedJoins;
        StringView TypeName;
        uintptr_t InstanceID = 0;
        std::shared_ptr<void> Instance = nullptr;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // Pointer ID Generator

    uintptr_t GetIDOfPtr(void* Ptr);
    uintptr_t GetIDOfPtr(void* Ptr)
    {
        return reinterpret_cast<uintptr_t>(Ptr);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Tracker Predicate

    template<typename TrackerMeta>
    struct TrackerPredicate
    {
        using is_transparent = void;
        Boole operator()(const TrackerMeta& First, const TrackerMeta& Second)
            { return First.InstanceID < Second.InstanceID; }
        Boole operator()(const TrackerMeta& Link, const uintptr_t ID)
            { return Link.InstanceID < ID; }
        Boole operator()(const uintptr_t ID, const TrackerMeta& Link)
            { return ID < Link.InstanceID; }
    };//TrackerPredicate

    ///////////////////////////////////////////////////////////////////////////////
    // PointerTracker

    class MEZZ_LIB SerializerPointerTracker
    {
    public:
        using CountContainer = SortedVector<ObjectCounter,TrackerPredicate<ObjectCounter>>;
        using CountIterator = CountContainer::iterator;
        using ConstCountIterator = CountContainer::const_iterator;
    protected:
        CountContainer PtrCounts;
    public:
        SerializerPointerTracker() = default;
        SerializerPointerTracker(const SerializerPointerTracker& Other) = delete;
        SerializerPointerTracker(SerializerPointerTracker&& Other) = delete;
        ~SerializerPointerTracker() = default;

        SerializerPointerTracker& operator=(const SerializerPointerTracker& Other) = delete;
        SerializerPointerTracker& operator=(SerializerPointerTracker&& Other) = delete;

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        //Serialization
        // Detect if the object pointed to has already been serialized.
        //  Error when a second owner is detected.
        //  Signal for the system to create a link to the original otherwise.

        const ObjectCounter& TrackObject(void* ToTrack)
        {
            uintptr_t PtrID = GetIDOfPtr(&ToTrack);
            CountIterator FoundIt = this->PtrCounts.find(PtrID);
            if( FoundIt != this->PtrCounts.end() ) {
                if( (*FoundIt).OwnerOrJoinCount != 0 ) {
                    throw std::runtime_error("Multiple owners for object found.");
                }else{
                    (*FoundIt).OwnerOrJoinCount = 1;
                    return (*FoundIt);
                }
            }else{
                ObjectCounter NewCounter;
                NewCounter.InstanceID = PtrID;
                NewCounter.OwnerOrJoinCount = 1;
                return *( this->PtrCounts.add(NewCounter) );
            }
        }

        template<typename ObjType>
        const ObjectCounter& TrackObject(ObjType& ToTrack)
            { return TrackObject( &ToTrack ); }

        const ObjectCounter& TrackPointer(void* ToTrack, const Boole IsOwner)
        {
            uintptr_t PtrID = GetIDOfPtr(ToTrack);
            CountIterator FoundIt = this->PtrCounts.find(PtrID);
            if( FoundIt != this->PtrCounts.end() ) {
                (*FoundIt).LinkCount++;
                if( IsOwner ) {
                    if( (*FoundIt).OwnerOrJoinCount != 0 ) {
                        throw std::runtime_error("Multiple owners for object found.");
                    }else{
                        (*FoundIt).OwnerOrJoinCount = 1;
                    }
                }
                return (*FoundIt);
            }else{
                ObjectCounter NewCounter;
                NewCounter.InstanceID = PtrID;
                NewCounter.LinkCount = 1;
                NewCounter.OwnerOrJoinCount = IsOwner;
                return *( this->PtrCounts.add(NewCounter) );
            }
        }
        const ObjectCounter& TrackSharedPointer(std::shared_ptr<void> ToTrack)
        {
            uintptr_t PtrID = GetIDOfPtr(ToTrack.get());
            CountIterator FoundIt = this->PtrCounts.find(PtrID);
            if( FoundIt != this->PtrCounts.end() ) {
                (*FoundIt).OwnerOrJoinCount++;
                return (*FoundIt);
            }else{
                ObjectCounter NewCounter;
                NewCounter.InstanceID = PtrID;
                NewCounter.OwnerOrJoinCount = 1;
                return *( this->PtrCounts.add(NewCounter) );
            }
        }

        //CountIterator GetCountBegin()
        //    { return this->PtrCounts.begin(); }
        ConstCountIterator GetCountBegin() const
            { return this->PtrCounts.cbegin(); }
        //CountIterator GetCountEnd()
        //    { return this->PtrCounts.end(); }
        ConstCountIterator GetCountEnd() const
            { return this->PtrCounts.cend(); }
    };//SerializerPointerTracker

    class MEZZ_LIB DeserializerPointerTracker
    {
    public:
        using LinkContainer = SortedVector<ObjectLink,TrackerPredicate<ObjectLink>>;
        using LinkIterator = LinkContainer::iterator;
        using JoinContainer = SortedVector<ObjectJoin,TrackerPredicate<ObjectJoin>>;
        using JoinIterator = JoinContainer::iterator;
    protected:
        LinkContainer TrackedLinks;
        JoinContainer TrackedJoins;

        template<typename PtrType>
        ObjectLink& GetOrCreateLink(uintptr_t PtrID)
        {
            LinkIterator TrackIt = TrackedLinks.find(PtrID);
            if( TrackIt == TrackedLinks.end() ) {
                ObjectLink NewLink;
                NewLink.TypeName = GetRegisteredName<std::remove_pointer_t<PtrType>>();
                NewLink.InstanceID = PtrID;
                NewLink.Instance = nullptr;
                return *(TrackedLinks.add(NewLink));
            }
            return *TrackIt;
        }
        template<typename PtrType>
        ObjectJoin& GetOrCreateJoin(uintptr_t PtrID)
        {
            JoinIterator TrackIt = TrackedJoins.find(PtrID);
            if( TrackIt == TrackedJoins.end() ) {
                ObjectJoin NewJoin;
                NewJoin.TypeName = GetRegisteredName<std::remove_pointer_t<PtrType>>();
                NewJoin.InstanceID = PtrID;
                NewJoin.Instance = nullptr;
                return *(TrackedJoins.add(NewJoin));
            }
            return *TrackIt;
        }
    public:
        DeserializerPointerTracker() = default;
        DeserializerPointerTracker(const DeserializerPointerTracker& Other) = delete;
        DeserializerPointerTracker(DeserializerPointerTracker&& Other) = delete;
        ~DeserializerPointerTracker() = default;

        DeserializerPointerTracker& operator=(const DeserializerPointerTracker& Other) = delete;
        DeserializerPointerTracker& operator=(DeserializerPointerTracker&& Other) = delete;

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        //Deserialization
        // Detect if the authoritive (non-link) copy has been deserialized/created.
        //  Queue a link resolution if it hasn't.
        //  Assign the value of the authoritive copy if it has.
        //   Additionally, resolve any pending links if the owned copy exists.

        template<typename PtrType>
        uintptr_t TrackPointer(PtrType& ToTrack, const Boole IsOwner)
        {
            static_assert(std::is_pointer_v<PtrType>,"Provided type is NOT a pointer.");

            PtrType NoRefPtr = ToTrack;
            uintptr_t PtrID = GetIDOfPtr(NoRefPtr);
            ObjectLink& FoundLink = this->GetOrCreateLink<PtrType>(PtrID);
            if( IsOwner ) {
                if( FoundLink.Instance ) {
                    throw std::runtime_error("Detected multiple owners to pointer.");
                }
                FoundLink.Instance = NoRefPtr;
                if( !FoundLink.UnresolvedLinks.empty() ) {
                    for( void*& Pending : FoundLink.UnresolvedLinks )
                        { Pending = NoRefPtr; }
                }
            }else{
                if( FoundLink.Instance ) {
                    ToTrack = FoundLink.Instance;
                }else{
                    FoundLink.UnresolvedLinks.push_back( std::ref(ToTrack) );
                }
            }
            return PtrID;
        }
        template<typename PtrType>
        uintptr_t TrackPointer(std::shared_ptr<PtrType>& ToTrack, const Boole IsOwner)
        {
            static_assert(std::is_pointer_v<PtrType>,"Provided type is NOT a pointer.");

            uintptr_t PtrID = GetIDOfPtr(ToTrack.get());
            ObjectJoin& FoundJoin = this->GetOrCreateJoin<PtrType>(PtrID);
            if( IsOwner ) {
                if( FoundJoin.Instance ) {
                    throw std::runtime_error("Detected multiple owners to pointer.");
                }
                FoundJoin.Instance = ToTrack;
                if( !FoundJoin.UnresolvedJoins.empty() ) {
                    for( std::shared_ptr<void>& Pending : FoundJoin.UnresolvedJoins )
                        { Pending = ToTrack; }
                }
            }else{
                if( FoundJoin.Instance ) {
                    ToTrack = FoundJoin.Instance;
                }else{
                    FoundJoin.UnresolvedJoins.push_back( std::ref(ToTrack) );
                }
            }
            return PtrID;
        }
    };//DeserializerPointerTracker

    class MEZZ_LIB SerializerTrackerHolder
    {
    protected:
        std::vector<SerializerPointerTracker*> SerializerTrackers;

        void RemoveAllTrackers()
        {
            for( SerializerPointerTracker* Ptr : this->SerializerTrackers )
                { delete Ptr; }
            this->SerializerTrackers.clear();
        }
    public:
        SerializerTrackerHolder() = default;
        SerializerTrackerHolder(const SerializerTrackerHolder& Other) = delete;
        SerializerTrackerHolder(SerializerTrackerHolder&& Other) = delete;

        SerializerTrackerHolder& operator=(const SerializerTrackerHolder& Other) = delete;
        SerializerTrackerHolder& operator=(SerializerTrackerHolder&& Other) = delete;

        ~SerializerTrackerHolder()
            { this->RemoveAllTrackers(); }
    };//SerializerTrackerHolder

    class MEZZ_LIB DeserializerTrackerHolder
    {
    protected:
        std::vector<DeserializerPointerTracker*> DeserializerTrackers;

        void RemoveAllTrackers()
        {
            for( DeserializerPointerTracker* Ptr : this->DeserializerTrackers )
                { delete Ptr; }
            this->DeserializerTrackers.clear();
        }
    public:
        DeserializerTrackerHolder() = default;
        DeserializerTrackerHolder(const DeserializerTrackerHolder& Other) = delete;
        DeserializerTrackerHolder(DeserializerTrackerHolder&& Other) = delete;

        DeserializerTrackerHolder& operator=(const DeserializerTrackerHolder& Other) = delete;
        DeserializerTrackerHolder& operator=(DeserializerTrackerHolder&& Other) = delete;

        ~DeserializerTrackerHolder()
            { this->RemoveAllTrackers(); }
    };//DeserializerTrackerHolder

    /// @}
}//Serialization
}//Mezzanine

#endif // Mezz_Foundation_SerializationPointerTracker_h
