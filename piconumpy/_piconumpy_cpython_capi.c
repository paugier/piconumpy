#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "structmember.h"

typedef struct {
  PyObject_HEAD
      /* Type-specific fields go here. */
      double *data;
  int size;
} ArrayObject;

static void Array_dealloc(ArrayObject *self) {
  free(self->data);
  Py_TYPE(self)->tp_free((PyObject *)self);
}

static int Array_init(ArrayObject *self, PyObject *args, PyObject *kwds) {
  static char *kwlist[] = {"data", NULL};
  int index;
  PyObject *data = NULL, *item;

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist, &data))
    return -1;

  if (!PyList_Check(data)) {
    PyErr_SetString(PyExc_TypeError, "parameter must be a list");
    return -1;
  }

  self->size = (int)PyList_Size(data);

  self->data = (double *)malloc(self->size * sizeof(double));
  if (self->data == NULL) {
    PyErr_NoMemory();
    return -1;
  }

  for (index = 0; index < self->size; index++) {
    item = PyList_GET_ITEM(data, index);
    self->data[index] = PyFloat_AsDouble(item);
  }

  return 0;
}

static PyMemberDef Array_members[] = {
    {"size", T_INT, offsetof(ArrayObject, size), 0, "size of the array"},
    {NULL} /* Sentinel */
};

static PyObject *Array_tolist(ArrayObject *self, PyObject *Py_UNUSED(ignored)) {
  int index;
  PyObject *result, *item;
  result = PyList_New(self->size);
  for (index = 0; index < self->size; index++) {
    item = PyFloat_FromDouble(self->data[index]);
    PyList_SetItem(result, index, item);
  }
  return result;
};

static ArrayObject *Array_empty(int size);

static ArrayObject *Array_multiply(PyObject *o1, PyObject *o2) {
  int index;
  double number;
  PyObject *obj_number = NULL;
  ArrayObject *result = NULL, *arr = NULL;

  if (PyNumber_Check(o2)) {
    obj_number = o2;
    arr = (ArrayObject *)o1;
  } else if (PyNumber_Check(o1)) {
    obj_number = o1;
    arr = (ArrayObject *)o2;
  }

  if (PyNumber_Check(o1) | PyNumber_Check(o2)) {
    number = PyFloat_AsDouble(obj_number);
    result = Array_empty(arr->size);
    for (index = 0; index < arr->size; index++) {
      result->data[index] = arr->data[index] * number;
    }
  }

  return result;
};

static ArrayObject *Array_add(PyObject *o1, PyObject *o2) {
  int index;
  ArrayObject *result = NULL, *a1, *a2;
  a1 = (ArrayObject *)o1;
  a2 = (ArrayObject *)o2;

  if (a1->size != a2->size)
    return result;

  result = Array_empty(a1->size);
  for (index = 0; index < a1->size; index++) {
    result->data[index] = a1->data[index] + a2->data[index];
  }

  return result;
};

static ArrayObject *Array_divide(PyObject *o1, PyObject *o2) {
  int index;
  double number;
  ArrayObject *result = NULL, *a1;

  if (!PyNumber_Check(o2)) {
    return result;
  }
  a1 = (ArrayObject *)o1;
  number = PyFloat_AsDouble(o2);
  result = Array_empty(a1->size);
  for (index = 0; index < a1->size; index++) {
    result->data[index] = a1->data[index] / number;
  }

  return result;
};

Py_ssize_t Array_length(ArrayObject *arr) {
  Py_ssize_t result = (Py_ssize_t)arr->size;
  return result;
};

PyObject *Array_item(ArrayObject *arr, Py_ssize_t index) {
  PyObject *item = NULL;
  if (index < 0 || index >= arr->size) {
    return item;
  }
  item = PyFloat_FromDouble(arr->data[index]);
  return item;
};

static PyMethodDef Array_methods[] = {
    {"tolist", (PyCFunction)Array_tolist, METH_NOARGS,
     "Return the data as a list"},
    {NULL} /* Sentinel */
};

static PyType_Slot Array_type_slots[] = {
    {Py_tp_new, PyType_GenericNew},
    {Py_tp_init, (initproc)Array_init},
    {Py_tp_dealloc, (destructor)Array_dealloc},
    {Py_tp_members, Array_members},
    {Py_tp_methods, Array_methods},
    {Py_nb_multiply, (binaryfunc)Array_multiply},
    {Py_nb_add, (binaryfunc)Array_add},
    {Py_nb_true_divide, (binaryfunc)Array_divide},
    {Py_sq_length, (lenfunc)Array_length},
    {Py_sq_item, (ssizeargfunc)Array_item},
    {0, NULL},
};

static PyType_Spec Array_type_spec = {
    .name = "_piconumpy_cpython_capi.array",
    .basicsize = sizeof(ArrayObject),
    .itemsize = 0,
    .flags = Py_TPFLAGS_DEFAULT,
    .slots = Array_type_slots,
};

PyTypeObject *ptr_ArrayType;

static ArrayObject *Array_empty(int size) {
  ArrayObject *new_array = NULL;
  new_array = PyObject_New(ArrayObject, ptr_ArrayType);
  new_array->size = size;
  new_array->data = (double *)malloc(size * sizeof(double));
  if (new_array->data == NULL) {
     PyErr_NoMemory();
     return NULL;
  }
  return new_array;
};

static ArrayObject *empty(PyObject *module, PyObject *arg) {
  int size;
  size = (int)PyLong_AsLong(arg);
  return Array_empty(size);
};

static ArrayObject *zeros(PyObject *module, PyObject *arg) {
  int size;
  size = (int)PyLong_AsLong(arg);
  ArrayObject *result = Array_empty(size);
  for(int i=0; i<size; i++)
      result->data[i] = 0;
  return result;
};


static PyMethodDef module_methods[] = {
    {"empty", (PyCFunction)empty, METH_O, "Create an empty array."},
    {"zeros", (PyCFunction)zeros, METH_O, "Createa zero-filled array."},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static PyModuleDef piconumpymodule = {
    PyModuleDef_HEAD_INIT, .m_name = "_piconumpy_cpython_capi",
    .m_doc = "piconumpy implemented with the CPython C-API.", .m_size = -1,
    module_methods};

PyMODINIT_FUNC PyInit__piconumpy_cpython_capi(void) {
  PyObject *m;

  m = PyModule_Create(&piconumpymodule);
  if (m == NULL)
    return NULL;

  ptr_ArrayType = (PyTypeObject *)PyType_FromSpec(&Array_type_spec);
  if (PyModule_AddObject(m, "array", (PyObject *)ptr_ArrayType) < 0) {
    Py_DECREF(ptr_ArrayType);
    Py_DECREF(m);
    return NULL;
  }

  return m;
}
