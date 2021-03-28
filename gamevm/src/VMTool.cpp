#include <VMTool.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <ctime>

std::string getTimeString()
{
    time_t rawtime;
    struct tm *timeinfo = new struct tm();
    time(&rawtime);
    localtime_s(timeinfo, &rawtime);
    char c[256];
    asctime_s(c, 256, timeinfo);
    return std::string(c);
}
std::vector<std::string> searchAllFileWithExtension(std::string location, std::string extension)
{
    std::vector<std::string> fileList;
    for (auto &p : std::filesystem::recursive_directory_iterator(location))
    {
        // if the extension matches
        if (p.path().extension().compare(extension) == 0)
        {
            // add to file list
            fileList.push_back(p.path().filename().string());
        }
    }
    return fileList;
}
std::string searchFileWithExtension(std::string location, std::string extension)
{
    // the target extension
    //std::string ext(".gdata");
    // search every file in that location
    for (auto &p : std::filesystem::recursive_directory_iterator(location))
    {
        // if the extension matches
        if (p.path().extension().compare(extension) == 0)
        {

            // return the target file name
            return p.path().string();
        }
    }
    // if not found returns empty file name
    return std::string();
}

std::string ReadFile(std::string fileName)
{
    std::ifstream ifs(fileName);
    std::string s = std::string(std::istreambuf_iterator<char>(ifs),
                                std::istreambuf_iterator<char>());
    return s;
}

void removeGame(std::string n)
{
    remove(n.c_str());
}

json *readGameDataFile(std::string f)
{
    json *s = new json();
    std::string ctt(ReadFile(f));
    *s = json::parse(ctt);
    return s;
}

Core::Game *CreateExampleGame()
{
    Core::Game *g = new Core::Game(std::string("example"));
    g->setAuthor(std::string("example mauthor"));
    g->setVersion(std::string("0.1.0"));
    time_t rawtime;
    struct tm *timeinfo = new struct tm();
    time(&rawtime);
    localtime_s(timeinfo, &rawtime);
    char c[256];
    asctime_s(c, 256, timeinfo);
    g->setLMTime(std::string(c));
    g->setNote(std::string("example Note"));
    // TODO page
    return g;
}

int WriteFile(std::string fileName, std::string content)
{
    std::ofstream newfile;
    newfile.open(fileName);
    if (!newfile.is_open())
    {
        return 1;
    }
    try
    {

        newfile << content;
    }
    catch (const std::exception &)
    {
        return 1;
    }
    newfile.close();
    return 0;
}

Core::Game *ConstructGame(std::string fileName)
{
    std::string s(ReadFile(fileName));
    json j = json::parse(s);
    Core::Game *g = new Core::Game(j["GameName"]);
    g->setAuthor(j["Author"]);
    g->setLMTime(j["LastModifiedTime"]);
    g->setNote(j["Note"]);
    g->setVersion(j["Version"]);
    return g;
}

int ProduceDataFile(Core::Game *g)
{
    json j;
    j["FileType"] = "Parchment Game Data";
    j["GameName"] = g->getGameName();
    j["Author"] = g->getAuthor();
    j["LastModifiedTime"] = g->getLMTime();
    j["Note"] = g->getNote();
    j["Version"] = g->getVersion();
    try
    {
        WriteFile(g->getGameName().append(".gdata"), j.dump());
    }
    catch (const std::exception &)
    {
        return 1;
    }
    return 0;
}
