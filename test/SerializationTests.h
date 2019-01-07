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
#ifndef Mezz_Foundation_SerializationTests_h
#define Mezz_Foundation_SerializationTests_h

/// @file
/// @brief A test of serialization by creating a fake serialization scheme

// Add other headers you need here
#include "MezzTest.h"

#include "BaseSerializationAttribute.h"
#include "BaseSerializationBackEnd.h"
#include "BaseSerializationRootNode.h"
#include "BaseSerializationNode.h"

// Fake sample serialization scheme

// Most serialization schemes would rely on the underlying implementation to do the work, and not implemented their own
// hierarchy scheme. So this is not a good example. Look at the

#include <memory>
#include <sstream>

// internals of stupidly simple serialization structure.

struct TssNode {
    TssNode* Parent; // null for root node
    std::vector<TssNode> children;
    std::stringstream data;

    // Methods
    TssNode() : Parent(nullptr) {}
    Mezzanine::Boole isRoot() const { return nullptr == Parent; }
};

// API of simple serialization scheme

template<typename SerializationSchemeType>
class TssSerializationBackEnd : public Mezzanine::BaseSerializationBackEnd<SerializationSchemeType>
{
public:

    // Most Serialization Back ends will redeclare these types.
    typedef SerializationSchemeType                                 SerializationScheme;
    typedef typename SerializationScheme::SerializationAttribute    SerializationAttribute;
    typedef typename SerializationScheme::SerializationBackEnd      SerializationBackEnd;
    typedef typename SerializationScheme::SerializationNode         SerializationNode;
    typedef typename SerializationScheme::SerializationRootNode     SerializationRootNode;

private:


public:

    // Derived methods
    virtual SerializationRootNode CreateRootNode() override { return SerializationRootNode(); }

};

template<typename SerializationSchemeType>
class TssSerializationAttribute : public Mezzanine::BaseSerializationAttribute<SerializationSchemeType>
{
public:
    typedef SerializationSchemeType                                 SerializationScheme;
    typedef typename SerializationScheme::SerializationAttribute    SerializationAttribute;
    typedef typename SerializationScheme::SerializationBackEnd      SerializationBackEnd;
    typedef typename SerializationScheme::SerializationNode         SerializationNode;
    typedef typename SerializationScheme::SerializationRootNode     SerializationRootNode;

};

template<typename SerializationSchemeType>
class TssSerializationRootNode : public Mezzanine::BaseSerializationRootNode<SerializationSchemeType>
{
public:
    typedef SerializationSchemeType                                 SerializationScheme;
    typedef typename SerializationScheme::SerializationAttribute    SerializationAttribute;
    typedef typename SerializationScheme::SerializationBackEnd      SerializationBackEnd;
    typedef typename SerializationScheme::SerializationNode         SerializationNode;
    typedef typename SerializationScheme::SerializationRootNode     SerializationRootNode;
private:


public:

};

template<typename SerializationSchemeType>
class TssSerializationNode : public Mezzanine::BaseSerializationNode<SerializationSchemeType>
{
public:
    typedef SerializationSchemeType                                 SerializationScheme;
    typedef typename SerializationScheme::SerializationAttribute    SerializationAttribute;
    typedef typename SerializationScheme::SerializationBackEnd      SerializationBackEnd;
    typedef typename SerializationScheme::SerializationNode         SerializationNode;
    typedef typename SerializationScheme::SerializationRootNode     SerializationRootNode;

};

// Every Serialization system needs a "Scheme" that will define every type in
class TestSerializationScheme
{
public:
    // Primitive types
    typedef Mezzanine::Int32                                    NodeCount;

    // Tree type heiarchy types.
    typedef TestSerializationScheme                             SerializationScheme;
    typedef TssSerializationAttribute<TestSerializationScheme>  SerializationAttribute;
    typedef TssSerializationBackEnd<TestSerializationScheme>    SerializationBackEnd;
    typedef TssSerializationRootNode<TestSerializationScheme>   SerializationRootNode;
    typedef TssSerializationNode<TestSerializationScheme>       SerializationNode;



};




// The actual tests
DEFAULT_TEST_GROUP(SerializationTests, Serialization)
{
    using namespace Mezzanine;


}

#endif
