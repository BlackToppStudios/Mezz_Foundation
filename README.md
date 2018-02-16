# README #

The Mezzanine is a set of multi platform C++ libraries designed to help create high performance
videos games. The Mezzanine is focused on games requiring 3d physics, 3d graphics, 3d sounds and
common facilities that such games need. This is just one component of many.

## About this Library ##

This library, the StaticFoundation, is responsible for detecting compilation options, compiler settings, doing work at
compile time (like string manipulation) and handling other platform specific details.

It does this by a mix a of use of preprocessor macros, constexpr C++11/14 and configuration time/CMake scripting
techniques.

## Build Status ##

The current Travis CI (Apple - GCC/Clang) build status is:
[![Build Status](https://travis-ci.org/BlackToppStudios/Mezz_StaticFoundation.svg?branch=master)](https://travis-ci.org/BlackToppStudios/Mezz_StaticFoundation)

The current Appveyor (Windows - MinGW/Msvc) build status is:
[![Build Status](https://ci.appveyor.com/api/projects/status/github/BlackToppStudios/Mezz_StaticFoundation?branch=master&svg=true)](https://ci.appveyor.com/project/Sqeaky/mezz-staticfoundation)

The current code coverage for tests is:
[![codecov](https://codecov.io/gh/BlackToppStudios/Mezz_StaticFoundation/branch/master/graph/badge.svg)](https://codecov.io/gh/BlackToppStudios/Mezz_StaticFoundation)

The current Jenkins, which covers Linux (Emscripten, Rasberry Pi, Ubuntu and Fedora), old Mac OS X (High Sierra) and old windows (7 64 bit msvc and mingw), build status is available on the [BTS Jenkins Instance](http://blacktopp.ddns.net:8080/blue/organizations/jenkins/Mezz_StaticFoundation/activity). The current status is: [![Build Status](http://blacktopp.ddns.net:8080/job/Mezz_StaticFoundation/job/master/badge/icon)](http://blacktopp.ddns.net:8080/blue/organizations/jenkins/Mezz_StaticFoundation/activity)
