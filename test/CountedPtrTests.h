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
#ifndef MEZZ_TEST_CountedPtr_h
#define MEZZ_TEST_CountedPtr_h

/// @file
/// @brief This file tests the functionality of the CountedPtr class.

#include "MezzTest.h"

#include "CountedPtr.h"

// Dummy classes for testing purposes.

namespace Mezzanine
{
    namespace CountedPtrTesting
    {
        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wweak-vtables")
        SUPPRESS_CLANG_WARNING("-Wpadded")
        SUPPRESS_VC_WARNING(4435)

        /// @brief A class to point at
        class FooExternal
        {
            public:
                /// @brief A pointer to a bool to indicate whether the object's destructor has been called.
                Boole* IsDestructed;
                /// @brief A value to use for testing purposes.
                Integer Value;

                /// @brief A constructor that allows setting the value.
                /// @param DestructFlag Where to save destruction results.
                /// @param Val A default value to use for testing.
                explicit FooExternal(Boole* DestructFlag, Integer Val = 0)
                    : IsDestructed(DestructFlag), Value(Val)
                    {}

                /// @brief A destructor that sets the target of IsDestructed to true.
                ~FooExternal()
                {
                    *IsDestructed = true;
                }
        };

        /// @brief A class to point at that uses its own reference counting internal mechanism.
        class FooInternal
        {
            private:
                /// @brief This is the counter that stores how many references exist.
                Whole RefCount;

            public:
                /// @brief A pointer a bool to indicate whether the object's destructor has been called.
                Boole* IsDestructed;
                /// @brief A value to use for testing purposes.
                Integer Value;

                /// @brief A constructor that allows setting the value.
                /// @param DestructFlag Where to save destruction results.
                /// @param Val A default value to use for testing.
                explicit FooInternal(Boole* DestructFlag, Integer Val = 0)
                    : RefCount(0), IsDestructed(DestructFlag), Value(Val)
                    {}

                explicit FooInternal()
                    : RefCount(0), Value(0)
                    {
                        Boole destructFlag = false;
                        IsDestructed = &destructFlag;
                    }

                /// @brief A destructor that sets the target of IsDestructed to true.
                virtual ~FooInternal()
                {
                    *IsDestructed = true;
                }

                /// @brief Increase the reference count by one and return the updated count.
                /// @return The updated count;
                Whole IncrementReferenceCount()
                    { return ++RefCount; }

                /// @brief Decrease the reference count by one and return the updated count.
                /// @return The updated count;
                Whole DecrementReferenceCount()
                    { return --RefCount; }

                /// @brief Gets the actual pointer to the target.
                /// @return A Pointer of the targeted type to the object being managed.
                FooInternal* GetReferenceCountTargetAsPointer()
                    { return this; }

                /// @brief Get the current amount of references.
                /// @return A Whole with the current reference count.
                Whole GetReferenceCount()
                    { return RefCount; }

                /// @brief Get a pointer to the most derived type of this class.
                /// @return A pointer cast to a void*, for use with CountedPtrCast
                virtual FooInternal* GetMostDerived()
                    { return this; }
        };

        /// @brief A class to point at that is derived from FooInternal.
        class FooDerived1 : public virtual FooInternal
        {
            public:
                /// @brief A value to use for testing purposes.
                Integer Value1;

                /// @brief Get a pointer to the most derived type of this class.
                /// @return A pointer for use with CountedPtrCast.
                virtual FooInternal* GetMostDerived()
                    { return this; }
        };

        /// @brief A second class to point at that is derived from FooInternal.
        class FooDerived2 : public virtual FooInternal
        {
            public:
                Integer Value2;

                /// @brief Get a pointer to the most derived type of this class.
                /// @return A pointer for use with CountedPtrCast.
                virtual FooInternal* GetMostDerived()
                    { return this; }
        };

        /// @brief A class derived from two classes in turn derived from FooInternal.
        class FooDiamond : public FooDerived1, public FooDerived2
        {
            public:
                Integer ValueDiamond;

