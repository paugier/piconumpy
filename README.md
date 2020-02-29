# PicoNumpy

**An experiment about Numpy and pyhandle/hpy**

The C API of CPython is one of the cause of the success of Python in scientific
computing. In particular, Numpy (and all the Python scientific stack) is built
on top of this API. However, some characteristics of this API start to be an
issue for the future of scientific Python (see [1] and [hpy]).

[1]: https://faster-cpython.readthedocs.io/
[hpy]: https://github.com/pyhandle/hpy

[hpy] is a very ambitious and promissing project to design a new and better C
API for interacting with Python interpreters. This should allow people to write
Python extensions that can be efficient on different interpreters (CPython,
PyPy, GraalPython, RustPython, etc.).

PyPy would be especially useful for some cases for scientific Python. For
example for Integration and ODEs
([scipy.integrate](https://docs.scipy.org/doc/scipy/reference/integrate.html)),
for which there are a lot of callbacks of very small functions. This repository
contains [a tiny benchmark](bench/without_numpy) showing that as long as Numpy
is not used, PyPy is very efficient for such task. Unfortunately, as soon as
Numpy is used, PyPy becomes very slow!

[bench/without_numpy]: https://github.com/paugier/piconumpy/blob/master/bench/without_numpy/

With PicoNumpy, I'd like to study if [hpy] could help for codes using Numpy and
callbacks of small Python functions.

We start by a [simple but realistic benchmark](bench/bench_array1d.py) (the
slow loops only involve pure-Python and very simple Numpy). We then wrote a
tiny ("pico") implementation of a Numpy like object (just sufficient to run the
benchmark).

The next task is to reimplement PicoNumpy using [hpy] and to check if PyPy
could efficiently accelerate [our main benchmark](bench/bench_array1d.py).

PicoNumpy is really tiny. It "just" has to provide an `array` class
(one-dimensional) and two functions (`cos` and `sin`) acting on floats. The
PicoNumpy array class "just" has to support:

- Instantiation from a list of floats
- Elementwise multiplication and division by a float
- Elementwise addition (of 2 arrays)
- Indexing
- `len`

A good acceleration by PyPy of our example would be a great proof that the
scientific Python community has to invest time and energy on [hpy].

In the script [bench_array1d.py](bench/bench_array1d.py), Transonic is used for
the benchmark and comparison. With Transonic-Pythran, we typically get a 100
speedup compared to CPython (and ~500 versus PyPy, which is still very slow for
such codes using Numpy).

[bench/bench_array1d.py]: https://github.com/paugier/piconumpy/blob/master/bench/bench_array1d.py
