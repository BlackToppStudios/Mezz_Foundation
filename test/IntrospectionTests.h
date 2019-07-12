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
#ifndef Mezz_Foundation_IntrospectionTests_h
#define Mezz_Foundation_IntrospectionTests_h

/// @file
/// @brief This file tests the functionality of the Introspection classes.

#include "MezzTest.h"

#include "Introspection.h"

namespace IntrospectTest
{
    using Mezzanine::UInt32;
    using Mezzanine::StringView;

    struct BaseStruct
    {
        std::string StringVar = "Hello";
        float FloatVar = 1.0f;
        int IntVar = 0;

        void SetFloatVar(const float Var)
            { this->FloatVar = Var; }
        float GetFloatVar() const
            { return this->FloatVar; }

        static StringView RegisterName()
            { return "BaseStruct"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            using SelfType = BaseStruct;

            return Members(
                MakeMemberAccessor("StringVar",&SelfType::StringVar),
                MakeMemberAccessor("FloatVar",&SelfType::SetFloatVar,&SelfType::GetFloatVar),
                MakeMemberAccessor("IntVar",&SelfType::IntVar)
            );
        }
    };

    SAVE_WARNING_STATE
    SUPPRESS_CLANG_WARNING("-Wweak-vtables")

    struct DerivedStructA : virtual public BaseStruct
    {
        float DerivedAFloatVar = 1.0f;
        int DerivedAIntVar = 0;
        unsigned DerivedAUnsignedVar = 0;

        static StringView RegisterName()
            { return "DerivedStructA"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            using SelfType = DerivedStructA;

            return MergeMembers(
                BaseStruct::RegisterMembers(),
                Members(
                    MakeMemberAccessor("DerivedAFloatVar",&SelfType::DerivedAFloatVar),
                    MakeMemberAccessor("DerivedAIntVar",&SelfType::DerivedAIntVar),
                    MakeMemberAccessor("DerivedAUnsignedVar",&SelfType::DerivedAUnsignedVar)
                )
            );
        }
    };

    struct DerivedStructB : virtual public BaseStruct
    {
        float DerivedBfloatVar = 1.0f;
        short DerivedBShortVar = 0;
        std::array<char,2> DerivedBCharArrayVar = { {0,0} };

        static StringView RegisterName()
            { return "DerivedStructB"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            using SelfType = DerivedStructB;

            return MergeMembers(
                BaseStruct::RegisterMembers(),
                Members(
                    MakeMemberAccessor("DerivedBfloatVar",&SelfType::DerivedBfloatVar),
                    MakeMemberAccessor("DerivedBShortVar",&SelfType::DerivedBShortVar),
                    MakeMemberAccessor("DerivedBCharArrayVar",&SelfType::DerivedBCharArrayVar)
                )
            );
        }
    };

    struct DiamondStruct final : public DerivedStructA, public DerivedStructB
    {
        std::string DiamondStringVar = "Hello";
        std::string DiamondStringVarDos = "Hola";

        static StringView RegisterName()
            { return "DiamondStruct"; }

        static auto RegisterMembers()
        {
            using namespace Mezzanine;
            using namespace std::string_view_literals;
            using SelfType = DiamondStruct;

            return Members(
                // BaseStruct
                MakeMemberAccessor("IntVar"sv,&SelfType::IntVar),
                MakeMemberAccessor("FloatVar"sv,&SelfType::SetFloatVar,&SelfType::GetFloatVar),
                MakeMemberAccessor("StringVar"sv,&SelfType::StringVar),
                // DerivedStructA
                MakeMemberAccessor("DerivedAFloatVar"sv,&SelfType::DerivedAFloatVar),
                MakeMemberAccessor("DerivedAIntVar"sv,&SelfType::DerivedAIntVar),
                MakeMemberAccessor("DerivedAUnsignedVar"sv,&SelfType::DerivedAUnsignedVar),
                // DerivedStructB
                MakeMemberAccessor("DerivedBfloatVar"sv,&SelfType::DerivedBfloatVar),
                MakeMemberAccessor("DerivedBShortVar"sv,&SelfType::DerivedBShortVar),
                MakeMemberAccessor("DerivedBCharArrayVar"sv,&SelfType::DerivedBCharArrayVar),
                // DiamondStruct
                MakeMemberAccessor("DiamondStringVar"sv,&SelfType::DiamondStringVar),
                MakeMemberAccessor("DiamondStringVarDos"sv,&SelfType::DiamondStringVarDos)
            );
        }
    };

