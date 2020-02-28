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
  PyObject *data = NULL, *tmp, *item;

  // PyObject *builtins = PyEval_GetBuiltins();
  // PyObject *print = PyDict_GetItemString(builtins, "print");

  if (!PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist, &data))
    return -1;

  if (!PyList_Check(data)) {
    PyErr_SetString(PyExc_TypeError, "parameter must be a list");
    return NULL;
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

static PyMethodDef Array_methods[] = {
    {"tolist", (PyCFunction)Array_tolist, METH_NOARGS,
     "Return the data as a list"},
    {NULL} /* Sentinel */
};

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
};

static PyModuleDef piconumpymodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "_piconumpy_cpython_capi",
    .m_doc = "piconumpy implemented with the CPython C-API.",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit__piconumpy_cpython_capi(void) {
  PyObject *m;
  if (PyType_Ready(&ArrayType) < 0)
    return NULL;

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
