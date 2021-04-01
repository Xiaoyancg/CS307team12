#include <unordered_map>
#include <string>
#include "Page.h"
#include "MapPage.h"

namespace Core
{
    class PageManager
    {
    private:
        /// \brief the in dsiplay page list, use iterator to traverse
        std::unordered_map<std::string, Page *> mcurrPages;

        // contains all pages
        // use unordered_map for editor to easily add/delete pages without
        // affecting other pages
        // use instance because this is the base storage of pages, no need to
        // use pointer
        std::unordered_map<std::string, Page> mpages;
        int mwidth, mheight;

    public:
        /// \brief add the page pointer to list
        /// \param page
        /// \return int 0 for success, 1 for used name
        int addPage(Page *page);

        /// \brief Create a Page object by name and add to page list
        /// \param pageName
        /// \return int 0 for success, 1 for used name
        int createPage(std::string &pageName_ref)
        {
            return addPage(new Page(pageName_ref));
        }

        int createMapPage(std::string &pageName_ref, SpriteManager *s)
        {
            return addPage(new MapPage(pageName_ref, s));
        }
        int createMapPage(std::string n, Map *m, SpriteManager *s)
        {
            return addPage(new MapPage(n, m,s));
        }

        int deletePage(Page *page_ptr)
        {
            return deletePage(page_ptr->getName());
        }

        /// \brief Get the Page object pointer
        ///
        /// \param pageName
        /// \return Page*
        Page *getPage(std::string &pageName)
        {
            try
            {
                return &mpages.at(pageName);
            }
            catch (const std::exception &e)
            {
                return nullptr;
            }
        }

        /// \brief delete page by name
        /// \param pageName_ref
        /// \return int 0 success, 1 not found
        int deletePage(std::string &pageName_ref);

        // set display page list
        void setCurrPages(
            std::unordered_map<std::string, Page *> &displayList)
        {
            mcurrPages = displayList;
        }

        /// \brief add page to display by name
        /// \param pageName_ref
        /// \return int 0 success, 1 not found
        int addPageToCurrPages(std::string &pageName_ref);

        /// \brief remove a page from displayList
        /// \param pageName_ref
        /// \return int 0 success, 1 not found
        int removePageFromCurrPages(std::string &pageName_ref);

        /// \brief Get the Pages list pointer
        /// \return std::unordered_map<std::string, Page>*
        /// \note use pointer to speed up and save memory
        std::unordered_map<std::string, Page> *getPages() { return &mpages; }

        /// \brief Get the Display List pointer
        /// \return std::unordered_map<std::string, Page *>*
        /// \note use pointer to speed up and save memory
        std::unordered_map<std::string, Page *> *getCurrPages()
        {
            return &mcurrPages;
        }

        /// \brief Construct a new Page Manager object
        /// \param width
        /// \param height
        PageManager(int &width, int &height) : mwidth(width), mheight(height) {}
        PageManager() {}
        ~PageManager()
        {
            ////            for (auto pagepair : mcurrPages)
            ////                delete pagepair.second;
            ////            for (auto pagepair : mpages)
            ////                delete pagepair.second;
        }
    };
}