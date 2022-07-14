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
#ifndef Mezz_Foundation_SerializationSimpleBackend_h
#define Mezz_Foundation_SerializationSimpleBackend_h

/// @file
/// @brief This file contains a basic backend for serialization that will create formatted output.

#ifndef SWIG
    #include "StringTools.h"
    #include "SerializationTree.h"
#endif // SWIG

namespace Mezzanine {
namespace Serialization {
    /// @brief A Name/Value pair serving as a serializable Attribute.
    class SimpleAttribute
    {
    public:
        /// @brief The character used to separate multiple Attributes in a Node.
        static const char SeparatorChar = ' ';
        /// @brief The character used to connect a value with it's name.
        static const char LinkChar = ':';
    protected:
        /// @brief The name of the Attribute.
        String Name;
        /// @brief The value of the Attribute.
        String Value;
    public:
        /// @brief No-init constructor.
        SimpleAttribute() = default;
        /// @brief Descriptive constructor.
        /// @param AttribName The name to be given to the Attribute.
        /// @param AttribValue The value to be given to the Attribute.
        SimpleAttribute(const StringView AttribName, const StringView AttribValue) :
            Name(AttribName),
            Value(AttribValue)
            {  }
        /// @brief Class destructor.
        ~SimpleAttribute() = default;

        /// @brief Sets the name of the Attribute.
        /// @param NewName The name to be given to the Attribute.
        inline void SetName(const StringView NewName)
            { this->Name = NewName; }
        /// @brief Gets the name of the Attribute.
        /// @return Returns the current name of the Attribute.
        [[nodiscard]]
        inline const String& GetName() const
            { return this->Name; }
        /// @brief Sets the value of the Attribute.
        /// @param NewValue The value to be given to the Attribute.
        inline void SetValue(const StringView NewValue)
            { this->Value = NewValue; }
        /// @brief Gets the value of the Attribute.
        /// @return Returns the current value of the Attribute.
        [[nodiscard]]
        inline const String& GetValue() const
            { return this->Value; }
    };//SimpleAttribute

    /// @brief Streams an Attribute to an output stream.
    /// @param Stream The output stream that will be streamed to.
    /// @param ToSerialize The Attribute to be streamed.
    /// @return Returns a reference to the Stream parameter.
    std::ostream& operator<<(std::ostream& Stream, const SimpleAttribute& ToSerialize);
    std::ostream& operator<<(std::ostream& Stream, const SimpleAttribute& ToSerialize)
    {
        Stream << ToSerialize.GetName() << ":" << ToSerialize.GetValue();
        return Stream;
    }
    /// @brief Streams an Attribute to an input stream.
    /// @param Stream The input stream that will be streamed to.
    /// @param ToSerialize The Attribute to be streamed.
    /// @return Returns a reference to the Stream parameter.
    std::istream& operator>>(std::istream& Stream, SimpleAttribute& ToDeserialize);
    std::istream& operator>>(std::istream& Stream, SimpleAttribute& ToDeserialize)
    {
        String Temp;
        std::getline(Stream,Temp,SimpleAttribute::LinkChar);
        ToDeserialize.SetName(Temp);
        std::getline(Stream,Temp,SimpleAttribute::SeparatorChar);
        ToDeserialize.SetValue(Temp);
        return Stream;
    }

    /// @brief A Node that forms a tree of serializable Attributes.
    class SimpleNode
    {
    public:
        /// @brief The character used to mark the start of a group of Attributes.
        static const char AttributeStartChar = '[';
        /// @brief The character used to mark the end of a group of Attributes.
        static const char AttributeEndChar = ']';
        /// @brief The character used to mark the start of a Node.
        static const char NodeStartChar = '{';
        /// @brief The character used to mark the end of a Node.
        static const char NodeEndChar = '}';
        /// @brief The character(s) used if/when indenting the formatted output.
        static const StringView Indent;
    protected:
        /// @brief The name of the Node.
        String Name;
        /// @brief A container of Attributes belonging to the Node.
        std::vector<SimpleAttribute> Attributes;
        /// @brief A container of child Nodes belonging to the Node.
        std::vector<SimpleNode*> Nodes; // Storing pointers is ick, but we need to avoid invalidation issues.
        /// @brief A pointer to the Nodes parent Node.
        SimpleNode* Parent;             // <- Because of this member.
    public:
        /// @brief Default Constructor.
        SimpleNode() = delete;
        /// @brief Simple Constructor.
        SimpleNode(SimpleNode* Creator) :
            Parent(Creator)
            {  }
        /// @brief Class destructor.
        ~SimpleNode()
            { this->ClearNodes(); }

