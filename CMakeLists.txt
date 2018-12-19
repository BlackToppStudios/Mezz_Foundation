# © Copyright 2010 - 2018 BlackTopp Studios Inc.
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
    set(JAGATI_Checksum "5417802bf9150088c086df70453e091b060b6d03edf005f90dd\
a39b504906f76bffa0dbcc02248597e536beceed7dc90c8c582aae9f4a98447efdf8d609b23c0"
        CACHE STRING "Check that when the Jagati is downloaded the right one is used (for consistency and security).")
    set(JAGATI_Url "https://raw.githubusercontent.com/BlackToppStudios/Jagati/0.25.1/Jagati.cmake"
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
# Source files
message(STATUS "Determining Source Files.")

AddHeaderFile("Base64.h")
AddHeaderFile("BinaryBuffer.h")
AddHeaderFile("BinaryFind.h")
AddHeaderFile("BitFieldTools.h")
AddHeaderFile("FlatMap.h")
AddHeaderFile("ManagedArray.h")
AddHeaderFile("SortedManagedArray.h")
AddHeaderFile("SortedVector.h")
AddHeaderFile("StaticAny.h")
AddHeaderFile("StreamLogging.h")
AddHeaderFile("Version.h")
ShowList("Header Files:" "\t" "${FoundationHeaderFiles}")

AddSourceFile("Base64.cpp")
AddSourceFile("BinaryBuffer.cpp")
AddSourceFile("StreamLogging.cpp")
ShowList("Source Files:" "\t" "${FoundationSourceFiles}")

#AddJagatiDoxInput("Dox.h") # Little d
ShowList("Doxygen Input Files:" "\t" "${FoundationDoxFiles}")

########################################################################################################################
# Basic Targets and Test declaration
AddJagatiLibrary()
CreateCoverageTarget(${FoundationLib} "${FoundationSourceFiles}")

AddTestFile("Base64Tests.h")
AddTestFile("BinaryBufferTests.h")
AddTestFile("BitFieldToolsTests.h")
AddTestFile("FlatMapTests.h")
AddTestFile("ManagedArrayTests.h")
AddTestFile("SortedManagedArrayTests.h")
AddTestFile("SortedVectorTests.h")
AddTestFile("StaticAnyTests.h")
AddTestFile("StreamLoggingTests.h")
EmitTestCode()
AddTestTarget()

SetCodeCoverage()
AddIDEVisibility("Jenkinsfile")
message(STATUS "${PROJECT_NAME} - Ending Configuration.")
