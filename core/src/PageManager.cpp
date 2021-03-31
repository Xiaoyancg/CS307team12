#include "PageManager.h"
namespace Core
{
    int PageManager::addPage(Page &page_ref)
    {
        if (mpages.find(page_ref.getName()) != mpages.end())
        {
            // 1 for used name
            return 1;
        }
        else
        {
            mpages[page_ref.getName()] = page_ref;
            // 0 for success
            return 0;
        }
    }

    int PageManager::deletePage(std::string &pageName_ref)
    {
        try
        {
            //delete mpages.at(pageName_ref);
            mpages.erase(pageName_ref);
            return 0;
        }
        catch (const std::exception &e)
        {
            return 1;
        }
    }

    int PageManager::addPageToCurrPages(std::string &pageName_ref)
    {
        try
        {
            mcurrPages.insert(
                std::pair<std::string, Page *>(pageName_ref,
                                               &mpages.at(pageName_ref)));
            return 0;
        }
        catch (const std::exception &e)
        {
            return 1;
        }
    }

    int PageManager::removePageFromCurrPages(std::string &pageName_ref)
    {
        try
        {
            mcurrPages.erase(pageName_ref);
            return 0;
        }
        catch (const std::exception &e)
        {
            return 1;
        }
    }
}