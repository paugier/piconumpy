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
list                          : 1.75e-05 s (  1.6 * Julia)
purepy                        : 1.95e-05 s (  1.8 * Julia)
_piconumpy_hpy                : 2.18e-04 s ( 20.5 * Julia)
_piconumpy_cpython_capi       : 1.19e-03 s (112.1 * Julia)
```

With CPython:

```
{'cache_tag': 'cpython-39',
 'version': sys.version_info(major=3, minor=9, micro=6, releaselevel='final', serial=0)}
list                          : 2.65e-04 s ( 25.0 * Julia)
purepy                        : 1.27e-03 s (120.0 * Julia)
_piconumpy_hpy                : 4.24e-04 s ( 39.9 * Julia)
_piconumpy_cpython_capi       : 3.50e-04 s ( 33.0 * Julia)
```