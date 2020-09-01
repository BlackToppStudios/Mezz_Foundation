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
/// development, but it isn't a magical panacea as some treat it. Specifically, it is great for data transformations
/// that aren't sensitive to a specific kind of performance. Functional designs can often get good bandwidth or good
/// latency but rarely both. The mechanisms for optimizing one way or the other are often buried deep in the functional
/// language or toolset, this has the nice effect of concealing a great deal of complexity allowing the developer to
/// focus on solving a problem. These tools are no different, it is designed for high bandwidth and ease of use. Code
/// built using imperative, procedural, or Object Oriented(OOP) methods would be much larger and complex but would allow
/// the developer using them to readily design for good latency or good bandwidth. This being a game development suite
/// is designed for an optimal mix of these depending on where the code goes. The Mezzanine like all game dev tools aims
/// for optimal per frame latency (FPS!) and as much bandwidth (How many triangles, sprites, filters in the game state).
/// This means there are places typical functional designs are poorly suited, such as the hot path or working with
/// hetergenous data needing complex manipulation and having side effects. There are places functional design is ideal
/// for because the game dev tool is imperfect such as creating tools, servers, and places where correctness and
/// rapidity of development is more important than rapidity of execution.
/// @n @n
/// @section FunctionToolsIntroToFP Intro to Functional Programming
/// For those not familar Functional Programming is a software developement paradign which focuses on manipulating
/// functions and parts of functions to create a different kind of abstraction from imperative or OOP designs. Rather
/// than creating new data structures or new algorithms a functional style focuses on manipulating re-usable components
/// of control flow. It can be used to create data structures or algorithmw but when doing this it is often implied in
/// the construction of containers and arrangement of functions.
/// @n @n
/// There are common things you might see in various programming paradigms. Or at least this how Sqeaky thinks a
/// comparison of these paradigms might be useful to people familar with OOP/Imperative and trying a Functional
/// paradigm for the first time:
/// @n
///   - Imperative/Procedural,
///     - Code Reuse,
///       - Functions are used for code reuse by calling with different parameters,
///       - Few mechanisms for older code to invoke newer code,
///
///     - Control Flow,
///       - Plenty of explicit control flow: Loops, Counters, Ifs, Switches,
///       - Control flow at the low and high level is usually obvious and usually conceptually linear,
///       - Not many ways to create new control flow,
///       - Application call graphs are likely cleanly hierarchical but difficult to change,
///
///     - Data Structures,
///       - Structured data held in structs and managed with pointers/references to make trees/lists/graphs,
///       - Data is generally mutable and controlling what can change it might be challenging,
///       - Data layout can (must?) be precisely controlled and is often optimized for size of speed,
///
///     - Reasoning About Code,
///       - Function side effects, like global state mutations, are common,
///       - Higher level abstractions not need or useful to consider application flow,
///
///     - Threading,
///       - Systems are often older and not multithreaded, threading mechanism when present are often simple.
///       - When present bugs tend to be numerous.
/// @n
///   - Object Oriented Programming (OOP)
///     - Code Reuse,
///       - Functions and objects are used for code reuse by changing parameters,
///       - Attaching functions on objects as parameters allows old code to call new code using runtime polymorphism,
///
///     - Control Flow,
///       - Plenty of explicit control flow: Loops, Counters, Ifs,
///       - Some switches replaced by inheritance and code snippets can be smaller and more maintainable,
///       - Control flow is obvious at low level in each object or function,
///       - Creation of new control flow can be created but generally treated as major efforts (DI, ECS),
///       - Control flow can be masked at higher conceptual levels by polymorphism or dependency injection,
///       - Application call graphs are likely cleanly hierarchical until inheritance is involved,
///
///     - Data Structures,
///       - Data layout can be explicit or implicit in objects,
///       - Data is often mutable and access is usually controlled by scope and encapsulation,
///       - Controlling (or not controlling) data layout generally very easy,
///
///     - Reasoning About Code,
///       - Functions often have side effects scoped to one object, class, or module,
///       - Classes considered in isolation are often simple to reason about,
///       - If inheritance is abused reasoning is impossible (Just keep your class heirachies small),
///       - Dependency injection containers and Entity Component Systems are used explicitly where reasoning is hard,
///
///     - Threading,
///       - System might be old or modern and may have a variety of threading models,
///       - Classes exist to manage threading primitives Threads, Mutexes, ThreadingPools, Atomic Variables, et al,
///       - When present bugs tend to be numerous.
/// @n
///   - Functional Programming (FP)
///     - Code Reuse,
///       - Functions can be reused by composing them into new functions or changing parameters,
///       - Functions commonly used as parameters heavily encourages direct reuse of older code,
///
///     - Control Flow,
///       - Little explicit control flow, lambdas, recursion, and lazy evaluation masks control flow,
///       - Control flow might be heavily masked by Lazy Evaluation, Currying, partial function application, et al,
///       - New control flow can be created with new functions,
///
///     - Data Structures,
///       - The specific structure of data is often an afterthought and some effort required to manage layout,
///       - Data is generally immutable and mutability simulated by copying data,
///
///     - Reasoning About Code,
///       - Functions rarely have side effects and can generally be used in new contexts without prior review,
///       - Results and type of transformation replace normal paradigms of control flow,
///       - Trying to consider application at once is often difficult,
///
///     - Threading,
///       - Systems likely to be newer and very thread friendly because of immutability and lack of preconditions
///       - Trivial to add threading correctly, often used to make up for suboptimal performance designs.
///
/// @n @n
/// These are just some heuristics, not every system matches all or any of these. Each paradign has pros and cons,
/// and C++ can do all three, but needs some tooling to do FP comfortably.
/// @n @n
/// In C++20 we will be getting a big upgrade to ranges, which is a latency optimized design based on iterators and lazy
/// evaluation. This design is great because if results are discard they are never computed, it doesn't make copies,
/// and allows for mutable or immutable functions. This design is bad because allow mutabilty has concurrency
/// implications and creating new functions to operate with ranges is complex and has tight preconditions.
/// @n @n
/// We wanted tools now, and tools that would be useful alongside or in additional to these tool so we made a different
/// design.
/// @n @n
/// @section FunctionToolsInTheMezzanine FP in the Mezzanine
/// The Mezzanine has some solid solutions to some of the problems discussed here already, and game development has
/// tight constraints on performance and data. Because of these constraints it is unlikely that FP will be used on
/// performance critical paths directly in games, and those tools in the hot path are less useful in general contexts.
/// FP does have plenty of uses in tooling, non-optimized paths, and places where it performs well.
/// @n @n
/// These tools generally operate on const references and make no attempt to perform lazy evaluation. This means there
/// will be a huge amount of copies, tools like C++20 ranges may be more applicable on heavyweight classes. Other than
/// operating on immutable data there are no specific attempts to integrate threading (This is likely to change with
/// Mezz_Threading). These design decisions were made to maximize simplicity and attempt to fit the problem space poorly
/// addressed elsewhere in the Mezzanine.
/// @n @n
/// Mezzanine::Functional provides 4 categories of components:
/// @n
///   - @ref FuncionalToolsThePipe, a tool for connecting Functors,
///   - An assortment of @ref FuncionalToolsFunctors that operate on containers and return containers,
///   - A few special purpose functions for getting data by using @ref FuncionalToolsDrains,
///   - A few @ref FuncionalToolsExtras that smooth out functional interaction.
/// @n @n
/// Some of these components are useful on their own, but the primary design pattern introduced is
/// @ref FunctionalToolsPipeFunctorDrainPattern .
/// The Functors can all be used on their own and encapsulate one little piece of control flow. Individually, they can
/// copy data into new containers, apply different changes, or select only specific portion of the data. Just using one
/// at a time the can seem simplistic, alone these can be seen as just a layer over many of the standard library
/// functions or simple pieces of control flow that might be a loop and an if. As syntactic sugar these might be nice
/// but not compelling.
/// @n
/// Through chaining several functor together they can allow incredibly nuanced and sophisted data manipulation with
/// minimal complexity. The functors can be nested in code just like normal function calls, but by starting with a pipe
/// chaining them can be streamlined. Through pipes counting parentheses and tracking large amounts of parameters should
/// be reduced.
///
/// @subsection FuncionalToolsThePipe The Pipe
/// todo
///
/// operator>> accepts a functor and returns a pipe with the data.
///
/// operator> accepts a function and returns the data, effectively terminating the pipe.
///
///
/// @subsection FuncionalToolsFunctors Included Functors
/// There are a collection of functions that operate on containers and return a container. All of the provided functors
/// provide a similar interface and similar set of operations. They all accept a container by const reference. They all
/// accept either data or a predicate. If they accept a predicate it will generally be applied to each member of the
/// input (invoked once for each item in the input passing that item to it). All of these functors can also accept
/// the return container type.
/// @n @n
/// All of the functors support direct invocation. if passed all the arguments they will perform the operation
/// immediately:
/// @code
/// auto SomeFreshContainer = Functor(Data, PredicateOrArgument);
/// @endcode
/// @n @n
/// All of the functors suport automatic currying of predicates. Currying is creating a new function by pre-filling some
/// of the parameters with some value and getting a new function. This as a concept can be alien to experienced
/// Imperative and OOP developers and some terms to research include "function currying",
/// "partial function application", and "std::bind". This is easy enough to use:
/// @code
/// auto SomeFreshFunction = Functor(PredicateOrArgument);
/// auto SomeFreshContainer = Functor(Data);
/// @endcode
/// @n @n
/// All of these functors also support explicit specification of return type. Internally they all operating on iterator
/// pairs and don't much care about the details of the output container. Be careful with this, it makes no sense to
/// sort or reverse a set. This works with immediately invoked or autocurrying forms of the functors:
/// @code
/// ContainerType SomeFreshContainer = Functor<ContainerType>(Data, PredicateOrArgument);
/// // Or
/// auto SomeFreshFunction = Functor<ContainerType>(PredicateOrArgument);
/// ContainerType SomeFreshContainer = Functor(Data);
/// @endcode
///
/// Here is a list of supplied functors that accept a container and return a container:
///
///     - @ref Convert - Apply the predicate to each item and return all the results.
///     - @ref DropBackN - Copy all the items except the last N, in reverse order
///     - @ref DropN - Skip the first N, then copy the rest.
///     - @ref DropBackWhile - Copy items from the back, but skip items while the predicate it true. Reverse order
///     - @ref DropWhile - Skip first items while predicate returns true, then copy the rest.
///     - @ref Filter - Alias for @ref Select .
///     - @ref Map - Alias for @ref Convert .
///     - @ref Reject - Copy All items that don't match a predicate.
///     - @ref Reverse - Return the continer in reverse order.
///     - @ref Select - Copy all items matching a predicate.
///     - @ref TakeBackN - Copy the last N items in reverse order.
///     - @ref TakeN - Copy the first N items.
///     - @ref TakeBackWhile - Copy from the back until the predicate returns false.
///     - @ref TakeWhile - Copy from the beginning until the predicate returns false in reverse order.
///     - @ref
///
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
/// @subsection FuncionalToolsDrains Funtional Drains
///
/// Drain is a term invented by this library to discuss functor that are likely to end pipes. This functor behave like
/// the rest of the functors but in isolation would seem preposterously simple. These are often a simple member function
/// call on the container or a similar arrangement of parameters that would be simple to invoke alone. The exist to
/// round out the API that the @ref Pipe and @ref FuncionalToolsFunctors provide and may be less useful alone.
/// @n @n
/// Like the @ref FuncionalToolsFunctors these try to follow a similar pattern but cannot do so perfectly because of
/// implementation details. All of these return a single value (or pair or optional of the data) and not a normal
/// container of data. Many, but not all of these support automatic currying. Any that accept a predicate do support it,
/// but some, like size, do not. If called without data it returns a version of itself doesn't support auto-currying.
/// Many of these do not support declaring the return type because is it implied by the operation performa and there is
/// no expenseive copying operating that can be co-opted to change the container.
///
/// Here is a list of supported for getting a single value and potentially draining a @ref Pipe:
///
///     - @ref Accumulate - Get a single value by Folding over the container.
///     - @ref AllOf - Do all of the items match some predicate?
///     - @ref AnyOf - Do any of the data match a predicate?
///     - @ref Collect - An alias for @ref Accumulate .
///     - @ref Contains - Is there an item that matches the predicate?
///     - @ref LeftFold - An alias for @ref Accumulate .
///     - @ref Includes - An alias for @ref Contains .
///     - @ref IsPartitioned - Do all the true items come before false items according to a predicated?
///     - @ref IsSorted - Is the container sorted according to some predicate?
///     - @ref Length - An alias for @ref Size .
///     - @ref Max - Get the largest value in passed container.
///     - @ref Min - Get the minimum value from container
///     - @ref MinMax - In a single pass get the minimum and maximum values.
///     - @ref NoneOf - Do none of the data match a predicate?
///     - @ref Reduce - An alias for @ref Accumulate .
///     - @ref Size - Get the size of the input data.
///

///
/// @subsection FuncionalToolsExtras Functional Extras
/// Currying - ✓
/// CreateRange - ✓
/// @n @n
/// @subsection FunctionalToolsPipeFunctorDrainPattern The Pipe >> Functor > Drain pattern
/// A chain of complex





////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Pipe drains are functions that get the value out of the pipe. The must implemented a "Drain" member function
    // which returns a functor. That functor accepts the final value of the pipe and returns some answer.



} //Mezzanine

#endif // Include guard
