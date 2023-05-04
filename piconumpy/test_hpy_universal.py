import pytest


from .test_cpython_capi import Tests as _Tests

try:
    from . import _piconumpy_hpy
except ImportError:
    _piconumpy_hpy = False


@pytest.mark.skipif(
    not _piconumpy_hpy, reason="ImportError piconumpy HPy Universal"
)
class TestsCPyABI(_Tests):
    piconumpy = _piconumpy_hpy
