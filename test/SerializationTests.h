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
#ifndef Mezz_Foundation_SerializationTests_h
#define Mezz_Foundation_SerializationTests_h

/// @file
/// @brief This file tests the functionality of the Serialization classes.

#include "MezzTest.h"

#include "Serialization.h"

SAVE_WARNING_STATE
SUPPRESS_CLANG_WARNING("-Wpadded")

namespace SerializationTest {
    using namespace Mezzanine;
    class SuperComposed;

    ///////////////////////////////////////////////////////////////////////////////
    // Classes to Seriailze

    struct Simple
    {
        Integer IntVarOne = -100;
        Integer IntVarTwo = 100;

        Simple() = default;
        Simple(const Integer One, const Integer Two) :
            IntVarOne(One),
            IntVarTwo(Two)
            {  }

        static constexpr StringView RegisterName()
            { return "Simple"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            return Members(
                MakeMemberAccessor("IntVarOne",&Simple::IntVarOne),
                MakeMemberAccessor("IntVarTwo",&Simple::IntVarTwo)
            );
        }
    };//Simple

    struct SimpleBase
    {
        String StringVar = "Base";

        SimpleBase() = default;
        SimpleBase(const String& Str) :
            StringVar(Str)
            {  }
        virtual ~SimpleBase() = default;

        static constexpr StringView RegisterName()
            { return "SimpleBase"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            return Members(
                MakeMemberAccessor("StringVar",&SimpleBase::StringVar)
            );
        }
    };//SimpleBase

    struct SimpleDerivedOne : public SimpleBase
    {
        double DoubleVar = 3.1415926;
        float FloatVarOne = -3.14f;
        float FloatVarTwo = 3.14f;

        SimpleDerivedOne() = default;
        SimpleDerivedOne(const double Dbl, const float One, const float Two) :
            DoubleVar(Dbl),
            FloatVarOne(One),
            FloatVarTwo(Two)
            {  }
        SimpleDerivedOne(const String& Str, const double Dbl, const float One, const float Two) :
            SimpleBase(Str),
            DoubleVar(Dbl),
            FloatVarOne(One),
            FloatVarTwo(Two)
            {  }
        virtual ~SimpleDerivedOne() = default;

        static constexpr StringView RegisterName()
            { return "SimpleDerivedOne"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            return MergeMembers(
                SimpleBase::RegisterMembers(),
                Members(
                    MakeMemberAccessor("DoubleVar",&SimpleDerivedOne::DoubleVar),
                    MakeMemberAccessor("FloatVarOne",&SimpleDerivedOne::FloatVarOne),
                    MakeMemberAccessor("FloatVarTwo",&SimpleDerivedOne::FloatVarTwo)
                )
            );
        }
    };//SimpleDerivedOne

    struct SimpleDerivedTwo : public SimpleBase
    {
        UInt32 UIntVar = 120000;
        Int32 IntVar = -120000;
        UInt16 ShortUIntVar = 42;
        Int16 ShortIntVar = -42;

        SimpleDerivedTwo() = default;
        SimpleDerivedTwo(const UInt32 LongOne,
                         const Int32 LongTwo,
                         const UInt16 ShortOne,
                         const UInt16 ShortTwo) :
            UIntVar(LongOne),
            IntVar(LongTwo),
            ShortUIntVar(ShortOne),
            ShortIntVar(ShortTwo)
            {  }
        SimpleDerivedTwo(const String& Str,
                         const UInt32 LongOne,
                         const Int32 LongTwo,
                         const UInt16 ShortOne,
                         const UInt16 ShortTwo) :
            SimpleBase(Str),
            UIntVar(LongOne),
            IntVar(LongTwo),
            ShortUIntVar(ShortOne),
            ShortIntVar(ShortTwo)
            {  }
        virtual ~SimpleDerivedTwo() = default;

        static constexpr StringView RegisterName()
            { return "SimpleDerivedTwo"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            return MergeMembers(
                SimpleBase::RegisterMembers(),
                Members(
                    MakeMemberAccessor("UIntVar",&SimpleDerivedTwo::UIntVar),
                    MakeMemberAccessor("IntVar",&SimpleDerivedTwo::IntVar),
                    MakeMemberAccessor("ShortUIntVar",&SimpleDerivedTwo::ShortUIntVar),
                    MakeMemberAccessor("ShortIntVar",&SimpleDerivedTwo::ShortIntVar)
                )
            );
        }
    };//SimpleDerivedTwo

    struct SimpleDerivedThree : public SimpleDerivedTwo
    {
        double DoubleVar = 8.675309;

