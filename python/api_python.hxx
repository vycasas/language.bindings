#if !defined(PYTHONLIB_API_HXX)
#define PYTHONLIB_API_HXX

#include <Python.h>

#define PYTHONLIB_API __attribute__ ((visibility ("default")))
#if defined(_MSC_VER)
    #undef PYTHONLIB_API
    #define PYTHONLIB_API __declspec(dllexport)
#endif // defined(_MSC_VER)

#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

PYTHONLIB_API
PyMODINIT_FUNC initpythonlib(void);

PYTHONLIB_API
PyObject* PLLibraryInitialize(PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLLibraryTerminate(PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLLibraryGetVersionString(PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLLibraryGetVersionMajor(PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLLibraryGetVersionMinor(PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLAddressCreate(PyObject*, PyObject*, PyObject*);
    
PYTHONLIB_API
PyObject* PLAddressDestroy(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLAddressGetStreetNum(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLAddressGetStreet(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLAddressGetCity(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLAddressGetProvince(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLAddressGetCountry(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLAddressGetZipCode(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLPersonCreate(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLPersonDestroy(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLPersonGetLastName(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLPersonGetFirstName(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLPersonGetAge(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLPersonGetAddress(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLPrinterCreate(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLPrinterDestroy(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLPrinterPrintInt(PyObject*, PyObject*, PyObject*);

PYTHONLIB_API
PyObject* PLPrinterPrintString(PyObject*, PyObject*, PyObject*);

#if defined(__cplusplus)
} // extern "C"
#endif // defined(__cplusplus)

#endif // !defined(PYTHONLIB_API_HXX)