        /// @brief Sets the name of the Node.
        /// @param NewName The new name to be given to the Node.
        void SetName(const StringView NewName)
            { this->Name = NewName; }
        /// @brief Gets the name of the Node.
        /// @return Returns the current name of the Node.
        [[nodiscard]]
        const String& GetName() const
            { return this->Name; }

        /// @brief Gets whether or not the Node is at the top of the tree.
        /// @return Returns true if this node has no parent, false otherwise.
        [[nodiscard]]
        Boole IsRoot() const
            { return this->Parent == nullptr; }
        /// @brief Gets a reference to the parent of this node.
        /// @remarks If the Node is a root Node then this function will assert.
        /// @return Returns a reference to the parent of the Node.
        [[nodiscard]]
        SimpleNode& GetParent() const
        {
            assert(this->Parent != nullptr && "Getting Parent of root Node.");
            return *( this->Parent );
        }
        /// @brief Gets the depth of the Node from the root Node.
        /// @return Returns the count of Nodes from this Node to the root Node.
        [[nodiscard]]
        size_t GetDepth() const
            { return ( this->Parent ? this->Parent->GetDepth() + 1 : 0 ); }

        /// @brief Creates a new Node as a child of the Node.
        /// @return Returns a reference to the newly created child Node.
        [[nodiscard]]
        SimpleNode& CreateChildNode()
            { return *( this->Nodes.emplace_back( new SimpleNode(this) ) ); }
        /// @brief Gets the number of child Nodes in the Node.
        /// @return Returns the count of child Nodes belonging to the Node.
        [[nodiscard]]
        size_t GetNumNodes() const
            { return this->Nodes.size(); }
        /// @brief Gets a child Node at a given index.
        /// @param Index The index of the Node to retrieve.
        /// @return Returns a reference to the child Node at the specified index.
        [[nodiscard]]
        SimpleNode& GetNode(const size_t Index)
            { return *( this->Nodes[Index] ); }
        /// @brief Gets a child Node at a given index.
        /// @param Index The index of the Node to retrieve.
        /// @return Returns a const reference to the child Node at the specified index.
        [[nodiscard]]
        const SimpleNode& GetNode(const size_t Index) const
            { return *( this->Nodes[Index] ); }
        /// @brief Gets the first child Node.
        /// @return Returns a reference to the first child Node.
        [[nodiscard]]
        SimpleNode& GetFirstNode()
            { return *( this->Nodes.front() ); }
        /// @brief Gets the first child Node.
        /// @return Returns a const reference to the first child Node.
        [[nodiscard]]
        const SimpleNode& GetFirstNode() const
            { return *( this->Nodes.front() ); }
        /// @brief Gets the last child Node.
        /// @return Returns a reference to the last child Node.
        [[nodiscard]]
        SimpleNode& GetLastNode()
            { return *( this->Nodes.back() ); }
        /// @brief Gets the last child Node.
        /// @return Returns a const reference to the last child Node.
        [[nodiscard]]
        const SimpleNode& GetLastNode() const
            { return *( this->Nodes.back() ); }
        /// @brief Removes all child Nodes from the Node.
        void ClearNodes()
        {
            for( SimpleNode* Ptr : Nodes )
                { delete Ptr; }
            this->Nodes.clear();
        }

