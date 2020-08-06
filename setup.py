from setuptools import setup, find_packages, Extension
import hpy.devel
from Cython.Build import cythonize

setup(
    name="piconumpy",
    packages=find_packages(exclude=["bench"]),
    extras_require={
        "dev": ["transonic", "numpy", "pytest", "pythran"],
        # black can't be installed with PyPy3!
        "full": ["black"],
    },
    ext_modules=[
        Extension(
            "piconumpy._piconumpy_cpython_capi",
            ["piconumpy/_piconumpy_cpython_capi.c"],
            extra_compile_args = [
                '-Wfatal-errors',    # stop after one error (unrelated to warnings)
                '-Werror',           # turn warnings into errors (all, for now)
            ]
        ),
        Extension(
            "piconumpy._piconumpy_hpy",
            ["piconumpy/_piconumpy_hpy.c"] + hpy.devel.get_sources(),
            include_dirs=[hpy.devel.get_include()],
            extra_compile_args = [
                '-Wfatal-errors',    # stop after one error (unrelated to warnings)
                '-Werror',           # turn warnings into errors (all, for now)
            ]
        ),
        *cythonize("piconumpy/_piconumpy_cython.pyx"),
    ],
)
