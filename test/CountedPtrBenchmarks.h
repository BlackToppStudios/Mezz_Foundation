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
#ifndef Mezz_Foundation_CountedPtrBenchmarks_h
#define Mezz_Foundation_CountedPtrBenchmarks_h

/// @file
/// @brief This contains benchmarks tests on the performance of the CountedPtr class.

#include "MezzTest.h"

#include "CountedPtr.h"

BENCHMARK_TEST_GROUP(CountedPtrBenchmarks,CountedPtrBenchmarks)
{
    using namespace Mezzanine;

    Integer OutputE = 0;
    Integer OutputI = 0;
    Integer OutputS = 0;

    Boole destructFlag = false; // Only used here for construction of the test classes.

    using ClockType = std::chrono::high_resolution_clock;
    using TimeType = ClockType::time_point;
    using DurationType = ClockType::duration;

    TimeType Begin;
    TimeType End;

    {
        TestLog << "\n" << "CountedPtr Benchmarks" << "\n\n"
                << "The objects being created all change a variable on destruction and have initializing, "
                << "but otherwise trivial constructors. This is useful only for comparing the speeds of the point "
                << "constructs on this platform, not for providing objective pointer dereferencing costs." << "\n\n";
        const Whole TestCount=10000000;

        /////////////////////////////////////
        // Create tests

        /// Raw Pointers
        Begin = ClockType::now();
        for( Whole Count = 0 ; Count < TestCount ; Count++ )
        {
            FooExternal* PtrR = new FooExternal(&destructFlag, 1);
            OutputS = PtrR->Value;
            delete PtrR;
        }
        End = ClockType::now();
        DurationType RawPtrTime = End - Begin;
        TestLog << OutputS << " - Creating and Dereferencing a raw pointer " << TestCount
                << " times without reference counting took: " << RawPtrTime.count() << " Microseconds."
                << "\n";

        /// Internal Reference Counter
        Begin = ClockType::now();
        for( Whole Count = 0 ; Count < TestCount ; Count++ )
        {
            CountedPtr<FooInternal> PtrI( new FooInternal(&destructFlag, 2) );
            OutputI = PtrI->Value;
        }
        End = ClockType::now();
        DurationType CountPtrInternalTime = End - Begin;
        TestLog << OutputI << " - Creating and Dereferencing a CountedPtr " << TestCount
                << " times with internal counting took: " << CountPtrInternalTime.count() << " Microseconds."
                << "\n";

        /// External Reference Counter
        Begin = ClockType::now();
        for( Whole Count = 0 ; Count < TestCount ; Count++ )
        {
            CountedPtr<FooExternal> PtrE( new FooExternal(&destructFlag, 3) );
            OutputE = PtrE->Value;
        }
        End = ClockType::now();
        DurationType CountPtrExternalTime = End - Begin;
        TestLog << OutputE << " - Creating and Dereferencing a CountedPtr " << TestCount
                << " times with external counting took: " << CountPtrExternalTime.count() << " Microseconds."
                << "\n";

        /// shared_ptr
        Begin = ClockType::now();
        for( Whole Count = 0 ; Count < TestCount ; Count++ )
        {
            std::shared_ptr<FooExternal> PtrS( new FooExternal(&destructFlag, 4) );
            OutputS = PtrS->Value;
        }
        End = ClockType::now();
        DurationType SharedPtrTime = End - Begin;
        TestLog << OutputS << " - Creating and Dereferencing a shared_ptr " << TestCount
                << " times with external counting took: " << SharedPtrTime.count() << " Microseconds."
                << "\n";

        /// shared_ptr Through make_shared
        Begin = ClockType::now();
        for( Whole Count = 0 ; Count < TestCount ; Count++ )
        {
            std::shared_ptr<FooExternal> PtrS = std::make_shared<FooExternal>(&destructFlag, 5);
            OutputS = PtrS->Value;
        }
        End = ClockType::now();
        DurationType MakeSharedTime = End - Begin;
        TestLog << OutputS << " - Creating and Dereferencing a shared_ptr from make_shared " << TestCount
                << " times with external counting took: " << MakeSharedTime.count() << " Microseconds."
                << "\n";

        /////////////////////////////////////
        // Copy tests

        /// Raw Pointers
        Begin = ClockType::now();
        for( Whole Count = 0 ; Count < TestCount ; Count++ )
        {
            FooExternal* PtrR = new FooExternal(&destructFlag, 6);
            FooExternal* PtrR2 = PtrR;
            OutputE = PtrR2->Value;
            delete PtrR2;
        }
        End = ClockType::now();
        DurationType RawPtrCopyTime = End - Begin;
        TestLog << OutputE << " - Creating, Dereferencing and Copying a raw pointer " << TestCount
                << " times without reference counting took: " << RawPtrCopyTime.count() << " Microseconds."
                << "\n";

        /// Internal Reference Counter
        Begin = ClockType::now();
        for( Whole Count = 0 ; Count < TestCount ; Count++ )
        {
            CountedPtr<FooInternal> PtrI( new FooInternal(&destructFlag, 7) );
            CountedPtr<FooInternal> PtrI2( PtrI );
            OutputI = PtrI2->Value;
        }
        End = ClockType::now();
        DurationType CountPtrCopyInternalTime = End - Begin;
        TestLog << OutputI << " - Creating, Dereferencing and Copying a CountedPtr " << TestCount
                << " times with internal counting took: " << CountPtrCopyInternalTime.count() << " Microseconds."
                << "\n";

        /// External Reference Counter
        Begin = ClockType::now();
        for( Whole Count = 0 ; Count < TestCount ; Count++ )
        {
            CountedPtr<FooExternal> PtrE( new FooExternal(&destructFlag, 8) );
            CountedPtr<FooExternal> PtrE2(PtrE);
            OutputE = PtrE2->Value;
        }
        End = ClockType::now();
        DurationType CountPtrCopyExternalTime = End - Begin;
        TestLog << OutputE << " - Creating, Dereferencing and Copying a CountedPtr " << TestCount
                << " times with external counting took: " << CountPtrCopyExternalTime.count() << " Microseconds."
                << "\n";


        /// shared_ptr
        Begin = ClockType::now();
        for( Whole Count = 0 ; Count < TestCount ; Count++ )
        {
            std::shared_ptr<FooExternal> PtrS( new FooExternal(&destructFlag, 9) );
            std::shared_ptr<FooExternal> PtrS2(PtrS);
            OutputS = PtrS2->Value;
        }
        End = ClockType::now();
        DurationType SharedPtrCopyTime = End - Begin;
        TestLog << OutputS << " - Creating, Dereferencing and Copying a shared_ptr " << TestCount
                << " times with internal counting took: " << SharedPtrCopyTime.count() << " Microseconds."
                << "\n";

        /// make_shared
        Begin = ClockType::now();
        for( Whole Count = 0; Count < TestCount ; Count++)
        {
            std::shared_ptr<FooExternal> PtrS = std::make_shared<FooExternal>(&destructFlag, 10);
            std::shared_ptr<FooExternal> PtrS2(PtrS);
            OutputS = PtrS2->Value;
        }
        End = ClockType::now();
        DurationType MakeSharedCopyTime = End - Begin;
        TestLog << OutputS << " - Creating, Dereferencing and Copying a shared_ptr from make_shared "
                << TestCount << " times with external counting took: " << MakeSharedCopyTime.count() << " Microseconds."
                << "\n\n";

        /////////////////////////////////////
        // Comparison of Benchmark Results

        TestLog << "Checking that raw pointers are fastest for sanity: "
                << ( RawPtrTime < CountPtrInternalTime && RawPtrTime < CountPtrExternalTime
                    && RawPtrTime<SharedPtrTime && RawPtrTime < MakeSharedTime )
                << " and "
                << ( RawPtrCopyTime < CountPtrCopyInternalTime && RawPtrCopyTime < CountPtrCopyExternalTime
                    && RawPtrCopyTime < SharedPtrCopyTime && RawPtrCopyTime < MakeSharedCopyTime )
                << ".\n";

        TEST_WARN( "SanityRawVsCountedPtr", RawPtrTime < CountPtrInternalTime && RawPtrTime < CountPtrExternalTime );
        TEST_WARN( "SanityRawVsCountedCopy", RawPtrCopyTime < CountPtrCopyInternalTime
                  && RawPtrCopyTime < CountPtrCopyExternalTime );
        TEST_WARN( "SanityRawVsShared", RawPtrTime < SharedPtrTime && RawPtrTime < MakeSharedTime);
        TEST_WARN( "SanityRawVsSharedCopy", RawPtrCopyTime < SharedPtrCopyTime &&
                  RawPtrCopyTime < MakeSharedCopyTime );
        TEST_WARN( "SanitySharedWoutMake", MakeSharedTime < SharedPtrTime); // Not very good sanity tests.
        // The assumption this tests is only very loosely associated with the CountedPtr.
        TEST_WARN( "SanitySharedWoutMakeCopy", MakeSharedCopyTime < SharedPtrCopyTime);

        TestLog << "Checking CountedPtr internal is faster than CountedPtr external: "
                << ( CountPtrInternalTime < CountPtrExternalTime )
                << " and "
                << ( CountPtrCopyInternalTime < CountPtrCopyExternalTime )
                << ".\n";

        TEST_WARN( "InternalvsExternalTime", CountPtrInternalTime < CountPtrExternalTime);
        TEST_WARN( "InternalvsExternalTimeCopy", CountPtrCopyInternalTime < CountPtrCopyExternalTime );

        TestLog << "Checking CountedPtr internal is faster than shared_ptr: "
                << ( CountPtrInternalTime < SharedPtrTime )
                << " and "
                << ( CountPtrCopyInternalTime < SharedPtrCopyTime )
                << " and "
                << ( CountPtrInternalTime < MakeSharedTime )
                << " and "
                << ( CountPtrCopyInternalTime < MakeSharedCopyTime )
                << ".\n";

        TEST_WARN( "InternalvsShared", CountPtrInternalTime < SharedPtrTime );
        TEST_WARN( "InternalvsSharedCopy", CountPtrCopyInternalTime < SharedPtrCopyTime );
        TEST_WARN( "InternalvsMakeShared", CountPtrInternalTime < MakeSharedTime );
        TEST_WARN( "InternalvsMakeSharedCopy", CountPtrCopyInternalTime < MakeSharedCopyTime );

        TestLog << "Checking CountedPtr External is faster than shared_ptr: "
                << ( CountPtrExternalTime < SharedPtrTime )
                << " and "
                << ( CountPtrCopyExternalTime < SharedPtrCopyTime )
                << " and "
                << ( CountPtrExternalTime < MakeSharedTime )
                << " and "
                << ( CountPtrCopyExternalTime < MakeSharedCopyTime )
                << ".\n";

        TEST_WARN( "ExternalvsShared", CountPtrExternalTime < SharedPtrTime );
        TEST_WARN( "ExternalvsSharedCopy", CountPtrCopyExternalTime < SharedPtrCopyTime );
        TEST_WARN( "ExternalvsMakeShared", CountPtrExternalTime < MakeSharedTime );
        TEST_WARN( "ExternalvsMakeSharedCopy", CountPtrCopyExternalTime < MakeSharedCopyTime );
    }
}

#endif
