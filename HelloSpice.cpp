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

auto main () -> int
{
    SpiceInt count;
    std::string kernel_de440s = "kernels/de440.bsp";
    
    auto obj_ids = get_obj_ids_from_kernel(kernel_de440s);

    
    for (auto &obj_id : obj_ids)
    {
        std::cout << obj_id.name << " " << obj_id.id << std::endl;
    }


    /*furnsh_c(kernel_de440s.c_str());
    ktotal_c ( "ALL", &count );
    
    std::cout << "Hello World!" << count << std::endl;


    kclear_c();
    ktotal_c ( "ALL", &count );*/
    return 0;
}