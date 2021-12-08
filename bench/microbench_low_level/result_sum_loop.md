# Microbenchmark sum_loop

We measure the performance for this function:

```python
def sum_loop(arr):
    result = 0.0
    for value in arr:
        result += value
    return result
```

One can run the benchmarks with `make`.

With PyPy3.7, I get:

```
bench sum_loop
hostname: voyage
{'cache_tag': 'pypy37',
 'version': sys.pypy_version_info(major=7, minor=3, micro=7, releaselevel='final', serial=0)}
list                          : 2.34e-05 s (  1.7 * Julia)
purepy                        : 2.41e-05 s (  1.8 * Julia)
numpy                         : 8.91e-03 s (654.4 * Julia)
_piconumpy_hpy                : 3.37e-04 s ( 24.8 * Julia)
_piconumpy_cpython_capi       : 2.04e-03 s (150.1 * Julia)
```

With CPython:

```
bench sum_loop
hostname: voyage
{'cache_tag': 'cpython-39',
 'version': sys.version_info(major=3, minor=9, micro=7, releaselevel='final', serial=0)}
list                          : 3.59e-04 s ( 26.4 * Julia)
purepy                        : 2.18e-03 s (160.3 * Julia)
numpy                         : 1.12e-03 s ( 82.5 * Julia)
_piconumpy_hpy                : 6.30e-04 s ( 46.2 * Julia)
_piconumpy_cpython_capi       : 5.04e-04 s ( 37.0 * Julia)
```
