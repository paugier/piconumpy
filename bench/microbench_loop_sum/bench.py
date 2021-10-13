import sys
from time import perf_counter
from pathlib import Path
from random import random

import numpy as np

tmp_result_julia = Path("tmp_result_julia.txt")
if tmp_result_julia.exists():
    with open("tmp_result_julia.txt") as file:
        norm = float(file.read())
else:
    print("tmp_result_julia.txt does not exist. First execute with `make`")

try:
    method = sys.argv[1]
except IndexError:
    method = "purepy"


def sum_loop(arr):
    result = 0.0
    for value in arr:
        result += value
    return result


if method == "_piconumpy_hpy":
    from piconumpy.util_hpy import import_ext

    ext = import_ext()
    array = ext.array
elif method == "list":
    array = list
else:
    d = {}
    exec(f"from piconumpy.{method} import array", d)
    array = d["array"]

# print(array)

size = 10000
times = []
nb_runs = 200
for _ in range(nb_runs):
    data_as_list = [random() for _ in range(size)]
    arr = array(data_as_list)
    t_start = perf_counter()
    sum_loop(arr)
    times.append(perf_counter() - t_start)

time = np.median(times)
print(f"{method:30s}: {time:.2e} s ({time / norm:5.1f} * Julia)")
