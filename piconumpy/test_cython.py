from .test_cpython_capi import Tests as _Tests

class Tests(_Tests):
    import _piconumpy_cython as piconumpy
