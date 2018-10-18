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

    // pl
    cout << "Writing bookshelf pl file." << endl;

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

    ofs << "NumNodes : " << component_umap.size() + pin_umap.size() << endl;
    ofs << "NumTerminals: " << pin_umap.size() << endl;

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
        auto w = lround(macro->size_x_ / site->x_);
        auto h = lround(macro->size_y_ / site->y_);

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
    ofs << "UCLA nodes 1.0" << endl;
    ofs << "# Created : ";
    ofs << get_current_time_stamp() << endl << endl;

    auto& net_umap = def_.get_net_umap();
    auto num_connections = 0;

    for (auto& n : net_umap) {
        auto net = n.second;
        num_connections += net->connections_.size();
    }

    // Count the number of pins
    ofs << "NumPins : " << num_connections << endl;

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
                name = c->component_->name_ + "/" + c->name_;
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



}
