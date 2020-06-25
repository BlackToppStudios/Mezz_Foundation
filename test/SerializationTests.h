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
    // Test Backend

    class Attribute
    {
    public:
        static const char SeparatorChar = ' ';
        static const char LinkChar = ':';
    protected:
        String Name;
        String Value;
    public:
        Attribute() = default;
        Attribute(const StringView AttribName, const StringView AttribValue) :
            Name(AttribName),
            Value(AttribValue)
            {  }
        ~Attribute() = default;

        void SetName(const StringView NewName)
            { this->Name = NewName; }
        const String& GetName() const
            { return this->Name; }
        void SetValue(const StringView NewValue)
            { this->Value = NewValue; }
        const String& GetValue() const
            { return this->Value; }
    };//Attribute

    std::ostream& operator<<(std::ostream& Stream, const Attribute& ToSerialize);
    std::ostream& operator<<(std::ostream& Stream, const Attribute& ToSerialize)
    {
        Stream << ToSerialize.GetName() << ":" << ToSerialize.GetValue();
        return Stream;
    }
    std::istream& operator>>(std::istream& Stream, Attribute& ToDeserialize);
    std::istream& operator>>(std::istream& Stream, Attribute& ToDeserialize)
    {
        String Temp;
        std::getline(Stream,Temp,Attribute::LinkChar);
        ToDeserialize.SetName(Temp);
        std::getline(Stream,Temp,Attribute::SeparatorChar);
        ToDeserialize.SetValue(Temp);
        return Stream;
    }

    class Node
    {
    public:
        static const char AttributeStartChar = '[';
        static const char AttributeEndChar = ']';
        static const char NodeStartChar = '{';
        static const char NodeEndChar = '}';
        static const StringView Indent = "    ";
    protected:
        String Name;
        std::vector<Attribute> Attributes;
        std::vector<Node> Nodes;
        Node* Parent;
    public:
        Node(Node* Creator) :
            Parent(Creator)
            {  }

        void SetName(const StringView NewName)
            { this->Name = NewName; }
        const String& GetName() const
            { return this->Name; }

        Boole IsRoot() const
            { return this->Parent == nullptr; }
        Node* GetParent() const
            { return this->Parent; }
        size_t GetDepth() const
            { return ( this->Parent ? this->Parent->GetDepth() + 1 : 0 ); }

        Node& CreateChildNode()
            { return this->Nodes.emplace_back(this); }
        size_t GetNumNodes() const
            { return this->Nodes.size(); }
        Node& GetNode(const size_t Index)
            { return this->Nodes[Index]; }
        const Node& GetNode(const size_t Index) const
            { return this->Nodes[Index]; }
        Node& GetFirstNode()
            { return this->Nodes.front(); }
        const Node& GetFirstNode() const
            { return this->Nodes.front(); }
        Node& GetLastNode()
            { return this->Nodes.back(); }
        const Node& GetLastNode() const
            { return this->Nodes.back(); }
        void ClearNodes()
            { this->Nodes.clear(); }

        Attribute& CreateChildAttribute()
            { return this->Attributes.emplace_back(); }
        Attribute& CreateChildAttribute(const String& AttribName, const String& AttribValue)
            { return this->Attributes.emplace_back(AttribName,AttribValue); }
        size_t GetNumAttributes() const
            { return this->Attributes.size(); }
        Attribute& GetAttribute(const size_t Index)
            { return this->Attributes[Index]; }
        const Attribute& GetAttribute(const size_t Index) const
            { return this->Attributes[Index]; }
        Attribute& GetFirstAttribute()
            { return this->Attributes.front(); }
        const Attribute& GetFirstAttribute() const
            { return this->Attributes.front(); }
        Attribute& GetLastAttribute()
            { return this->Attributes.back(); }
        const Attribute& GetLastAttribute() const
            { return this->Attributes.back(); }
        void ClearAttributes()
            { this->Attributes.clear(); }
    };//Node

    std::ostream& operator<<(std::ostream& Stream, const Node& ToSerialize);
    std::ostream& operator<<(std::ostream& Stream, const Node& ToSerialize)
    {
        Stream << '\n';
        size_t NodeDepth = ToSerialize.GetDepth();
        for( size_t IndentCount = 0 ; IndentCount < NodeDepth ; ++IndentCount )
            { Stream << Node::Indent; }
        Stream << Node::NodeStartChar << ToSerialize.GetName() << Attribute::SeparatorChar;
        if( ToSerialize.GetNumAttributes() > 0 ) {
            Stream << Node::AttributeStartChar << Attribute::SeparatorChar;
            for( size_t AttribIndex = 0 ; AttribIndex < ToSerialize.GetNumAttributes() ; ++AttribIndex )
                { Stream << ToSerialize.GetAttribute(AttribIndex) << Attribute::SeparatorChar; }
            Stream << Node::AttributeEndChar;
        }
        for( size_t NodeIndex = 0 ; NodeIndex < ToSerialize.GetNumNodes() ; ++NodeIndex )
            { Stream << ToSerialize.GetNode(NodeIndex); }
        Stream << '\n';
        for( size_t IndentCount = 0 ; IndentCount < NodeDepth ; ++IndentCount )
            { Stream << Node::Indent; }
        Stream << Node::NodeEndChar;
        return Stream;
    }
    std::istream& operator>>(std::istream& Stream, Node& ToDeserialize);
    std::istream& operator>>(std::istream& Stream, Node& ToDeserialize)
    {
        String Temp;
        Stream.ignore(std::numeric_limits<std::streamsize>::max(),Node::NodeStartChar);
        std::getline(Stream,Temp,Attribute::SeparatorChar);
        ToDeserialize.SetName(Temp);
        while( Stream.peek() != Node::NodeEndChar || Stream.peek() != std::istream::traits_type::eof() )
        {
            if( Stream.peek() == Node::AttributeStartChar ) {
                Stream.ignore(2);
                while( Stream.peek() != Node::AttributeEndChar )
                    { Stream >> ToDeserialize.CreateChildAttribute(); }
                Stream.ignore();
            }else if( Stream.peek() == Node::NodeStartChar ) {
                Stream >> ToDeserialize.CreateChildNode();
            }else{
                Stream.ignore();
            }
        }
        return Stream;
    }

    class BackendAttribute : public Serialization::AttributeWalker
    {
    protected:
    public:
        BackendAttribute() = default;
        virtual ~BackendAttribute() = default;
    };//BackendAttribute

    class BackendNode : public Serialization::ObjectWalker
    {
    protected:
    public:
        BackendNode() = default;
        virtual ~BackendNode() = default;
    };//BackendNode

    class Backend : public Serialization::BackendBase
    {
    protected:
    public:
        Backend() = default;
        virtual ~Backend() = default;
    };//Backend

    ///////////////////////////////////////////////////////////////////////////////
    // Classes to Seriailze

    struct Simple
    {
        Integer IntVarOne = -100;
        Integer IntVarTwo = 100;

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
    protected:
        Simple SimpleObj;
        SimpleBase* SimplePtr;
        SuperComposed* Parent;
    public:
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
                MakeMemberAccessor<MemberTags::NotOwned>("SimplePtr",&ComposedOne::SimplePtr),
                MakeMemberAccessor<MemberTags::NotOwned>("Parent",&ComposedOne::Parent)
            );
        }
    };//ComposedOne

    class ComposedTwo
    {
    protected:
        ComposedOne* OwnedComposed;
        SimpleBase* NonOwnedSimpleObj;
        SuperComposed* Parent;
        std::shared_ptr<Simple> SharedSimplePtr;
    public:
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
                MakeMemberAccessor<MemberTags::NotOwned>("NonOwnedSimpleObj",&ComposedTwo::NonOwnedSimpleObj),
                MakeMemberAccessor("SharedSimplePtr",&ComposedTwo::SharedSimplePtr),
                MakeMemberAccessor<MemberTags::NotOwned>("Parent",&ComposedTwo::Parent)
            );
        }
    };//ComposedTwo

    class SuperComposed
    {
    protected:
        std::vector<ComposedTwo*> ComposedVector;
        std::map<int,ComposedOne> ComposedMap;
        SimpleDerivedThree FirstDerived;
        SimpleDerivedTwo SecondDerived;
        SimpleDerivedOne ThirdDerived;
    public:
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

    RegisterCasters<SimpleBase,SimpleDerivedOne,SimpleDerivedTwo>();

    SuperComposed FakeManager;
}

#endif