        /// @brief Creates a new Attribute for the Node.
        /// @return Returns a reference to the new Attribute.
        SimpleAttribute& CreateChildAttribute()
            { return this->Attributes.emplace_back(); }
        /// @brief Creates a new Attribute for the Node.
        /// @param AttribName The name to be given to the new Attribute.
        /// @param AttribValue The value to be given to the new Attribute.
        /// @return Returns a reference to the new Attribute.
        SimpleAttribute& CreateChildAttribute(const String& AttribName, const String& AttribValue)
            { return this->Attributes.emplace_back(AttribName,AttribValue); }
        /// @brief Gets the number of child Attributes in the Node.
        /// @return Returns the current number of child Attributes belonging to the Node.
        [[nodiscard]]
        size_t GetNumAttributes() const
            { return this->Attributes.size(); }
        /// @brief Gets an Attribute by index.
        /// @param Index The index of the Attribute to retrieve.
        /// @return Returns a reference to the Attribute at the specified index.
        [[nodiscard]]
        SimpleAttribute& GetAttribute(const size_t Index)
            { return this->Attributes.at(Index); }
        /// @brief Gets an Attribute by Index.
        /// @param Index The index of the Attribute to retrieve.
        /// @return Returns a const reference to the Attribute at the specified index.
        [[nodiscard]]
        const SimpleAttribute& GetAttribute(const size_t Index) const
            { return this->Attributes.at(Index); }
        /// @brief Gets an Attribute by name.
        /// @param AttribName The name of the Attribute to retrieve.
        /// @return Returns a reference to the specified Attribute.
        [[nodiscard]]
        SimpleAttribute& GetAttribute(const StringView AttribName)
        {
            for( SimpleAttribute& Attrib : this->Attributes )
            {
                if( Attrib.GetName() == AttribName ) {
                    return Attrib;
                }
            }
            throw std::runtime_error("Attribute not found.");
        }
        /// @brief Gets an Attribute by name.
        /// @param AttribName The name of the Attribute to retrieve.
        /// @return Returns a const reference to the specified Attribute.
        [[nodiscard]]
        const SimpleAttribute& GetAttribute(const StringView AttribName) const
        {
            for( const SimpleAttribute& Attrib : this->Attributes )
            {
                if( Attrib.GetName() == AttribName ) {
                    return Attrib;
                }
            }
            throw std::runtime_error("Attribute not found.");
        }

        /// @brief Gets the first Attribute.
        /// @return Returns a reference to the first Attribute in the Node.
        [[nodiscard]]
        SimpleAttribute& GetFirstAttribute()
            { assert( this->GetNumAttributes() );  return this->Attributes.front(); }
        /// @brief Gets the first Attribute.
        /// @return Returns a const reference to the first Attribute in the Node.
        [[nodiscard]]
        const SimpleAttribute& GetFirstAttribute() const
            { assert( this->GetNumAttributes() );  return this->Attributes.front(); }
        /// @brief Gets the last Attribute.
        /// @return Returns a reference to the last Attribute in the Node.
        [[nodiscard]]
        SimpleAttribute& GetLastAttribute()
            { assert( this->GetNumAttributes() );  return this->Attributes.back(); }
        /// @brief Gets the last Attribute.
        /// @return Returns a const reference to the last Attribute in the Node.
        [[nodiscard]]
        const SimpleAttribute& GetLastAttribute() const
            { assert( this->GetNumAttributes() );  return this->Attributes.back(); }
        /// @brief Removes all Attributes from the Node.
        void ClearAttributes()
            { this->Attributes.clear(); }
    };//SimpleNode

    const StringView SimpleNode::Indent = "    ";

