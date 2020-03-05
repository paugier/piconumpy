# PicoNumpy

**An experiment about Numpy and pyhandle/hpy**

The C API of CPython is one of the cause of the success of Python in scientific
computing. In particular, Numpy (and all the Python scientific stack) is built
on top of this API. However, some characteristics of this API start to be an
issue for the future of scientific Python (see [1], [2], [HPy]).

[1]: https://faster-cpython.readthedocs.io/
[2]: https://morepypy.blogspot.com/2019/12/hpy-kick-off-sprint-report.html
[HPy]: https://github.com/pyhandle/hpy

[HPy] is a very ambitious and promissing project to design a new and better C
API for interacting with Python interpreters. It should allow people to write
Python extensions efficient on different interpreters (CPython, PyPy, Jython,
IronPython, GraalPython, RustPython, etc.).

PyPy would be especially useful for some scientific applications. For example
for Integration and ODEs
([scipy.integrate](https://docs.scipy.org/doc/scipy/reference/integrate.html)),
for which there are a lot of callbacks of very small functions. This repository
contains [a tiny benchmark](bench/without_numpy) showing that as long as Numpy
is not used, PyPy is very efficient for such task. Unfortunately, as soon as
Numpy is used, PyPy becomes very slow!

[bench/without_numpy]: https://github.com/paugier/piconumpy/blob/master/bench/without_numpy/

With PicoNumpy, I'd like to study if [HPy] could help for codes using Numpy and
callbacks of small Python functions.

We start by a [simple but realistic benchmark](bench/bench_array1d.py) (the
slow loops only involve pure-Python and very simple Numpy). We then wrote a
tiny ("pico") implementation of a Numpy like object (just sufficient to run the
benchmark).

The next task is to reimplement PicoNumpy using [HPy] and to check if PyPy
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
scientific Python community has to invest time and energy on [HPy].

In the script [bench_array1d.py](bench/bench_array1d.py), Transonic is used for
the benchmark and comparison. With Transonic-Pythran, we typically get a 100
speedup compared to CPython (and ~500 versus PyPy, which is still very slow for
such codes using Numpy).

[bench/bench_array1d.py]: https://github.com/paugier/piconumpy/blob/master/bench/bench_array1d.py

## Install and run the benchmarks

`make` should install the package in editable mode. `cd bench; make` should run
the benchmarks. For the benchmarks, Julia is used for a good comparison point
so the command `julia` has to be available.

For PyPy, the Makefiles are sensible to the environment variable `PYTHON`, so
you could do:

```bash
export PYTHON=pypy3
make
cd bench
make
```

## Few results

As of today (5 March 2020), HPy is not yet ready!

### On my computer at work (meige8pcpa79)

- With CPython

```raw
Julia                      :     1 * norm = 0.00545 s
Transonic-Pythran          : 0.552 * norm = 0.00301 s
Numpy                      :  42.3 * norm =    0.23 s
PicoNumpy (purepy)         :    23 * norm =   0.125 s
PicoNumpy (purepy_array)   :  20.8 * norm =   0.113 s
PicoNumpy (Cython)         :  15.4 * norm =  0.0841 s
PicoNumpy (CPython C-API)  :  4.64 * norm =  0.0253 s
```

- With PyPy3

```raw
Julia                      :     1 * norm = 0.00545 s
Transonic-Pythran          : 0.591 * norm = 0.00322 s
Numpy                      :   351 * norm =    1.91 s
PicoNumpy (purepy)         :  2.36 * norm =  0.0129 s
PicoNumpy (purepy_array)   :  3.68 * norm =  0.0201 s
PicoNumpy (Cython)         :   135 * norm =   0.737 s
PicoNumpy (CPython C-API)  :  29.1 * norm =   0.158 s
```

## CPython C-API usage in PicoNumpy

It is the first time that I wrote a Python extension by hand (without Cython or
Pythran) and moreover, there are nearly no checks and I didn't pay attention to
reference counting!

List obtained from `grep -rwoh 'Py[_A-Z].[a-zA-Z_]*' | sort | uniq`:

```raw
PyArg_ParseTupleAndKeywords
PyCFunction
Py_DECREF
PyErr_NoMemory
PyErr_SetString
PyExc_TypeError
PyFloat_AsDouble
PyFloat_FromDouble
PyFloatObject
Py_INCREF
PyList_Check
PyList_GET_ITEM
PyList_New
PyList_SetItem
PyList_Size
PyLong_AsLong
PyMemberDef
PyMethodDef
PyMODINIT_FUNC
PyModule_AddObject
PyModule_Create
PyModuleDef
PyModuleDef_HEAD_INIT
PyNumber_Check
PyNumberMethods
PyObject
PyObject_HEAD
PyObject_New
PySequenceMethods
Py_ssize_t
Py_TPFLAGS_DEFAULT
Py_TYPE
PyType_GenericNew
PyTypeObject
PyType_Ready
Py_UNUSED
PyVarObject_HEAD_INIT
```
