# © Copyright 2010 - 2021 BlackTopp Studios Inc.
# This file is part of The Mezzanine Engine.
#
#    The Mezzanine Engine is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    The Mezzanine Engine is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
#
#   The original authors have included a copy of the license specified above in the
#   'Docs' folder. See 'gpl.txt'
#
#   We welcome the use of the Mezzanine engine to anyone, including companies who wish to
#   Build professional software and charge for their product.
#
#   However there are some practical restrictions, so if your project involves
#   any of the following you should contact us and we will try to work something
#   out:
#    - DRM or Copy Protection of any kind(except Copyrights)
#    - Software Patents You Do Not Wish to Freely License
#    - Any Kind of Linking to Non-GPL licensed Works
#    - Are Currently In Violation of Another Copyright Holder's GPL License
#    - If You want to change our code and not add a few hundred MB of stuff to
#        your distribution
#
#   These and other limitations could cause serious legal problems if you ignore
#   them, so it is best to simply contact us or the Free Software Foundation, if
#   you have any questions.
#
#   Joseph Toppi - toppij@gmail.com
#   John Blackwood - makoenergy02@gmail.com

########################################################################################################################
cmake_minimum_required(VERSION 3.0)
project(Foundation)
message(STATUS "${PROJECT_NAME} - Starting Configuration.")

############################################################################|###########################################
# Download, Verify and Setup the Jagati
if(NOT JAGATI_File)
    set(JAGATI_File "${${PROJECT_NAME}_BINARY_DIR}/Jagati.cmake" CACHE FILEPATH
        "The file to load the Jagati from and potentially to download it to.")
endif(NOT JAGATI_File)
if(NOT JAGATI_Download)
    option(JAGATI_Download "Should the Jagati be downloaded automatically" ON)
endif(NOT JAGATI_Download)
if(JAGATI_Download)
    set(JAGATI_Checksum "6aa2eb83eca66c9a06519a1138e4d72b7a6243da13026020eb0\
644ffd4c89e566e9716cc16a9fcee15651b65a517fbc61d4002b2f9e0883143a54c6684aabcb4"
        CACHE STRING "Check that when the Jagati is downloaded the right one is used (for consistency and security).")
    set(JAGATI_Url "https://raw.githubusercontent.com/BlackToppStudios/Jagati/0.30.2/Jagati.cmake"
        CACHE STRING "Where to download the Jagati from.")
    file(DOWNLOAD "${JAGATI_Url}" "${JAGATI_File}" EXPECTED_HASH SHA512=${JAGATI_Checksum})
endif(JAGATI_Download)
include("${JAGATI_File}")

########################################################################################################################
# The rest of the Jagati setup
StandardJagatiSetup()
IncludeJagatiPackage("StaticFoundation")
IncludeJagatiPackage("Test")

########################################################################################################################
# Setup Exception Sources

# This if is for upgrading from the version of the Jagati without this to a version that has it.
if(COMMAND IntializeExceptions)
    IntializeExceptions()
endif(COMMAND IntializeExceptions)

# This uses as a goal and description design for creating nest hierarchies of exceptions. It seems more desirable to
# catch all Container or all IO errors rather than all Index or Range errors? Following this thought prcoess Argument,
# Index, Range, and Name errors can be handled as subclasses of the more important system level categorization.

# I have added some remarks for possible entries

# Parent Exceptions
# Audio
AddJagatiException("Container" "Base" "Base Exception for containers.")
# Database
AddJagatiException("Encoding" "Base" "Base Exception for converting data from one form to another")
# Graphics
AddJagatiException("InputOutput" "Base" "Base Exception for all IO.")
AddJagatiException("Introspection" "Base" "Base Exception for everyhing related to the introspection system.")
# TODO: When multiple inheritance is supported add std::logic_error
AddJagatiException("LogicError" "Base" "Base Exception for exceptions that almost certainly indicate bugs")
# Math
# Physics
# Security
# Serialization
# Scripting
# Threading
# Unknown - What could cause us to throw and not Know?
# User Interface

