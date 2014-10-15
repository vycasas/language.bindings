#include "api_python.hxx"

#include <iomanip>
#include <iostream>
#include <memory>

#include <cxx/api.hxx>

#define BEGIN_EX_GUARD \
    try {
#define END_EX_GUARD \
    } \
    catch (CXXLib::Exception& e) { \
        PyErr_SetString(PyExc_Exception, e.getMessage().c_str()); \
        return (nullptr); \
    } \
    catch (std::exception& e) { \
        PyErr_SetString(PyExc_Exception, e.what()); \
        return (nullptr); \
    }

namespace PythonLibCore
{
    namespace Utils
    {
        // The Pybject* argument must be an Pythong long type
        template<typename T>
        inline T* GetCorePtr(PyObject* pyobj)
        {
            auto address = PyLong_AsUnsignedLongLong(pyobj);
            return (reinterpret_cast<T*>(address));
        }
    } // namespace Utils

    class PythonLibException final : public CXXLib::Exception
    {
    public:
        PythonLibException(void) : CXXLib::Exception()
        { return; }
        
        PythonLibException(CLibErrNum errNum) : CXXLib::Exception(errNum)
        { return; }
    }; // class Exception

    class PythonLibGeneratorImpl final : public CXXLib::GeneratorBase
    {
    public:
        PythonLibGeneratorImpl(PyObject* impl);

        virtual ~PythonLibGeneratorImpl(void) override;

        virtual int generateInt(int data) const override;
        virtual std::string generateString(int data) const override;

        PyObject* _impl;
    }; // class PythonLibGeneratorImpl

    PythonLibGeneratorImpl::PythonLibGeneratorImpl(PyObject* impl) : _impl(nullptr)
    {
        Py_INCREF(impl);
        _impl = impl;
        return;
    }

    PythonLibGeneratorImpl::~PythonLibGeneratorImpl(void)
    {
        if (_impl != nullptr) {
            Py_DECREF(_impl);
            _impl = nullptr;
        }
        return;
    }

    int PythonLibGeneratorImpl::generateInt(int data) const
    {
        char methodName[] = "generate_int";
        char argsFormat[] = "(i)";
        PyObject* callbackResult = PyObject_CallMethod(_impl, methodName, argsFormat, data);
        int result = static_cast<int>(PyInt_AsLong(callbackResult));
        Py_DECREF(callbackResult);
        return (result);
    }

    std::string PythonLibGeneratorImpl::generateString(int data) const
    {
        char methodName[] = "generate_string";
        char argsFormat[] = "(i)";
        PyObject* callbackResult = PyObject_CallMethod(_impl, methodName, argsFormat, data);
        std::string result(PyString_AsString(callbackResult));
        Py_DECREF(callbackResult);
        return (result);
    }
} // namespace PythonLibCore

