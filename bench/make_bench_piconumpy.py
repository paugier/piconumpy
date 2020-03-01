
with open("bench_array1d.py") as file:
    code = file.read()

code = code.split("# begin code functions (don't remove this line)")[1]
code_functions = code.split("# end code functions (don't remove this line)")[0]

code = (
    """
import numpy as np
from piconumpy.purepy import pi, cos, sin, array
"""
    + code_functions)

with open("tmp_purepy.py", "w") as file:
    file.write(code)


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

n_sleds = 10
n_time = 200

g = locals()
norm = timeit("bench_pythran(n_sleds, n_time)", globals=g)
timeit("bench_numpy(n_sleds, n_time)", globals=g, norm=norm)
timeit("bench_piconumpy_purepy(n_sleds, n_time)", globals=g, norm=norm)
bench_piconumpy = bench
timeit("bench_piconumpy(n_sleds, n_time)", globals=g, norm=norm)
"""
)

with open("tmp.py", "w") as file:
    file.write(code)
