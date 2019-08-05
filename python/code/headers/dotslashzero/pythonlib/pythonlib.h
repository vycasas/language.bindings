#pragma once

#define PY_SSIZE_T_CLEAN
#if defined(_MSC_VER)
    #pragma warning(push, 3)
#endif /* defined(_MSC_VER) */
#include <Python.h>
#if defined(_MSC_VER)
    #pragma warning(pop)
#endif /* defined(_MSC_VER) */

#if (PY_MAJOR_VERSION < 3)
    #error Python3 required for building pythonlib
#endif /* (PY_MAJOR_VERSION < 3) */

#define DSZ_PYTHONLIB_MODULE_VISIBILITY __attribute__ ((visibility ("default")))
#if defined(_MSC_VER)
    #undef DSZ_PYTHONLIB_MODULE_VISIBILITY
    #define DSZ_PYTHONLIB_MODULE_VISIBILITY __declspec(dllexport)
#endif /* defined(_MSC_VER) */

#define DSZ_PYTHONLIB_CALLING_CONVENTION
#if defined(_MSC_VER)
    #undef DSZ_PYTHONLIB_CALLING_CONVENTION
    #define DSZ_PYTHONLIB_CALLING_CONVENTION __stdcall
#endif /* defined(_MSC_VER) */

#if defined(__cplusplus)
    #define DSZ_PYTHONLIB_API(rt) \
        extern "C" \
        DSZ_PYTHONLIB_MODULE_VISIBILITY rt DSZ_PYTHONLIB_CALLING_CONVENTION
#else /* defined(__cplusplus) */
    #define DSZ_PYTHONLIB_API(rt) \
        DSZ_PYTHONLIB_MODULE_VISIBILITY rt DSZ_PYTHONLIB_CALLING_CONVENTION
#endif /* defined(__cplusplus) */

#if !defined(_MSC_VER)
DSZ_PYTHONLIB_MODULE_VISIBILITY
#endif /* !defined(_MSC_VER) */
PyMODINIT_FUNC DSZ_PYTHONLIB_CALLING_CONVENTION PyInit_pythonlib_native(void);

#if !defined(_MSC_VER)
DSZ_PYTHONLIB_MODULE_VISIBILITY
#endif /* !defined(_MSC_VER) */
PyMODINIT_FUNC DSZ_PYTHONLIB_CALLING_CONVENTION PyInitU_pythonlib_native(void);

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibLibraryInitialize(
        PyObject* pSelf,
        PyObject* pArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibLibraryUninitialize(
        PyObject* pSelf,
        PyObject* pArgs);

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibLibraryGetVersionString(
        PyObject* pSelf,
        PyObject* pArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibLibraryGetVersionMajor(
        PyObject* pSelf,
        PyObject* pArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibLibraryGetVersionMinor(
        PyObject* pSelf,
        PyObject* pArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibLibraryGetVersionPatch(
        PyObject* pSelf,
        PyObject* pArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibLibraryGetVersionExtra(
        PyObject* pSelf,
        PyObject* pArgs);

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressCreate(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressDestroy(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressGetStreetNum(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressGetStreet(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressGetCity(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressGetProvince(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressGetZipCode(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressGetCountry(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibAddressToString(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPersonCreate(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPersonDestroy(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPersonGetLastName(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPersonGetFirstName(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPersonGetAge(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPersonGetAddress(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPersonToString(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);

DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPrinterCreate(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPrinterDestroy(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPrinterPrintInt(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
DSZ_PYTHONLIB_API(PyObject*) DszPythonLibPrinterPrintString(
        PyObject* pSelf,
        PyObject* pArgs,
        PyObject* pKwArgs);
