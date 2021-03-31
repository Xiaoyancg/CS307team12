#include "PageManager.h"
namespace Core
{
    int PageManager::addPage(Page *page)
    {
        if (mpages[page->getName()] == nullptr)
        {
            mpages[page->getName()] = page;
            // 0 for success
            return 0;
        }
        else
        {
            // 1 for used name
            return 1;
        }
    }

    int PageManager::deletePage(std::string &pageName_ref)
    {
        try
        {
            delete mpages.at(pageName_ref);
            mpages.erase(pageName_ref);
            return 0;
        }
        catch (const std::exception &e)
        {
            return 1;
        }
    }

    int PageManager::addPageToDisplay(std::string &pageName_ref)
    {
        try
        {
            mdisplayList.insert(
                std::pair<std::string, Page *>(pageName_ref,
                                               mpages.at(pageName_ref)));
            return 0;
        }
        catch (const std::exception &e)
        {
            return 1;
        }
    }

    int PageManager::removePageFromDisplay(std::string &pageName_ref)
    {
        try
        {
            delete mdisplayList.at(pageName_ref);
            mdisplayList.erase(pageName_ref);
            return 0;
        }
        catch (const std::exception &e)
        {
            return 1;
        }
    }
}