##########################################################
# Container Exceptions
# TODO: When multiple inheritance is supported add std::out_of_range
AddJagatiException("OutOfRange" "Container" "When attempting to access an item not in a container.")
# TODO: When multiple inheritance is supported add std::length_error
AddJagatiException("InvalidLength" "Container" "When trying to make containers of zero or negative size.")
AddJagatiException("CapacityConsumed" "Container" "Not enough space or couldn't allocate enough memory.")
AddJagatiException("BadCast" "Container" "Bad conversion while storing or retrieving contained items.")

##########################################################
# Encoding/Conversion

# Encoding Base64
# TODO: When multiple inheritance is supported add std::invalid_argument
AddJagatiException("Base64" "Encoding" "A problem with encoding or decoding Base64.")
AddJagatiException("Base64Source" "Base64" "A Base64 function called with an invalid input buffer.")
AddJagatiException("Base64Destination" "Base64" "A Base64 function called with invalid output buffer.")
# TODO: When multiple inheritance is supported add std::range_error
AddJagatiException("Base64BadChar" "Base64Source" "A Base64 function called with bogus characters.")

# Conversion
AddJagatiException("HexConversion" "Encoding" "A problem convert to or from hex strings.")

##########################################################
# InputOutput

# File
    # FileNotFound
# Network
    # DNS

##########################################################
# Introspection
AddJagatiException("IntrospectionNullptr" "Introspection" "Base Exception for everyhing related.")


##########################################################
# Post Exception Cleanup.
EmitExceptionSource()
AddExceptionSource()

########################################################################################################################
# Normal Source files
message(STATUS "Determining Source Files.")

AddHeaderFile("Base64.h")
AddHeaderFile("BinaryBuffer.h")
AddHeaderFile("BinaryFind.h")
AddHeaderFile("BitFieldTools.h")
AddHeaderFile("ContainerTools.h")
AddHeaderFile("CountedPtr.h")
AddHeaderFile("CommandLine.h")
AddHeaderFile("FlatMap.h")
AddHeaderFile("Introspection.h")
AddHeaderFile("ManagedArray.h")
AddHeaderFile("MurmurHash.h")
AddHeaderFile("SortedManagedArray.h")
AddHeaderFile("SortedVector.h")
AddHeaderFile("StaticAny.h")
AddHeaderFile("StreamLogging.h")
AddHeaderFile("StringTools.h")
AddHeaderFile("TupleTools.h")
AddHeaderFile("Version.h")
ShowList("Header Files:" "\t" "${FoundationHeaderFiles}")

AddSourceFile("Base64.cpp")
AddSourceFile("BinaryBuffer.cpp")
AddSourceFile("CommandLine.cpp")
AddSourceFile("MurmurHash.cpp")
AddSourceFile("StreamLogging.cpp")
AddSourceFile("StringTools.cpp")
ShowList("Source Files:" "\t" "${FoundationSourceFiles}")

#AddJagatiDoxInput("Dox.h") # Little d
ShowList("Doxygen Input Files:" "\t" "${FoundationDoxFiles}")

########################################################################################################################
# Basic Targets and Test declaration
AddJagatiLibrary()
CreateCoverageTarget(${FoundationLib} "${FoundationSourceFiles}")

AddTestFile("Base64Benchmarks.h")
AddTestFile("Base64Tests.h")
AddTestFile("BinaryBufferTests.h")
AddTestFile("BitFieldToolsTests.h")
AddTestFile("ContainerToolsTests.h")
AddTestFile("CountedPtrBenchmarks.h")
AddTestFile("CountedPtrTests.h")
AddTestFile("CommandLineTests.h")
AddTestFile("ExceptionTests.h")
AddTestFile("FlatMapTests.h")
AddTestFile("IntrospectionTests.h")
AddTestFile("ManagedArrayTests.h")
AddTestFile("ManagedArraySequenceTests.h")
AddTestFile("MurmurHashTests.h")
AddTestFile("SortedManagedArrayTests.h")
AddTestFile("SortedVectorTests.h")
AddTestFile("StaticAnyTests.h")
AddTestFile("StreamLoggingTests.h")
AddTestFile("StringToolsTests.h")
AddTestFile("TupleToolsTests.h")
EmitTestCode()
AddTestTarget()

SetCodeCoverage()
AddIDEVisibility("Jenkinsfile")
message(STATUS "${PROJECT_NAME} - Ending Configuration.")
