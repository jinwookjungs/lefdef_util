/**
 * @file    def_to_boxes.cpp
 * @author  Jinwook Jung (jinwookjung@ibm.com)
 * @date    2019-08-12 23:41:04
 *
 * Created on Mon 12 Aug 2019 11:28:32 PM EDT.
 */

#include "DefToBoxes.h"
#include "StringUtil.h"
#include "Watch.h"
#include "Logger.h"

namespace my_lefdef
{

/**
 * Default ctor.
 */
DefToBoxes::DefToBoxes () : lef_(lef::Lef::get_instance()),
                                def_(def::Def::get_instance())
{
    //
}


/**
 * Returns the singleton object of the detailed router.
 */
DefToBoxes& DefToBoxes::get_instance ()
{
    static DefToBoxes dtb;
    return dtb;
}


/**
 * Read a LEF file @a filename.
 */
void DefToBoxes::read_lef (string filename)
{
    lef_.read_lef(filename);
    lef_.report();
}

/**
 *
 */
void DefToBoxes::read_def (string filename) 
{
    def_.read_def(filename);
    def_.report();
}

/**
 *
 */
static string get_current_time_stamp ()
{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    ostringstream oss;
    oss << std::put_time(&tm, "%m/%d/%Y %H:%M:%S");

    return oss.str();
}

/**
 *
 */
void DefToBoxes::write_boxes (string filename) const
{
    ofstream ofs(filename);
    ofs << "# Created on: ";
    ofs << get_current_time_stamp() << endl;

    auto& component_umap = def_.get_component_umap();
    auto& pin_umap = def_.get_pin_umap();

    auto num_terminals = pin_umap.size();
    for (auto c : component_umap) {
        if (c.second->is_fixed_) {
            num_terminals++;
        }
    }

    ofs << "# Total Instances: " << component_umap.size() + pin_umap.size() << endl;
    ofs << "# Total Terminals: " << num_terminals << endl << endl;

    for (auto p : pin_umap) {
        auto x = (double)p.second->x_ / def_.get_dbu();
        auto y = (double)p.second->y_ / def_.get_dbu();

        ofs << std::setw(15) << std::left << "terminal";
        ofs << std::setw(15) << std::left << p.first;
        ofs << "\t" << std::setw(12) << std::right << x;
        ofs << "\t" << std::setw(12) << std::right << y;
        ofs << "\t" << std::setw(12) << std::right << 1;
        ofs << "\t" << std::setw(12) << std::right << 1 << endl;
    }

    for (auto c : component_umap) {
        auto x = (double)c.second->x_ / def_.get_dbu();
        auto y = (double)c.second->y_ / def_.get_dbu();

        // Get width and height
        auto macro = c.second->lef_macro_;
        auto w = macro->size_x_;
        auto h = macro->size_y_;

        ofs << std::setw(15) << std::left << macro->name_;
        ofs << std::setw(15) << std::left << c.first;
        ofs << "\t" << std::setw(12) << std::right << x;
        ofs << "\t" << std::setw(12) << std::right << y;
        ofs << "\t" << std::setw(12) << std::right << w;
        ofs << "\t" << std::setw(12) << std::right << h;
        
        if (c.second->is_fixed_) {
            ofs << "\t" << "terminal";
        }
        ofs << endl;
    }
    ofs.close();
}


}
