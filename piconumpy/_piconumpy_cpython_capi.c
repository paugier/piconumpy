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

  // PyObject *builtins = PyEval_GetBuiltins();
  // PyObject *print = PyDict_GetItemString(builtins, "print");

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist, &data))
    return -1;

  if (!PyList_Check(data)) {
    PyErr_SetString(PyExc_TypeError, "parameter must be a list");
    return -1;
  }

  // PyEval_CallFunction(print, "(O)", data);

  self->size = (int)PyList_Size(data);

  self->data = (double *)malloc(self->size * sizeof(double));
  if (self->data == NULL)
    return PyErr_NoMemory();

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

static PyMethodDef Array_methods[] = {
    {"tolist", (PyCFunction)Array_tolist, METH_NOARGS,
     "Return the data as a list"},
    {NULL} /* Sentinel */
};

static PyNumberMethods number_methods = {NULL};
static PySequenceMethods sequence_methods = {NULL};

static PyTypeObject ArrayType = {
    PyVarObject_HEAD_INIT(NULL, 0)

        .tp_name = "_piconumpy_cpython_capi.array",
    .tp_doc = "Array objects",
    .tp_basicsize = sizeof(ArrayObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
    .tp_init = (initproc)Array_init,
    .tp_dealloc = (destructor)Array_dealloc,
    .tp_members = Array_members,
    .tp_methods = Array_methods,
    .tp_as_number = &number_methods,
    .tp_as_sequence = &sequence_methods,
};

static ArrayObject *Array_empty(int size) {
  ArrayObject *new_array = NULL;
  new_array = PyObject_New(ArrayObject, &ArrayType);
  new_array->size = size;
  new_array->data = (double *)malloc(size * sizeof(double));
  if (new_array->data == NULL)
    return PyErr_NoMemory();
  return new_array;
};

static ArrayObject *empty(PyObject *module, PyObject *arg) {
  int size;
  size = (int)PyLong_AsLong(arg);
  return Array_empty(size);
};

static PyObject *module_cos(PyObject *module, PyObject *arg) {
  PyObject *result = NULL;
  if (PyNumber_Check(arg)) {
    result = PyFloat_FromDouble(cos(PyFloat_AsDouble(arg)));
  }
  return result;
};

static PyObject *module_sin(PyObject *module, PyObject *arg) {
  PyObject *result = NULL;
  if (PyNumber_Check(arg)) {
    result = PyFloat_FromDouble(sin(PyFloat_AsDouble(arg)));
  }
  return result;
};

static ArrayObject *Array_multiply(PyObject *o1, PyObject *o2) {
  int index;
  double number;
  PyObject *obj_number;
  ArrayObject *result = NULL, *arr;

  if (PyNumber_Check(o2)) {
    obj_number = o2;
    arr = o1;
  } else if (PyNumber_Check(o1)) {
    obj_number = o1;
    arr = o2;
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
  a1 = o1;
  a2 = o2;

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
  a1 = o1;
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

PyFloatObject *Array_item(ArrayObject *arr, Py_ssize_t index) {
  PyFloatObject *item = NULL;
  if (index < 0 | index >= arr->size) {
    return item;
  }
  item = PyFloat_FromDouble(arr->data[index]);
  return item;
};

static PyMethodDef module_methods[] = {
    {"empty", (PyCFunction)empty, METH_O, "Create an empty array."},
    {"cos", (PyCFunction)module_cos, METH_O, "cosinus."},
    {"sin", (PyCFunction)module_sin, METH_O, "sinus."},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static PyModuleDef piconumpymodule = {
    PyModuleDef_HEAD_INIT, .m_name = "_piconumpy_cpython_capi",
    .m_doc = "piconumpy implemented with the CPython C-API.", .m_size = -1,
    module_methods};

PyMODINIT_FUNC PyInit__piconumpy_cpython_capi(void) {
  PyObject *m;
  if (PyType_Ready(&ArrayType) < 0)
    return NULL;

  ArrayType.tp_as_number->nb_multiply = (binaryfunc)Array_multiply;
  ArrayType.tp_as_number->nb_add = (binaryfunc)Array_add;
  ArrayType.tp_as_number->nb_true_divide = (binaryfunc)Array_divide;

  ArrayType.tp_as_sequence->sq_length = (lenfunc)Array_length;
  ArrayType.tp_as_sequence->sq_item = (ssizeargfunc)Array_item;

  m = PyModule_Create(&piconumpymodule);
  if (m == NULL)
    return NULL;

  Py_INCREF(&ArrayType);
  if (PyModule_AddObject(m, "array", (PyObject *)&ArrayType) < 0) {
    Py_DECREF(&ArrayType);
    Py_DECREF(m);
    return NULL;
  }

  return m;
}
