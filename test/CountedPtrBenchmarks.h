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
#ifndef Mezz_Foundation_CountedPtrBenchmarks_h
#define Mezz_Foundation_CountedPtrBenchmarks_h

/// @file
/// @brief This contains benchmarks tests on the performance of the CountedPtr class.

#include "MezzTest.h"
#include "TimingTools.h"

#include "CountedPtr.h"
#include "CountedPtrTests.h"

BENCHMARK_TEST_GROUP(CountedPtrBenchmarks,CountedPtrBenchmarks)
{
    using namespace Mezzanine;
    using namespace CountedPtrTesting;

    using Mezzanine::Testing::MicroBenchmark;
    using Mezzanine::Testing::MicroBenchmarkResults;

    Integer OutputE = 0;
    Integer OutputI = 0;
    Integer OutputS = 0;

    Boole destructFlag = false; // Only used here for construction of the test classes.

    {
        TestLog << "\n" << "CountedPtr Benchmarks" << "\n\n"
                << "The objects being created all change a variable on destruction and have initializing, "
                << "but otherwise trivial constructors. This is useful only for comparing the speeds of the point "
                << "constructs on this platform, not for providing objective pointer dereferencing costs.\n\n";
        //const Whole TestCount = 10000000;
        const std::chrono::seconds TestTime(1);

        /////////////////////////////////////
        // Create tests

        /*
        /// Raw Pointers (PreTest)
        auto RawPtrCreatePre = [&](){
            FooExternal* PtrR = new FooExternal(&destructFlag, 1);
            OutputS = PtrR->Value;
            delete PtrR;
        };
        const MicroBenchmarkResults RawPtrCreatePreResult = MicroBenchmark(TestTime,std::move(RawPtrCreatePre));
        TestLog << OutputS << " - Creating and Dereferencing a raw pointer (Pre) "
                << RawPtrCreatePreResult.Iterations
                << " times without reference counting took: "
                << Mezzanine::Testing::PrettyDurationString(RawPtrCreatePreResult.WallTotal)
                << "\n";
        */

        /// Raw Pointers
        auto RawPtrCreate = [&](){
            FooExternal* PtrR = new FooExternal(&destructFlag, 1);
            OutputS = PtrR->Value;
            delete PtrR;
        };
        const MicroBenchmarkResults RawPtrCreateResult = MicroBenchmark(TestTime,std::move(RawPtrCreate));
        TestLog << OutputS << " - Creating and Dereferencing a raw pointer "
                << RawPtrCreateResult.Iterations
                << " times without reference counting took: "
                << Mezzanine::Testing::PrettyDurationString(RawPtrCreateResult.WallTotal)
                << "\n";

        /// Internal Reference Counter
        auto InternalRefCreate = [&](){
            CountedPtr<FooInternal> PtrI( new FooInternal(&destructFlag, 2) );
            OutputI = PtrI->Value;
        };
        const MicroBenchmarkResults InternalRefCreateResult = MicroBenchmark(TestTime,std::move(InternalRefCreate));
        TestLog << OutputI << " - Creating and Dereferencing a CountedPtr "
                << InternalRefCreateResult.Iterations
                << " times with internal counting took: "
                << Mezzanine::Testing::PrettyDurationString(InternalRefCreateResult.WallTotal)
                << "\n";

        /// External Reference Counter
        auto ExternalRefCreate = [&](){
            CountedPtr<FooExternal> PtrE( new FooExternal(&destructFlag, 3) );
            OutputE = PtrE->Value;
        };
        const MicroBenchmarkResults ExternalRefCreateResult = MicroBenchmark(TestTime,std::move(ExternalRefCreate));
        TestLog << OutputE << " - Creating and Dereferencing a CountedPtr "
                << ExternalRefCreateResult.Iterations
                << " times with external counting took: "
                << Mezzanine::Testing::PrettyDurationString(ExternalRefCreateResult.WallTotal)
                << "\n";

        /// shared_ptr
        auto StdPtrCreate = [&](){
            std::shared_ptr<FooExternal> PtrS( new FooExternal(&destructFlag, 4) );
            OutputS = PtrS->Value;
        };
        const MicroBenchmarkResults StdPtrCreateResult = MicroBenchmark(TestTime,std::move(StdPtrCreate));
        TestLog << OutputS << " - Creating and Dereferencing a shared_ptr "
                << StdPtrCreateResult.Iterations
                << " times with external counting took: "
                << Mezzanine::Testing::PrettyDurationString(StdPtrCreateResult.WallTotal)
                << "\n";

        /// shared_ptr Through make_shared
        auto MakeSharedCreate = [&](){
            std::shared_ptr<FooExternal> PtrS = std::make_shared<FooExternal>(&destructFlag, 5);
            OutputS = PtrS->Value;
        };
        const MicroBenchmarkResults MakeSharedCreateResult = MicroBenchmark(TestTime,std::move(MakeSharedCreate));
        TestLog << OutputS << " - Creating and Dereferencing a shared_ptr from make_shared "
                << MakeSharedCreateResult.Iterations
                << " times with external counting took: "
                << Mezzanine::Testing::PrettyDurationString(MakeSharedCreateResult.WallTotal)
                << "\n";

        /////////////////////////////////////
        // Copy tests

        /// Raw Pointers
        auto RawPtrCopy = [&](){
            FooExternal* PtrR = new FooExternal(&destructFlag, 6);
            FooExternal* PtrR2 = PtrR;
            OutputE = PtrR2->Value;
            delete PtrR2;
        };
        const MicroBenchmarkResults RawPtrCopyResult = MicroBenchmark(TestTime,std::move(RawPtrCopy));
        TestLog << OutputE << " - Creating, Dereferencing, and Copying a raw pointer "
                << RawPtrCopyResult.Iterations
                << " times without reference counting took: "
                << Mezzanine::Testing::PrettyDurationString(RawPtrCopyResult.WallTotal)
                << "\n";

        /// Internal Reference Counter
        auto InternalRefCopy = [&](){
            CountedPtr<FooInternal> PtrI( new FooInternal(&destructFlag, 7) );
            CountedPtr<FooInternal> PtrI2( PtrI );
            OutputI = PtrI2->Value;
        };
        const MicroBenchmarkResults InternalRefCopyResult = MicroBenchmark(TestTime,std::move(InternalRefCopy));
        TestLog << OutputI << " - Creating, Dereferencing, and Copying a CountedPtr "
                << InternalRefCopyResult.Iterations
                << " times with internal counting took: "
                << Mezzanine::Testing::PrettyDurationString(InternalRefCopyResult.WallTotal)
                << "\n";

        /// External Reference Counter
        auto ExternalRefCopy = [&](){
            CountedPtr<FooExternal> PtrE( new FooExternal(&destructFlag, 8) );
            CountedPtr<FooExternal> PtrE2(PtrE);
            OutputE = PtrE2->Value;
        };
        const MicroBenchmarkResults ExternalRefCopyResult = MicroBenchmark(TestTime,std::move(ExternalRefCopy));
        TestLog << OutputE << " - Creating, Dereferencing, and Copying a CountedPtr "
                << ExternalRefCopyResult.Iterations
                << " times with external counting took: "
                << Mezzanine::Testing::PrettyDurationString(ExternalRefCopyResult.WallTotal)
                << "\n";


        /// shared_ptr
        auto StdPtrCopy = [&](){
            std::shared_ptr<FooExternal> PtrS( new FooExternal(&destructFlag, 9) );
            std::shared_ptr<FooExternal> PtrS2(PtrS);
            OutputS = PtrS2->Value;
        };
        const MicroBenchmarkResults StdPtrCopyResult = MicroBenchmark(TestTime,std::move(StdPtrCopy));
        TestLog << OutputS << " - Creating, Dereferencing, and Copying a shared_ptr "
                << StdPtrCopyResult.Iterations
                << " times with external counting took: "
                << Mezzanine::Testing::PrettyDurationString(StdPtrCopyResult.WallTotal)
                << "\n";

        /// make_shared
        auto MakeSharedCopy = [&](){
            std::shared_ptr<FooExternal> PtrS = std::make_shared<FooExternal>(&destructFlag, 10);
            std::shared_ptr<FooExternal> PtrS2(PtrS);
            OutputS = PtrS2->Value;
        };
        const MicroBenchmarkResults MakeSharedCopyResult = MicroBenchmark(TestTime,std::move(MakeSharedCopy));
        TestLog << OutputS << " - Creating, Dereferencing, and Copying a shared_ptr from make_shared "
                << MakeSharedCopyResult.Iterations
                << " times with external counting took: "
                << Mezzanine::Testing::PrettyDurationString(MakeSharedCopyResult.WallTotal)
                << "\n\n";

        /////////////////////////////////////
        // Comparison of Benchmark Results

        TEST_WARN( "FixMe", true );

        /*
        TestLog << "Checking that raw pointers are fastest for sanity.\n";
        TEST_WARN( "RawPtrCreateVsInternal",
                   RawPtrCreateResult.Average < InternalRefCreateResult.Average );
        TEST_WARN( "RawPtrCreateVsExternal",
                   RawPtrCreateResult.Average < ExternalRefCreateResult.Average );
        TEST_WARN( "RawPtrCreateVsStdShared",
                   RawPtrCreateResult.Average < StdPtrCreateResult.Average );
        TEST_WARN( "RawPtrCreateVsStdMakeShared",
                   RawPtrCreateResult.Average < MakeSharedCreateResult.Average );
        TEST_WARN( "RawPtrCopyVsInternalCopy",
                   RawPtrCopyResult.Average < InternalRefCopyResult.Average );
        TEST_WARN( "RawPtrCopyVsExternalCopy",
                   RawPtrCopyResult.Average < ExternalRefCopyResult.Average );
        TEST_WARN( "RawPtrCopyVsStdSharedCopy",
                   RawPtrCopyResult.Average < StdPtrCopyResult.Average );
        TEST_WARN( "RawPtrCopyVsStdMakeSharedCopy",
                   RawPtrCopyResult.Average < MakeSharedCopyResult.Average );

        TestLog << "Checking CountedPtr internal is faster than CountedPtr external.\n";
        TEST_WARN( "InternalVsExternal",
                   InternalRefCreateResult.Average < ExternalRefCreateResult.Average );
        TEST_WARN( "InternalVsExternalCopy",
                   InternalRefCopyResult.Average < ExternalRefCopyResult.Average );

        TestLog << "Checking CountedPtr internal is faster than std::shared_ptr.\n";
        TEST_WARN( "InternalVsShared",
                   InternalRefCreateResult.Average < StdPtrCreateResult.Average );
        TEST_WARN( "InternalCopyVsStdSharedCopy",
                   InternalRefCopyResult.Average < StdPtrCopyResult.Average );

        TestLog << "Checking CountedPtr internal is faster than std::make_shared.\n";
        TEST_WARN( "InternalVsStdMakeShared",
                   InternalRefCreateResult.Average < MakeSharedCreateResult.Average );
        TEST_WARN( "InternalCopyVsStdMakeSharedCopy",
                   InternalRefCopyResult.Average < MakeSharedCopyResult.Average );

        // Don't care about these tests as much.
        TestLog << "Checking CountedPtr External is faster than shared_ptr.\n";
        TEST_WARN( "ExternalVsShared",
                   ExternalRefCreateResult.Average < StdPtrCreateResult.Average );
        TEST_WARN( "ExternalVsSharedCopy",
                   ExternalRefCopyResult.Average < StdPtrCopyResult.Average );
        TEST_WARN( "ExternalVsMakeShared",
                   ExternalRefCreateResult.Average < MakeSharedCreateResult.Average );
        TEST_WARN( "ExternalVsMakeSharedCopy",
                   ExternalRefCopyResult.Average < MakeSharedCopyResult.Average );
        */
    }
}

#endif