extern "C"
{

static
PyMethodDef _G_METHODS[] =
{
    // Library
    { "library_initialize", PLLibraryInitialize, METH_NOARGS, nullptr },
    { "library_terminate", PLLibraryTerminate, METH_NOARGS, nullptr },

    // Address
    { "address_create", (PyCFunction) PLAddressCreate, METH_VARARGS | METH_KEYWORDS, nullptr },
    { "address_destroy", (PyCFunction) PLAddressDestroy, METH_VARARGS | METH_KEYWORDS, nullptr },
    { "address_get_street_num", (PyCFunction) PLAddressGetStreetNum, METH_VARARGS | METH_KEYWORDS, nullptr },
    { "address_get_street", (PyCFunction) PLAddressGetStreet, METH_VARARGS | METH_KEYWORDS, nullptr },
    { "address_get_city", (PyCFunction) PLAddressGetCity, METH_VARARGS | METH_KEYWORDS, nullptr },
    { "address_get_province", (PyCFunction) PLAddressGetProvince, METH_VARARGS | METH_KEYWORDS, nullptr },
    { "address_get_country", (PyCFunction) PLAddressGetProvince, METH_VARARGS | METH_KEYWORDS, nullptr },
    { "address_get_zip_code", (PyCFunction) PLAddressGetZipCode, METH_VARARGS | METH_KEYWORDS, nullptr },

    // Person
    { "person_create", (PyCFunction) PLPersonCreate, METH_VARARGS | METH_KEYWORDS, nullptr },
    { "person_destroy", (PyCFunction) PLPersonDestroy, METH_VARARGS | METH_KEYWORDS, nullptr },
    { "person_get_last_name", (PyCFunction) PLPersonGetLastName, METH_VARARGS | METH_KEYWORDS, nullptr },
    { "person_get_first_name", (PyCFunction) PLPersonGetFirstName, METH_VARARGS | METH_KEYWORDS, nullptr },
    { "person_get_age", (PyCFunction) PLPersonGetAge, METH_VARARGS | METH_KEYWORDS, nullptr },
    { "person_get_address", (PyCFunction) PLPersonGetAddress, METH_VARARGS | METH_KEYWORDS, nullptr },

    // Printer
    { "printer_create", (PyCFunction) PLPrinterCreate, METH_VARARGS | METH_KEYWORDS, nullptr },
    { "printer_destroy", (PyCFunction) PLPrinterDestroy, METH_VARARGS | METH_KEYWORDS, nullptr },
    { "printer_print_int", (PyCFunction) PLPrinterPrintInt, METH_VARARGS | METH_KEYWORDS, nullptr },
    { "printer_print_string", (PyCFunction) PLPrinterPrintString, METH_VARARGS | METH_KEYWORDS, nullptr },

    { nullptr, nullptr, 0, nullptr }
};

PYTHONLIB_API
PyMODINIT_FUNC init_pythonlib_native(void)
{
    Py_InitModule("_pythonlib_native", _G_METHODS);
    return;
}

PYTHONLIB_API
PyObject* PLLibraryInitialize(PyObject*, PyObject*)
{
    CXXLib::Library::initialize();
    return (Py_BuildValue("i", 0));
}

PYTHONLIB_API
PyObject* PLLibraryTerminate(PyObject*, PyObject*)
{
    CXXLib::Library::terminate();
    return (Py_BuildValue("i", 0));
}
    
PYTHONLIB_API
PyObject* PLAddressCreate(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = {
        "street_num", "street", "city", "province", "country", "zip_code", nullptr
    };
    int pyResult = 1;
    int streetNum = 0;
    char* street;
    char* city;
    char* province;
    char* country;
    char* zipCode;

    BEGIN_EX_GUARD;
    pyResult = PyArg_ParseTupleAndKeywords(
        args, keywords, "isssss", (char**) keyword_list,
        &streetNum, &street, &city, &province, &country, &zipCode
    );

    if (pyResult == 0) {
        throw (PythonLibCore::PythonLibException(1));
    }
    
    std::unique_ptr<CXXLib::Address> address(
        new CXXLib::Address(
            streetNum, street, city, province, country, zipCode
        )
    );

    PyObject* result = Py_BuildValue("K", reinterpret_cast<unsigned PY_LONG_LONG>(address.get()));
    address.release();

    return (result);
    END_EX_GUARD;
}
    
PYTHONLIB_API
PyObject* PLAddressDestroy(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = { "impl", nullptr };
    PyObject* coreAddress;

    PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &coreAddress
    );

    std::unique_ptr<CXXLib::Address> ownedImpl(
        PythonLibCore::Utils::GetCorePtr<CXXLib::Address>(coreAddress)
    );

    return (Py_BuildValue("i", 0));
}

PYTHONLIB_API
PyObject* PLAddressGetStreetNum(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = { "impl", nullptr };
    PyObject* coreAddress;

    BEGIN_EX_GUARD;

    PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &coreAddress
    );

    auto* coreAddressPtr = PythonLibCore::Utils::GetCorePtr<CXXLib::Address>(coreAddress);

    return (Py_BuildValue("i", coreAddressPtr->getStreetNum()));

    END_EX_GUARD;
}

