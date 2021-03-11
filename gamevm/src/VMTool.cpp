#include <VMTool.h>
#include <fstream>
std::string ReadFile ( std::string fileName )
{
    std::ifstream ifs ( fileName );
    std::string s = std::string ( std::istreambuf_iterator<char> ( ifs ),
                                  std::istreambuf_iterator<char> () );
    return s;
}


int WriteFile ( std::string fileName, std::string content )
{
    std::ofstream newfile;
    newfile.open ( fileName );
    if ( !newfile.is_open () )
    {
        return 1;
    }
    try
    {

        newfile << content;
    }
    catch ( const std::exception & )
    {
        return 1;
    }
    newfile.close ();
    return 0;
}


Core::Game * ConstructGame ( std::string fileName )
{
    std::string s ( ReadFile ( fileName ) );
    json j = json::parse ( s );
    Core::Game * g = new Core::Game (j["GameName"]);
    return g;
}


int ProduceDataFile ( Core::Game *g )
{
    json j;
    j["GameName"] = g->GetGameName ().c_str ();
    try
    {
        WriteFile ( g->GetGameName ().append ( ".gdata" ), j.dump () );
    }
    catch ( const std::exception & )
    {
        return 1;
    }
    return 0;
}

