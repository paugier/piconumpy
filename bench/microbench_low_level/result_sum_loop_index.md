# Microbenchmark sum_loop_index

We measure the performance for this function:

```python
def sum_loop_index(arr):
    result = 0.0
    for index in range(5000):
        result += arr[index]
    return result
```

One can run the benchmarks with `make bench_sum_loop_index`.

With PyPy3.7, I get:

```
bench sum_loop_index
hostname: voyage
{'cache_tag': 'pypy37',
 'version': sys.pypy_version_info(major=7, minor=3, micro=7, releaselevel='final', serial=0)}
list                          : 1.19e-05 s (  2.0 * Julia)
purepy                        : 1.20e-05 s (  2.1 * Julia)
numpy                         : 4.07e-03 s (692.9 * Julia)
_piconumpy_hpy                : 1.65e-04 s ( 28.2 * Julia)
_piconumpy_cpython_capi       : 9.95e-04 s (169.5 * Julia)
```

With CPython:

```
bench sum_loop_index
hostname: voyage
{'cache_tag': 'cpython-39',
 'version': sys.version_info(major=3, minor=9, micro=7, releaselevel='final', serial=0)}
list                          : 3.91e-04 s ( 66.6 * Julia)
purepy                        : 1.10e-03 s (186.9 * Julia)
numpy                         : 8.95e-04 s (152.5 * Julia)
_piconumpy_hpy                : 4.85e-04 s ( 82.5 * Julia)
_piconumpy_cpython_capi       : 4.15e-04 s ( 70.6 * Julia)
```
