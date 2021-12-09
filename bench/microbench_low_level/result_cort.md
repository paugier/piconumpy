# Microbenchmark cort

We measure the performance for this function:

```python
def cort(arr):
    return _cort(arr, arr)

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
```

One can run the benchmarks with `make bench_cort`.

With PyPy3.7, I get:

```
bench cort
hostname: voyage
{'cache_tag': 'pypy37',
 'version': sys.pypy_version_info(major=7, minor=3, micro=7, releaselevel='final', serial=0)}
list                          : 4.29e-05 s (  1.8 * Julia)
purepy                        : 3.89e-05 s (  1.6 * Julia)
numpy                         : 4.72e-02 s (1957.9 * Julia)
_piconumpy_hpy                : 1.36e-03 s ( 56.5 * Julia)
_piconumpy_cpython_capi       : 8.03e-03 s (332.8 * Julia)
```

With CPython:

```
bench cort
hostname: voyage
{'cache_tag': 'cpython-39',
 'version': sys.version_info(major=3, minor=9, micro=7, releaselevel='final', serial=0)}
list                          : 4.47e-03 s (185.4 * Julia)
purepy                        : 1.08e-02 s (448.6 * Julia)
numpy                         : 9.69e-03 s (401.6 * Julia)
_piconumpy_hpy                : 5.32e-03 s (220.5 * Julia)
_piconumpy_cpython_capi       : 4.80e-03 s (198.9 * Julia)
```
