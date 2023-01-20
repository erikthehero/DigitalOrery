#include <iostream>
#include <string>

extern "C" {
#include "include/SpiceUsr.h"
}

auto main () -> int
{
    SpiceInt count;
    std::string kernel_de440s = "../../kernels/de440s.bsp";
    
    
    furnsh_c(kernel_de440s.c_str());
    ktotal_c ( "ALL", &count );
    
    std::cout << "Hello World!" << count << std::endl;


    kclear_c();
    ktotal_c ( "ALL", &count );
    return 0;
}