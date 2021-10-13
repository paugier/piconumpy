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
{'cache_tag': 'pypy37',
 'version': sys.pypy_version_info(major=7, minor=3, micro=6, releaselevel='final', serial=0)}
list                          : 1.73e-05 s (  1.6 * Julia)
purepy                        : 1.97e-05 s (  1.9 * Julia)
numpy                         : 4.12e-03 s (388.6 * Julia)
_piconumpy_hpy                : 2.14e-04 s ( 20.2 * Julia)
_piconumpy_cpython_capi       : 1.19e-03 s (112.1 * Julia)
```

With CPython:

```
{'cache_tag': 'cpython-39',
 'version': sys.version_info(major=3, minor=9, micro=6, releaselevel='final', serial=0)}
list                          : 2.62e-04 s ( 24.7 * Julia)
purepy                        : 1.25e-03 s (118.2 * Julia)
numpy                         : 8.66e-04 s ( 81.6 * Julia)
_piconumpy_hpy                : 4.22e-04 s ( 39.8 * Julia)
_piconumpy_cpython_capi       : 3.53e-04 s ( 33.3 * Julia)
```