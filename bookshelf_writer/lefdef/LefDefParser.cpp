/**
 * @file    LefDefParser.cpp
 * @author  Jinwook Jung (jinwookjung@kaist.ac.kr)
 * @date    2018-10-18 10:40:18
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
    cout << "Writing bookshelf nodes file." << endl;
    write_bookshelf_nodes (filename + ".nodes");

    // nets
    cout << "Writing bookshelf nets file." << endl;
    write_bookshelf_nets (filename + ".nets");

    // wts
    cout << "Writing bookshelf wts file." << endl;
    write_bookshelf_wts (filename + ".wts");

    // scl
    cout << "Writing bookshelf scl file." << endl;
    write_bookshelf_scl (filename + ".scl");

    // pl
    cout << "Writing bookshelf pl file." << endl;
    write_bookshelf_pl (filename + ".pl");

    // aux
    cout << "Writing bookshelf aux file." << endl;
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
void LefDefParser::write_bookshelf_nodes (string filename) const
{
    ofstream ofs(filename);
    ofs << "UCLA nodes 1.0" << endl;
    ofs << "# Created : ";
    ofs << get_current_time_stamp() << endl;

    auto& component_umap = def_.get_component_umap();
    auto& pin_umap = def_.get_pin_umap();

    const auto x_pitch = lef_.get_min_x_pitch();
    const auto y_pitch = lef_.get_min_y_pitch();

    ofs << "NumNodes : " << component_umap.size() + pin_umap.size() << endl;
    ofs << "NumTerminals : " << pin_umap.size() << endl;

    for (auto& p : pin_umap) {
        ofs << "\t" << std::setw(40) << std::left  << p.first;
        ofs << "\t" << std::setw(8)  << std::right << 1;
        ofs << "\t" << std::setw(8)  << std::right << 1;
        ofs << "\t" << "terminal" << endl;
    }

    for (auto& c : component_umap) {
        ofs << "\t" << std::setw(40) << std::left << c.first;

        // Get width and height
        auto macro = c.second->lef_macro_;
        auto site = macro->site_;
        auto w = lround(macro->size_x_ / x_pitch);
        auto h = lround(macro->size_y_ / y_pitch);

        ofs << "\t" << std::setw(8) << std::right << w;
        ofs << "\t" << std::setw(8) << std::right << h << endl;
    }
    ofs.close();
}

/**
 *
 */
void LefDefParser::write_bookshelf_nets (string filename) const
{
    ofstream ofs(filename);
    ofs << "UCLA nets 1.0" << endl;
    ofs << "# Created : ";
    ofs << get_current_time_stamp() << endl << endl;

    auto& net_umap = def_.get_net_umap();
    auto num_connections = 0;

    for (auto& n : net_umap) {
        auto net = n.second;
        num_connections += net->connections_.size();
    }

    // Count the number of pins
    ofs << "NumNets : " << net_umap.size() << endl;
    ofs << "NumPins : " << num_connections << endl << endl;

    for (auto& n : net_umap) {
        auto net = n.second;
        ofs << "NetDegree : " << std::setw(8) << std::right
            << net->connections_.size() << "\t" << n.first << endl;

        for (auto& c : net->connections_) {
            // Populate the name and the direction of the pin
            string name;
            PinDir direction;

            if (c->lef_pin_ == nullptr) {
                name = c->name_;
                direction = c->pin_->dir_;
            }
            else {
                name = c->component_->name_;
                direction = c->lef_pin_->dir_;
            }

            // Print out
            ofs << "\t" << std::setw(20) << std::left << name;
            if (direction == PinDir::input) {
                ofs << " I  :";
            }
            else if (direction == PinDir::output) {
                ofs << " O  :";
            }
            else {
                // FIXME
                ofs << " I  :";
            }

            // Offset
            ofs << " 0.5 0.5" << endl;
        }
    }

    ofs.close();
}

/**
 *
 */
void LefDefParser::write_bookshelf_wts (string filename) const
{
    ofstream ofs(filename);
    ofs << "UCLA wts 1.0" << endl;
    ofs << "# Created : ";
    ofs << get_current_time_stamp() << endl << endl;

    auto& net_umap = def_.get_net_umap();

    for (auto& n : net_umap) {
        ofs << std::setw(40) << std::left << n.first << "\t1" << endl;
    }

    ofs.close();
}

/**
 *
 */
void LefDefParser::write_bookshelf_scl (string filename) const
{
    ofstream ofs(filename);
    ofs << "UCLA scl 1.0" << endl;
    ofs << "# Created : ";
    ofs << get_current_time_stamp() << endl << endl;

    auto& rows = def_.get_rows();

    // Minimum Y- and X-track pitch
    auto x_pitch = lef_.get_min_x_pitch();
    auto y_pitch = lef_.get_min_y_pitch();
    auto x_pitch_dbu = lef_.get_min_x_pitch_dbu();
    auto y_pitch_dbu = lef_.get_min_y_pitch_dbu();

    ofs << "NumRows : " << rows.size() << endl << endl;

    for (auto& r : rows) {
        auto site = lef_.get_site(r->macro_);
        string sym_str;
        if (site->symmetry_ == SiteSymmetry::x) {
            sym_str = "X"; 
        }
        else if (site->symmetry_ == SiteSymmetry::y) {
            sym_str = "Y"; 
        }
        else if (site->symmetry_ == SiteSymmetry::r90) {
            sym_str = "R90"; 
        }
        else {
            // FIXME
            sym_str = "Y";
        }

        ofs << "CoreRow Horizontal" << endl;
        ofs << "\tCoordinate   : " << r->y_ / y_pitch_dbu << endl;
        ofs << "\tHeight       : " << site->y_ / y_pitch << endl;
        ofs << "\tSitewidth    : " << site->x_ / x_pitch << endl;
        ofs << "\tSitespacing  : " << r->step_x_ / x_pitch_dbu << endl;
        ofs << "\tSiteorient   : " << r->orient_str_ << endl;
        ofs << "\tSitesymmetry : " << sym_str << endl;

        ofs << "\tSubrowOrigin : " << r->x_ / x_pitch_dbu;
        ofs << "\tNumSites : " << r->num_x_ << endl;
        ofs << "End" << endl;
    }

    ofs.close();
}

/**
 *
 */
void LefDefParser::write_bookshelf_pl (string filename) const
{
    ofstream ofs(filename);
    ofs << "UCLA pl 1.0" << endl;
    ofs << "# Created : ";
    ofs << get_current_time_stamp() << endl << endl;

    auto& component_umap = def_.get_component_umap();
    auto& pin_umap = def_.get_pin_umap();

    auto x_pitch_dbu = lef_.get_min_x_pitch_dbu();
    auto y_pitch_dbu = lef_.get_min_y_pitch_dbu();

    for (auto it : component_umap) {
        auto c = it.second;
        ofs << std::setw(40) << std::left << c->name_;
        ofs << "\t" << c->x_ / x_pitch_dbu 
               << "\t" << c->y_ / y_pitch_dbu 
               << "\t: " << c->orient_str_ << endl;
    }

    for (auto& it : pin_umap) {
        auto p = it.second;
        ofs << std::setw(40) << std::left  << p->name_;
        ofs << "\t" << p->x_ / x_pitch_dbu
            << "\t" << p->y_ / y_pitch_dbu 
            << "\t: " << p->orient_str_ << endl;
    }



    ofs.close();
}

/**
 *
 */
void LefDefParser::update_def (string bookshelf_pl)
{
    // Read bookshelf placement file.

}

}
