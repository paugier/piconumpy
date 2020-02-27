from setuptools import setup, find_packages, Extension

setup(
    name="piconumpy",
    packages=find_packages(exclude=["bench"]),
    extras_require={"dev": ["black", "transonic", "numpy", "pytest"]},
    ext_modules=[
        Extension(
            "piconumpy._piconumpy_cpython_capi",
            ["piconumpy/_piconumpy_cpython_capi.c"],
        )
    ],
)
