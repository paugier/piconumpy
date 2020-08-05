from ._piconumpy_hpy import array
from .test_cpython_capi import Tests as _Tests


class Tests(_Tests):
    _array = array
