with open("bench_array1d.py") as file:
    code = file.read()

code = code.split("# begin code functions (don't remove this line)")[1]
code = code.split("# end code functions (don't remove this line)")[0]

code = (
    """

import numpy as np
from piconumpy import pi, cos, sin, array

"""
    + code
    + """

from transonic.util import timeit_verbose as timeit

n_sleds = 10
n_time = 200

g = locals()
norm = timeit("bench(n_sleds, n_time)", globals=g)

"""
)

with open("tmp.py", "w") as file:
    file.write(code)
