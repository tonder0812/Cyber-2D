#include "Python/Python.h"

PyModuleDef CyberModule = {
    PyModuleDef_HEAD_INIT,
    .m_name = "Cyber",
    .m_doc = "Cyber module.",
    .m_size = -1,
};

PyObject* PyInit_Cyber(void)
{
    PyObject* m;

    m = PyModule_Create(&CyberModule);
    if (m == NULL)
        return NULL;

    return m;
}