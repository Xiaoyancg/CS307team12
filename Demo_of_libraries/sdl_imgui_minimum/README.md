How to compile in VS2019
preparations:
1. download glad from https://glad.dav1d.de/ ( in the page select API-gl- Version 4.6. No need to touch other options ). unzip it under ..\dependency
2. download imgui from https://codeload.github.com/ocornut/imgui/zip/master. unzip it under ..\denpendency
3. download SDL2 from https://libsdl.org/release/SDL2-devel-2.0.14-VC.zip. unzip it under ..\denpendency
4. copy the include folder in glad and SDL2 to dependency\ . Then copy all .h files EXCEPT the imconfig.h in imgui-master\ and imgui-master\backends to dependency\include
what you will get should be like mine https://ibb.co/2vNXwJz

////////////////////////
Don't overwrite the imconfig.h file!
Here's no damage, but we may need to write some functions in there. 
it is made for developers to include functions in imgui namesapce
///////////////////

5. copy src folder in glad to denpendency\ folder. Copy all .cpp file in imgui-master\ and imgui-master\backends to dependency\src folder.
6. copy files in \SDL2-2.0.14\lib\x64 to dependency\lib folder. dll is dynamic library, we'll copy it to the executable later.

Compile: 
1. open sdl_imgui_minimum.sln in vs2019.
2. check the project setting by right clicking on the project name sdl_imgui_minimum on the solution explorer on the right side and select properties at the bottom.
3. see if your vc++ direction setting is like this https://ibb.co/cbtvCHm .
4. Then check libraries names under linker\input https://ibb.co/0jLdvy6 
5. change the Platforms to x64 if it's in 32 bit. 
6. check if there's cpp file include in the solution explorer. if not please inlcude them as existing files. https://ibb.co/6vFCKY3
7. Local Windows Debugger go. https://ibb.co/D56MQfc

All locations in the project configuration are in relative type.
( Using macro: "$(ProjectDir)\..\dependency\)" )

5 things to notice:
  1. include all imgui*.cpp and header 
  2. change platform to 64
  3. add include and library directory
  4. add lib name in linker
  5. add dll to the executable file folder
  6. add .cpp file and glad.c to the solution.