from math import cos, sin, pi

__all__ = ["cos", "sin", "array", "pi"]

import array as _array


class array(_array.array):

    def __new__(cls, *args):
        return super().__new__(cls, "f", *args)

    def __init__(self, data):
        self.size = len(self)

    def __add__(self, other):
        return self.__class__(number + other[index] for index, number in enumerate(self))

    def __mul__(self, other):
        return self.__class__(other * number for number in self)

    __rmul__ = __mul__

    def __truediv__(self, other):
        return self.__class__(number / other for number in self)
