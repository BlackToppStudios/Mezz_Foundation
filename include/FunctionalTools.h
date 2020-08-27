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
#ifndef Mezz_Foundation_FunctionalTools_h
#define Mezz_Foundation_FunctionalTools_h

/// @file
/// @brief This is the main Header for the FuntionalTools library Component.

#ifndef SWIG
    #include "DataTypes.h"
    #include "ContainerTools.h"
    #include "MezzException.h"
#endif


#include "FunctionalExtras.h"
#include "FunctionalFunctorsContainerToContainer.h"
#include "FunctionalFunctorsContainerToData.h"
#include "FunctionalPipe.h"

namespace Mezzanine
{

/// @page FuncionalTools Funcional Tools
/// Functional programming is all the craze in many circles. There are places where it can be useful, even in game
/// development, but it isn't a magical panacea that some treat it like. Specifically, it is great for data
/// transformations that aren't performance sensitive and if built using imperative, procedural, or Object Oriented(OOP)
/// methods would be much larger and complex. There are places it is poorly suited, such as the hot path on hetergenous
/// data needing complex manipulation and having side effects.
/// @n @n
/// @section FunctionToolsIntroToFP Intro to Functional Programming
/// For those not familar Functional Programming is a software developement paradign which focuses on manipulating
/// functions and parts of functions to create a different kind of abstraction from imperative or OOP. Rather than
/// creating new data structures or new algorithms a functional style focuses on manipulating re-usable components of
/// control flow. It can be used to create data structures or algorithm but when doing this it is often implied in the
/// construction of containers and arrangement of functions.
/// @n @n
/// There are common things you might see in various programming paradigms, or at least this how Sqeaky views
/// comparisons of the paradigms:
/// @n
///   - Imperative/Procedural
///     - Functions used for code reuse by calling with different parameters,
///     - Plenty of Explicit control flow: Loops, Counters, Ifs, Switches,
///     - Few mechanisms for Older code to invoke newer code,
///     - Structured data held in structs and managed with pointers/references to make trees/lists/graphs,
///     - Data is generally mutable and controlling what can change it might be challenging,
///     - Data layout can (must?) be precisely controlled and is often optimized for size of speed,
///     - Function side effect, like global state mutations are common,
///     - Control flow at the low and high level is usually obvious and usually conceptually linear,
///     - Systems are often older and not Multithreaded, threading mechanism when present are often simple.
/// @n
///   - Object Oriented Programming (OOP)
///     - Functions and objects used for code reuse by changing parameters,
///     - Plenty of Explicit control flow: Loops, Counters, Ifs,
///     - Some switches replaced by inheritance and code snippets can be smaller and more maintainable,
///     - Attaching functions on objects as parameters allows old code to call new code using runtime polymorphism,
///     - Data is often mutable and access is usually controlled by scope and encapsulation,
///     - Data layout can be explicit or implicit in objects,
///     - Functions often have side effects scoped to one object, class, or module,
///     - Control flow is obvious at low level in each object or function,
///     - Control flow can be masked at higher conceptual levels by polymorphism or dependency injection,
///     - System might be old or modern and may have a variety of threading models.
/// @n
///   - Functional Programming (FP)
///     - Functions can be reused by composing them into new functions,
///     - Little explicit control flow, recursion a little more common and lazy evaluation masks control flow,
///     - Functions commonly used as parameters heavily encourages direct code reuse,
///     - The specific structure of data is often an afterthought and some effort required to manage layout,
///     - Data is generally immutable and mutability simulated by copying data,
///     - Functions rarely have side effects and can generally be used in new context without prior research,
///     - Control flow might be heavily masked, Lazy Evaluation, Currying, partial function application...,
///     - Systems likely to be newer and very thread friendly because of immutability and lack of preconditions.
/// @n @n
/// These are just some heuristics, not every system matches all or any of these. Each paradign has pros and cons,
/// and C++ can do all three, but needs some tooling to do FP comfortably.
/// @n @n
/// @section FunctionToolsInTheMezzanine FP in the Mezzanine
/// The Mezzanine has some solid solutions to some of the problems discussed here already, and game development has
/// tight constraints on performance and data. Because of these constraints it is unlikely that FP will be used on
/// performance critical paths directly in games. It does have plenty of uses in tooling, non-optimized paths or places
/// where it performs well.
/// @n @n
/// These tools generally operate on const references and make no attempt to perform lazy evaluation. There are also no
/// specific attempts to integrate threading (This is likely to change with Mezz_Threading). These design decisions were
/// made to maximize simplicity and attempt to fit the problem space poorly addressed elsewhere.
/// @n @n
/// Mezzanine::Functional provides 4 categories of components:
/// @n
///   - @ref FuncionalToolsThePipe, a tool for connecting Functors,
///   - An assortment of @ref FuncionalToolsFunctors that operate on containers,
///   - A few special purpose functions for getting data by @ref FuncionalToolsDrains,
///   - A few @ref FuncionalToolsExtras that smooth out functional interaction.
/// @n @n
/// Some of these component are useful on their own, but the primary design pattern introduced is Pipe/Functor/Drain.
/// The Functors can all be used on their own and encapsulate one little piece of control flow. Individually, they can
/// copy data into new containers and apply different changes or select only specific portion of the data. Just one at
/// a time the can seem simplistic, but chaining several together can allow incredibly nuanced and sophisted data
/// manipulation with minimal complexity. The functors can be nested in code just like normal function calls, but
/// starting with a pipe the chaining can be streamlined and counting parentheses and tracking large amounts of
/// parameters should be reduced.
/// @n @n
///
///
/// Pipe >> Functor >> Drain pattern.
///
/// @subsection FuncionalToolsThePipe The Pipe
/// @subsection FuncionalToolsFunctors Included Functors
///
/// Select/Filter - ✓
/// Reject - ✓
/// Convert/Map - ✓
/// TakeN - ✓
/// TakeBackN - ✓
/// TakeWhile - ✓
/// TakeBackWhile - ✓
/// DropN - ✓
/// DropBackN - ✓
/// DropWhile - ✓
/// DropBackWhile - ✓
///
/// Reverse
/// Sort
/// Zip
/// Unzip
/// Compact - Drops nulls
/// Slice (int Start, int End)
/// Reverse
/// Unique
/// Shuffle
/// ForEach
/// Sample
/// Flatten - Will be difficult with type system.
/// Rotate
/// Partition
///
/// @subsection FuncionalToolsDrains Draining the Pipe
///
/// Max - ✓
/// Min - ✓
/// MinMax - ✓
/// Size/Length - ✓
/// Find - ✓
/// Includes/Contains - ✓
/// AllOf  - ✓
/// AnyOf - ✓
/// NoneOf - ✓
/// IsSorted - ✓
/// IsPartitioned - ✓
/// Reduce/Collect/Accumulate
///
/// @subsection FuncionalToolsExtras Functional Extras
/// Currying - ✓
/// CreateRange - ✓





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Pipe drains are functions that get the value out of the pipe. The must implemented a "Drain" member function
    // which returns a functor. That functor accepts the final value of the pipe and returns some answer.



} //Mezzanine

#endif // Include guard
