/**
 * @file    main.cpp
 * @author  Jinwook Jung (jinwookjung@kaist.ac.kr)
 * @date    2017-12-23 22:12:10
 *
 * Created on Sat Dec 23 22:12:10 2017.
 */

#include "Logger.h"
#include "Watch.h"
#include "ArgParser.h"
#include "LefDefParser.h"

#include <iostream>

using namespace std;

void show_usage ();
void show_banner ();
void show_cmd_args ();
int omp_thread_count();

#ifndef UNIT_TEST

/**
 * Main.
 */
int main (int argc, char* argv[])
{
    util::Watch watch;

    // Parsing command line arguments
    auto& ap = ArgParser::get();

    ap.initialize(argc, argv);
    auto filename_lef = ap.get_argument("--lef");
    auto filename_def = ap.get_argument("--def");
    auto filename_bookshelf = ap.get_argument("--bookshelf");

    if (filename_lef == "" or filename_def == "") {
        show_usage();
        return -1;
    }
    if (filename_bookshelf == "") {
        filename_bookshelf = "out";
    }


    show_banner();
    show_cmd_args();

    // Run detaile drouter
    auto& ldp = my_lefdef::LefDefParser::get_instance();
    ldp.read_lef(filename_lef);
    ldp.read_def(filename_def);

    ldp.write_bookshelf(filename_bookshelf);

    cout << endl << "Done." << endl;

    return 0;
}

void show_usage ()
{
    cout << endl;
    cout << "Usage:" << endl;
    cout << "bookshelf_writer --lef <lef> --def <def> [--bookshelf <bookshelf>]" << endl << endl;
}

/**
 * @brief  Show banner of this binary.
 * @author Jinwook Jung (jinwookjung@kaist.ac.kr)
 * @date   2017-12-23 22:20:22
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
    auto filename_lef = ap.get_argument("--lef");
    auto filename_def = ap.get_argument("--def");
    auto filename_bookshelf = ap.get_argument("--bookshelf");
    cout << "  LEF file   : " << filename_lef << endl;
    cout << "  DEF file   : " << filename_def << endl;
}

int omp_thread_count() {
    int n = 0;
#pragma omp parallel reduction(+:n)
    n += 1;
    return n;
}

#else

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Simple testcases
#include <boost/test/unit_test.hpp>

#endif