PYTHONLIB_API
PyObject* PLAddressGetStreet(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = { "impl", nullptr };
    PyObject* coreAddress;

    BEGIN_EX_GUARD;

    PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &coreAddress
    );

    auto* coreAddressPtr = PythonLibCore::Utils::GetCorePtr<CXXLib::Address>(coreAddress);

    return (Py_BuildValue("s", coreAddressPtr->getStreet().c_str()));

    END_EX_GUARD;
}

PYTHONLIB_API
PyObject* PLAddressGetCity(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = { "impl", nullptr };
    PyObject* coreAddress;

    BEGIN_EX_GUARD;

    PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &coreAddress
    );

    auto* coreAddressPtr = PythonLibCore::Utils::GetCorePtr<CXXLib::Address>(coreAddress);

    return (Py_BuildValue("s", coreAddressPtr->getCity().c_str()));

    END_EX_GUARD;
}

PYTHONLIB_API
PyObject* PLAddressGetProvince(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = { "impl", nullptr };
    PyObject* coreAddress;

    BEGIN_EX_GUARD;

    PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &coreAddress
    );

    auto* coreAddressPtr = PythonLibCore::Utils::GetCorePtr<CXXLib::Address>(coreAddress);

    return (Py_BuildValue("s", coreAddressPtr->getProvince().c_str()));

    END_EX_GUARD;
}

PYTHONLIB_API
PyObject* PLAddressGetCountry(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = { "impl", nullptr };
    PyObject* coreAddress;

    BEGIN_EX_GUARD;

    PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &coreAddress
    );

    auto* coreAddressPtr = PythonLibCore::Utils::GetCorePtr<CXXLib::Address>(coreAddress);

    return (Py_BuildValue("s", coreAddressPtr->getCountry().c_str()));

    END_EX_GUARD;
}

PYTHONLIB_API
PyObject* PLAddressGetZipCode(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = { "impl", nullptr };
    PyObject* coreAddress;

    BEGIN_EX_GUARD;

    PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &coreAddress
    );

    auto* coreAddressPtr = PythonLibCore::Utils::GetCorePtr<CXXLib::Address>(coreAddress);

    return (Py_BuildValue("s", coreAddressPtr->getZipCode().c_str()));

    END_EX_GUARD;
}

PYTHONLIB_API
PyObject* PLPersonCreate(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = {
        "last_name", "first_name", "age", "address_impl", nullptr
    };
    int pyResult = 1;
    char* lastName;
    char* firstName;
    int age;
    PyObject* coreAddress;

    BEGIN_EX_GUARD;
    pyResult = PyArg_ParseTupleAndKeywords(
        args, keywords, "ssiO", (char**) keyword_list,
        &lastName, &firstName, &age, &coreAddress
    );

    if (pyResult == 0) {
        throw (PythonLibCore::PythonLibException(1));
    }

    auto* coreAddressPtr = PythonLibCore::Utils::GetCorePtr<CXXLib::Address>(coreAddress);

    std::unique_ptr<CXXLib::Person> person(
        new CXXLib::Person(
            lastName, firstName, age, *coreAddressPtr
        )
    );

    PyObject* result = Py_BuildValue("K", reinterpret_cast<unsigned PY_LONG_LONG>(person.get()));
    person.release();

    return (result);
    END_EX_GUARD;
}

PYTHONLIB_API
PyObject* PLPersonDestroy(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = { "impl", nullptr };
    PyObject* corePerson;

    PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &corePerson
    );

    std::unique_ptr<CXXLib::Person> ownedImpl(
        PythonLibCore::Utils::GetCorePtr<CXXLib::Person>(corePerson)
    );

    return (Py_BuildValue("i", 0));
}

PYTHONLIB_API
PyObject* PLPersonGetLastName(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = { "impl", nullptr };
    PyObject* corePerson;

    BEGIN_EX_GUARD;

    PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &corePerson
    );

    auto* corePersonPtr = PythonLibCore::Utils::GetCorePtr<CXXLib::Person>(corePerson);

    return (Py_BuildValue("s", corePersonPtr->getLastName().c_str()));

    END_EX_GUARD;
}

