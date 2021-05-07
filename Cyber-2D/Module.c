#include "Python/Python.h"

PyModuleDef Matrixmodule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "matrix",
    .m_doc = "Matrix module.",
    .m_size = -1,
};

PyObject* PyInit_matrix(void)
{
    PyObject* m;
    if (PyType_Ready(&MatrixType) < 0)
        return NULL;
    if (PyType_Ready(&MatrixRowType) < 0)
        return NULL;

    m = PyModule_Create(&Matrixmodule);
    if (m == NULL)
        return NULL;

    Py_INCREF(&MatrixType);
    if (PyModule_AddObject(m, "Matrix4", (PyObject*)&MatrixType) < 0) {
        Py_DECREF(&MatrixType);
        Py_DECREF(m);
        return NULL;
    }

    Py_INCREF(&MatrixRowType);
    if (PyModule_AddObject(m, "Matrix4Row", (PyObject*)&MatrixRowType) < 0) {
        Py_DECREF(&MatrixType);
        Py_DECREF(&MatrixRowType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}

PyModuleDef Matrixmodule2 = {
    PyModuleDef_HEAD_INIT,
    .m_name = "matrix.matrix2",
    .m_doc = "Matrix module.",
    .m_size = -1,
};

PyObject* PyInit_matrix2(void)
{
    PyObject* m;
    if (PyType_Ready(&MatrixType) < 0)
        return NULL;
    if (PyType_Ready(&MatrixRowType) < 0)
        return NULL;

    m = PyModule_Create(&Matrixmodule2);
    if (m == NULL)
        return NULL;

    Py_INCREF(&MatrixType);
    if (PyModule_AddObject(m, "Matrix4", (PyObject*)&MatrixType) < 0) {
        Py_DECREF(&MatrixType);
        Py_DECREF(m);
        return NULL;
    }

    Py_INCREF(&MatrixRowType);
    if (PyModule_AddObject(m, "Matrix4Row", (PyObject*)&MatrixRowType) < 0) {
        Py_DECREF(&MatrixType);
        Py_DECREF(&MatrixRowType);
        Py_DECREF(m);
        return NULL;
    }

    return m;
}