import numpy as np

from . import _piconumpy_cpython_capi


class Tests:
    piconumpy = _piconumpy_cpython_capi
    def _array(self, *args):
        return self.piconumpy.array(*args)

    def test_empty(self):
        a = self.piconumpy.empty(12)
        assert type(a) is self.piconumpy.array
        assert a.size == 12

    def test_zeros(self):
        a = self.piconumpy.zeros(5)
        assert type(a) is self.piconumpy.array
        assert a.size == 5
        assert a.tolist() == [0, 0, 0, 0, 0]

    def test_init_array(self):
        a = self._array([1.0, 2.0])
        assert a.size == 2

    def test_init_array_numpy(self):
        np_a = np.array([1.0, 2.0, 0.0, 0.0])
        a = self._array(np_a.tolist())
        assert a.size == np_a.size
        assert a.tolist() == np_a.tolist()

    def test_multiply(self):
        a = self._array([1.0, 2.0])
        assert (2 * a).tolist() == [2.0, 4.0]
        assert (a * 3).tolist() == [3.0, 6.0]

    def test_add(self):
        a = self._array([1.0, 2.0])
        assert (a + 2 * a).tolist() == [3.0, 6.0]

    def test_divide(self):
        a = self._array([1.0, 2.0])
        assert (a / 2).tolist() == [0.5, 1.0]

    def test_sequence(self):
        a = self._array([1.0, 2.0])
        assert len(a) == 2
        assert a[1] == 2.0
