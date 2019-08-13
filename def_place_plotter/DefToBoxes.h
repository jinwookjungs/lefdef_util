/**
 * @file    DefToBoxes.h
 * @author  Jinwook Jung (jinwookjung@ibm.com)
 * @date    2019-08-12 23:25:57
 *
 * Created on Mon 12 Aug 2019 11:25:57 PM EDT.
 */

#ifndef DEF_PLOTTER_H
#define DEF_PLOTTER_H

#include "common_header.h"

#include "Lef.h"
#include "Def.h"
#include "util.h"

namespace my_lefdef
{

class DefToBoxes 
{
public:
    void read_lef (string filename);
    void read_def (string filename);

    void write_boxes (string filename) const;
    static DefToBoxes& get_instance ();

private:
    lef::Lef&    lef_;
    def::Def&    def_;

    // Do not allow instantiation of this class.
    DefToBoxes ();
    ~DefToBoxes () = default;
    DefToBoxes (const DefToBoxes&) = delete;
    DefToBoxes& operator= (const DefToBoxes&) = delete;
    DefToBoxes (DefToBoxes&&) = delete;
    DefToBoxes& operator= (DefToBoxes&&) = delete;
};

}

#endif
