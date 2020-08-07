#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "structmember.h"

#include "hpy.h"

typedef struct {
  HPyObject_HEAD
  /* Type-specific fields go here. */
  double *data;
  int size;
} ArrayObject;

HPyDef_SLOT(Array_destroy, HPy_tp_destroy, Array_destroy_impl, HPyFunc_DESTROYFUNC)
static void Array_destroy_impl(void *obj) {
  ArrayObject *self = (ArrayObject *)obj;
  free(self->data);
}

HPyDef_SLOT(Array_init, HPy_tp_init, Array_init_impl, HPyFunc_INITPROC)
static int Array_init_impl(HPyContext ctx, HPy h_self, HPy *args,
                           HPy_ssize_t nargs, HPy kw) {
  static const char *kwlist[] = {"data", NULL};
  ArrayObject *self = HPy_CAST(ctx, ArrayObject, h_self);
  int index;
  HPy h_data = HPy_NULL;

  if (!HPyArg_ParseKeywords(ctx, args, nargs, kw, "|O", kwlist, &h_data))
    return -1;

  if (!HPyList_Check(ctx, h_data)) {
    HPyErr_SetString(ctx, ctx->h_TypeError, "parameter must be a list");
    return -1;
  }

  self->size = (int)HPy_Length(ctx, h_data);

  self->data = (double *)malloc(self->size * sizeof(double));
  if (self->data == NULL) {
    HPyErr_NoMemory(ctx);
    return -1;
  }

  // XXX: this is not doing any error check (but the original C-API version
  // doesn't either :shrug:
  for (index = 0; index < self->size; index++) {
    HPy h_item = HPy_GetItem_i(ctx, h_data, index);
    self->data[index] = HPyFloat_AsDouble(ctx, h_item);
    HPy_Close(ctx, h_item);
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

static HPy Array_empty(HPyContext ctx, int size, ArrayObject **result);

HPyDef_SLOT(Array_multiply, HPy_nb_multiply, Array_multiply_impl,
            HPyFunc_BINARYFUNC)
static HPy Array_multiply_impl(HPyContext ctx, HPy h1, HPy h2) {
  int index;
  double number;
  HPy h_number = HPy_NULL;
  ArrayObject *result = NULL, *arr = NULL;
  HPy h_result = HPy_NULL;

  if (HPyNumber_Check(ctx, h2)) {
    h_number = h2;
    arr = HPy_CAST(ctx, ArrayObject, h1);
  } else if (HPyNumber_Check(ctx, h1)) {
    h_number = h1;
    arr = HPy_CAST(ctx, ArrayObject, h2);
  }

  if (HPyNumber_Check(ctx, h1) || HPyNumber_Check(ctx, h2)) {
    number = HPyFloat_AsDouble(ctx, h_number);
    h_result = Array_empty(ctx, arr->size, &result);
    for (index = 0; index < arr->size; index++) {
      result->data[index] = arr->data[index] * number;
    }
  }
  /* XXX exception if result is still NULL here */
  return h_result;
};

HPyDef_SLOT(Array_add, HPy_nb_add, Array_add_impl, HPyFunc_BINARYFUNC)
static HPy Array_add_impl(HPyContext ctx, HPy h1, HPy h2) {
  int index;
  ArrayObject *result = NULL, *a1, *a2;
  HPy h_result = HPy_NULL;
  a1 = HPy_CAST(ctx, ArrayObject, h1);
  a2 = HPy_CAST(ctx, ArrayObject, h2);

  if (a1->size != a2->size)
    return HPy_NULL;   /* XXX should raise an exception */

  h_result = Array_empty(ctx, a1->size, &result);
  for (index = 0; index < a1->size; index++) {
    result->data[index] = a1->data[index] + a2->data[index];
  }
  return h_result;
};

HPyDef_SLOT(Array_divide, HPy_nb_true_divide, Array_divide_impl, HPyFunc_BINARYFUNC)
static HPy Array_divide_impl(HPyContext ctx, HPy h1, HPy h2) {
  int index;
  double number;
  ArrayObject *result = NULL, *a1;
  HPy h_result = HPy_NULL;

  if (!HPyNumber_Check(ctx, h2)) {
    return HPy_NULL;
  }
  a1 = HPy_CAST(ctx, ArrayObject, h1);
  number = HPyFloat_AsDouble(ctx, h2);
  h_result = Array_empty(ctx, a1->size, &result);
  for (index = 0; index < a1->size; index++) {
    result->data[index] = a1->data[index] / number;
  }
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


HPyDef_SLOT(Array_new, HPy_tp_new, HPyType_GenericNew, HPyFunc_KEYWORDS)

static PyType_Slot Array_type_slots[] = {
    {Py_tp_members, Array_members},
    {Py_tp_methods, Array_methods},
    {0, NULL},
};

static HPyDef *Array_defines[] = {
    &Array_new,
    &Array_init,
    &Array_destroy,
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

static HPy Array_empty(HPyContext ctx, int size, ArrayObject **result) {
  ArrayObject *new_array;
  HPy h_new_array = HPy_New(ctx, h_ArrayType, &new_array);
  new_array->size = size;
  new_array->data = (double *)malloc(size * sizeof(double));
  if (new_array->data == NULL) {
     return HPyErr_NoMemory(ctx);
  }
  *result = new_array;
  return h_new_array;
};

/* XXX add the docstring: "Create an empty array" */
HPyDef_METH(empty, "empty", empty_impl, HPyFunc_O)
static HPy empty_impl(HPyContext ctx, HPy module, HPy arg) {
  int size;
  ArrayObject *result;
  size = (int)HPyLong_AsLong(ctx, arg);
  return Array_empty(ctx, size, &result);
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
