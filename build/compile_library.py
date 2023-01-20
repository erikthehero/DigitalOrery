import os

def compile_cspice_library():
    cspice_base_dir = "/home/cicada/Documents/dev/digital_orery/CSPICE_test/cspice"
    cspice_include_dir = f"{cspice_base_dir}/include"
    cspice_src_dir = f"{cspice_base_dir}/src/cspice"
    obj_dir = f"{cspice_base_dir}/obj"
    lib_dir = f"{cspice_base_dir}/lib_custom"

    # compile
    compile_cmd = f"gcc -c -I {cspice_include_dir} {cspice_src_dir}/*.c"
    #os.system(compile_cmd)

    # move
    if not os.path.isdir(obj_dir):
        os.mkdir(obj_dir)

    if not os.path.isdir(lib_dir):
        os.mkdir(lib_dir)

    move_cmd = f"mv ./*.o {obj_dir}"
    os.system(move_cmd)

    # archive
    archive_cmd = f"ar -rc {lib_dir}/cspice.a {obj_dir}/*.o"
    os.system(archive_cmd)

if __name__=="__main__":
    compile_cspice_library()