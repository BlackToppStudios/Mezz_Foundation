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
#include "RuntimeStatics.h"

#include "CountedPtr.h"
#include "CountedPtrTests.h"

BENCHMARK_TEST_GROUP(CountedPtrBenchmarks,CountedPtrBenchmarks)
{
    // The memory model of JS garbage collection does not play nicely with this test.
    if(Mezzanine::RuntimeStatic::CompilerIsEmscripten())
        { return; }

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
        const std::chrono::seconds TestTime(2);

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

        SAVE_WARNING_STATE
        SUPPRESS_GCC_WARNING("-Wconversion")

        using Count = Mezzanine::Testing::MicroBenchmarkResults::CountType;

        // Internal Count Comparisons
        Count InternalToRawPercentageCopy =
            InternalRefCopyResult.Iterations * Count{100} / RawPtrCopyResult.Iterations;
        Count InternalToRawPercentageCreate =
            InternalRefCreateResult.Iterations * Count{100} / RawPtrCreateResult.Iterations;

        Count InternalToExternalPercentageCopy =
            InternalRefCopyResult.Iterations * Count{100} / ExternalRefCopyResult.Iterations;
        Count InternalToExternalPercentageCreate =
            InternalRefCreateResult.Iterations * Count{100} / ExternalRefCreateResult.Iterations;

        Count InternalToSharedPercentageCopy =
            InternalRefCopyResult.Iterations * Count{100} / StdPtrCopyResult.Iterations;
        Count InternalToSharedPercentageCreate =
            InternalRefCreateResult.Iterations * Count{100} / StdPtrCreateResult.Iterations;

        Count InternalToMakeSharedPercentageCopy =
            InternalRefCopyResult.Iterations * Count{100} / MakeSharedCopyResult.Iterations;
        Count InternalToMakeSharedPercentageCreate =
            InternalRefCreateResult.Iterations * Count{100} / MakeSharedCreateResult.Iterations;

        // External Count Comparisons
        Count ExternalToSharedPercentageCopy =
            ExternalRefCopyResult.Iterations * Count{100} / StdPtrCopyResult.Iterations;
        Count ExternalToSharedPercentageCreate =
            ExternalRefCreateResult.Iterations * Count{100} / StdPtrCreateResult.Iterations;

        Count ExternalToMakeSharedPercentageCopy =
            ExternalRefCopyResult.Iterations * Count{100} / MakeSharedCopyResult.Iterations;
        Count ExternalToMakeSharedPercentageCreate =
            ExternalRefCreateResult.Iterations * Count{100} / MakeSharedCreateResult.Iterations;

        RESTORE_WARNING_STATE

        TestLog << "The Internal Counted pointer gets about "
                << InternalToRawPercentageCopy << "% as many working iterations as raw pointers.\n"
                << "The Internal Counted pointer gets about "
                << InternalToRawPercentageCreate << "% as many creation iterations as raw pointers.\n\n"

                << "The Internal Counted pointer gets about "
                << InternalToExternalPercentageCopy << "% as many working iterations as External Counted pointers.\n"
                << "The Internal Counted pointer gets about "
                << InternalToExternalPercentageCreate
                << "% as many creation iterations as External Counted pointers.\n\n"

                << "The Internal Counted pointer gets about "
                << InternalToSharedPercentageCopy << "% as many working iterations as shared pointers.\n"
                << "The Internal Counted pointer gets about "
                << InternalToSharedPercentageCreate << "% as many creation iterations as shared pointers.\n\n"

                << "The Internal Counted pointer gets about "
                << InternalToMakeSharedPercentageCopy << "% as many working iterations as make_shared pointers.\n"
                << "The Internal Counted pointer gets about "
                << InternalToMakeSharedPercentageCreate << "% as many creation iterations as make_shared pointers.\n\n"

                << "The External Counted pointer gets about "
                << ExternalToSharedPercentageCopy << "% as many working iterations as shared pointers.\n"
                << "The External Counted pointer gets about "
                << ExternalToSharedPercentageCreate << "% as many creation iterations as shared pointers.\n\n"

                << "The External Counted pointer gets about "
                << ExternalToMakeSharedPercentageCopy << "% as many working iterations as make_shared pointers.\n"
                << "The External Counted pointer gets about "
                << ExternalToMakeSharedPercentageCreate << "% as many creation iterations as make_shared pointers.\n\n"
                ;

        // Benchmarks don't make sense in Debug and shouldn't warn.
        if(Mezzanine::RuntimeStatic::Debug())
            { return; }

        // Internal Percentage based comparisons
        // Is 85% as much work close enough?
        TEST_WARN( "InternalDoesASimilarAmountOfCreateIterationsToRawPointers",
                   85 < InternalToRawPercentageCreate )
        TEST_WARN( "InternalDoesASimilarAmountOfWorkIterationsToRawPointers",
                   85 < InternalToRawPercentageCopy )

        TEST_WARN( "InternalDoesMoreCreateIterationsThanExternalReferenceCountOrIsClose",
                   99 < InternalToExternalPercentageCreate )
        TEST_WARN( "InternalDoesMoreWorkIterationsThanExternalReferenceCountOrIsClose",
                   99 < InternalToExternalPercentageCopy )

        TEST_WARN( "InternalDoesMoreCreateIterationsThanSharedpointers",
                   100 < InternalToSharedPercentageCreate )
        TEST_WARN( "InternalDoesMoreWorkIterationsThanSharedpointers",
                   100 < InternalToSharedPercentageCopy )
        TEST_WARN( "InternalDoesMoreCreateIterationsThanMakeSharedpointers",
                   100 < InternalToMakeSharedPercentageCreate )
        TEST_WARN( "InternalDoesMoreWorkIterationsThanMakeSharedpointers",
                   100 < InternalToMakeSharedPercentageCopy )

        // External Percentage based comparisons
        TEST_WARN( "ExternalDoesMoreCreateIterationsThanSharedpointers",
                   99 < ExternalToSharedPercentageCreate )
        TEST_WARN( "ExternalDoesMoreWorkIterationsThanSharedpointers",
                   99 < ExternalToSharedPercentageCopy )
        TEST_WARN( "InternalDoesntSuckAtCreateIterationsComparedToMakeSharedpointers",
                   50 < ExternalToMakeSharedPercentageCreate )
        TEST_WARN( "InternalDoesntSuckAtWorlIterationsComparedToMakeSharedpointers",
                   50 < ExternalToMakeSharedPercentageCopy )
    }
}

#endif