    RESTORE_WARNING_STATE

    struct SingleVarStruct
    {
        UInt32 UIntVar = 0;

        SingleVarStruct(const UInt32 Var) :
            UIntVar(Var)
            {  }

        void SetUIntVar(const UInt32 Var)
            { this->UIntVar = Var; }
        UInt32 GetUIntVar() const
            { return this->UIntVar; }

        bool operator==(const SingleVarStruct& Other) const
            { return this->UIntVar == Other.UIntVar; }

        static StringView GetSerializableName()
            { return "SingleVarStruct"; }

        static auto GetSerializableMembers()
        {
            using namespace Mezzanine;
            using SelfType = SingleVarStruct;

            return Members(
                MakeMemberAccessor("UIntVar",&SelfType::SetUIntVar,&SelfType::GetUIntVar)
            );
        }
    };

    std::ostream& operator<<(std::ostream& Stream, const SingleVarStruct& Var);
    std::ostream& operator<<(std::ostream& Stream, const SingleVarStruct& Var)
        { Stream << Var.UIntVar;  return Stream; }

    struct ContainerStruct
    {
        std::vector<BaseStruct> VectorVar;
        std::string StringVar = "Hello";

        std::vector<BaseStruct>& GetVectorVar()
            { return VectorVar; }
        std::string& GetStringVar()
            { return StringVar; }
    };

    SAVE_WARNING_STATE
    SUPPRESS_CLANG_WARNING("-Wpadded")

    struct PackedStruct
    {
        long double BigDoubleVar = 1.0l;
        std::string StringVar = "Hello";
        std::vector<SingleVarStruct> VectorVar = { {1}, {2} };
        std::map<std::string,unsigned int> MapVar = { {"First",1}, {"Second",2} };
        float ArrayVar[10];
        double DoubleVar = 1.0;
        long unsigned int BigUIntVar = 0;
        float FloatVar = 1.0f;
        unsigned int UIntVar = 0;
        short UShortVar = 0;
        char CharVar = 'A';
    };

    RESTORE_WARNING_STATE
}

namespace Mezzanine
{
    template<>
    constexpr StringView RegisterName<IntrospectTest::ContainerStruct>()
        { return "ContainerStruct"; }

    template<>
    inline auto RegisterMembers<IntrospectTest::ContainerStruct>()
    {
        using SelfType = IntrospectTest::ContainerStruct;

        return Members(
            MakeMemberAccessor<MemberTags::Local>("VectorVar",&SelfType::GetVectorVar),
            MakeMemberAccessor<MemberTags::Local>("StringVar",&SelfType::GetStringVar)
        );
    }
}

