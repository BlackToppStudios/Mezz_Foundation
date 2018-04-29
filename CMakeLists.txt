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

##########################################################################|#############################################
# Download, Verify and setup the Jagati
set(JagatiChecksum "d17080276fb7f2bfae1b7bfc1cb0812e8f9b44bf20ff2c2387fbee\
397b186643f7da522855ebb8e641ffacaeffc0deabb26688f350382e444f82074e751f512e")
file(DOWNLOAD
    "https://raw.githubusercontent.com/BlackToppStudios/Jagati/0.22.3/Jagati.cmake"
    "${${PROJECT_NAME}_BINARY_DIR}/Jagati.cmake"
    EXPECTED_HASH SHA512=${JagatiChecksum}
)

include("${${PROJECT_NAME}_BINARY_DIR}/Jagati.cmake")

StandardJagatiSetup()
IncludeJagatiPackage("StaticFoundation")
IncludeJagatiPackage("Test")

########################################################################################################################
# Source files
message(STATUS "Determining Source Files.")

AddHeaderFile("BitFieldTools.h")
AddHeaderFile("ManagedArray.h")
AddHeaderFile("StreamLogging.h")
AddHeaderFile("Version.h")
ShowList("Header Files:" "\t" "${FoundationHeaderFiles}")

AddSourceFile("StreamLogging.cpp")
ShowList("Source Files:" "\t" "${FoundationSourceFiles}")

#AddJagatiDoxInput("Dox.h") # Little d
ShowList("Doxygen Input Files:" "\t" "${FoundationDoxFiles}")

########################################################################################################################
# Basic Targets and Test declaration
AddJagatiLibrary()
CreateCoverageTarget(${FoundationLib} "${FoundationSourceFiles}")

AddTestFile("BitFieldToolsTests.h")
AddTestFile("ManagedArrayTests.h")
AddTestFile("StreamLoggingTests.h")
EmitTestCode()
AddTestTarget()

SetCodeCoverage()
AddIDEVisibility("Jenkinsfile")
message(STATUS "${PROJECT_NAME} - Ending Configuration.")
