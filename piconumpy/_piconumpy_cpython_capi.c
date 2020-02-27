#define PY_SSIZE_T_CLEAN
#include <Python.h>

typedef struct {
    PyObject_HEAD
    /* Type-specific fields go here. */
} ArrayObject;

static PyTypeObject ArrayType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "_piconumpy_cpython_capi.array",
    .tp_doc = "Array objects",
    .tp_basicsize = sizeof(ArrayObject),
    .tp_itemsize = 0,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = PyType_GenericNew,
};

static PyModuleDef piconumpymodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "piconumpy_cpython_capi",
    .m_doc = "piconumpy implemented with the CPython C-API.",
    .m_size = -1,
};

PyMODINIT_FUNC
PyInit__piconumpy_cpython_capi(void)
{
    PyObject *m;
    if (PyType_Ready(&ArrayType) < 0)
        return NULL;

    m = PyModule_Create(&piconumpymodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&ArrayType);
    if (PyModule_AddObject(m, "array", (PyObject *) &ArrayType) < 0) {
        Py_DECREF(&ArrayType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}