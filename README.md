# Simple C++ onshape STL export

A minimal implementation of an onshape STL exporter written in c++. It supports two options for onshape authorization.

# Requirements
* Windows 10 or 11
* Visual Studio 2022 (others might work)

# Installation
* Install https://premake.github.io/
* Install https://github.com/microsoft/vcpkg
* vcpkg install cpp-httplib:x64-windows
* vcpkg install cppcodec:x64-windows
* vcpkg install curlpp:x64-windows
* vcpkg install nlohmann-json:x64-windows
* Change the vckgdir variable at the top of premake5.lua to match your vcpkg directory.
* In the directory where premake5.lua is located run: premake5 vs2022
* Open vs2022/vertualfs.sln in Visual Studio 2022
* F5 to build and run.

