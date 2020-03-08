

__all__ = ["array"]

import array as _array


# class array(_array.array):

#     def __new__(cls, *args):
#         return super().__new__(cls, "f", *args)

#     def __init__(self, data):
#         self.size = len(self)

#     def __add__(self, other):
#         return self.__class__(number + other[index] for index, number in enumerate(self))

#     def __mul__(self, other):
#         return self.__class__(other * number for number in self)

#     __rmul__ = __mul__

#     def __truediv__(self, other):
#         return self.__class__(number / other for number in self)


class array:
    __slots__ = ["data", "size"]

    def __init__(self, data):
        self.data = _array.array("f", data)
        self.size = len(self.data)

    def __add__(self, other):
        return array(
            number + other.data[index] for index, number in enumerate(self.data)
        )

    def __mul__(self, other):
        return array(other * number for number in self.data)

    __rmul__ = __mul__

    def __truediv__(self, other):
        return array(number / other for number in self.data)

    def tolist(self):
        return list(self.data)

    def __len__(self):
        return len(self.data)

    def __getitem__(self, index):
        return self.data[index]
