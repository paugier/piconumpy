# Microbenchmark sum_loop

We measure the performance for this function:

```python
def sum_loop(arr):
    result = 0.0
    for value in arr:
        result += value
    return result
```

One can run the benchmarks with `make bench_sum_loop`.

With PyPy3.7, I get:

```
bench sum_loop
hostname: voyage
{'cache_tag': 'pypy37',
 'version': sys.pypy_version_info(major=7, minor=3, micro=7, releaselevel='final', serial=0)}
list                          : 2.35e-05 s (  1.8 * Julia)
purepy                        : 2.60e-05 s (  2.0 * Julia)
numpy                         : 8.97e-03 s (677.0 * Julia)
_piconumpy_hpy                : 3.73e-04 s ( 28.2 * Julia)
_piconumpy_cpython_capi       : 1.75e-03 s (132.4 * Julia)
```

With CPython:

```
bench sum_loop
hostname: voyage
{'cache_tag': 'cpython-39',
 'version': sys.version_info(major=3, minor=9, micro=7, releaselevel='final', serial=0)}
list                          : 3.65e-04 s ( 27.5 * Julia)
purepy                        : 2.17e-03 s (164.1 * Julia)
numpy                         : 1.09e-03 s ( 82.2 * Julia)
_piconumpy_hpy                : 7.39e-04 s ( 55.8 * Julia)
_piconumpy_cpython_capi       : 5.07e-04 s ( 38.3 * Julia)

```

With Python 3.8.5 (GraalVM CE Native 21.3.0)

```
bench sum_loop
hostname: voyage
{'cache_tag': 'graalpython-38',
 'version': sys.version_info(major=3, minor=8, micro=5, releaselevel='alpha', serial=0)}
list                          : 1.92e-05 s (  1.4 * Julia)
purepy                        : 3.61e-05 s (  2.7 * Julia)
numpy                         : ImportError numpy
_piconumpy_hpy                : 5.03e-04 s ( 38.0 * Julia)
_piconumpy_cpython_capi       : 2.90e-03 s (219.1 * Julia)
```
