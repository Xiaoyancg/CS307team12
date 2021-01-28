How to compile in VS2019
1. open project in vs2019
2. use Makefile to download required dependency into ../../dependency

All locations in the project configuration are in relative type.
( Using macro: "$(ProjectDir)\..\dependency\)" )

5 things to notice:
  1. include all imgui.cpp and header 
  2. change platform to 64
  3. add include and library directory
  4. add lib name in linker
  5. add dll to the executable file folder