AUTOMATIC_TEST_GROUP(IntrospectionTests,Introspection)
{
    using namespace Mezzanine;
    using namespace IntrospectTest;

    {// MemberAccessor/PackedStruct Tests
        PackedStruct PackedTest;

        auto StringAccessor = MakeMemberAccessor<MemberTags::Local>("StringVar",&PackedStruct::StringVar);
        TEST_EQUAL("MemberAccessor::GetName()_const-StringVar",
                   String("StringVar"),StringAccessor.GetName());
        TEST_EQUAL("MemberAccessor::GetTags()-StringVar",
                   MemberTags::Local,StringAccessor.GetTags());
        std::string NewStringValue = "Goodbye";
        TEST_EQUAL("MemberAccessor::SetValue(T)-StringVar-Before",
                   "Hello",PackedTest.StringVar);
        StringAccessor.SetValue(PackedTest,NewStringValue);
        TEST_EQUAL("MemberAccessor::SetValue(T)-StringVar-After",
                   NewStringValue,PackedTest.StringVar);
        TEST_EQUAL("MemberAccessor::GetValue()_const-StringVar",
                   NewStringValue,StringAccessor.GetValue(PackedTest));//*/

        auto VectorAccessor = MakeMemberAccessor("VectorVar",&PackedStruct::VectorVar);
        TEST_EQUAL("MemberAccessor::GetName()_const-VectorVar",
                   String("VectorVar"),VectorAccessor.GetName());
        TEST_EQUAL("MemberAccessor::GetTags()-VectorVar",
                   MemberTags::None,VectorAccessor.GetTags());
        std::vector<SingleVarStruct> NewVectorValue = { {9} };
        TEST_EQUAL("MemberAccessor::SetValue(T)-VectorVar-Before-Size",
                   PackedTest.VectorVar.size(),size_t(2));
        TEST_EQUAL("MemberAccessor::SetValue(T)-VectorVar-Before-Element1",
                   PackedTest.VectorVar[0],SingleVarStruct{1});
        TEST_EQUAL("MemberAccessor::SetValue(T)-VectorVar-Before-Element2",
                   PackedTest.VectorVar[1],SingleVarStruct{2});
        VectorAccessor.SetValue(PackedTest,NewVectorValue);
        TEST_EQUAL("MemberAccessor::SetValue(T)-VectorVar-After-Size",
                   NewVectorValue.size(),PackedTest.VectorVar.size());
        TEST_EQUAL("MemberAccessor::SetValue(T)-VectorVar-After-Element1",
                   NewVectorValue[0],PackedTest.VectorVar[0]);
        TEST_EQUAL("MemberAccessor::GetValue()_const-VectorVar-Size",
                   NewVectorValue.size(),VectorAccessor.GetValue(PackedTest).size());
        TEST_EQUAL("MemberAccessor::GetValue()_const-VectorVar-Element1",
                   NewVectorValue[0],VectorAccessor.GetValue(PackedTest)[0]);//*/

        auto MapAccessor = MakeMemberAccessor<MemberTags::Local>("MapVar",&PackedStruct::MapVar);
        TEST_EQUAL("MemberAccessor::GetName()_const-MapVar",
                   String("MapVar"),MapAccessor.GetName());
        TEST_EQUAL("MemberAccessor::GetTags()-MapVar",
                   MemberTags::Local,MapAccessor.GetTags());
        std::map<std::string,unsigned int> NewMapValue = { {"Tenth",10} };
        TEST_EQUAL("MemberAccessor::SetValue(T)-MapVar-Before-Size",
                   size_t(2),PackedTest.MapVar.size());
        TEST_EQUAL("MemberAccessor::SetValue(T)-MapVar-Before-Element1-First",
                   std::string("First"),std::next(PackedTest.MapVar.begin(),0)->first);
        TEST_EQUAL("MemberAccessor::SetValue(T)-MapVar-Before-Element1-Second",
                   1u,std::next(PackedTest.MapVar.begin(),0)->second);
        TEST_EQUAL("MemberAccessor::SetValue(T)-MapVar-Before-Element2-First",
                   std::string("Second"),std::next(PackedTest.MapVar.begin(),1)->first);
        TEST_EQUAL("MemberAccessor::SetValue(T)-MapVar-Before-Element2-Second",
                   2u,std::next(PackedTest.MapVar.begin(),1)->second);
        MapAccessor.SetValue(PackedTest,NewMapValue);
        TEST_EQUAL("MemberAccessor::SetValue(T)-MapVar-After-Size",
                   NewMapValue.size(),
                   PackedTest.MapVar.size());
        TEST_EQUAL("MemberAccessor::SetValue(T)-MapVar-After-Element1-First",
                   std::next(NewMapValue.begin(),0)->first,
                   std::next(PackedTest.MapVar.begin(),0)->first);
        TEST_EQUAL("MemberAccessor::SetValue(T)-MapVar-After-Element1-Second",
                   std::next(NewMapValue.begin(),0)->second,
                   std::next(PackedTest.MapVar.begin(),0)->second);
        TEST_EQUAL("MemberAccessor::GetValue()_const-MapVar-Size",
                   NewMapValue.size(),
                   MapAccessor.GetValue(PackedTest).size());
        TEST_EQUAL("MemberAccessor::GetValue()_const-MapVar-Element1-First",
                   std::next(NewMapValue.begin(),0)->first,
                   std::next(MapAccessor.GetValue(PackedTest).begin(),0)->first);
        TEST_EQUAL("MemberAccessor::GetValue()_const-MapVar-Element1-Second",
                   std::next(NewMapValue.begin(),0)->second,
                   std::next(MapAccessor.GetValue(PackedTest).begin(),0)->second);//*/

        auto BigDoubleAccessor = MakeMemberAccessor("BigDoubleVar",&PackedStruct::BigDoubleVar);
        TEST_EQUAL("MemberAccessor::GetName()_const-BigDoubleVar",
                   String("BigDoubleVar"),BigDoubleAccessor.GetName());
        TEST_EQUAL("MemberAccessor::GetTags()-BigDoubleVar",
                   MemberTags::None,BigDoubleAccessor.GetTags());
        long double NewBigDoubleValue = 25.0l;
        TEST_EQUAL_EPSILON("MemberAccessor::SetValue(T)-BigDoubleVar-Before",
                           1.0l,PackedTest.BigDoubleVar);
        BigDoubleAccessor.SetValue(PackedTest,NewBigDoubleValue);
        TEST_EQUAL_EPSILON("MemberAccessor::SetValue(T)-BigDoubleVar-After",
                           NewBigDoubleValue,PackedTest.BigDoubleVar);
        TEST_EQUAL_EPSILON("MemberAccessor::GetValue()_const-BigDoubleVar",
                           NewBigDoubleValue,BigDoubleAccessor.GetValue(PackedTest));//*/

        auto DoubleAccessor = MakeMemberAccessor("DoubleVar",&PackedStruct::DoubleVar);
        TEST_EQUAL("MemberAccessor::GetName()_const-DoubleVar",
                   String("DoubleVar"),DoubleAccessor.GetName());
        TEST_EQUAL("MemberAccessor::GetTags()-DoubleVar",
                   MemberTags::None,DoubleAccessor.GetTags());
        double NewDoubleValue = 15.0;
        TEST_EQUAL_EPSILON("MemberAccessor::SetValue(T)-DoubleVar-Before",
                           1.0,PackedTest.DoubleVar);
        DoubleAccessor.SetValue(PackedTest,NewDoubleValue);
        TEST_EQUAL_EPSILON("MemberAccessor::SetValue(T)-DoubleVar-After",
                           NewDoubleValue,PackedTest.DoubleVar);
        TEST_EQUAL_EPSILON("MemberAccessor::GetValue()_const-DoubleVar",
                           NewDoubleValue,DoubleAccessor.GetValue(PackedTest));//*/

        auto BigUIntAccessor = MakeMemberAccessor<MemberTags::Generated>("BigUIntVar",&PackedStruct::BigUIntVar);
        TEST_EQUAL("MemberAccessor::GetName()_const-BigUIntVar",
                   String("BigUIntVar"),BigUIntAccessor.GetName());
        TEST_EQUAL("MemberAccessor::GetTags()-BigUIntVar",
                   MemberTags::Generated,BigUIntAccessor.GetTags());
        long unsigned int NewBigUIntValue = 5;
        TEST_EQUAL("MemberAccessor::SetValue(T)-BigUIntVar-Before",
                   0lu,PackedTest.BigUIntVar);
        BigUIntAccessor.SetValue(PackedTest,NewBigUIntValue);
        TEST_EQUAL("MemberAccessor::SetValue(T)-BigUIntVar-After",
                   NewBigUIntValue,PackedTest.BigUIntVar);
        TEST_EQUAL("MemberAccessor::GetValue()_const-BigUIntVar",
                   NewBigUIntValue,BigUIntAccessor.GetValue(PackedTest));//*/

        auto FloatAccessor = MakeMemberAccessor("FloatVar",&PackedStruct::FloatVar);
        TEST_EQUAL("MemberAccessor::GetName()_const-FloatVar",
                   String("FloatVar"),FloatAccessor.GetName());
        TEST_EQUAL("MemberAccessor::GetTags()-FloatVar",
                   MemberTags::None,FloatAccessor.GetTags());
        float NewFloatValue = 5.0f;
        TEST_EQUAL_EPSILON("MemberAccessor::SetValue(T)-FloatVar-Before",
                           1.0f,PackedTest.FloatVar);
        FloatAccessor.SetValue(PackedTest,NewFloatValue);
        TEST_EQUAL_EPSILON("MemberAccessor::SetValue(T)-FloatVar-After",
                           NewFloatValue,PackedTest.FloatVar);
        TEST_EQUAL_EPSILON("MemberAccessor::GetValue()_const-FloatVar",
                           NewFloatValue,FloatAccessor.GetValue(PackedTest));//*/

        auto UIntAccessor = MakeMemberAccessor<MemberTags::Local>("UIntVar",&PackedStruct::UIntVar);
        TEST_EQUAL("MemberAccessor::GetName()_const-UIntVar",
                   String("UIntVar"),UIntAccessor.GetName());
        TEST_EQUAL("MemberAccessor::GetTags()-UIntVar",
                   MemberTags::Local,UIntAccessor.GetTags());
        unsigned int NewUIntValue = 5;
        TEST_EQUAL("MemberAccessor::SetValue(T)-UIntVar-Before",
                   0u,PackedTest.UIntVar);
        UIntAccessor.SetValue(PackedTest,NewUIntValue);
        TEST_EQUAL("MemberAccessor::SetValue(T)-UIntVar-After",
                   NewUIntValue,PackedTest.UIntVar);
        TEST_EQUAL("MemberAccessor::GetValue()_const-UIntVar",
                   NewUIntValue,UIntAccessor.GetValue(PackedTest));//*/

        auto UShortAccessor = MakeMemberAccessor("UShortVar",&PackedStruct::UShortVar);
        TEST_EQUAL("MemberAccessor::GetName()_const-UShortVar",
                   String("UShortVar"),UShortAccessor.GetName());
        TEST_EQUAL("MemberAccessor::GetTags()-UShortVar",
                   MemberTags::None,UShortAccessor.GetTags());
        short NewUShortValue = 15;
        TEST_EQUAL("MemberAccessor::SetValue(T)-UShortVar-Before",
                   short(0),PackedTest.UShortVar);
        UShortAccessor.SetValue(PackedTest,NewUShortValue);
        TEST_EQUAL("MemberAccessor::SetValue(T)-UShortVar-After",
                   NewUShortValue,PackedTest.UShortVar);
        TEST_EQUAL("MemberAccessor::GetValue()_const-UShortVar",
                   NewUShortValue,UShortAccessor.GetValue(PackedTest));//*/

        auto CharAccessor = MakeMemberAccessor("CharVar",&PackedStruct::CharVar);
        TEST_EQUAL("MemberAccessor::GetName()_const-CharVar",
                   String("CharVar"),CharAccessor.GetName());
        TEST_EQUAL("MemberAccessor::GetTags()-CharVar",
                   MemberTags::None,CharAccessor.GetTags());
        char NewCharValue = 'Z';
        TEST_EQUAL("MemberAccessor::SetValue(T)-CharVar-Before",
                   'A',PackedTest.CharVar);
        CharAccessor.SetValue(PackedTest,NewCharValue);
        TEST_EQUAL("MemberAccessor::SetValue(T)-CharVar-After",
                   NewCharValue,PackedTest.CharVar);
        TEST_EQUAL("MemberAccessor::GetValue()_const-CharVar",
                   NewCharValue,CharAccessor.GetValue(PackedTest));//*/
    }// MemberAccessor/PackedStruct Tests

    {// MakeMemberAccessor/BaseStruct
        constexpr MemberTags Combined = MemberTags::Local | MemberTags::Generated;

        using FirstMethodType = decltype(&BaseStruct::IntVar);
        using FirstMemberType = MemberAccessor<FirstMethodType,FirstMethodType,MemberTags::None>;
        using SecondGetMethodType = decltype(&SingleVarStruct::GetUIntVar);
        using SecondSetMethodType = decltype(&SingleVarStruct::SetUIntVar);
        using SecondMemberType = MemberAccessor<SecondSetMethodType,SecondGetMethodType,MemberTags::None>;
        using ThirdMethodType = decltype(&ContainerStruct::GetVectorVar);
        using ThirdMemberType = MemberAccessor<ThirdMethodType,ThirdMethodType,MemberTags::Local>;
        using FourthGetMethodType = decltype(&BaseStruct::GetFloatVar);
        using FourthSetMethodType = decltype(&BaseStruct::SetFloatVar);
        using FourthMemberType = MemberAccessor<FourthSetMethodType,FourthGetMethodType,Combined>;

        auto FirstAccessor = MakeMemberAccessor("IntVar",&BaseStruct::IntVar);
        auto SecondAccessor = MakeMemberAccessor("FloatVar",&SingleVarStruct::SetUIntVar,&SingleVarStruct::GetUIntVar);
        auto ThirdAccessor = MakeMemberAccessor<MemberTags::Local>("VectorVar",&ContainerStruct::GetVectorVar);
        auto FourthAccessor=MakeMemberAccessor<Combined>("FloatVar",&BaseStruct::SetFloatVar,&BaseStruct::GetFloatVar);

        Boole FirstResult = std::is_same_v<FirstMemberType,decltype(FirstAccessor)>;
        Boole SecondResult = std::is_same_v<SecondMemberType,decltype(SecondAccessor)>;
        Boole ThirdResult = std::is_same_v<ThirdMemberType,decltype(ThirdAccessor)>;
        Boole FourthResult = std::is_same_v<FourthMemberType,decltype(FourthAccessor)>;
        TEST_EQUAL("MakeMemberAccessor(const_StringView,SetterType,GetterType)-FirstMember",true,FirstResult);
        TEST_EQUAL("MakeMemberAccessor(const_StringView,SetterType,GetterType)-SecondMember",true,SecondResult);
        TEST_EQUAL("MakeMemberAccessor(const_StringView,SetterType,GetterType)-ThirdMember",true,ThirdResult);
        TEST_EQUAL("MakeMemberAccessor(const_StringView,SetterType,GetterType)-FourthMember",true,FourthResult);
    }// MakeMemberAccessor/BaseStruct

    {// Registration Methods/ContainerStruct
        TEST_EQUAL("RegisterName()-Dummy",
                   StringView(""),RegisterName<std::vector<int>>());

        const Boole FirstMatch = std::is_same_v<std::tuple<>,decltype(RegisterMembers<std::vector<int>>())>;
        TEST_EQUAL("RegisterMembers()-Dummy",
                   true,FirstMatch);

        TEST_EQUAL("RegisterName()-ContainerStruct",
                   StringView("ContainerStruct"),RegisterName<ContainerStruct>());

        constexpr MemberTags Local = MemberTags::Local;
        using ContainerVectorType = decltype(MakeMemberAccessor<Local>("Test",&ContainerStruct::GetVectorVar));
        using ContainerStringType = decltype(MakeMemberAccessor<Local>("Test",&ContainerStruct::GetStringVar));
        using ContainerMembersType = std::tuple<ContainerVectorType,ContainerStringType>;
        const Boole SecondMatch = std::is_same_v<ContainerMembersType,decltype(RegisterMembers<ContainerStruct>())>;
        TEST_EQUAL("RegisterMembers()-ContainerStruct",
                   true,SecondMatch);
    }// Registration Methods/ContainerStruct

    {// Registration Query
        TEST_EQUAL("IsRegistered()-DerivedStructA",
                   true,IsRegistered<DerivedStructA>());
        TEST_EQUAL("IsRegistered()-ContainerStruct",
                   true,IsRegistered<ContainerStruct>());
        TEST_EQUAL("IsRegistered()-vector",
                   false,IsRegistered<std::vector<int>>());

        TEST_EQUAL("GetRegisteredName()-DerivedStructB",
                   StringView("DerivedStructB"),GetRegisteredName<DerivedStructB>());
        TEST_EQUAL("GetRegisteredName()-ContainerStruct",
                   StringView("ContainerStruct"),GetRegisteredName<ContainerStruct>());
        TEST_EQUAL("GetRegisteredName()-vector",
                   StringView(""),GetRegisteredName<std::vector<int>>());

        TEST_EQUAL("GetMemberCount()-BaseStruct",
                   size_t(3),GetMemberCount<BaseStruct>());
        TEST_EQUAL("GetMemberCount()-DerivedStructA",
                   size_t(6),GetMemberCount<DerivedStructA>());
        TEST_EQUAL("GetMemberCount()-DerivedStructB",
                   size_t(6),GetMemberCount<DerivedStructB>());
        TEST_EQUAL("GetMemberCount()-DiamondStruct",
                   size_t(11),GetMemberCount<DiamondStruct>());
        TEST_EQUAL("GetMemberCount()-ContainerStruct",
                   size_t(2),GetMemberCount<ContainerStruct>());
        TEST_EQUAL("GetMemberCount()-vector",
                   size_t(0),GetMemberCount<std::vector<int>>());
    }// Registration Query

    {// GetRegisteredMembers/HasMember
        using BaseRegisterType = decltype(BaseStruct::RegisterMembers());
        using BaseGetType = std::decay_t<decltype(GetRegisteredMembers<BaseStruct>())>;
        constexpr Boole BaseResult = std::is_same_v<BaseRegisterType,BaseGetType>;
        TEST_EQUAL("GetMembers()-BaseStruct",
                   true,BaseResult);

        using ContainerRegisterType = decltype(RegisterMembers<ContainerStruct>());
        using ContainerGetType = std::decay_t<decltype(GetRegisteredMembers<ContainerStruct>())>;
        constexpr Boole ContainerResult = std::is_same_v<ContainerRegisterType,ContainerGetType>;
        TEST_EQUAL("GetMembers()-ContainerStruct",
                   true,ContainerResult);

        using VectorGetType = decltype(GetRegisteredMembers<std::vector<int>>());
        constexpr Boole VectorResult = std::is_same_v<std::tuple<>,VectorGetType>;
        TEST_EQUAL("GetMembers()-vector",
                   false,VectorResult);

        TEST_EQUAL("HasMember()-BaseStruct-Pass",
                   true,HasMember<BaseStruct>("IntVar"));
        TEST_EQUAL("HasMember()-BaseStruct-Fail",
                   false,HasMember<BaseStruct>("StringViewVar"));

        TEST_EQUAL("HasMember()-ContainerStruct-Pass",
                   true,HasMember<ContainerStruct>("VectorVar"));
        TEST_EQUAL("HasMember()-ContainerStruct-Fail",
                   false,HasMember<ContainerStruct>("MapVar"));

        TEST_EQUAL("HasMember()-vector",
                   false,HasMember<std::vector<int>>("Start"));
    }// GetRegisteredMembers/HasMember

    {// Do For Members
        DiamondStruct TestStruct;
        TestStruct.DiamondStringVar = "Goodbye";

        size_t SlowMemberCount = 0;
        DoForAllMembers<DiamondStruct>( [&](const auto& Member) {
            ++SlowMemberCount;  (void)Member;
        } );
        TEST_EQUAL("DoForAllMembers-DiamondStruct",
                   size_t(11),SlowMemberCount);

        std::string StrMemberValue;
        DoForMember<DiamondStruct,std::string>( "DiamondStringVar", [&](const auto& Member) {
            StrMemberValue = Member.GetValue(TestStruct);
        } );
        TEST_EQUAL("DoForMember-DiamondStruct-DiamondStringVar",
                   std::string("Goodbye"),StrMemberValue);//*/
    }// Do For Members

    {// (Get/Set)MemberValue
        UInt32 FirstOldVal = 50;
        SingleVarStruct FirstTestStruct(FirstOldVal);
        TEST_EQUAL("GetMemberValue-SingleVarStruct-UInt32",
                   FirstOldVal,GetMemberValue<UInt32>(FirstTestStruct,"UIntVar"));

        UInt32 FirstNewVal = 100;
        SetMemberValue<UInt32>(FirstTestStruct,"UIntVar",FirstNewVal);
        TEST_EQUAL("SetMemberValue-SingleVarStruct-UInt32",
                   FirstNewVal,FirstTestStruct.UIntVar);

        std::string SecondOldVal = "Hello";
        ContainerStruct SecondTestStruct;
        TEST_EQUAL("GetMemberValue-ContainerStruct-String",
                   SecondOldVal,GetMemberValue<String>(SecondTestStruct,"StringVar"));

        std::string SecondNewVal = "kkthxbye";
        SetMemberValue<std::string>(SecondTestStruct,"StringVar",SecondNewVal);
        TEST_EQUAL("SetMemberValue-ContainerStruct-String",
                   SecondNewVal,SecondTestStruct.StringVar);

        std::string ThirdOldVal = "Hello";
        BaseStruct ThirdTestStruct;
        TEST_EQUAL("GetMemberValue-BaseStruct-String",
                   ThirdOldVal,GetMemberValue<std::string>(ThirdTestStruct,"StringVar"));

        std::string ThirdNewVal = "ohai";
        SetMemberValue<std::string>(ThirdTestStruct,"StringVar",ThirdNewVal);
        TEST_EQUAL("SetMemberValue-BaseStruct-String",
                   ThirdNewVal,ThirdTestStruct.StringVar);
    }// (Get/Set)MemberValue
}

#endif
