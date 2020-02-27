from setuptools import setup, find_packages

setup(
    name="piconumpy",
    packages=find_packages(exclude=["bench"]),
    extras_require={"dev": ["black", "transonic", "numpy"]},
)
