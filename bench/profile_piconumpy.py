import cProfile
import pstats

# from bench_array1d import bench
from tmp_purepy import bench
# from tmp_purepy_array import bench
# from tmp_cython import bench

n_sleds = 10
n_time = 200

cProfile.runctx("bench(n_sleds, n_time)", globals(), locals(), "tmp.pstats")

s = pstats.Stats("tmp.pstats")
s.strip_dirs().sort_stats("time").print_stats(10)