                /// @brief Get a pointer to the most derived type of this class.
                /// @return A pointer for use with CountedPtrCast.
                virtual FooInternal* GetMostDerived()
                    { return this; }
        };

        /// @brief A class to point at that uses its own referencing counting internal mechanism, can be static cast.
        class VehicleTest
        {
            private:
                /// @brief This is the counter that stores how many references exist.
                Whole RefCount;

            public:
                /// @brief Increase the reference count by one and return the updated count.
                /// @return The updated count;
                Whole IncrementReferenceCount()
                    { return ++RefCount; }

                /// @brief Decrease the reference count by one and return the updated count.
                /// @return The updated count;
                Whole DecrementReferenceCount()
                    { return --RefCount; }

                /// @brief Gets the actual pointer to the target.
                /// @return A Pointer of the targeted type to the object being managed.
                VehicleTest* GetReferenceCountTargetAsPointer()
                    { return this; }

                /// @brief Get the current amount of references.
                /// @return A Whole with the current reference count
                Whole GetReferenceCount()
                    { return RefCount; }

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer for use with CountedPtrCast
                virtual VehicleTest* GetMostDerived()
                    { return this; }

                String StartEngine()
                    { return "Unknown Engine"; }

                virtual ~VehicleTest()
                    {}
        };

        /// @brief A class to point at that inherits from VehicleTest and can be static cast.
        class CarTest : public VehicleTest
        {
            public:
                String StartEngine()
                    { return "Starting V6"; }

                /// @brief Get a pointer to the most Derived type of this class
                /// @return A pointer for use with CountedPtrCast
                virtual CarTest* GetMostDerived()
                    { return this; }

                virtual ~CarTest()
                    {}
        };

        RESTORE_WARNING_STATE
    } // End Testing Namespace

    using namespace CountedPtrTesting;

    template <>
    class ReferenceCountTraits <FooInternal>
    {
        public:
            using RefCountType = FooInternal;

            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            enum { IsCastable = CastDynamic };
    };

    template <>
    class ReferenceCountTraits <FooDerived1>
    {
        public:
            using RefCountType = FooDerived1;

            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            enum { IsCastable = CastDynamic };
    };

    template <>
    class ReferenceCountTraits <FooDerived2>
    {
        public:
            using RefCountType = FooDerived2;

            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            enum { IsCastable = CastDynamic };
    };

    template <>
    class ReferenceCountTraits <FooDiamond>
    {
        public:
            using RefCountType = FooDiamond;

            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            enum { IsCastable = CastDynamic };
    };

    template <>
    class ReferenceCountTraits <VehicleTest>
    {
        public:
            typedef VehicleTest RefCountType;

            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            enum { IsCastable = CastStatic };
    };

    template <>
    class ReferenceCountTraits <CarTest>
    {
        public:
            typedef CarTest RefCountType;

            static RefCountType* ConstructionPointer(RefCountType* Target)
                { return Target; }

            enum { IsCastable = CastStatic };
    };
}

// Actual Tests

