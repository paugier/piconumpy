import numpy as np

from . import array, sin, cos


def test_init_array():
    a = array([1.0, 2.0])
    assert a.size == 2


def test_init_array_numpy():
    np_a = np.array([1.0, 2.0, 0.0, 0.0])
    a = array(np_a.tolist())
    assert a.size == np_a.size
    assert a.tolist() == np_a.tolist()


def test_trigo():
    a = array([0, np.pi / 2, np.pi, -np.pi / 2])
    assert np.allclose(cos(a).tolist(), [1, 0, -1, 0])
    assert np.allclose(sin(a).tolist(), [0, 1, 0, -1])


def test_multiply():
    a = array([1.0, 2.0])
    assert (2 * a).tolist() == [2.0, 4.0]
    assert (a * 3).tolist() == [3.0, 6.0]


def test_add():
    a = array([1.0, 2.0])
    assert (a + 2 * a).tolist() == [3.0, 6.0]
