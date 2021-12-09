# Microbenchmark sum_init_zeros

We measure the performance for this function:

```python
def init_zeros(arr):
    for index in range(len(arr)):
        arr[index] = 0.0
```

One can run the benchmarks with `make bench_init_zeros`.

With PyPy3.7, I get:

```
bench init_zeros
hostname: voyage
{'cache_tag': 'pypy37',
 'version': sys.pypy_version_info(major=7, minor=3, micro=7, releaselevel='final', serial=0)}
list                          : 2.53e-05 s (  5.2 * Julia)
purepy                        : 2.65e-05 s (  5.4 * Julia)
numpy                         : 9.01e-03 s (1848.7 * Julia)
_piconumpy_hpy                : 4.17e-04 s ( 85.6 * Julia)
_piconumpy_cpython_capi       : 1.10e-03 s (224.9 * Julia)
```

With CPython:

```
bench init_zeros
hostname: voyage
{'cache_tag': 'cpython-39',
 'version': sys.version_info(major=3, minor=9, micro=7, releaselevel='final', serial=0)}
list                          : 5.18e-04 s (106.3 * Julia)
purepy                        : 1.94e-03 s (397.8 * Julia)
numpy                         : 1.17e-03 s (239.3 * Julia)
_piconumpy_hpy                : 6.46e-04 s (132.5 * Julia)
_piconumpy_cpython_capi       : 5.43e-04 s (111.4 * Julia)
```
