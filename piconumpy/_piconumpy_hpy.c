#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#include "hpy.h"

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

HPyDef_SLOT(Array_multiply, HPy_nb_multiply, Array_multiply_impl,
            HPyFunc_BINARYFUNC)
static HPy Array_multiply_impl(HPyContext ctx, HPy h1, HPy h2) {
  int index;
  double number;
  HPy h_number = HPy_NULL;
  ArrayObject *result = NULL, *arr = NULL;

  if (HPyNumber_Check(ctx, h2)) {
    h_number = h2;
    arr = HPy_CAST(ctx, ArrayObject, h1);
  } else if (HPyNumber_Check(ctx, h1)) {
    h_number = h1;
    arr = HPy_CAST(ctx, ArrayObject, h2);
  }

  if (HPyNumber_Check(ctx, h1) || HPyNumber_Check(ctx, h2)) {
    number = HPyFloat_AsDouble(ctx, h_number);
    result = Array_empty(arr->size);
    for (index = 0; index < arr->size; index++) {
      result->data[index] = arr->data[index] * number;
    }
  }
  /* XXX exception if result is still NULL here */

  HPy h_result = HPy_FromPyObject(ctx, (PyObject *)result);
  Py_DECREF(result);
  return h_result;
};

HPyDef_SLOT(Array_add, HPy_nb_add, Array_add_impl, HPyFunc_BINARYFUNC)
static HPy Array_add_impl(HPyContext ctx, HPy h1, HPy h2) {
  int index;
  ArrayObject *result = NULL, *a1, *a2;
  a1 = HPy_CAST(ctx, ArrayObject, h1);
  a2 = HPy_CAST(ctx, ArrayObject, h2);

  if (a1->size != a2->size)
    return HPy_NULL;   /* XXX should raise an exception */

  result = Array_empty(a1->size);
  for (index = 0; index < a1->size; index++) {
    result->data[index] = a1->data[index] + a2->data[index];
  }

  HPy h_result = HPy_FromPyObject(ctx, (PyObject *)result);
  Py_DECREF(result);
  return h_result;
};

HPyDef_SLOT(Array_divide, HPy_nb_true_divide, Array_divide_impl, HPyFunc_BINARYFUNC)
static HPy Array_divide_impl(HPyContext ctx, HPy h1, HPy h2) {
  int index;
  double number;
  ArrayObject *result = NULL, *a1;

  if (!HPyNumber_Check(ctx, h2)) {
    return HPy_NULL;
  }
  a1 = HPy_CAST(ctx, ArrayObject, h1);
  number = HPyFloat_AsDouble(ctx, h2);
  result = Array_empty(a1->size);
  for (index = 0; index < a1->size; index++) {
    result->data[index] = a1->data[index] / number;
  }

  HPy h_result = HPy_FromPyObject(ctx, (PyObject *)result);
  Py_DECREF(result);
  return h_result;
};


HPyDef_SLOT(Array_length, HPy_sq_length, Array_length_impl, HPyFunc_LENFUNC)
HPy_ssize_t Array_length_impl(HPyContext ctx, HPy h_arr) {
  ArrayObject *arr = HPy_CAST(ctx, ArrayObject, h_arr);
  HPy_ssize_t result = (HPy_ssize_t)arr->size;
  return result;
};


HPyDef_SLOT(Array_item, HPy_sq_item, Array_item_impl, HPyFunc_SSIZEARGFUNC)
HPy Array_item_impl(HPyContext ctx, HPy h_arr, HPy_ssize_t index) {
  ArrayObject *arr = HPy_CAST(ctx, ArrayObject, h_arr);
  if (index < 0 || index >= arr->size) {
    return HPy_NULL;
  }
  HPy item = HPyFloat_FromDouble(ctx, arr->data[index]);
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
    {0, NULL},
};

static HPyDef *Array_defines[] = {
    &Array_add,
    &Array_multiply,
    &Array_divide,
    &Array_item,
    &Array_length,
    NULL
};

static HPyType_Spec Array_type_spec = {
    .name = "_piconumpy_hpy.array",
    .basicsize = sizeof(ArrayObject),
    .itemsize = 0,
    .flags = HPy_TPFLAGS_DEFAULT,
    .legacy_slots = Array_type_slots,
    .defines = Array_defines,
};

PyTypeObject *ptr_ArrayType;
HPy h_ArrayType;

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

/* XXX add the docstring: "Create an empty array" */
HPyDef_METH(empty, "empty", empty_impl, HPyFunc_O)
static HPy empty_impl(HPyContext ctx, HPy module, HPy arg) {
  int size;
  size = (int)HPyLong_AsLong(ctx, arg);
  PyObject *result = (PyObject *)Array_empty(size);
  HPy h_result = HPy_FromPyObject(ctx, result);
  Py_DECREF(result);
  return h_result;
};


static HPyDef *module_defines[] = {
    &empty,
    NULL
};

static HPyModuleDef piconumpymodule = {
    HPyModuleDef_HEAD_INIT,
    .m_name = "_piconumpy_hpy",
    .m_doc = "piconumpy implemented with the HPy API.",
    .m_size = -1,
    .defines = module_defines,
};

HPy_MODINIT(_piconumpy_hpy)
static HPy init__piconumpy_hpy_impl(HPyContext ctx) {
  HPy hm = HPyModule_Create(ctx, &piconumpymodule);
  if (HPy_IsNull(hm))
    return HPy_NULL;

  h_ArrayType = HPyType_FromSpec(ctx, &Array_type_spec);
  if (HPy_IsNull(h_ArrayType))
      return HPy_NULL;
  ptr_ArrayType = (PyTypeObject *)HPy_AsPyObject(ctx, h_ArrayType);

  if (HPy_SetAttr_s(ctx, hm, "array", h_ArrayType) < 0) {
    HPy_Close(ctx, h_ArrayType);
    Py_DECREF(ptr_ArrayType);
    HPy_Close(ctx, hm);
    return HPy_NULL;
  }

  return hm;
}
