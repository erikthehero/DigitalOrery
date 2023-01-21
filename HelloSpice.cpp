#include <iostream>
#include <string>
#include <vector>

extern "C" {
#include "include/SpiceUsr.h"
}

struct kernel_obj_info
{
    std::string name;
    int id;
};

auto convert_obj_id_to_name(int id) -> std::string
{
    const int max_len = 32;
    SpiceBoolean            found;
    SpiceChar               name [max_len];

    bodc2n_c ( id, max_len, name, &found );
    return std::string(name);
}

auto get_obj_ids_from_kernel(std::string &kernel_filename) -> std::vector<kernel_obj_info>
{
    std::vector<kernel_obj_info> kernel_obj_ids = {};
    const int max_objects = 1000;
    SPICEINT_CELL(ids, max_objects);
    spkobj_c (kernel_filename.c_str(), &ids);
    
    for ( int i = 0;  i < card_c( &ids );  i++  )
    {
        int id = SPICE_CELL_ELEM_I( &ids, i );
        auto name = convert_obj_id_to_name(id);
        kernel_obj_ids.push_back({name, id});
    }
    
    return kernel_obj_ids;
}

struct Position
{
    double x;
    double y;
    double z;
};

struct Velocity
{
    double dx;
    double dy;
    double dz;
};

struct State
{
    Position pos;
    Velocity vel;
};

auto compute_relative_position(std::string obj_1, std::string obj_2) -> State
{
    State state = {};

    SpiceDouble   et;
    SpiceDouble   s[6];
    SpiceDouble   lt;
    str2et_c ("2006 JAN 31 01:00", &et );
    spkezr_c (obj_1.c_str(), et, "J2000", "NONE", obj_2.c_str(), s, &lt);
    state.pos.x = s[0];
    state.pos.y = s[1];
    state.pos.z = s[2];
    state.vel.dx = s[3];
    state.vel.dy = s[4];
    state.vel.dz = s[5];
    return state;
}

auto main () -> int
{
    SpiceInt count;
    std::string kernel_de440s = "kernels/de440.bsp";
    std::string kernel_naif0012 = "kernels/naif0012.tls";
    
    auto obj_ids = get_obj_ids_from_kernel(kernel_de440s);

    
    for (auto &obj_id : obj_ids)
    {
        std::cout << obj_id.name << " " << obj_id.id << std::endl;
    }

    // load kernels
    furnsh_c(kernel_naif0012.c_str());
    furnsh_c(kernel_de440s.c_str());

    auto state = compute_relative_position("SUN", "EARTH");

    ktotal_c ( "ALL", &count );
    
    std::cout << "Hello World!" << count << std::endl;


    kclear_c();
    ktotal_c ( "ALL", &count );
    return 0;
}