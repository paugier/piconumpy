from setuptools import setup, find_packages, Extension
from Cython.Build import cythonize

setup(
    name="piconumpy",
    packages=find_packages(exclude=["bench"]),
    # Workaround: HPy adds files to the sources list and uses absolute paths.
    # Newer setuptools complain about that if package data should be included.
    # Therefore, we explicitly disable this here.
    include_package_data=False,
    ext_modules=[
        Extension(
            "piconumpy._piconumpy_cpython_capi",
            ["piconumpy/_piconumpy_cpython_capi.c"],
            extra_compile_args=[
                "-Wfatal-errors",  # stop after one error (unrelated to warnings)
                "-Werror",  # turn warnings into errors (all, for now)
            ],
        ),
        *cythonize("piconumpy/_piconumpy_cython.pyx"),
    ],
    hpy_ext_modules=[
        Extension(
            "piconumpy._piconumpy_hpy",
            ["piconumpy/_piconumpy_hpy.c"],
            extra_compile_args=[
                "-Wfatal-errors",
                "-Werror",
            ],
        ),
    ],
)
