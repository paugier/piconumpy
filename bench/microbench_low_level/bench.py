import sys
from time import perf_counter
from pathlib import Path
from random import random
from math import sqrt

import numpy as np

try:
    method = sys.argv[1]
except IndexError:
    method = "purepy"

try:
    name_bench = sys.argv[2]
except IndexError:
    name_bench = "sum_loop"


tmp_result_julia = Path(f"tmp_julia_{name_bench}.txt")
if tmp_result_julia.exists():
    with open(tmp_result_julia) as file:
        norm = float(file.read())
else:
    print(f"{tmp_result_julia} does not exist. First execute with `make`")


def sum_loop(arr):
    result = 0.0
    for value in arr:
        result += value
    return result


def sum_loop_index(arr):
    result = 0.0
    for index in range(5000):
        result += arr[index]
    return result


def init_zeros(arr):
    for index in range(len(arr)):
        arr[index] = 0.0


def _cort(s1, s2):
    num = 0.0
    sum_square_x = 0.0
    sum_square_y = 0.0
    for t in range(len(s1) - 1):
        slope_1 = s1[t + 1] - s1[t]
        slope_2 = s2[t + 1] - s2[t]
        num += slope_1 * slope_2
        sum_square_x += slope_1 * slope_1
        sum_square_y += slope_2 * slope_2
    return num / (sqrt(sum_square_x * sum_square_y))


def cort(arr):
    return _cort(arr, arr)


compute_from_arr = locals()[name_bench]


if method == "_piconumpy_hpy":
    from piconumpy.util_hpy import import_ext

    ext = import_ext()
    array = ext.array
elif method == "list":
    array = list
elif method == "numpy":
    array = np.array
else:
    d = {}
    exec(f"from piconumpy.{method} import array", d)
    array = d["array"]

# print(array)

size = 10000

# warming during ~ 1s
data_as_list = [random() for _ in range(size)]
arr = array(data_as_list)
t_start = perf_counter()
compute_from_arr(arr)
t_first = perf_counter() - t_start
for _ in range(round(1 / t_first)):
    compute_from_arr(arr)

# estimate time after warming
times = []
nb_runs = 10
for _ in range(nb_runs):
    data_as_list = [random() for _ in range(size)]
    arr = array(data_as_list)
    t_start = perf_counter()
    compute_from_arr(arr)
    times.append(perf_counter() - t_start)

# better estimate of the time
nb_runs = max(20, round(0.5 / np.median(times)))
times = []
for _ in range(nb_runs):
    data_as_list = [random() for _ in range(size)]
    arr = array(data_as_list)
    t_start = perf_counter()
    compute_from_arr(arr)
    times.append(perf_counter() - t_start)

time = np.median(times)
print(f"{method:30s}: {time:.2e} s ({time / norm:5.1f} * Julia)")