        SimpleDerivedThree() = default;
        SimpleDerivedThree(const double Dbl) :
            DoubleVar(Dbl)
            {  }
        SimpleDerivedThree(const String& Str,
                           const UInt32 LongOne,
                           const Int32 LongTwo,
                           const UInt16 ShortOne,
                           const UInt16 ShortTwo,
                           const double Dbl) :
            SimpleDerivedTwo(Str,LongOne,LongTwo,ShortOne,ShortTwo),
            DoubleVar(Dbl)
            {  }
        virtual ~SimpleDerivedThree() = default;

        static constexpr StringView RegisterName()
            { return "SimpleDerivedThree"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            return MergeMembers(
                SimpleDerivedTwo::RegisterMembers(),
                Members(
                    MakeMemberAccessor("DoubleVar",&SimpleDerivedThree::DoubleVar)
                )
            );
        }
    };//SimpleDerivedThree

    class ComposedOne
    {
    public:
        Simple SimpleObj;
        SimpleBase* SimplePtr = nullptr;
        SuperComposed* Parent = nullptr;

        ComposedOne() = default;
        ComposedOne(SimpleBase* Ptr, SuperComposed* Creator) :
            SimplePtr(Ptr),
            Parent(Creator)
            {  }

        static constexpr StringView RegisterName()
            { return "ComposedOne"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            return Members(
                MakeMemberAccessor("SimpleObj",&ComposedOne::SimpleObj),
                MakeMemberAccessor<MemberTags::Own>("SimplePtr",&ComposedOne::SimplePtr),
                MakeMemberAccessor<MemberTags::None>("Parent",&ComposedOne::Parent)
            );
        }
    };//ComposedOne

    class ComposedTwo
    {
    public:
        ComposedOne* OwnedComposed = nullptr;
        SimpleBase* NonOwnedSimpleObj = nullptr;
        SuperComposed* Parent = nullptr;
        std::shared_ptr<Simple> SharedSimplePtr;

        ComposedTwo() = default;
        ComposedTwo(ComposedOne* Owned, SimpleBase* NonOwned, SuperComposed* Super, std::shared_ptr<Simple> Shared) :
            OwnedComposed(Owned),
            NonOwnedSimpleObj(NonOwned),
            Parent(Super),
            SharedSimplePtr(Shared)
            {  }

        ~ComposedTwo()
            { delete OwnedComposed; }

        static constexpr StringView RegisterName()
            { return "ComposedTwo"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            return Members(
                MakeMemberAccessor("OwnedComposed",&ComposedTwo::OwnedComposed),
                MakeMemberAccessor<MemberTags::None>("NonOwnedSimpleObj",&ComposedTwo::NonOwnedSimpleObj),
                MakeMemberAccessor<MemberTags::None>("Parent",&ComposedTwo::Parent),
                MakeMemberAccessor("SharedSimplePtr",&ComposedTwo::SharedSimplePtr)
            );
        }
    };//ComposedTwo

    class SuperComposed
    {
    public:
        std::vector<ComposedTwo*> ComposedVector;
        std::map<int,ComposedOne> ComposedMap;
        SimpleDerivedThree FirstDerived;
        SimpleDerivedTwo SecondDerived;
        SimpleDerivedOne ThirdDerived;

        SuperComposed()
        {
            std::shared_ptr<Simple> SharedSimple = std::make_shared<Simple>();
            // Populate the ComposedVector
            ComposedVector.push_back(
                new ComposedTwo( new ComposedOne(&FirstDerived,this), &SecondDerived, this, SharedSimple )
            );
            ComposedVector.push_back(
                new ComposedTwo( new ComposedOne(&SecondDerived,this), &FirstDerived, this, SharedSimple )
            );
            ComposedVector.push_back(
                new ComposedTwo( new ComposedOne(&FirstDerived,this), &FirstDerived, this, SharedSimple )
            );
            ComposedVector.push_back(
                new ComposedTwo( new ComposedOne(&SecondDerived,this), &SecondDerived, this, SharedSimple )
            );
            // Populate the ComposedMap
            ComposedMap.try_emplace(1,&FirstDerived,this);
            ComposedMap.try_emplace(2,&SecondDerived,this);
        }

        ~SuperComposed()
        {
            for( ComposedTwo* CurrBase : ComposedVector )
                { delete CurrBase; }
        }

