#include <VMTool.h>
#include <fstream>
std::string ReadFile ( std::string fileName )
{
    std::ifstream ifs ( fileName );
    std::string s = std::string ( std::istreambuf_iterator<char> ( ifs ),
                                  std::istreambuf_iterator<char> () );
    return s;
}

Core::Game *CreateExampleGame ()
{
    Core::Game *g = new Core::Game ( std::string ( "example" ) );
    g->SetAuthor ( std::string ( "example author" ) );
    g->SetVersion ( std::string ( "0.1.0" ) );
    time_t rawtime;
    struct tm *timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    g->SetLMTime ( std::string ( asctime ( timeinfo ) ) );
    g->SetNote ( std::string ( "example Note" ) );
    // TODO page
    return g;
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
    g->SetAuthor(j["Author"]);
    g->SetLMTime ( j["LastModifiedTime"] );
    g->SetNote ( j["Note"] );
    g->SetVersion ( j["Version"] );
    return g;
}


int ProduceDataFile ( Core::Game *g )
{
    json j;
    j["FileType"] = "Parchment Game Data";
    j["GameName"] = g->GetGameName ();
    j["Author"] = g->GetAuthor ();
    j["LastModifiedTime"] = g->GetLMTime ();
    j["Note"] = g->GetNote ();
    j["Version"] = g->GetVersion ();
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

