/**
 * @file    LefDefParser.cpp
 * @author  Jinwook Jung (jinwookjung@kaist.ac.kr)
 * @date    2018-10-16 12:31:28
 *
 * Created on Tue Oct 16 12:31:28 2018.
 */

#include "LefDefParser.h"
#include "StringUtil.h"
#include "Watch.h"
#include "Logger.h"

namespace my_lefdef
{

/**
 * Default ctor.
 */
LefDefParser::LefDefParser () : lef_(lef::Lef::get_instance()),
                                def_(def::Def::get_instance())
{
    //
}

/**
 * Returns the singleton object of the detailed router.
 */
LefDefParser& LefDefParser::get_instance ()
{
    static LefDefParser ldp;
    return ldp;
}

/**
 * Read a LEF file @a filename.
 */
void LefDefParser::read_lef (string filename)
{
    lef_.read_lef(filename);
    lef_.report();
}

/**
 * Read a DEF file @a filename.
 */
void LefDefParser::read_def (string filename)
{
    def_.read_def(filename);
    def_.report();
}

/**
 *
 */
void LefDefParser::write_bookshelf (string filename) const
{
    write_bookshelf_nodes (filename + ".nodes");

    // nets
    // wts
    // scl
    // pl
    // aux
}

/**
 *
 */
void LefDefParser::write_bookshelf_nodes (string filename) const
{
    auto& component_umap = def_.get_component_umap();
    for (auto& c : component_umap) {
        cout << "Component: " << c.first;

        // Get width and height
        auto lef = c.second->lef_macro_;
        auto site = lef->site_;
        auto w = site->x_, h = site->y_;

        cout << "\t" << w << "\t" << h << endl;
    }
}

}
