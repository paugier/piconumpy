with open("bench_array1d.py") as file:
    code = file.read()

code = code.split("# begin code functions (don't remove this line)")[1]
code_functions = code.split("# end code functions (don't remove this line)")[0]


def create_tmp_file(name_module):
    code = (
        f"""
import numpy as np
from piconumpy.{name_module} import pi, cos, sin, array
    """
        + code_functions
    )

    if name_module.startswith("_piconumpy_"):
        name = name_module[len("_piconumpy_") :]
    else:
        name = name_module

    with open(f"tmp_{name}.py", "w") as file:
        file.write(code)


create_tmp_file("purepy")
create_tmp_file("purepy_array")
create_tmp_file("_piconumpy_cython")


code = (
    """
import numpy as np
from piconumpy import pi, cos, sin, array
"""
    + code_functions
    + """
from transonic.util import timeit_verbose as timeit

from bench_array1d import bench as bench_numpy, bench_pythran
from tmp_purepy import bench as bench_piconumpy_purepy
from tmp_purepy_array import bench as bench_piconumpy_purepy_array
from tmp_cython import bench as bench_cython

n_sleds = 10
n_time = 200

g = locals()
norm = timeit("bench_pythran(n_sleds, n_time)", globals=g)
timeit("bench_numpy(n_sleds, n_time)", globals=g, norm=norm)
timeit("bench_piconumpy_purepy(n_sleds, n_time)", globals=g, norm=norm)
timeit("bench_piconumpy_purepy_array(n_sleds, n_time)", globals=g, norm=norm)
timeit("bench_cython(n_sleds, n_time)", globals=g, norm=norm)
bench_piconumpy = bench
timeit("bench_piconumpy(n_sleds, n_time)", globals=g, norm=norm)
"""
)

with open("tmp.py", "w") as file:
    file.write(code)