    /// @brief Streams a Node to an output stream.
    /// @param Stream The output stream that will be streamed to.
    /// @param ToSerialize The Node to be streamed.
    /// @return Returns a reference to the Stream parameter.
    std::ostream& operator<<(std::ostream& Stream, const SimpleNode& ToSerialize);
    std::ostream& operator<<(std::ostream& Stream, const SimpleNode& ToSerialize)
    {
        size_t NodeDepth = ToSerialize.GetDepth();
        for( size_t IndentCount = 0 ; IndentCount < NodeDepth ; ++IndentCount )
            { Stream << SimpleNode::Indent; }
        Stream << SimpleNode::NodeStartChar << ToSerialize.GetName() << SimpleAttribute::SeparatorChar;
        if( ToSerialize.GetNumAttributes() > 0 ) {
            Stream << SimpleNode::AttributeStartChar << SimpleAttribute::SeparatorChar;
            for( size_t AttribIndex = 0 ; AttribIndex < ToSerialize.GetNumAttributes() ; ++AttribIndex )
                { Stream << ToSerialize.GetAttribute(AttribIndex) << SimpleAttribute::SeparatorChar; }
            Stream << SimpleNode::AttributeEndChar;
        }
        for( size_t NodeIndex = 0 ; NodeIndex < ToSerialize.GetNumNodes() ; ++NodeIndex )
            { Stream << '\n' << ToSerialize.GetNode(NodeIndex); }
        if( ToSerialize.GetNumNodes() > 0 ) {
            Stream << '\n';
            for( size_t IndentCount = 0 ; IndentCount < NodeDepth ; ++IndentCount )
                { Stream << SimpleNode::Indent; }
        }
        Stream << SimpleNode::NodeEndChar;
        return Stream;
    }
    /// @brief Streams a Node to an input stream.
    /// @param Stream The input stream that will be streamed to.
    /// @param ToSerialize The Node to be streamed.
    /// @return Returns a reference to the Stream parameter.
    std::istream& operator>>(std::istream& Stream, SimpleNode& ToDeserialize);
    std::istream& operator>>(std::istream& Stream, SimpleNode& ToDeserialize)
    {
        String Temp;
        Stream.ignore(std::numeric_limits<std::streamsize>::max(),SimpleNode::NodeStartChar);
        std::getline(Stream,Temp,SimpleAttribute::SeparatorChar);
        ToDeserialize.SetName(Temp);
        constexpr std::istream::int_type EoFBit = std::istream::traits_type::eof();
        while( Stream.peek() != SimpleNode::NodeEndChar && Stream.peek() != EoFBit )
        {
            if( Stream.peek() == SimpleNode::AttributeStartChar ) {
                Stream.ignore(2);
                while( Stream.peek() != SimpleNode::AttributeEndChar )
                    { Stream >> ToDeserialize.CreateChildAttribute(); }
                Stream.ignore();
            }else if( Stream.peek() == SimpleNode::NodeStartChar ) {
                Stream >> ToDeserialize.CreateChildNode();
                Stream.ignore();
            }else{
                Stream.ignore();
            }
        }
        return Stream;
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Interface Implementation

    /// @brief An implementation of a TreeWalker that uses SimpleNode/SimpleAttribute.
    class SimpleWalker : public Serialization::TreeWalker
    {
    public:
        /// @brief Convenience type for a storable reference to a SimpleNode.
        using NodeRef = std::reference_wrapper<SimpleNode>;
    protected:
        /// @brief A reference to the Node the walker is currently on/visiting/pointing to.
        NodeRef SelfNode;
        /// @brief The index SelfNode has within its parent.
        size_t SelfIndex;

        /// @brief Gets a reference to the internal Node.
        /// @return Returns a reference to the Node this walker is currently visiting.
        SimpleNode& GetSelfNode()
            { return this->SelfNode.get(); }
        /// @brief Gets a reference to the internal Node.
        /// @return Returns a const reference to the Node this walker is currently visiting.
        const SimpleNode& GetSelfNode() const
            { return this->SelfNode.get(); }

        /// @brief Gets a reference to the parent of the internal Node.
        /// @return Returns a reference to the parent of the Node this walker is currently visiting.
        SimpleNode& GetParentNode()
            { return this->SelfNode.get().GetParent(); }
        /// @brief Gets a reference to the parent of the internal Node.
        /// @return Returns a const reference to the parent of the Node this walker is currently visiting.
        const SimpleNode& GetParentNode() const
            { return this->SelfNode.get().GetParent(); }

        /// @brief Gets a reference to an Attribute belonging to the internal Node.
        /// @param Name The name of the Attribute to retrieve.
        /// @return Returns a reference to the specified Attribute on the Node this walker is currently visiting.
        SimpleAttribute& GetAttrib(const StringView Name)
            { return this->GetSelfNode().GetAttribute(Name); }
        /// @brief Gets a reference to an Attribute belonging to the internal Node.
        /// @param Name The name of the Attribute to retrieve.
        /// @return Returns a const reference to the specified Attribute on the Node this walker is currently visiting.
        const SimpleAttribute& GetAttrib(const StringView Name) const
            { return this->GetSelfNode().GetAttribute(Name); }

        ///////////////////////////////////////////////////////////////////////////////
        // Attribute Access

        /// @copydoc Serialization::TreeWalker::SetString(const StringView,const StringView)
        virtual void SetString(const StringView Name, const StringView Value)
            { this->GetAttrib(Name).SetValue(Value); }
        /// @copydoc Serialization::TreeWalker::GetString(const StringView) const
        [[nodiscard]]
        virtual std::optional<StringView> GetString(const StringView Name) const
            { return this->GetAttrib(Name).GetValue(); }

        /// @copydoc Serialization::TreeWalker::SetLongDouble(const StringView,const StringView)
        virtual void SetLongDouble(const StringView Name, const long double Value)
            { this->GetAttrib(Name).SetValue( StringTools::ConvertToString(Value) ); }
        /// @copydoc Serialization::TreeWalker::GetLongDouble(const StringView) const
        [[nodiscard]]
        virtual std::optional<long double> GetLongDouble(const StringView Name) const
            { return StringTools::ConvertFromString<long double>( this->GetAttrib(Name).GetValue() ); }

        /// @copydoc Serialization::TreeWalker::SetDouble(const StringView,const StringView)
        virtual void SetDouble(const StringView Name, const double Value)
            { this->GetAttrib(Name).SetValue( StringTools::ConvertToString(Value) ); }
        /// @copydoc Serialization::TreeWalker::GetDouble(const StringView) const
        [[nodiscard]]
        virtual std::optional<double> GetDouble(const StringView Name) const
            { return StringTools::ConvertFromString<double>( this->GetAttrib(Name).GetValue() ); }

        /// @copydoc Serialization::TreeWalker::SetFloat(const StringView,const StringView)
        virtual void SetFloat(const StringView Name, const float Value)
            { this->GetAttrib(Name).SetValue( StringTools::ConvertToString(Value) ); }
        /// @copydoc Serialization::TreeWalker::GetFloat(const StringView) const
        [[nodiscard]]
        virtual std::optional<float> GetFloat(const StringView Name) const
            { return StringTools::ConvertFromString<float>( this->GetAttrib(Name).GetValue() ); }

        /// @copydoc Serialization::TreeWalker::SetUInt64(const StringView,const StringView)
        virtual void SetUInt64(const StringView Name, const UInt64 Value)
            { this->GetAttrib(Name).SetValue( StringTools::ConvertToString(Value) ); }
        /// @copydoc Serialization::TreeWalker::GetUInt64(const StringView) const
        [[nodiscard]]
        virtual std::optional<UInt64> GetUInt64(const StringView Name) const
            { return StringTools::ConvertFromString<UInt64>( this->GetAttrib(Name).GetValue() ); }

        /// @copydoc Serialization::TreeWalker::SetInt64(const StringView,const StringView)
        virtual void SetInt64(const StringView Name, const Int64 Value)
            { this->GetAttrib(Name).SetValue( StringTools::ConvertToString(Value) ); }
        /// @copydoc Serialization::TreeWalker::GetInt64(const StringView) const
        [[nodiscard]]
        virtual std::optional<Int64> GetInt64(const StringView Name) const
            { return StringTools::ConvertFromString<Int64>( this->GetAttrib(Name).GetValue() ); }

        /// @copydoc Serialization::TreeWalker::SetUInt32(const StringView,const StringView)
        virtual void SetUInt32(const StringView Name, const UInt32 Value)
            { this->GetAttrib(Name).SetValue( StringTools::ConvertToString(Value) ); }
        /// @copydoc Serialization::TreeWalker::GetUInt32(const StringView) const
        [[nodiscard]]
        virtual std::optional<UInt32> GetUInt32(const StringView Name) const
            { return StringTools::ConvertFromString<UInt32>( this->GetAttrib(Name).GetValue() ); }

        /// @copydoc Serialization::TreeWalker::SetInt32(const StringView,const StringView)
        virtual void SetInt32(const StringView Name, const Int32 Value)
            { this->GetAttrib(Name).SetValue( StringTools::ConvertToString(Value) ); }
        /// @copydoc Serialization::TreeWalker::GetInt32(const StringView) const
        [[nodiscard]]
        virtual std::optional<Int32> GetInt32(const StringView Name) const
            { return StringTools::ConvertFromString<Int32>( this->GetAttrib(Name).GetValue() ); }

        /// @copydoc Serialization::TreeWalker::SetUInt16(const StringView,const StringView)
        virtual void SetUInt16(const StringView Name, const UInt16 Value)
            { this->GetAttrib(Name).SetValue( StringTools::ConvertToString(Value) ); }
        /// @copydoc Serialization::TreeWalker::GetUInt16(const StringView) const
        [[nodiscard]]
        virtual std::optional<UInt16> GetUInt16(const StringView Name) const
            { return StringTools::ConvertFromString<UInt16>( this->GetAttrib(Name).GetValue() ); }

        /// @copydoc Serialization::TreeWalker::SetInt16(const StringView,const StringView)
        virtual void SetInt16(const StringView Name, const Int16 Value)
            { this->GetAttrib(Name).SetValue( StringTools::ConvertToString(Value) ); }
        /// @copydoc Serialization::TreeWalker::GetInt16(const StringView) const
        [[nodiscard]]
        virtual std::optional<Int16> GetInt16(const StringView Name) const
            { return StringTools::ConvertFromString<Int16>( this->GetAttrib(Name).GetValue() ); }

        /// @copydoc Serialization::TreeWalker::SetUInt8(const StringView,const StringView)
        virtual void SetUInt8(const StringView Name, const UInt8 Value)
            { this->GetAttrib(Name).SetValue( StringTools::ConvertToString(Value) ); }
        /// @copydoc Serialization::TreeWalker::GetUInt8(const StringView) const
        [[nodiscard]]
        virtual std::optional<UInt8> GetUInt8(const StringView Name) const
            { return StringTools::ConvertFromString<UInt8>( this->GetAttrib(Name).GetValue() ); }

        /// @copydoc Serialization::TreeWalker::SetInt8(const StringView,const StringView)
        virtual void SetInt8(const StringView Name, const Int8 Value)
            { this->GetAttrib(Name).SetValue( StringTools::ConvertToString(Value) ); }
        /// @copydoc Serialization::TreeWalker::GetInt8(const StringView) const
        [[nodiscard]]
        virtual std::optional<Int8> GetInt8(const StringView Name) const
            { return StringTools::ConvertFromString<Int8>( this->GetAttrib(Name).GetValue() ); }

        /// @copydoc Serialization::TreeWalker::SetBool(const StringView,const StringView)
        virtual void SetBool(const StringView Name, const Boole Value)
            { this->GetAttrib(Name).SetValue( StringTools::ConvertToString(Value) ); }
        /// @copydoc Serialization::TreeWalker::GetBool(const StringView) const
        [[nodiscard]]
        virtual std::optional<Boole> GetBool(const StringView Name) const
            { return StringTools::ConvertFromString<Int8>( this->GetAttrib(Name).GetValue() ); }
    public:
        /// @brief Default constructor.
        SimpleWalker() = delete;
        /// @brief Descriptive constructor.
        /// @param Internal A reference to the internal node in the tree the walker will begin at.
        /// @param Index The index of the internal node within it's parent.
        SimpleWalker(NodeRef Internal, const size_t Index) :
            SelfNode(Internal),
            SelfIndex(Index)
            {  }
        /// @brief Class destructor.
        virtual ~SimpleWalker() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // I/O Operations

        /// @copydoc Serialization::TreeWalker::ReadFromStream(std::istream&)
        virtual std::istream& ReadFromStream(std::istream& Stream) override
            { return ( Stream >> this->GetSelfNode() ); }
        /// @copydoc Serialization::TreeWalker::WriteToStream(std::ostream&)
        virtual std::ostream& WriteToStream(std::ostream& Stream) const override
            { return ( Stream << this->GetSelfNode() ); }

        ///////////////////////////////////////////////////////////////////////////////
        // Name Operations

        /// @copydoc Serialization::TreeWalker::SetName(const StringView)
        virtual void SetName(const StringView Name) override
            { this->GetSelfNode().SetName(Name); }
        /// @copydoc Serialization::TreeWalker::GetName() const
        [[nodiscard]]
        virtual StringView GetName() const override
            { return this->GetSelfNode().GetName(); }

        ///////////////////////////////////////////////////////////////////////////////
        // Navigation

        /// @copydoc Serialization::TreeWalker::HasParent() const
        [[nodiscard]]
        virtual Boole HasParent() const override
            { return !this->GetSelfNode().IsRoot(); }
        /// @copydoc Serialization::TreeWalker::MoveToParent()
        virtual Serialization::TreeWalker& MoveToParent() override
        {
            SimpleNode& Parent = this->GetParentNode();
            for( size_t Index = 0 ; Index < Parent.GetNumNodes() ; ++Index )
            {
                if( &( Parent.GetNode(Index) ) == &( this->GetSelfNode() ) ) {
                    this->SelfIndex = Index;
                    break;
                }
            }
            this->SelfNode = Parent;
            return *this;
        }

        /// @copydoc Serialization::TreeWalker::HasNextSibling() const
        [[nodiscard]]
        virtual Boole HasNextSibling() const override
        {
            if( !this->HasParent() ) {
                return false;
            }
            return ( this->SelfIndex < this->GetParentNode().GetNumNodes() - 1 );
        }
        /// @copydoc Serialization::TreeWalker::MoveToNext()
        virtual Serialization::TreeWalker& MoveToNext() override
        {
            if( this->HasNextSibling() ) {
                this->SelfIndex++;
                this->SelfNode = this->GetParentNode().GetNode(this->SelfIndex);
            }
            return *this;
        }
        /// @copydoc Serialization::TreeWalker::HasPreviousSibling() const
        [[nodiscard]]
        virtual Boole HasPreviousSibling() const override
        {
            return ( this->SelfIndex > 0 );
        }
        /// @copydoc Serialization::TreeWalker::MoveToPrevious()
        virtual Serialization::TreeWalker& MoveToPrevious() override
        {
            if( this->HasPreviousSibling() ) {
                this->SelfIndex--;
                this->SelfNode = this->GetParentNode().GetNode(this->SelfIndex);
            }
            return *this;
        }

        /// @copydoc Serialization::TreeWalker::HasChildren() const
        [[nodiscard]]
        virtual Boole HasChildren() const override
            { return ( this->GetSelfNode().GetNumNodes() > 0 ); }
        /// @copydoc Serialization::TreeWalker::HasChild(const StringView) const
        [[nodiscard]]
        virtual Boole HasChild(const StringView Name) const override
        {
            for( size_t Index = 0 ; Index < this->GetSelfNode().GetNumNodes() ; ++Index )
            {
                if( this->GetSelfNode().GetNode(Index).GetName() == Name ) {
                    return true;
                }
            }
            return false;
        }
        /// @copydoc Serialization::TreeWalker::MoveToFirstChild()
        virtual Serialization::TreeWalker& MoveToFirstChild() override
        {
            this->SelfNode = this->GetSelfNode().GetNode(0);
            this->SelfIndex = 0;
            return *this;
        }
        /// @copydoc Serialization::TreeWalker::MoveToChild(const StringView)
        [[nodiscard]]
        virtual Boole MoveToChild(const StringView Name) override
        {
            for( size_t Index = 0 ; Index < this->GetSelfNode().GetNumNodes() ; ++Index )
            {
                SimpleNode& CurrNode = this->GetSelfNode().GetNode(Index);
                if( CurrNode.GetName() == Name ) {
                    this->SelfIndex = Index;
                    this->SelfNode = CurrNode;
                    return true;
                }
            }
            return false;
        }

        /// @copydoc Serialization::TreeWalker::CreateChild(const StringView, const MemberTags, const Boole)
        [[nodiscard]]
        virtual Boole CreateChild(const StringView Name, const MemberTags Tags, const Boole Move) override
        {
            (void)Tags;
            for( size_t Index = 0 ; Index < this->GetSelfNode().GetNumNodes() ; ++Index )
            {
                if( this->GetSelfNode().GetNode(Index).GetName() == Name ) {
                    return false;
                }
            }
            if( Move ) {
                this->SelfIndex = this->GetSelfNode().GetNumNodes();
                this->SelfNode = this->GetSelfNode().CreateChildNode();
                this->GetSelfNode().SetName(Name);
            }else{
                this->GetSelfNode().CreateChildNode().SetName(Name);
            }
            return true;
        }

        ///////////////////////////////////////////////////////////////////////////////
        // Attributes

        /// @copydoc Serialization::TreeWalker::HasAttributes() const
        [[nodiscard]]
        virtual Boole HasAttributes() const override
            { return ( this->GetSelfNode().GetNumAttributes() > 0 ); }
        /// @copydoc Serialization::TreeWalker::HasAttribute(const StringView) const
        [[nodiscard]]
        virtual Boole HasAttribute(const StringView Name) const override
        {
            for( size_t Index = 0 ; Index < this->GetSelfNode().GetNumAttributes() ; ++Index )
            {
                if( this->GetSelfNode().GetAttribute(Index).GetName() == Name ) {
                    return true;
                }
            }
            return false;
        }
        /// @copydoc Serialization::TreeWalker::CreateAttribute(const StringView, const MemberTags)
        [[nodiscard]]
        virtual Boole CreateAttribute(const StringView Name, const MemberTags Tags) override
        {
            (void)Tags;
            for( size_t Index = 0 ; Index < this->GetSelfNode().GetNumAttributes() ; ++Index )
            {
                if( this->GetSelfNode().GetAttribute(Index).GetName() == Name ) {
                    return false;
                }
            }
            this->GetSelfNode().CreateChildAttribute().SetName(Name);
            return true;
        }
    };//SimpleWalker

    /// @brief An implementation of a BackendBase using the "Simple" serialization implementations.
    class SimpleBackend : public Serialization::BackendBase
    {
    protected:
        /// @brief The Node at the top of the tree.
        SimpleNode RootNode;
        /// @brief The walker visiting the Nodes in the tree.
        SimpleWalker CurrentNode;
    public:
        /// @brief Class destructor.
        SimpleBackend() :
            RootNode(nullptr),
            CurrentNode(std::ref(RootNode),0)
            { this->RootNode.SetName("Root"); }
        /// @brief Class destructor.
        virtual ~SimpleBackend() = default;

        ///////////////////////////////////////////////////////////////////////////////
        // Query

        /// @copydoc Serialization::BackendBase::GetImplementationName() const
        virtual StringView GetImplementationName() const
            { return "SimpleBackend"; }

        ///////////////////////////////////////////////////////////////////////////////
        // Root Object

        /// @copydoc Serialization::BackendBase::GetWalker()
        virtual Serialization::TreeWalker& GetWalker()
            { return CurrentNode; }

        ///////////////////////////////////////////////////////////////////////////////
        // Input and Output

        /// @copydoc Serialization::BackendBase::Read(std::istream&)
        virtual void Read(std::istream& Input)
            { Input >> RootNode; }

        /// @copydoc Serialization::BackendBase::Write(std::ostream&)
        virtual void Write(std::ostream& Output)
            { Output << RootNode; }
    };//SimpleBackend
}//Serialization
}//Mezzanine

#endif
