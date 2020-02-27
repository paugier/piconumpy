"""
pypy3 purepython_callback.py
bench                            : 1.000 * norm
norm = 4.24e-04 s
bench_pythran                    : 1.061 * norm
time pythran: 0.450 ms

python purepython_callback.py
bench                            : 1.000 * norm
norm = 2.57e-02 s
bench_pythran                    : 0.017 * norm
time pythran: 0.449 ms

- PyPy is 60 times faster than CPython.
- PyPy is a bit faster than Pythran and Julia!

"""

from transonic.util import timeit_verbose as tiv
from transonic import jit, wait_for_all_extensions


def rober(u):
    k1 = 0.04
    k2 = 3e7
    k3 = 1e4
    y1, y2, y3 = u
    dy1 = -k1 * y1 + k3 * y2 * y3
    dy2 = k1 * y1 - k2 * y2 * y2 - k3 * y2 * y3
    dy3 = k2 * y2 * y2
    return dy1, dy2, dy3


def call_function(func):
    u = (1.0, 0.0, 0.0)
    for _ in range(int(1e5)):
        u = func(u)
    return u


def bench():
    return call_function(rober)


bench_pythran = jit(bench)

bench()
bench_pythran()

wait_for_all_extensions()

g = globals()
duration = 10
norm = tiv("bench()", globals=g, total_duration=duration)
time_pythran = tiv(
    "bench_pythran()", globals=g, total_duration=duration, norm=norm
)
print(f"time pythran: {time_pythran*1000:.3f} ms")
