#include <unordered_map>
#include <string>
#include "Page.h"

namespac
{
    class PageManager
    {
    private:
        /// \brief the in dsiplay page list, use iterator to traverse
        std::unordered_map<std::string, Page *> mdisplayList;

        // contains all pages
        // use unordered_map for editor to easily add/delete pages without
        // affecting other pages
        std::unordered_map<std::string, Page *> mpageList;

    public:
        Page *addPage(Page *p)
        {
            pageList->push_back(p);
            return p;
        }
        Page *createPage(std::string n)
        {
            Page *p = new Page(n);
            return addPage(p);
        }
        MapPage *createMapPage()
        {
            MapPage *mp = new MapPage();
            return (MapPage *)addPage(mp);
        }
        MapPage *createMapPage(std::string n)
        {
            MapPage *mp = new MapPage(n);
            return (MapPage *)addPage(mp);
        }
        MapPage *createMapPage(std::string n, Map *m)
        {
            MapPage *mp = new MapPage(n, m);
            return (MapPage *)addPage(mp);
        }

        void deletePage(Page *dp)
        {
            // TODO Use something like: std::vector<Page*>::iterator ptr =
            // find(pageList.begin(), pageList.end(), dp); if (ptr !=
            // pageList.end())
            for (std::vector<Page *>::iterator ptr = pageList->begin();
                 ptr != pageList->end();
                 ptr++)
            {
                if (*ptr == dp)
                {
                    pageList->erase(ptr);
                    delete (dp);
                    dp = nullptr;
                    break;
                }
            }
        }

        void deletePage(std::string s)
        {
            for (std::vector<Page *>::iterator ptr = pageList->begin();
                 ptr != pageList->end();
                 ptr++)
            {
                if (!(*ptr)->getName().compare(s))
                {
                    Page *p = *ptr;
                    pageList->erase(ptr);
                    delete (p);
                    p = nullptr;
                    break;
                }
            }
        }

        void deleteSprite(int id)
        {
            spriteManager->deleteSprite(id);
        }

        // set display page list
        void setCurrentPage(std::unordered_map<int, Page *> displayList)
        {
            mdisplayList = displayList;
        }
        void addPageToDisplay(std::pair<int, Page *> page)
        {
            mdisplayList.insert(page);
        }
        void removePageFromDisplay(int pageID) { mdisplayList.erase(pageID); }
        PageManager() {}
        ~PageManager() {}
    };
}