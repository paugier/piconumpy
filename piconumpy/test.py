import numpy as np

from . import array


def test_init_array():

    a = array([1.0, 2.0])
    assert a.size == 2


def test_init_array_numpy():

    np_a = np.array([1.0, 2.0, 0.0, 0.0])

    a = array(np_a.tolist())
    assert a.size == np_a.size
    assert a.tolist() == np_a.tolist()
