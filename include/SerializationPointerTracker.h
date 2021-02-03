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

    /// @brief A simple struct for counting occurrences of owned and non-owned pointers to an object.
    /// @remarks This is used during Serialization (not Deserialization) for raw and shared_ptr's.
    struct MEZZ_LIB ObjectCounter
    {
        /// @brief The unique ID of the object being pointed to.
        uintptr_t InstanceID = 0;
        /// @brief A count of the non-owned copies of the pointer.
        UInt32 LinkCount = 0;
        /// @brief A count of the owned copies of the pointer.
        /// @remarks For raw pointers this should never exceed 1, but exceeding one is common for shared_ptr's.
        UInt32 OwnerOrJoinCount = 0;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // ObjectLink (for handling non-owned pointers between objects)

    /// @brief A struct for tracking Links (non-owned copies of pointers).
    struct MEZZ_LIB ObjectLink
    {
        /// @brief A container of non-owned pointer instances that are unassigned.
        /// @remarks These instances remain unassigned until the owner is deserialized.
        std::vector< std::function<void(void*) > UnresolvedLinks;
        /// @brief The registered name of the type being pointed to.
        StringView TypeName;
        /// @brief The unique ID of the object being pointed to.
        uintptr_t InstanceID = 0;
        /// @brief A usable pointer to the object. Should only be valid after successful deserialization.
        void* Instance = nullptr;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // ObjectJoin (for handling shared ownership pointers between objects)

    /// @brief A struct for tracking Links (shared ownership of copies of pointers).
    struct MEZZ_LIB ObjectJoin
    {
        /// @brief A container of unassigned shared_ptr's.
        /// @remarks Despite each pointer being an owner, shared_ptr's will only serialize one instance
        /// in order to avoid data duplication and the issues that introduces. Additionally, we don't know
        /// exactly what order the shared_ptr's will be deserialized. So this exists to work around that.
        std::vector< std::function<void(void*) > UnresolvedJoins;
        /// @brief The registered name of the type being pointed to.
        StringView TypeName;
        /// @brief The unique ID of the object being pointed to.
        uintptr_t InstanceID = 0;
        /// @brief A usable pointer to the object. Should only be valid after successful deserialization.
        std::shared_ptr<void> Instance = nullptr;
    };

    ///////////////////////////////////////////////////////////////////////////////
    // Pointer ID Generator

    /// @brief Gets/generates an ID for an object pointer to by a pointer.
    /// @remarks The ID generated by this function will be unique, but only to the one run of the application
    /// it was generated in.  Further, the uniqueness applies to the object, and not the pointer itself.  Multiple
    /// instances of a pointer all pointing to the same object will generate the same value from this function.
    /// @return Returns a unique ID belonging to the pointer provided.
    uintptr_t GetIDOfPtr(void* Ptr);
    uintptr_t GetIDOfPtr(void* Ptr)
    {
        return reinterpret_cast<uintptr_t>(Ptr);
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Tracker Predicate

    /// @brief A somewhat simple and flexible predicate class for sorting any of the pointer tracking types.
    /// @tparam TrackerMeta The metadata used by the pointer tracker.  This type should be ObjectCounter,
    /// ObjectLink, or ObjectJoin.
    template<typename TrackerMeta>
    struct TrackerPredicate
    {
        /// @brief A simple type used to communicate to standard containers that multiple () operator overloads exist.
        using is_transparent = void;
        /// @brief Double TrackerMeta sorting operator.
        /// @param First The first TrackerMeta to be checked.
        /// @param Second The second TrackerMeta to be checked.
        /// @return Returns true if First should be placed before Second, false other
        Boole operator()(const TrackerMeta& First, const TrackerMeta& Second)
            { return First.InstanceID < Second.InstanceID; }
        /// @brief TrackerMeta/ID sorting operator.
        /// @param Link The TrackerMeta to be checked (will just access the InstanceID member).
        /// @param ID The ID to check against.
        /// @return Returns true if the Link parameter should be placed before the ID parameter, false otherwise.
        Boole operator()(const TrackerMeta& Link, const uintptr_t ID)
            { return Link.InstanceID < ID; }
        /// @brief ID/TrackerMeta sorting operator.
        /// @param ID The ID to check against.
        /// @param Link The TrackerMeta to be checked (will just access the InstanceID member).
        /// @return Returns true if the ID parameter should be placed before the Link parameter, false otherwise.
        Boole operator()(const uintptr_t ID, const TrackerMeta& Link)
            { return ID < Link.InstanceID; }
    };//TrackerPredicate

    ///////////////////////////////////////////////////////////////////////////////
    // PointerTracker

    /// @brief This class tracks the counts of pointers to objects in a serialization tree.
    class MEZZ_LIB SerializerPointerTracker
    {
    public:
        /// @brief Convenience type for storing Object/Pointer counts.
        using CountContainer = SortedVector<ObjectCounter,TrackerPredicate<ObjectCounter>>;
        /// @brief Iterator type for stored Object/Pointer counts.
        using CountIterator = CountContainer::iterator;
        /// @brief Const Iterator type stored Object/Pointer counts.
        using ConstCountIterator = CountContainer::const_iterator;
    protected:
        /// @brief Container storing current counts for the number of pointers to a given object.
        CountContainer PtrCounts;
    public:
        /// @brief Default constuctor.
        SerializerPointerTracker() = default;
        /// @brief Deleted Copy Constructor.
        /// @param Other The other tracker that WON'T be copied.
        SerializerPointerTracker(const SerializerPointerTracker& Other) = delete;
        /// @brief Deleted Move Constructor.
        /// @param Other The other track that WON'T be moved.
        SerializerPointerTracker(SerializerPointerTracker&& Other) = delete;
        /// @brief Class destructor.
        ~SerializerPointerTracker() = default;

        /// @brief Deleted Copy Assignment Operator.
        /// @param Other The other tracker that WON'T be copied.
        /// @return Returns a reference to this, or at least...it would.
        SerializerPointerTracker& operator=(const SerializerPointerTracker& Other) = delete;
        /// @brief Deleted Move Assignment Operator.
        /// @param Other The other tracker that WON'T be moved.
        /// @return Returns a reference to this, or at least...it would.
        SerializerPointerTracker& operator=(SerializerPointerTracker&& Other) = delete;

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        //Serialization
        // Detect if the object pointed to has already been serialized.
        //  Error when a second owner is detected.
        //  Signal for the system to create a link to the original otherwise.

        /// @brief Tracks an object as if it were a pointer.
        /// @tparam ObjType The deduced type to track.
        /// @remarks This function exists for the edge cases where a data member on a class may have a pointer
        /// pointing to it.  In such cases the data wasn't new'd (at least directly) and didn't start it's existance
        /// as a pointer.  But if it is viewed through a pointer elsewhere then it is still an owned pointer from the
        /// serialization systems perspective.
        /// @param ToTrack The object instance to track as if it were a owned pointer.
        /// @return Returns an ObjectCounter reference that gives basic information about the tracked object/pointer.
        template<typename ObjType>
        const ObjectCounter& TrackObject(ObjType& ToTrack)
            { return TrackObject( &ToTrack ); }

        /// @brief Tracks a regular pointer (owned or not).
        /// @remarks The purpose of this function is to keep track of the pointers that exist in the serialization
        /// tree and how many copies exist of each throughout the serialization tree.
        /// @param ToTrack The pointer to be tracked.
        /// @param IsOwner Whether or not the pointer instance being tracked is an owning pointer.
        /// @return Returns an ObjectCounter reference that gives basic information about the tracked pointer.
        const ObjectCounter& TrackPointer(void* ToTrack, const Boole IsOwner = false)
        {
            uintptr_t PtrID = GetIDOfPtr(ToTrack);
            CountIterator FoundIt = this->PtrCounts.find(PtrID);
            if( FoundIt != this->PtrCounts.end() ) {
                ObjectCounter& FoundCounter = *FoundIt;
                FoundCounter.LinkCount++;
                if( IsOwner ) {
                    if( FoundCounter.OwnerOrJoinCount != 0 ) {
                        throw std::runtime_error("Multiple owners for object found.");
                    }else{
                        FoundCounter.OwnerOrJoinCount = 1;
                    }
                }
                return FoundCounter;
            }else{
                ObjectCounter NewCounter;
                NewCounter.InstanceID = PtrID;
                NewCounter.LinkCount = 1;
                NewCounter.OwnerOrJoinCount = IsOwner;
                return *( this->PtrCounts.add(NewCounter) );
            }
        }
        /// @brief Tracks a shared_ptr.
        /// @remarks The purpose of this function is to keep track of the pointers that exist in the serialization
        /// tree and how many copies exist of each throughout the serialization tree.
        /// @param ToTrack The shared_ptr to be tracked.
        /// @return Returns an ObjectCounter reference that gives basic information about the tracked pointer.
        const ObjectCounter& TrackSharedPointer(std::shared_ptr<void> ToTrack)
        {
            uintptr_t PtrID = GetIDOfPtr(ToTrack.get());
            CountIterator FoundIt = this->PtrCounts.find(PtrID);
            if( FoundIt != this->PtrCounts.end() ) {
                ObjectCounter& FoundCounter = *FoundIt;
                FoundCounter.OwnerOrJoinCount++;
                return FoundCounter;
            }else{
                ObjectCounter NewCounter;
                NewCounter.InstanceID = PtrID;
                NewCounter.OwnerOrJoinCount = 1;
                return *( this->PtrCounts.add(NewCounter) );
            }
        }

        /// @brief Gets the beginning of the range of Object/Pointer counts.
        /// @return Returns a const iterator to the beginning of the Object/Pointer count range.
        [[nodiscard]]
        ConstCountIterator GetCountBegin() const
            { return this->PtrCounts.cbegin(); }
        /// @brief Gets the end of the range of Object/Pointer counts.
        /// @return Returns a const iterator to the end of the Object/Pointer count range.
        [[nodiscard]]
        ConstCountIterator GetCountEnd() const
            { return this->PtrCounts.cend(); }
    };//SerializerPointerTracker

    /// @brief This class tracks and initializes all the pointers in the deserialization tree.
    class MEZZ_LIB DeserializerPointerTracker
    {
    public:
        /// @brief Convenience type for the container storing metadata for non-shared pointers.
        using LinkContainer = SortedVector<ObjectLink,TrackerPredicate<ObjectLink>>;
        /// @brief Iterator type for stored non-shared pointer metadata.
        using LinkIterator = LinkContainer::iterator;
        /// @brief Convenience type for the container storing metadata for shared pointers.
        using JoinContainer = SortedVector<ObjectJoin,TrackerPredicate<ObjectJoin>>;
        /// @brief Iterator type for stored shared pointer metadata.
        using JoinIterator = JoinContainer::iterator;
    protected:
        /// @brief The container storing metadata for each non-shared pointer in the deserialization tree.
        LinkContainer TrackedLinks;
        /// @brief The container storing metadata for each shared pointer in the deserialization tree.
        JoinContainer TrackedJoins;
        /// @brief A pointer to the last Link that was created/manipulated.
        ObjectLink* LastLink = nullptr;
        /// @brief A pointer to the last Join that was created/manipulated.
        ObjectJoin* LastJoin = nullptr;

        /// @brief Creates a new ObjectLink with the provided ID, or gets a pre-existing one if it exists.
        /// @sa ObjectLink
        /// @tparam PtrType The pointer type of the Link.  Only used when creating and only for the registered name.
        /// @param PtrID The ID used to identify the pointer/object instance during deserialization.
        /// @return Returns an ObjectLink reference belonging to the provided PtrID.
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
        /// @brief Creates a new ObjectJoin with the provided ID, or gets a pre-existing one if it exists.
        /// @sa ObjectJoin
        /// @tparam PtrType The pointer type of the Join.  Only used when creating and only for the registered name.
        /// @param PtrID The ID used to identify the pointer/object instance during deserialization.
        /// @return Returns an ObjectJoin reference belonging to the provided PtrID.
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
        /// @brief Default constuctor.
        DeserializerPointerTracker() = default;
        /// @brief Deleted Copy Constructor.
        /// @param Other The other tracker that WON'T be copied.
        DeserializerPointerTracker(const DeserializerPointerTracker& Other) = delete;
        /// @brief Deleted Move Constructor.
        /// @param Other The other track that WON'T be moved.
        DeserializerPointerTracker(DeserializerPointerTracker&& Other) = delete;
        /// @brief Class destructor.
        ~DeserializerPointerTracker() = default;

        /// @brief Deleted Copy Assignment Operator.
        /// @param Other The other tracker that WON'T be copied.
        /// @return Returns a reference to this, or at least...it would.
        DeserializerPointerTracker& operator=(const DeserializerPointerTracker& Other) = delete;
        /// @brief Deleted Move Assignment Operator.
        /// @param Other The other tracker that WON'T be moved.
        /// @return Returns a reference to this, or at least...it would.
        DeserializerPointerTracker& operator=(DeserializerPointerTracker&& Other) = delete;

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        //Deserialization
        // Detect if the authoritive (non-link) copy has been deserialized/created.
        //  Queue a link resolution if it hasn't.
        //  Assign the value of the authoritive copy if it has.
        //   Additionally, resolve any pending links if the owned copy exists.

        template<typename PtrType>
        void TrackPointer(uintptr_t PtrID, PtrType OwnedPtr)
        {
            static_assert(std::is_pointer_v<PtrType>,"Provided type is NOT a pointer.");

            // If the provided pointer is an owning copy, mark it as created and apply all pending links.

            // Check of an owning copy of the pointer in question exists.

            // If it does, use Funct to assign it immediately.

            // If it doesn't, then store funct in a sensical way that we can use.

            std::function<void(void*, const std::type_info&)> PendingSetter = [=] (void* ToCast, const std::type_info& Info) {
                PtrType NonVoid = static_cast<PtrType>(ToCast);
                Funct(NonVoid);
            }
        }

        template<typename ApplyType>
        void TrackPointer(uintptr_t PtrID, ApplyType&& Funct)
        {
            static_assert(std::is_pointer_v<PtrType>,"Provided type is NOT a pointer.");

            // Check of an owning copy of the pointer in question exists.

            // If it does, use Funct to assign it immediately.

            // If it doesn't, then store funct in a sensical way that we can use.

            std::function<void(void*, const std::type_info&)> PendingSetter = [=] (void* ToCast) {
                PtrType NonVoid = static_cast<PtrType>(ToCast);
                Funct(NonVoid);
            }
        }

        std::optional<ObjectLink*> NeedsLinkApply(uintptr_t PtrID)
        {

        }

        std::optional<ObjectJoin*> NeedsJoinApply(uintptr_t PtrID)
        {

        }

        /// @brief Tracks a regular pointer (owned or not).
        /// @tparam PtrType The type of the pointer to be tracked. Will static_assert if type isn't a pointer.
        /// @param ToTrack The pointer to be tracked.
        /// @param IsOwner Whether or not the pointer instance being tracked is an owning pointer.
        /// @return Returns the tracked pointers ID that can be used for validation.
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
        /// @brief Tracks a shared_ptr.
        /// @tparam PtrType The type of the pointer to be tracked. Will static_assert if type isn't a pointer.
        /// @param ToTrack The shared_ptr to be tracked.
        /// @return Returns the tracked pointers ID that can be used for validation.
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
/*
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
//*/
    /// @}
}//Serialization
}//Mezzanine

#endif // Mezz_Foundation_SerializationPointerTracker_h
