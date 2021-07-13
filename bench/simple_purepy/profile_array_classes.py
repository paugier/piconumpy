import cProfile
import pstats

import numpy as np

import sys

from bench import solver, array, board

if "list" in sys.argv:
    key = "list"
else:
    key = "array"


def bench(n_sleds, n_time):

    x_init = np.zeros(n_sleds)
    y_init = np.random.rand(n_sleds)
    v_init = np.zeros(n_sleds)
    u_init = np.zeros(n_sleds) + 3.5

    x_init = array(x_init.tolist())
    y_init = array(y_init.tolist())
    u_init = array(u_init.tolist())
    v_init = array(v_init.tolist())

    solver(board, x_init, y_init, u_init, v_init, 0.01, n_time)


n_sleds = 100
n_time = 200

cProfile.runctx(
    "bench(n_sleds, n_time)", globals(), locals(), f"tmp_{key}.pstats"
)

s = pstats.Stats(f"tmp_{key}.pstats")
s.strip_dirs().sort_stats("time").print_stats(10)
