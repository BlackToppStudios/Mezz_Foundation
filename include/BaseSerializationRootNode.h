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

#ifndef Mezz_Foundation_BaseSerializationRootNode_h
#define Mezz_Foundation_BaseSerializationRootNode_h

// Some headers we don't want swig to see.
#ifndef SWIG
    #include "DataTypes.h"
#endif



namespace Mezzanine {

/// @brief A handle for working with the root of a serialization heirarchy.
/// @details This is intended to house code and logic that would affect a whole hiearchy but not
/// multiple hiearchies ( for code affecting multiple hiearchies see @ref BaseSerializationBackEnd
/// ). This does not provide normal facilities for navigaiting the serialization hiearchy.
///
template <typename SerializationSchemeType>
class BaseSerializationRootNode {
public:
    // Scheme type
    typedef SerializationSchemeType SerializationScheme;

    // Primitive types
    typedef typename SerializationScheme::NodeCount                 NodeCount;

    // Tree type heiarchy types.
    typedef typename SerializationScheme::SerializationAttribute    SerializationAttribute;
    typedef typename SerializationScheme::SerializationBackEnd      SerializationBackEnd;
    typedef typename SerializationScheme::SerializationNode         SerializationNode;
    typedef typename SerializationScheme::SerializationRootNode     SerializationRootNode;

    // end type boilerplate

    //virtual SerializationNode GetAsNode() { return *this; }

};

} // \Mezzanine Namespace

#endif //_logging_h
