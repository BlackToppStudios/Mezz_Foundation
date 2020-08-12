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
        Attribute()
            { std::cout << "\nCreating Internal Attribute at address " << this << ".\n"; }
        Attribute(const StringView AttribName, const StringView AttribValue) :
            Name(AttribName),
            Value(AttribValue)
            { std::cout << "\nCreating Internal Attribute at address " << this << ".\n"; }
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
        static const StringView Indent;
    protected:
        String Name;
        std::vector<Attribute> Attributes;
        std::vector<Node*> Nodes; // Storing pointers is ick, but we need to avoid invalidation issues.
        Node* Parent; // Because of this member.
    public:
        Node(Node* Creator) :
            Parent(Creator)
            //{  }
            { std::cout << "\nCreating Internal Node at address " << this << ".\n"; }
        ~Node()
            //{ this->ClearNodes(); }
            { std::cout << "\nDestroying Node (and all child nodes) at address " << this << ".\n";  this->ClearNodes(); }

        void SetName(const StringView NewName)
            { this->Name = NewName; }
        const String& GetName() const
            { return this->Name; }

        Boole IsRoot() const
            { return this->Parent == nullptr; }
        Node& GetParent() const
            { return *( this->Parent ); }
        size_t GetDepth() const
            { return ( this->Parent ? this->Parent->GetDepth() + 1 : 0 ); }

        Node& CreateChildNode()
            { return *( this->Nodes.emplace_back( new Node(this) ) ); }
        size_t GetNumNodes() const
            { return this->Nodes.size(); }
        Node& GetNode(const size_t Index)
            { return *( this->Nodes[Index] ); }
        const Node& GetNode(const size_t Index) const
            { return *( this->Nodes[Index] ); }
        Node& GetFirstNode()
            { return *( this->Nodes.front() ); }
        const Node& GetFirstNode() const
            { return *( this->Nodes.front() ); }
        Node& GetLastNode()
            { return *( this->Nodes.back() ); }
        const Node& GetLastNode() const
            { return *( this->Nodes.back() ); }
        void ClearNodes()
        {
            for( Node* Ptr : Nodes )
                { delete Ptr; }
            this->Nodes.clear();
        }

        Attribute& CreateChildAttribute()
            { return this->Attributes.emplace_back(); }
        Attribute& CreateChildAttribute(const String& AttribName, const String& AttribValue)
            { return this->Attributes.emplace_back(AttribName,AttribValue); }
        size_t GetNumAttributes() const
            { return this->Attributes.size(); }
        Attribute& GetAttribute(const size_t Index)
            //{ return this->Attributes[Index]; }
            {
                std::cout << "\nAttribute requested at index " << Index;
                Attribute& TempAttrib = this->Attributes.at(Index);
                std::cout << " and address " << &TempAttrib << " from Node " << this << ".\n";
                return this->Attributes[Index];
            }
        const Attribute& GetAttribute(const size_t Index) const
            //{ return this->Attributes[Index]; }
            {
                std::cout << "\nAttribute requested at index " << Index;
                const Attribute& TempAttrib = this->Attributes.at(Index);
                std::cout << " and address " << &TempAttrib << " from Node " << this << ".\n";
                return this->Attributes[Index];
            }
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

    const StringView Node::Indent = "    ";

    std::ostream& operator<<(std::ostream& Stream, const Node& ToSerialize);
    std::ostream& operator<<(std::ostream& Stream, const Node& ToSerialize)
    {
        std::cout << "\nSerializing Node: \"" << ToSerialize.GetName() << "\".\n";
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

    ///////////////////////////////////////////////////////////////////////////////
    // Interface Implementation

    class BackendAttribute : public Serialization::AttributeWalker
    {
    public:
        using NodeRef = std::reference_wrapper<SerializationTest::Node>;
    protected:
        NodeRef ParentNode;
        size_t Index = 0;

        SerializationTest::Attribute& GetAttrib()
            //{ return this->ParentNode.get().GetAttribute(this->Index); }
            {
                std::cout << "\nEntering \"GetAttrib\" from BackendAttrib " << this << ".\n";
                std::cout << "\nAttribute Index: " << this->Index << ".\n";
                Node& TempParent = this->ParentNode.get();
                std::cout << "\nParentNode Address: " << &TempParent << ".\n";
                Attribute& TempAttrib = TempParent.GetAttribute(this->Index);
                std::cout << "\nTempAttrib Address: " << &TempAttrib << ".\n";
                std::cout << "\nExiting \"GetAttrib\".\n";
                return this->ParentNode.get().GetAttribute(this->Index);
            }
        const SerializationTest::Attribute& GetAttrib() const
            //{ return this->ParentNode.get().GetAttribute(this->Index); }
            {
                std::cout << "\nEntering \"GetAttribConst\" from BackendAttrib " << this << ".\n";
                std::cout << "\nAttribute Index: " << this->Index << ".\n";
                Node& TempParent = this->ParentNode.get();
                std::cout << "\nParentNode Address: " << &TempParent << ".\n";
                Attribute& TempAttrib = TempParent.GetAttribute(this->Index);
                std::cout << "\nTempAttrib Address: " << &TempAttrib << ".\n";
                std::cout << "\nExiting \"GetAttrib\".\n";
                return this->ParentNode.get().GetAttribute(this->Index);
            }
    public:
        BackendAttribute(NodeRef Parent, const size_t AttribIndex) :
            ParentNode(Parent),
            Index(AttribIndex)
            //{  }
            { std::cout << "\nCreating BackendAttrib " << this << " from ParentNode " << &Parent << ".\n"; }
        virtual ~BackendAttribute() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Name Operations

        virtual void SetName(const StringView Name)
            //{ this->GetAttrib().SetName(Name); }
            {
                this->GetAttrib().SetName(Name);
                std::cout << "\nAttribute \"" << Name << "\" name set.\n";
            }
        [[nodiscard]]
        virtual StringView GetName() const
            { return this->GetAttrib().GetName(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Value Operations

        virtual void SetString(const StringView Value)
            { this->GetAttrib().SetValue(Value); }
        [[nodiscard]]
        virtual std::optional<StringView> GetString() const
            { return this->GetAttrib().GetValue(); }

        virtual void SetLongDouble(const long double Value)
            { this->GetAttrib().SetValue( StringTools::ConvertToString(Value) ); }
        [[nodiscard]]
        virtual std::optional<long double> GetLongDouble() const
            { return StringTools::ConvertFromString<long double>( this->GetAttrib().GetValue() ); }

        virtual void SetDouble(const double Value)
            { this->GetAttrib().SetValue( StringTools::ConvertToString(Value) ); }
        [[nodiscard]]
        virtual std::optional<double> GetDouble() const
            { return StringTools::ConvertFromString<double>( this->GetAttrib().GetValue() ); }

        virtual void SetFloat(const float Value)
            { this->GetAttrib().SetValue( StringTools::ConvertToString(Value) ); }
        [[nodiscard]]
        virtual std::optional<float> GetFloat() const
            { return StringTools::ConvertFromString<float>( this->GetAttrib().GetValue() ); }

        virtual void SetUInt64(const UInt64 Value)
            { this->GetAttrib().SetValue( StringTools::ConvertToString(Value) ); }
        [[nodiscard]]
        virtual std::optional<UInt64> GetUInt64() const
            { return StringTools::ConvertFromString<UInt64>( this->GetAttrib().GetValue() ); }

        virtual void SetInt64(const Int64 Value)
            { this->GetAttrib().SetValue( StringTools::ConvertToString(Value) ); }
        [[nodiscard]]
        virtual std::optional<Int64> GetInt64() const
            { return StringTools::ConvertFromString<Int64>( this->GetAttrib().GetValue() ); }

        virtual void SetUInt32(const UInt32 Value)
            { this->GetAttrib().SetValue( StringTools::ConvertToString(Value) ); }
        [[nodiscard]]
        virtual std::optional<UInt32> GetUInt32() const
            { return StringTools::ConvertFromString<UInt32>( this->GetAttrib().GetValue() ); }

        virtual void SetInt32(const Int32 Value)
            { this->GetAttrib().SetValue( StringTools::ConvertToString(Value) ); }
        [[nodiscard]]
        virtual std::optional<Int32> GetInt32() const
            { return StringTools::ConvertFromString<Int32>( this->GetAttrib().GetValue() ); }

        virtual void SetUInt16(const UInt16 Value)
            { this->GetAttrib().SetValue( StringTools::ConvertToString(Value) ); }
        [[nodiscard]]
        virtual std::optional<UInt16> GetUInt16() const
            { return StringTools::ConvertFromString<UInt16>( this->GetAttrib().GetValue() ); }

        virtual void SetInt16(const Int16 Value)
            { this->GetAttrib().SetValue( StringTools::ConvertToString(Value) ); }
        [[nodiscard]]
        virtual std::optional<Int16> GetInt16() const
            { return StringTools::ConvertFromString<Int16>( this->GetAttrib().GetValue() ); }

        virtual void SetUInt8(const UInt8 Value)
            { this->GetAttrib().SetValue( StringTools::ConvertToString(Value) ); }
        [[nodiscard]]
        virtual std::optional<UInt8> GetUInt8() const
            { return StringTools::ConvertFromString<UInt8>( this->GetAttrib().GetValue() ); }

        virtual void SetInt8(const Int8 Value)
            { this->GetAttrib().SetValue( StringTools::ConvertToString(Value) ); }
        [[nodiscard]]
        virtual std::optional<Int8> GetInt8() const
            { return StringTools::ConvertFromString<Int8>( this->GetAttrib().GetValue() ); }

        virtual void SetBool(const Boole Value)
            { this->GetAttrib().SetValue( StringTools::ConvertToString(Value) ); }
        [[nodiscard]]
        virtual std::optional<Boole> GetBool() const
            { return StringTools::ConvertFromString<Int8>( this->GetAttrib().GetValue() ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Navigation

        [[nodiscard]]
        virtual Boole AtBegin() const
            { return ( Index ==  0 ); }
        [[nodiscard]]
        virtual Boole AtEnd() const
            { return ( Index == this->ParentNode.get().GetNumAttributes() - 1 ); }

        virtual Serialization::AttributeWalker& Next()
        {
            if( !this->AtEnd() ) {
                ++Index;
            }
            return *this;
        }
        virtual Serialization::AttributeWalker& Previous()
        {
            if( !this->AtBegin() ) {
                --Index;
            }
            return *this;
        }

        void SetIndex(const size_t NewIndex)
        {
            this->Index = NewIndex;
        }
        void SetParentNode(NodeRef NewParent)
        {
            this->ParentNode = NewParent;
        }
    };//BackendAttribute

    class BackendNode : public Serialization::ObjectWalker
    {
    public:
        using NodeRef = std::reference_wrapper<SerializationTest::Node>;
    protected:
        BackendAttribute CurrentAttribute;
        NodeRef SelfNode;
        size_t SelfIndex;

        Node& GetSelfNode()
            { return this->SelfNode.get(); }
        const Node& GetSelfNode() const
            { return this->SelfNode.get(); }

        Node& GetParentNode()
            { return this->SelfNode.get().GetParent(); }
        const Node& GetParentNode() const
            { return this->SelfNode.get().GetParent(); }
    public:
        BackendNode(NodeRef Internal, const size_t Index) :
            CurrentAttribute(Internal,0),
            SelfNode(Internal),
            SelfIndex(Index)
            //{  }
            { std::cout << "\nCreating BackendNode at address " << this << " with Internal Node at " << &Internal.get() << ".\n"; }
        virtual ~BackendNode() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Name Operations

        virtual void SetName(const StringView Name)
            { this->GetSelfNode().SetName(Name); }
        [[nodiscard]]
        virtual StringView GetName() const
            { return this->GetSelfNode().GetName(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Navigation

        [[nodiscard]]
        virtual Boole AtRoot() const
            { return this->GetSelfNode().IsRoot(); }
        [[nodiscard]]
        virtual Boole IsFirstChild() const
            { return ( &( this->GetParentNode().GetFirstNode() ) == &( this->GetSelfNode() ) ); }
        [[nodiscard]]
        virtual Boole IsLastChild() const
            { return ( &( this->GetParentNode().GetLastNode() ) == &( this->GetSelfNode() ) ); }
        [[nodiscard]]
        virtual Boole HasChildren() const
            { return ( this->GetSelfNode().GetNumNodes() > 0 ); }
        [[nodiscard]]
        virtual Boole HasChild(const StringView Name) const
        {
            for( size_t Index = 0 ; Index < this->GetSelfNode().GetNumNodes() ; ++Index )
            {
                if( this->GetSelfNode().GetNode(Index).GetName() == Name ) {
                    return true;
                }
            }
            return false;
        }
        [[nodiscard]]
        virtual Boole HasNextSibling() const
        {
            return ( this->SelfIndex < this->GetParentNode().GetNumNodes() - 1 );
        }
        [[nodiscard]]
        virtual Boole HasPreviousSibling() const
        {
            return ( this->SelfIndex > 0 );
        }

        virtual Serialization::ObjectWalker& Next()
        {
            if( this->HasNextSibling() ) {
                this->SelfIndex++;
                this->SelfNode = this->GetParentNode().GetNode(this->SelfIndex);
                this->CurrentAttribute.SetParentNode(this->SelfNode);
            }
            return *this;
        }
        virtual Serialization::ObjectWalker& Previous()
        {
            if( this->HasPreviousSibling() ) {
                this->SelfIndex--;
                this->SelfNode = this->GetParentNode().GetNode(this->SelfIndex);
                this->CurrentAttribute.SetParentNode(this->SelfNode);
            }
            return *this;
        }
        virtual Serialization::ObjectWalker& Parent()
        {
            Node& Parent = this->GetParentNode();
            for( size_t Index = 0 ; Index < Parent.GetNumNodes() ; ++Index )
            {
                if( &( Parent.GetNode(Index) ) == &( this->GetSelfNode() ) ) {
                    this->SelfIndex = Index;
                    break;
                }
            }
            this->SelfNode = Parent;
            this->CurrentAttribute.SetParentNode(this->SelfNode);
            return *this;
        }
        virtual Serialization::ObjectWalker& FirstChild()
        {
            this->SelfNode = this->GetSelfNode().GetNode(0);
            this->SelfIndex = 0;
            this->CurrentAttribute.SetParentNode(this->SelfNode);
            return *this;
        }
        [[nodiscard]]
        virtual Boole Child(const StringView Name)
        {
            for( size_t Index = 0 ; Index < this->GetSelfNode().GetNumNodes() ; ++Index )
            {
                Node& CurrNode = this->GetSelfNode().GetNode(Index);
                if( CurrNode.GetName() == Name ) {
                    this->SelfIndex = Index;
                    this->SelfNode = CurrNode;
                    this->CurrentAttribute.SetParentNode(this->SelfNode);
                    return true;
                }
            }
            return false;
        }

        [[nodiscard]]
        virtual Boole CreateChild(const StringView Name, const MemberTags Tags, const Boole Move)
        {
            std::cout << "\nEntering \"CreateChild(Node)\" with name \"" << Name << "\".\n";
            (void)Tags;
            for( size_t Index = 0 ; Index < this->GetSelfNode().GetNumNodes() ; ++Index )
            {
                if( this->GetSelfNode().GetNode(Index).GetName() == Name ) {
                    std::cout << "\nExiting \"CreateChild(Node)\" after finding duplicate of new node \"" << Name << "\".\n";
                    return false;
                }
            }
            if( Move ) {
                std:: cout << "Moving from parent node " << &this->GetSelfNode() << '/' << this->GetSelfNode().GetName();
                this->SelfIndex = this->GetSelfNode().GetNumNodes();
                this->SelfNode = this->GetSelfNode().CreateChildNode();
                this->GetSelfNode().SetName(Name);
                this->CurrentAttribute.SetParentNode(this->SelfNode);
                std:: cout << "To child node " << &this->GetSelfNode() << '/' << this->GetSelfNode().GetName() << ".\n";
            }else{
                this->GetSelfNode().CreateChildNode().SetName(Name);
            }
            std::cout << "\nExiting \"CreateChild(Node)\" after successfully creating new node \"" << Name << "\".\n";
            return true;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Attributes

        [[nodiscard]]
        virtual Boole HasAttributes() const
            { return ( this->GetSelfNode().GetNumAttributes() > 0 ); }
        [[nodiscard]]
        virtual Boole HasAttribute(const StringView Name) const
        {
            for( size_t Index = 0 ; Index < this->GetSelfNode().GetNumAttributes() ; ++Index )
            {
                if( this->GetSelfNode().GetAttribute(Index).GetName() == Name ) {
                    return true;
                }
            }
            return false;
        }
        [[nodiscard]]
        virtual Serialization::AttributeWalker& GetAttributes()
        {
            return this->CurrentAttribute;
        }
        [[nodiscard]]
        virtual Serialization::AttributeWalker& GetAttribute(const StringView Name)
        {
            std::cout << "\nEntering \"GetAttribute\" with parameter \"" << Name << "\".\n";
            for( size_t Index = 0 ; Index < this->GetSelfNode().GetNumAttributes() ; ++Index )
            {
                if( this->GetSelfNode().GetAttribute(Index).GetName() == Name ) {
                    std::cout << "\nAttribute Found.\n";
                    this->CurrentAttribute.SetIndex(Index);
                    break;
                }
            }
            std::cout << "\nExiting \"GetAttribute\".\n";
            return this->CurrentAttribute;
        }
        [[nodiscard]]
        virtual Boole CreateAttribute(const StringView Name, const MemberTags Tags)
        {
            (void)Tags;
            std::cout << "\nEntering \"CreateAttribute\" on node " << &this->GetSelfNode() << " with parameter \"" << Name << "\".\n";
            for( size_t Index = 0 ; Index < this->GetSelfNode().GetNumAttributes() ; ++Index )
            {
                if( this->GetSelfNode().GetAttribute(Index).GetName() == Name ) {
                    std::cout << "\nExiting \"CreateAttribute\" after finding duplicate of \"" << Name << "\".\n";
                    return false;
                }
            }
            this->GetSelfNode().CreateChildAttribute().SetName(Name);
            std::cout << "\nExiting \"CreateAttribute\" after creating \"" << Name << "\".\n";
            return true;
        }
    };//BackendNode

    class Backend : public Serialization::BackendBase
    {
    protected:
        Node RootNode;
        BackendNode CurrentNode;
    public:
        Backend() :
            RootNode(nullptr),
            CurrentNode(std::ref(RootNode),0)
            //{ this->RootNode.SetName("Root"); }
            { std::cout << "\nCreating Backend at address " << this << ".\n";  this->RootNode.SetName("Root"); }
        virtual ~Backend() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        virtual StringView GetImplementationName() const
            { return "TestBackend"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Root Object

        virtual Serialization::ObjectWalker& GetWalker()
            { return CurrentNode; }

        ///////////////////////////////////////////////////////////////////////////////
        // Input and Output

        virtual void Read(std::istream& Input)
            { Input >> RootNode; }

        virtual void Write(std::ostream& Output)
            { Output << RootNode; }
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

    SuperComposed FakeManagerOne;
    SuperComposed FakeManagerTwo;

    Backend SerializerBack;
    Serialization::SerializerWalker Walker(SerializerBack.GetWalker());

    Mezzanine::Serialize("FakeManager",FakeManagerOne,Serialization::LatestVersion,Walker);
    //StringStream SerializedManager;
    std::ofstream SerializedManager("SerializedOutput.txt");
    SerializerBack.Write(SerializedManager);
}

#endif
