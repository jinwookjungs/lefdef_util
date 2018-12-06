/**
 * @file    Def.h
 * @author  Jinwook Jung (jinwookjung@kaist.ac.kr)
 * @date    2017-12-24 23:05:20
 *
 * Created on Sun Dec 24 23:05:20 2017.
 */

#ifndef DEF_H
#define DEF_H

#include "def/defrReader.hpp"
#include "common_header.h"
#include "common_enum.h"

#include "Lef.h"

namespace def
{

// Forward declaration.
struct Row;
struct Track;
struct GCellGrid;
struct Component;
struct Pin;
struct Via;
struct RoutingPoint;
struct WireSegment;
struct Wire;
struct Connection;
struct Net;
struct SpecialNet;

// Alias to basic data structures
using RowPtr          = shared_ptr<Row>;
using TrackPtr        = shared_ptr<Track>;
using GCellGridPtr    = shared_ptr<GCellGrid>;
using ComponentPtr    = shared_ptr<Component>;
using PinPtr          = shared_ptr<Pin>;
using ViaPtr          = shared_ptr<Via>;
using WirePtr         = shared_ptr<Wire>;
using RoutingPointPtr = shared_ptr<RoutingPoint>;
using WireSegmentPtr  = shared_ptr<WireSegment>;
using ConnectionPtr   = shared_ptr<Connection>;
using NetPtr          = shared_ptr<Net>;
using SpecialNetPtr   = shared_ptr<SpecialNet>;

// Some containers
using RowVec          = vector<RowPtr>;
using TrackVec        = vector<TrackPtr>;
using GCellGridVec    = vector<GCellGridPtr>;
using ComponentUMap   = unordered_map<string, ComponentPtr>;
using PinUMap         = unordered_map<string, PinPtr>;
using NetUMap         = unordered_map<string, NetPtr>;
using SpecialNetUMap  = unordered_map<string, SpecialNetPtr>;

ostream& operator<< (ostream& os, const Row&);
ostream& operator<< (ostream& os, const Track&);
ostream& operator<< (ostream& os, const Component&);
ostream& operator<< (ostream& os, const Pin&);
ostream& operator<< (ostream& os, const Via&);
ostream& operator<< (ostream& os, const RoutingPoint&);
ostream& operator<< (ostream& os, const Wire&);
ostream& operator<< (ostream& os, const WireSegment&);
ostream& operator<< (ostream& os, const Connection&);
ostream& operator<< (ostream& os, const Net&);

/**
 * A class to represent a row.
 */
struct Row
{
    string name_;
    string macro_;     ///< Site name

    string orient_str_;    
    int orient_;

    uint32_t x_;            ///< x coordinate at the origin.
    uint32_t y_;            ///< y coordinate at the origin.
    uint32_t num_x_;             ///< Number of sites in x-direction.
    uint32_t num_y_;             ///< Number of sites in y-direction.
    uint32_t step_x_;
    uint32_t step_y_;
};


/**
 * A class to represent a track.
 */
struct Track
{
    TrackDir direction_;
    uint32_t location_;
    uint32_t num_tracks_;
    uint32_t step_;
    uint32_t len_;          // Length of the track = step * num_tracks
    int num_layers_;

    string layer_;            ///< The first layer
    vector<string> layers_;   ///< If multiple layers defined.
};


/**
 * A class to represent a gcell grid.
 */
struct GCellGrid
{
    TrackDir direction_;
    uint32_t location_;
    uint32_t num_;
    uint32_t step_;
};

/**
 * A class to represent a component.
 */
struct Component
{
    string name_;
    string ref_name_;
    bool is_fixed_;
    bool is_placed_;
    uint32_t x_;
    uint32_t y_;
    string orient_str_;
    int orient_;

    lef::MacroPtr lef_macro_;
};


/**
 * A class to represent a pin.
 */
struct Pin 
{
    string name_;
    string net_name_;
    string layer_;
    PinDir dir_;

    uint32_t x_;
    uint32_t y_;
    string orient_str_;
    int orient_;

    uint32_t lx_;
    uint32_t ly_;
    uint32_t ux_;
    uint32_t uy_;
};


/**
 *
 */
struct RoutingPoint
{
    uint32_t x_;
    uint32_t y_;
    uint32_t ext_;
    bool has_via_;

