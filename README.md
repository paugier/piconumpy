# piconumpy

An experiment about Numpy and pyhandle/hpy

piconumpy could be implemented using [hpy](https://github.com/pyhandle/hpy) so
that PyPy should be able to accelerate [such code](bench/bench_array1d.py) (the
slow loops only involve pure-Python and piconumpy).

piconumpy could be a really tiny package. It "just" has to provide an `array`
class (one-dimensional) and two functions (`cos` and `sin`) acting on piconumpy
arrays.

The piconumpy array class "just" has to support:

- instantiation from a list of floats
- multiplication and division by a float
- term by term multiplication and addition

A good acceleration by PyPy of our example would be a great proof that the
scientific Python community has to invest time and energy on hpy.

In the script [bench_array1d.py](bench/bench_array1d.py), Transonic is used for
the benchmark and comparison. With Transonic-Pythran, we typically get a 100
speedup compared to CPython (and ~500 versus PyPy, which is still very slow for
such codes using Numpy).

[bench/bench_array1d.py]: https://github.com/paugier/piconumpy/blob/master/bench/bench_array1d.py
