/**
 * @file    main.cpp
 * @author  Jinwook Jung (jinwookjung@ibm.com)
 * @date    2019-08-12 23:33:03
 *
 * Created on Mon 12 Aug 2019 11:31:54 PM EDT.
 */

#include "Logger.h"
#include "Watch.h"
#include "ArgParser.h"
#include "DefToBoxes.h"

#include <iostream>

using namespace std;

void show_usage ();
void show_banner ();
void show_cmd_args ();

/**
 * Main.
 */
int main (int argc, char* argv[])
{
    util::Watch watch;

    // Parsing command line arguments
    auto& ap = ArgParser::get();

    ap.initialize(argc, argv);
    auto filename_lef = ap.get_argument("-lef");
    auto filename_def = ap.get_argument("-def");

    if (filename_lef == "" or filename_def == "") {
        show_usage();
        return -1;
    }

    show_banner();
    show_cmd_args();

    // Run detaile drouter
    auto& dtb = my_lefdef::DefToBoxes::get_instance();
    dtb.read_lef(filename_lef);
    dtb.read_def(filename_def);
    dtb.write_boxes("temp");

    cout << endl << "Done." << endl;

    return 0;
}

void show_usage ()
{
    cout << endl;
    cout << "Usage:" << endl;
    cout << "lefdef_parser -lef <lef> -def <def>" << endl << endl;
}

/**
 * @brief  Show banner of this binary.
:* @author Jinwook Jung (jinwookjung@ibm.com)
 * @date   
 */
void show_banner ()
{
    cout << endl;
    cout << string(79, '=') << endl;
    cout << "LEF/DEF Parser" << endl;
    cout << "Author: Jinwook Jung" << endl;
    cout << string(79, '=') << endl;
}

void show_cmd_args ()
{
    auto& ap = ArgParser::get();
    auto filename_def = ap.get_argument("-def");
    cout << "  DEF file   : " << filename_def << endl;
}