PYTHONLIB_API
PyObject* PLPersonGetFirstName(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = { "impl", nullptr };
    PyObject* corePerson;

    BEGIN_EX_GUARD;

    PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &corePerson
    );

    auto* corePersonPtr = PythonLibCore::Utils::GetCorePtr<CXXLib::Person>(corePerson);

    return (Py_BuildValue("s", corePersonPtr->getFirstName().c_str()));

    END_EX_GUARD;
}

PYTHONLIB_API
PyObject* PLPersonGetAge(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = { "impl", nullptr };
    PyObject* corePerson;

    BEGIN_EX_GUARD;

    PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &corePerson
    );

    auto* corePersonPtr = PythonLibCore::Utils::GetCorePtr<CXXLib::Person>(corePerson);

    return (Py_BuildValue("i", corePersonPtr->getAge()));

    END_EX_GUARD;
}

PYTHONLIB_API
PyObject* PLPersonGetAddress(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = { "impl", nullptr };
    PyObject* corePerson;

    BEGIN_EX_GUARD;

    PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &corePerson
    );

    auto* corePersonPtr = PythonLibCore::Utils::GetCorePtr<CXXLib::Person>(corePerson);

    auto coreAddress = corePersonPtr->getAddress();

    std::unique_ptr<CXXLib::Address> address(
        new CXXLib::Address(
            coreAddress.getStreetNum(), coreAddress.getStreet(),
            coreAddress.getCity(), coreAddress.getProvince(),
            coreAddress.getCountry(), coreAddress.getZipCode()
        )
    );

    PyObject* result = Py_BuildValue("K", reinterpret_cast<unsigned PY_LONG_LONG>(address.get()));
    address.release();

    return (result);

    END_EX_GUARD;
}

PYTHONLIB_API
PyObject* PLPrinterCreate(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = {
        "generator", nullptr
    };
    int pyResult = 1;
    PyObject* generatorInstance;

    BEGIN_EX_GUARD;
    pyResult = PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &generatorInstance
    );

    if (pyResult == 0) {
        throw (PythonLibCore::PythonLibException(1));
    }

    std::unique_ptr<PythonLibCore::PythonLibGeneratorImpl> generator(
        new PythonLibCore::PythonLibGeneratorImpl(generatorInstance)
    );

    std::unique_ptr<CXXLib::Printer> printer(new CXXLib::Printer(std::move(generator)));

    PyObject* result = Py_BuildValue("K", reinterpret_cast<unsigned PY_LONG_LONG>(printer.get()));
    printer.release();

    return (result);
    END_EX_GUARD;
}

PYTHONLIB_API
PyObject* PLPrinterDestroy(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = { "impl", nullptr };
    PyObject* corePrinter;

    PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &corePrinter
    );

    std::unique_ptr<CXXLib::Printer> ownedImpl(
        PythonLibCore::Utils::GetCorePtr<CXXLib::Printer>(corePrinter)
    );

    return (Py_BuildValue("i", 0));
}

PYTHONLIB_API
PyObject* PLPrinterPrintInt(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = { "impl", nullptr };
    PyObject* corePrinter;

    BEGIN_EX_GUARD;

    PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &corePrinter
    );

    auto* corePrinterPtr = PythonLibCore::Utils::GetCorePtr<CXXLib::Printer>(corePrinter);
    corePrinterPtr->printInt();

    return (Py_BuildValue("i", 0));

    END_EX_GUARD;
}

PYTHONLIB_API
PyObject* PLPrinterPrintString(PyObject*, PyObject* args, PyObject* keywords)
{
    const char* const keyword_list[] = { "impl", nullptr };
    PyObject* corePrinter;

    BEGIN_EX_GUARD;

    PyArg_ParseTupleAndKeywords(
        args, keywords, "O", (char**) keyword_list, &corePrinter
    );

    auto* corePrinterPtr = PythonLibCore::Utils::GetCorePtr<CXXLib::Printer>(corePrinter);
    corePrinterPtr->printString();

    return (Py_BuildValue("i", 0));

    END_EX_GUARD;
}

} // extern "C"
