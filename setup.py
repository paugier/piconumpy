from setuptools import setup, find_packages, Extension

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
        )
    ],
)
