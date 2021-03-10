#include <VMTool.h>
#include <fstream>
std::string ReadFile ( std::string fileName )
{
    std::ifstream ifs ( fileName );
    std::string s = std::string ( std::istreambuf_iterator<char> ( ifs ),
                                  std::istreambuf_iterator<char> () );
    return s;
}