        static constexpr StringView RegisterName()
            { return "SuperComposed"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            return Members(
                MakeMemberAccessor("ComposedVector",&SuperComposed::ComposedVector),
                MakeMemberAccessor("ComposedMap",&SuperComposed::ComposedMap),
                MakeMemberAccessor("FirstDerived",&SuperComposed::FirstDerived),
                MakeMemberAccessor("SecondDerived",&SuperComposed::SecondDerived),
                MakeMemberAccessor("ThirdDerived",&SuperComposed::ThirdDerived)
            );
        }
    };//SuperComposed
}//SerializationTest

RESTORE_WARNING_STATE

AUTOMATIC_TEST_GROUP(SerializationTests,Serialization)
{
    using namespace Mezzanine;
    using namespace SerializationTest;

    RegisterCasters<SimpleBase,SimpleDerivedOne,SimpleDerivedTwo,SimpleDerivedThree>();

    auto RootWrap = [](const String& ToWrap) -> String {
        return "{Root \n    " + ToWrap + "\n}";
    };

    auto OutputToFile = [](const StringStream& Out) {
        std::ofstream FileStream("SerializeOutput.txt");
        FileStream << Out.str();
        FileStream.close();
    };//*/

    {//Simple
        const String SimpleText{
            "{TestSimple [ TypeName:Simple Version:0 IntVarOne:12 IntVarTwo:24 ]}"
        };
        const String FullSimpleText = RootWrap(SimpleText);
        Simple TestSimple(12,24);

        {//Serialize
            StringStream SerializeStream;
            Serialization::SimpleBackend SerializerBack;
            Serialization::SerializerWalker Walker(SerializerBack.GetWalker());

            Mezzanine::Serialize("TestSimple",TestSimple,Serialization::LatestVersion,Walker);
            SerializerBack.Write(SerializeStream);

            TEST_EQUAL("Simple-Serialize",FullSimpleText,SerializeStream.str())
        }//Serialize

        {//Deserialize
            Simple Result;
            Serialization::SimpleBackend SerializerBack;
            StringStream DeserializeStream(FullSimpleText);
            SerializerBack.Read(DeserializeStream);

            Serialization::DeserializerWalker Walker(SerializerBack.GetWalker());
            Mezzanine::Deserialize("TestSimple",Result,Walker);

            TEST_EQUAL("Simple-Deserialize-IntVarOne",TestSimple.IntVarOne,Result.IntVarOne);
            TEST_EQUAL("Simple-Deserialize-IntVarTwo",TestSimple.IntVarTwo,Result.IntVarTwo);
        }//Deserialize
    }//Simple

    {//SimpleBase
        const String SimpleBaseText{
            "{TestSimpleBase [ TypeName:SimpleBase Version:0 StringVar:OnlyBase ]}"
        };
        const String FullSimpleBaseText = RootWrap(SimpleBaseText);
        SimpleBase TestSimpleBase("OnlyBase");

        {//Serialize
            StringStream SerializeStream;
            Serialization::SimpleBackend SerializerBack;
            Serialization::SerializerWalker Walker(SerializerBack.GetWalker());

            Mezzanine::Serialize("TestSimpleBase",TestSimpleBase,Serialization::LatestVersion,Walker);
            SerializerBack.Write(SerializeStream);

            TEST_EQUAL("SimpleBase-Serialize",FullSimpleBaseText,SerializeStream.str())
        }//Serialize

        {//Deserialize
            SimpleBase Result;
            Serialization::SimpleBackend SerializerBack;
            StringStream DeserializeStream(FullSimpleBaseText);
            SerializerBack.Read(DeserializeStream);

            Serialization::DeserializerWalker Walker(SerializerBack.GetWalker());
            Mezzanine::Deserialize("TestSimpleBase",Result,Walker);

            TEST_EQUAL("SimpleBase-Deserialize-StringVar",TestSimpleBase.StringVar,Result.StringVar);
        }//Deserialize
    }//SimpleBase

    {//SimpleDerivedOne
        const String DerivedOneText{
            "{TestDerivedOne [ TypeName:SimpleDerivedOne Version:0 "
            "StringVar:DerivedOne DoubleVar:2.71828 FloatVarOne:0.5 FloatVarTwo:-0.5 ]}"
        };
        const String FullDerivedOneText = RootWrap(DerivedOneText);
        SimpleDerivedOne TestDerivedOne("DerivedOne",2.71828,0.5,-0.5);

        {//Serialize
            StringStream SerializeStream;
            Serialization::SimpleBackend SerializerBack;
            Serialization::SerializerWalker Walker(SerializerBack.GetWalker());

            Mezzanine::Serialize("TestDerivedOne",TestDerivedOne,Serialization::LatestVersion,Walker);
            SerializerBack.Write(SerializeStream);

            TEST_EQUAL("SimpleDerivedOne-Serialize",FullDerivedOneText,SerializeStream.str())
        }//Serialize

        {//Deserialize
            SimpleDerivedOne Result;
            Serialization::SimpleBackend SerializerBack;
            StringStream DeserializeStream(FullDerivedOneText);
            SerializerBack.Read(DeserializeStream);

            Serialization::DeserializerWalker Walker(SerializerBack.GetWalker());
            Mezzanine::Deserialize("TestDerivedOne",Result,Walker);

            TEST_EQUAL("SimpleDerivedOne-Deserialize-StringVar",TestDerivedOne.StringVar,Result.StringVar);
            TEST_EQUAL("SimpleDerivedOne-Deserialize-DoubleVar",TestDerivedOne.DoubleVar,Result.DoubleVar);
            TEST_EQUAL("SimpleDerivedOne-Deserialize-FloatVarOne",TestDerivedOne.FloatVarOne,Result.FloatVarOne);
            TEST_EQUAL("SimpleDerivedOne-Deserialize-FloatVarTwo",TestDerivedOne.FloatVarTwo,Result.FloatVarTwo);
        }//Deserialize
    }//SimpleDerivedOne

    {//SimpleDerivedTwo
        const String DerivedTwoText{
            "{TestDerivedTwo [ TypeName:SimpleDerivedTwo Version:0 "
            "StringVar:DerivedTwo UIntVar:420 IntVar:-420 ShortUIntVar:666 ShortIntVar:-666 ]}"
        };
        const String FullDerivedTwoText = RootWrap(DerivedTwoText);
        SimpleDerivedTwo TestDerivedTwo("DerivedTwo",420,-420,666,-666);

        {//Serialize
            StringStream SerializeStream;
            Serialization::SimpleBackend SerializerBack;
            Serialization::SerializerWalker Walker(SerializerBack.GetWalker());

            Mezzanine::Serialize("TestDerivedTwo",TestDerivedTwo,Serialization::LatestVersion,Walker);
            SerializerBack.Write(SerializeStream);

            TEST_EQUAL("SimpleDerivedTwo-Serialize",FullDerivedTwoText,SerializeStream.str())
        }//Serialize

        {//Deserialize
            SimpleDerivedTwo Result;
            Serialization::SimpleBackend SerializerBack;
            StringStream DeserializeStream(FullDerivedTwoText);
            SerializerBack.Read(DeserializeStream);

            Serialization::DeserializerWalker Walker(SerializerBack.GetWalker());
            Mezzanine::Deserialize("TestDerivedTwo",Result,Walker);

            TEST_EQUAL("SimpleDerivedTwo-Deserialize-StringVar",TestDerivedTwo.StringVar,Result.StringVar);
            TEST_EQUAL("SimpleDerivedTwo-Deserialize-UIntVar",TestDerivedTwo.UIntVar,Result.UIntVar);
            TEST_EQUAL("SimpleDerivedTwo-Deserialize-IntVar",TestDerivedTwo.IntVar,Result.IntVar);
            TEST_EQUAL("SimpleDerivedTwo-Deserialize-ShortUIntVar",TestDerivedTwo.ShortUIntVar,Result.ShortUIntVar);
            TEST_EQUAL("SimpleDerivedTwo-Deserialize-ShortIntVar",TestDerivedTwo.ShortIntVar,Result.ShortIntVar);
        }//Deserialize
    }//SimpleDerivedTwo

    {//SimpleDerivedThree
        const String DerivedThreeText{
            "{TestDerivedThree [ TypeName:SimpleDerivedThree Version:0 "
            "StringVar:DerivedThree UIntVar:150 IntVar:-150 ShortUIntVar:69 ShortIntVar:-69 DoubleVar:3.1415926 ]}"
        };
        const String FullDerivedThreeText = RootWrap(DerivedThreeText);
        SimpleDerivedThree TestDerivedThree("DerivedThree",150,-150,69,-69,3.1415926);

        {//Serialize
            StringStream SerializeStream;
            Serialization::SimpleBackend SerializerBack;
            Serialization::SerializerWalker Walker(SerializerBack.GetWalker());

            Mezzanine::Serialize("TestDerivedThree",TestDerivedThree,Serialization::LatestVersion,Walker);
            SerializerBack.Write(SerializeStream);

            TEST_EQUAL("SimpleDerivedThree-Serialize",FullDerivedThreeText,SerializeStream.str())
        }//Serialize

        {//Deserialize
            SimpleDerivedThree Result;
            Serialization::SimpleBackend SerializerBack;
            StringStream DeserializeStream(FullDerivedThreeText);
            SerializerBack.Read(DeserializeStream);

            Serialization::DeserializerWalker Walker(SerializerBack.GetWalker());
            Mezzanine::Deserialize("TestDerivedThree",Result,Walker);

            TEST_EQUAL("SimpleDerivedThree-Deserialize-StringVar",TestDerivedThree.StringVar,Result.StringVar);
            TEST_EQUAL("SimpleDerivedThree-Deserialize-UIntVar",TestDerivedThree.UIntVar,Result.UIntVar);
            TEST_EQUAL("SimpleDerivedThree-Deserialize-IntVar",TestDerivedThree.IntVar,Result.IntVar);
            TEST_EQUAL("SimpleDerivedThree-Deserialize-ShortUIntVar",TestDerivedThree.ShortUIntVar,Result.ShortUIntVar);
            TEST_EQUAL("SimpleDerivedThree-Deserialize-ShortIntVar",TestDerivedThree.ShortIntVar,Result.ShortIntVar);
            TEST_EQUAL("SimpleDerivedThree-Deserialize-DoubleVar",TestDerivedThree.DoubleVar,Result.DoubleVar);
        }//Deserialize
    }//SimpleDerivedThree
    std::cout << "\n\n\n";
    {//ComposedOne
        const String ComposedOneTextP1{
            "{TestComposedOne [ TypeName:ComposedOne Version:0 ]\n"
        "        {SimpleObj [ TypeName:Simple Version:0 IntVarOne:-100 IntVarTwo:100 ]}\n"
        "        {SimplePtr [ CanDeserialize:true InstanceID:"
        };
        const String ComposedOneTextP2{
                    " TypeName:SimpleDerivedOne Version:0 "
                    "StringVar:DerivedOne DoubleVar:12.3456 FloatVarOne:7.5 FloatVarTwo:-7.5 ]}\n"
        "        {Parent [ CanDeserialize:null TypeName:SuperComposed ]}\n"
        "    }"
        };

        SimpleDerivedOne TestDerivedOne("DerivedOne",12.3456,7.5,-7.5);
        ComposedOne TestComposedOne(&TestDerivedOne,nullptr);

        StringStream ComposedOneStream;
        ComposedOneStream << ComposedOneTextP1 << uintptr_t(TestComposedOne.SimplePtr) << ComposedOneTextP2;
        const String FullComposedOneText = RootWrap(ComposedOneStream.str());

        {//Serialize
            StringStream SerializeStream;
            Serialization::SimpleBackend SerializerBack;
            Serialization::SerializerWalker Walker(SerializerBack.GetWalker());

            Mezzanine::Serialize("TestComposedOne",TestComposedOne,Serialization::LatestVersion,Walker);
            SerializerBack.Write(SerializeStream);

            OutputToFile(SerializeStream);

            TEST_EQUAL("ComposedOne-Serialize",FullComposedOneText,SerializeStream.str())
        }//Serialize
        std::cout << "\n";
        {//Deserialize
            SimpleDerivedOne ResultMember;
            ComposedOne Result(&ResultMember,nullptr);
            Serialization::SimpleBackend SerializerBack;
            StringStream DeserializeStream(FullComposedOneText);
            SerializerBack.Read(DeserializeStream);

            Serialization::DeserializerWalker Walker(SerializerBack.GetWalker());
            Mezzanine::Deserialize("TestComposedOne",Result,Walker);

            TEST_EQUAL("ComposedOne-Deserialize-IntVarOne",
                       TestComposedOne.SimpleObj.IntVarOne,Result.SimpleObj.IntVarOne);
            TEST_EQUAL("ComposedOne-Deserialize-IntVarTwo",
                       TestComposedOne.SimpleObj.IntVarTwo,Result.SimpleObj.IntVarTwo);
            TEST_EQUAL("ComposedOne-Deserialize-StringVar",
                       TestDerivedOne.StringVar,ResultMember.StringVar);
            TEST_EQUAL("ComposedOne-Deserialize-DoubleVar",
                       TestDerivedOne.DoubleVar,ResultMember.DoubleVar);
            TEST_EQUAL("ComposedOne-Deserialize-FloatVarOne",
                       TestDerivedOne.FloatVarOne,ResultMember.FloatVarOne);
            TEST_EQUAL("ComposedOne-Deserialize-FloatVarTwo",
                       TestDerivedOne.FloatVarTwo,ResultMember.FloatVarTwo);
        }//Deserialize
    }//ComposedOne
}

#endif