DEFAULT_TEST_GROUP(CountedPtrTests,CountedPtr)
{
    using namespace Mezzanine;

    { // Construction
        Boole destructFlagExternal = false;
        CountedPtr<FooExternal> PtrInitial( new FooExternal(&destructFlagExternal, 1) );

        TEST_EQUAL("CountedPtr(ElementType)", Whole(1), PtrInitial.UseCount());

        CountedPtr<FooExternal> PtrCopy( PtrInitial );

        TEST_EQUAL("CountedPtr(CountedPtr&)-Ptr1", Whole(2), PtrInitial.UseCount());
        TEST_EQUAL("CountedPtr(CountedPtr&)-Ptr2", Whole(2), PtrInitial.UseCount());

        CountedPtr<FooExternal> PtrMove( std::move(PtrCopy) );

        TEST_EQUAL("CountedPtr(CountedPtr&&)-NewPointer", Whole(2), PtrMove.UseCount());
        TEST_EQUAL("CountedPtr(CountedPtr&&)-OldPointer", Whole(0), PtrCopy.UseCount());
    }

    { // Automated destruction
        Boole destructFlagExternal = false;
        Boole destructFlagInternal = false;

        {
            CountedPtr<FooExternal> PtrE( new FooExternal(&destructFlagExternal, 1) );
            CountedPtr<FooInternal> PtrI( new FooInternal(&destructFlagInternal, 3) );

            TEST_EQUAL("AutomatedDestruction-ExternalPreDestruction", destructFlagExternal, false);
            TEST_EQUAL("AutomatedDestruction-InternalPreDestruction", destructFlagInternal, false);
        } // When pointers fall out of scope

        TEST_EQUAL("AutomatedDestruction-ExternalPostDestruction", destructFlagExternal, true);
        TEST_EQUAL("AutomatedDestruction-InternalPostDestruction", destructFlagInternal, true);
    }

    { // Use count and dual handle delete
        Boole destructFlagExternal = false;
        Boole destructFlagInternal = false;

        {
            CountedPtr<FooExternal> PtrE( new FooExternal(&destructFlagExternal, 1) );
            CountedPtr<FooExternal> PtrE2( PtrE );

            TEST_EQUAL("UseCount()-External", Whole(2), PtrE.UseCount())

            CountedPtr<FooInternal> PtrI( new FooInternal(&destructFlagInternal, 3) );
            CountedPtr<FooInternal> PtrI2( PtrI );

            TEST_EQUAL("UseCount()-Internal", Whole(2), PtrI.UseCount())

            {
                CountedPtr<FooExternal> PtrE3( PtrE );
                CountedPtr<FooInternal> PtrI3( PtrI );

                TEST_EQUAL("UseCount()-ExternalFullCount", Whole(3), PtrE.UseCount())
                TEST_EQUAL("UseCount()-InternalFullCount", Whole(3), PtrI.UseCount())
            }

            TEST_EQUAL("UseCount()-ExternalPartialCount", Whole(2), PtrE.UseCount())
            TEST_EQUAL("UseCount()-InternalPartialCount", Whole(2), PtrI.UseCount())
            TEST_EQUAL("AutomatedDestructionUse2-ExternalPreDestruction", destructFlagExternal, false);
            TEST_EQUAL("AutomatedDestructionUse2-InternalPreDestruction", destructFlagInternal, false);
        } // When pointers fall out of scope

        TEST_EQUAL("AutomatedDestructionUse2-ExternalPostDestruction", destructFlagExternal, true);
        TEST_EQUAL("AutomatedDestructionUse2-InternalPostDestruction", destructFlagInternal, true);
    }

    { // Release and dual handles
        Boole destructFlagExternal = false;
        Boole destructFlagInternal = false;

        CountedPtr<FooExternal> PtrE( new FooExternal(&destructFlagExternal, 1) );
        CountedPtr<FooExternal> PtrE2( PtrE );

        TEST_EQUAL("Reset()-ExternalPreReset", destructFlagExternal, false);
        PtrE.Reset();
        TEST_EQUAL("Reset()-ExternalPostReset1", destructFlagExternal, false);
        PtrE2.Reset();
        TEST_EQUAL("Reset()-ExternalPostReset2", destructFlagExternal, true);

        CountedPtr<FooInternal> PtrI( new FooInternal(&destructFlagInternal, 3) );
        CountedPtr<FooInternal> PtrI2( PtrI );

        TEST_EQUAL("Reset()-InternalPreReset", destructFlagInternal, false);
        PtrI.Reset();
        TEST_EQUAL("Reset()-InternalPostReset1", destructFlagInternal, false);
        PtrI2.Reset();
        TEST_EQUAL("Reset()-InternalPostReset2", destructFlagInternal, true);
    }


    {// Reset
        CountedPtr<int> IntPtr(new int(0));
        IntPtr.Reset();
        TEST_EQUAL("Reset()", Whole(0), IntPtr.UseCount());

        IntPtr.Reset(new int(10));
        TEST_EQUAL("Reset(TypePointedTo*)", 10, *IntPtr);
        TEST_EQUAL("Reset(TypePointedTo*)-UseCount", Whole(1), IntPtr.UseCount());

        CountedPtr<int> TwentyTwo(new int(22));
        IntPtr.Reset(TwentyTwo);
        TEST_EQUAL("Reset(CountedPtr<TypePointedTo>&)", 22, *IntPtr);
        TEST_EQUAL("Reset(CountedPtr<TypePointedto>&)-UseCount", Whole(2), IntPtr.UseCount());
    }

    { // Dereferencing
        Boole destructFlagExternal = false;
        Boole destructFlagInternal = false;

        CountedPtr<FooExternal> PtrE( new FooExternal(&destructFlagExternal, 1) );
        CountedPtr<FooInternal> PtrI( new FooInternal(&destructFlagInternal, 3) );

        CountedPtr<FooExternal> PtrE2( PtrE );
        CountedPtr<FooInternal> PtrI2( PtrI );

        TEST_EQUAL("operator*-External1", (*PtrE).Value, 1);
        TEST_EQUAL("operator*-External2", (*PtrE2).Value, 1);
        TEST_EQUAL("operator*-Internal1", (*PtrI).Value, 3);
        TEST_EQUAL("operator*-Internal2", (*PtrI2).Value, 3);

        TEST_EQUAL("operator->-External1", PtrE->Value, 1);
        TEST_EQUAL("operator->-External2", PtrE2->Value, 1);
        TEST_EQUAL("operator->-Internal1", PtrI->Value, 3);
        TEST_EQUAL("operator->-Internal2", PtrI2->Value, 3);
    }

    { // Reseating pointers and equality
        Boole destructFlagExternal = false;
        Boole destructFlagInternal = false;

        Boole destructFlagExternal2 = false;
        Boole destructFlagInternal2 = false;

        CountedPtr<FooExternal> PtrE( new FooExternal(&destructFlagExternal, 1) );
        CountedPtr<FooInternal> PtrI( new FooInternal(&destructFlagInternal, 3) );

        CountedPtr<FooExternal> PtrE2( new FooExternal(&destructFlagExternal2, 2) );
        CountedPtr<FooInternal> PtrI2( new FooInternal(&destructFlagInternal2, 4) );

        TEST_EQUAL("operator!=-ExternalTrue", true, PtrE != PtrE2);
        TEST_EQUAL("operator!=-InternalTrue", true, PtrI != PtrI2);
        TEST_EQUAL("operator==-ExternalFalse", false, PtrE == PtrE2);
        TEST_EQUAL("operator==-InternalFalse", false, PtrI == PtrI2);

        PtrE2 = PtrE;
        PtrI2 = PtrI;

        TEST_EQUAL("operator=-ExternalReseatFalse", destructFlagExternal, false);
        TEST_EQUAL("operator=-ExternalReseatTrue", destructFlagExternal2, true);
        TEST_EQUAL("operator=-InternalReseatFalse", destructFlagInternal, false);
        TEST_EQUAL("operator=-InternalReseatTrue", destructFlagInternal2, true);

        TEST_EQUAL("operator==-ExternalTrue", true, PtrE == PtrE2);
        TEST_EQUAL("operator==-InternalTrue", true, PtrI == PtrI2);
        TEST_EQUAL("operator!=-ExternalFalse", false, PtrE != PtrE2);
        TEST_EQUAL("operator!=-InternalFalse", false, PtrI != PtrI2);

        TEST_EQUAL("operator=-ExternalValue", PtrE->Value, 1);
        TEST_EQUAL("operator=-External2Value", PtrE2->Value, 1);
        TEST_EQUAL("operator=-InternalValue", PtrI->Value, 3);
        TEST_EQUAL("operator=-Internal2Value", PtrI2->Value, 3);
    }

    { // Get(), Unique(), operatorBoole()
        Boole destructFlagExternal = false;
        Boole destructFlagInternal = false;

        FooExternal* FooE = new FooExternal(&destructFlagExternal, 1);
        FooInternal* FooI = new FooInternal(&destructFlagInternal, 3);
        CountedPtr<FooExternal> PtrE(FooE);
        CountedPtr<FooInternal> PtrI(FooI);

        TEST_EQUAL("Get()-External", FooE, PtrE.Get());
        TEST_EQUAL("Get()-Internal", FooI, PtrI.Get());

        TEST_EQUAL("Unique()-ExternalTrue", true, PtrE.Unique());
        TEST_EQUAL("Unique()-InternalTrue", true, PtrI.Unique());

        {
            CountedPtr<FooExternal> PtrE2(PtrE);
            CountedPtr<FooInternal> PtrI2(PtrI);

            TEST_EQUAL("Unique()-ExternalFalse", false, PtrE.Unique());
            TEST_EQUAL("Unique()-InternalFalse", false, PtrI.Unique());
        } // Only second reference falls out of scope

        TEST_EQUAL("Unique()-ExternalTrue2", true, PtrE.Unique());
        TEST_EQUAL("Unique()-InternalTrue2", true, PtrI.Unique());

        TEST_EQUAL("operatorBoole()-ExternalTrue", true, PtrE);
        TEST_EQUAL("operatorBoole()-InternalTrue", true, PtrI);

        PtrE.Reset();
        PtrI.Reset();

        TEST_EQUAL("operatorBoole()-ExternalFalse", false, PtrE);
        TEST_EQUAL("operatorBoole()-InternalFalse", false, PtrI);
    }

    { // Inheritance and casting
        CountedPtr<FooDiamond> DiamondPtr(new FooDiamond);

        DiamondPtr->Value = 0;
        DiamondPtr->Value1 = 1;
        DiamondPtr->Value2 = 2;
        DiamondPtr->ValueDiamond = 3;

        CountedPtr<FooInternal> InternalPtrFromImplicitCast = CountedPtrCast<FooInternal>(DiamondPtr);
        CountedPtr<FooInternal> InternalPtrFromCrossConstruction(DiamondPtr);

        TEST_EQUAL("CountedPtrCast()-DiamondExplicit", 0, InternalPtrFromImplicitCast->Value);
        TEST_EQUAL("CountedPtrCast()-DiamondCastCrossConstruction", 0, InternalPtrFromCrossConstruction->Value);


        VehicleTest* Car = new CarTest;

        CountedPtr<VehicleTest> CarPtr(Car);

        CountedPtr<CarTest> CarPtrAfterStaticCast = CountedPtrStaticCast<CarTest>(CarPtr);
        CountedPtr<VehicleTest> VehiclePtrAfterStaticCast = CountedPtrStaticCast<VehicleTest>(CarPtrAfterStaticCast);

        TEST_EQUAL("CountedPtrStaticCast()-VehicleToCar", "Starting V6",CarPtrAfterStaticCast->StartEngine());
        TEST_EQUAL("CountedPtrStaticCast()-CarToVehicle", "Unknown Engine",VehiclePtrAfterStaticCast->StartEngine());

        SAVE_WARNING_STATE
        SUPPRESS_VC_WARNING(4437)

        CountedPtr<CarTest> CarPtrAfterDynamicCast = CountedPtrDynamicCast<CarTest>(CarPtr);
        CountedPtr<VehicleTest> VehiclePtrAfterDynamicCast = CountedPtrDynamicCast<VehicleTest>(CarPtrAfterDynamicCast);

        TEST_EQUAL("CountedPtrDynamicCast()-VehicleToCar", "Starting V6",CarPtrAfterDynamicCast->StartEngine());
        TEST_EQUAL("CountedPtrDynamicCast()-CarToVehicle", "Unknown Engine",VehiclePtrAfterDynamicCast->StartEngine());
        TEST_EQUAL("CountedPtrDynamicCast()-CastFail", false, CountedPtrDynamicCast<FooExternal>(CarPtr));

        RESTORE_WARNING_STATE
    }
}

#endif
