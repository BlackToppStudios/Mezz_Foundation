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
#ifndef Mezz_Foundation_Serialization_h
#define Mezz_Foundation_Serialization_h

/// @file
/// @brief The collection of utilities that form the Serialization front-end.

#ifndef SWIG
    #include "SerializationUtility.h"
    #include "SerializationTree.h"
    #include "SerializationPolymorphicCaster.h"
    #include "SerializationPointerTracker.h"
    #include "SerializationSerializer.h"
    #include "SerializationDeserializer.h"
#endif

// - Investigate the need to support c-string serialization.
// + Track serialized pointers to prevent loops/infinite recurrsion during serialization.
// + Investigate way to register pointer types to better check if pointers to different types in the inheritance heirarchy is pointing to the same object. (Pointer math with the RTTI pointer?)
// + Track and store objects with shared ownership.
// + Implement a way to generate/construct child objects for deserialization. ("Create" MemberTag value?)(Require default constructors for serialized types?)
// + Consider better methods of storing unique IDs of linked objects and methods of getting at them. (Maybe encourage more designs that only store the ID and fetch pointers per invocation)
// + Investigate templatizing the backend for a full template solution (instead of polymorphic backends).
// + Investigate the possibility of having a single versioned save/load function that calls the Serialize/Deserialize template specializations, allowing overloads at different levels.

#endif
