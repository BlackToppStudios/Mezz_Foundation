# © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
set(JagatiChecksum "1168e9aa1ea49d5bc863618153fb162e41f2cb02f6bddfe1da6793\
4dccb44a2dc8d4f3a633786b16302adead50f08d316eceee34e875da5144f59184081c25f5")

file(DOWNLOAD
    "https://raw.githubusercontent.com/BlackToppStudios/Jagati/0.5.2/Jagati.cmake"
    "${${PROJECT_NAME}_BINARY_DIR}/Jagati.cmake"
    EXPECTED_HASH SHA512=${JagatiChecksum}
)

include("${${PROJECT_NAME}_BINARY_DIR}/Jagati.cmake")

StandardJagatiSetup()

IncludeJagatiPackage("StaticFoundation")
IncludeJagatiPackage("Test")

########################################################################################################################
message(STATUS "Determining Source Files.")
set(FoundationHeaderFiles
    "${FoundationIncludeDir}asdf.h"
)
ShowList("Source Files:" "\t" "${FoundationHeaderFiles}")

set(FoundationSourceFiles
    "${FoundationSourceDir}asdf.cpp"
)
ShowList("Source Files:" "\t" "${FoundationSourceFiles}")

set(DoxFiles
    "${TestDoxDir}dox.h"
)
ShowList("Doxygen Input Files:" "\t" "${DoxFiles}")
AddJagatiDoxInput("${DoxFiles}")

########################################################################################################################
# This setup is unusual but could be useful.

include_directories(
    ${${PROJECT_NAME}IncludeDir}
    ${FoundationIncludeDir}
    ${StaticFoundationIncludeDir}
    ${StaticFoundationGenHeadersDir}
)

# A normal project would put declaration of targets here, a really good project would house the bulk of its
# functionality in a library so that it could easily be linked to the testing library and the final executable.
#add_library(${YourLibraryHere} ${MEZZ_LibraryBuildType} ${YourHeaders} ${YourSources})

AddJagatiLibrary("Foundation")

# This has only one test and could be added as follows.
AddTest("vector3tests")

# Spit out the C++ file
EmitTestCode()
AddTestTarget("")

# You need to link the test library to your stuff to be tested.
#target_link_libraries(${TestLib} ${YourLibraryHere})



########################################################################################################################
# Set up Build targets



# A Project to do nothing but get All the Source files visible in IDEs
add_custom_target(
    ${PROJECT_NAME}_IDE_Visibility
    DEPENDS ${PROJECT_NAME}_Tester
            "${${PROJECT_NAME}BinaryDir}Guidelines.md"
            ExternalProjectDownload
    SOURCES ${FoundationHeaderFiles}
            ${FoundationSourceFiles}
            ${TestSourceFiles}
            ${SwigFiles}
            ${StaticFoundationConfigFilename}
            README.md
            COPYING.md
            .travis.yml
            appveyor.yml
            "${DoxFiles}"
)

message(STATUS "${PROJECT_NAME} - Ending Configuration.")