    RoutingPoint (int x, int y, int ext) : x_(x), y_(y), ext_(ext) {}
};


/**
 *
 */
struct WireSegment
{
    string layer_name_;
    int width_;
    vector<RoutingPointPtr> rpoints_;    
};

/**
 * A class to represent a wire.
 */
struct Wire
{
    string wire_type_;
    string layer_;
    vector<WireSegmentPtr> wire_segments_;
};


/**
 * A class to represent a via.
 */
struct Via
{
    string layer_;
};


struct Connection
{
    string name_;
    ComponentPtr component_;
    lef::PinPtr lef_pin_;
    PinPtr pin_;

    uint32_t lx_;
    uint32_t ly_;
    uint32_t ux_;
    uint32_t uy_;

    Connection (string name, ComponentPtr component, lef::PinPtr lef_pin,
                uint32_t lx, uint32_t ly, uint32_t ux, uint32_t uy)
        : name_(name), component_(component), lef_pin_(lef_pin), pin_(nullptr),
          lx_(lx), ly_(ly), ux_(ux), uy_(uy) {}

    Connection (string name, PinPtr pin, 
                uint32_t lx, uint32_t ly, uint32_t ux, uint32_t uy)
        : name_(name), component_(nullptr), lef_pin_(nullptr), pin_(pin),
          lx_(lx), ly_(ly), ux_(ux), uy_(uy) {}
};

/**
 * A class to represent a net.
 */
struct Net
{
    string name_;
    vector<ConnectionPtr> connections_;

    vector<WirePtr> wires_;
    vector<ViaPtr> vias_;
};


/**
 * A special net.
 */
struct SpecialNet : public Net
{

};

/**
 * A class to keep the information in a DEF file.
 */
class Def
{
public:
    static Def& get_instance ();

    string get_design_name () const;
    int get_dbu () const;

    const RowVec& get_rows () const;
    const TrackVec& get_tracks () const;
    const GCellGridVec& get_gcell_grids () const;
    const ComponentUMap& get_component_umap () const;
    const PinUMap& get_pin_umap () const;
    const NetUMap& get_net_umap () const;
    const SpecialNetUMap& get_special_net_umap () const;

    NetPtr get_net (string name);
    ComponentPtr get_component (string name);
    PinPtr get_pin (string name);

    void read_def (string filename);
    void report () const;
    void report_verbose () const;

    uint32_t get_die_lx () const;
    uint32_t get_die_ly () const;
    uint32_t get_die_ux () const;
    uint32_t get_die_uy () const;

private:
    struct Impl;
    unique_ptr<Impl> pimpl_;   ///< Pointer to the implementation.

    friend class DefParser;

    Def ();
    ~Def () = default;
    Def (const Def&) = delete;
    Def& operator= (const Def&) = delete;
    Def (Def&&) = delete;
    Def& operator= (Def&&) = delete;
};


/**
 * A set of call-back functions for the si2 def parser.
 */
class DefParser
{
public:
    static int set_design_name (defrCallbackType_e, const char*, defiUserData);
    static int set_units (defrCallbackType_e, double, defiUserData);
    static int set_die_area (defrCallbackType_e, defiBox*, defiUserData);
    static int set_track (defrCallbackType_e, defiTrack*, defiUserData);
    static int set_gcell_grid (defrCallbackType_e, defiGcellGrid*, defiUserData);
    static int set_row (defrCallbackType_e, defiRow*, defiUserData);
    static int set_component_start (defrCallbackType_e, int, defiUserData);
    static int set_component (defrCallbackType_e, defiComponent*, defiUserData);
    static int set_pin (defrCallbackType_e, defiPin*, defiUserData);
    static int set_net_start (defrCallbackType_e, int, defiUserData);
    static int set_net (defrCallbackType_e, defiNet*, defiUserData);
    static int set_special_net_start (defrCallbackType_e, int, defiUserData);
    static int set_special_net (defrCallbackType_e, defiNet*, defiUserData);

private:
    DefParser () = default;
    ~DefParser () = default;
    DefParser (const DefParser& rhs) = delete;
    DefParser& operator= (const DefParser& rhs) = delete;
};

}       // End of namespace lefdef

#endif
