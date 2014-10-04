// Compile with C++ compiler

#include "api_java.hxx"

#include <algorithm>
#include <codecvt>
#include <cstdint>
#include <locale>
#include <memory>

#include <cxx/api.hxx>

// helper macros
#define GET_CORE_CLASS_PTR(impl, T) \
    reinterpret_cast<T*>(static_cast<std::uintptr_t>(impl))

#define GET_CORE_EXCEPTION_PTR(impl) GET_CORE_CLASS_PTR(impl, CXXLib::Exception)
#define GET_CORE_ADDRESS_PTR(impl) GET_CORE_CLASS_PTR(impl, CXXLib::Address)
#define GET_CORE_PERSON_PTR(impl) GET_CORE_CLASS_PTR(impl, CXXLib::Person)
#define GET_CORE_PRINTER_PTR(impl) GET_CORE_CLASS_PTR(impl, CXXLib::Printer)

#define BEGIN_EX_GUARD(jenv) \
    try {
#define END_EX_GUARD(jenv) \
    } \
    catch (CXXLib::Exception& e) { \
        jthrowable jniEx = JavaLibCore::MakeJNIException(jenv, e); \
        if (jniEx == nullptr) \
            return (2); \
        jenv->Throw(jniEx); \
    } \
    catch (std::exception& e) { \
        jthrowable jniEx = JavaLibCore::MakeJNIException(jenv, CXXLib::Exception(e)); \
        if (jniEx == nullptr) \
            return (2); \
        jenv->Throw(jniEx); \
    }

namespace JavaLibCore
{
    // utility wrapper to adapt locale-bound facets for wstring/wbuffer convert
    template<class facetT>
    struct deletable_facet : facetT
    {
        template<class... argsT>
        deletable_facet(argsT&&... args) : facetT(std::forward<argsT>(args)...) { return; }
        ~deletable_facet(void) { return; }
    };

    jthrowable MakeJNIException(JNIEnv* jenv, const CXXLib::Exception& e)
    {
        jclass javaLibExceptionClass = jenv->FindClass("net/dotslashzero/javalib/JavaLibException");

        if (javaLibExceptionClass == nullptr)
            return (nullptr);

        jmethodID constructorID = jenv->GetMethodID(javaLibExceptionClass, "<init>", "(J)V");

        if (constructorID == nullptr)
            return (nullptr);

        std::unique_ptr<CXXLib::Exception> coreExPtr(new CXXLib::Exception(e));

        jthrowable result = static_cast<jthrowable>(
            jenv->NewObject(
                javaLibExceptionClass,
                constructorID,
                reinterpret_cast<jlong>(coreExPtr.get())
            )
        );

        if (result != nullptr)
            coreExPtr.release();

        return (result);
    }

    class JavaLibException final : public CXXLib::Exception
    {
    public:
        JavaLibException(void) : CXXLib::Exception()
        { return; }

        JavaLibException(CLibErrNum errNum) : CXXLib::Exception(errNum)
        { return; }
    }; // class Exception

    class JavaLibGeneratorImpl final : public CXXLib::GeneratorBase
    {
    public:
        JavaLibGeneratorImpl(JNIEnv* jenv, jobject impl);

        virtual ~JavaLibGeneratorImpl(void) override;

        virtual int generateInt(int data) const override;
        virtual std::string generateString(int data) const override;

        JNIEnv* _jenv;
        jobject _impl;
    }; // class JavaLibGeneratorImpl

    JavaLibGeneratorImpl::JavaLibGeneratorImpl(JNIEnv* jenv, jobject impl) :
        _jenv(jenv)
    {
        // Inform JVM to make this object as "do not garbage collect!"
        _impl = _jenv->NewGlobalRef(impl);
        return;
    }

    JavaLibGeneratorImpl::~JavaLibGeneratorImpl(void)
    {
        // Inform JVM that this object can be cleaned by garbage collector.
        _jenv->DeleteGlobalRef(_impl);
        return;
    }

    int JavaLibGeneratorImpl::generateInt(int data) const
    {
        if (_jenv == nullptr || _impl == nullptr) {
            throw (JavaLibException(2));
        }

        jclass igeneratorClass /* net.dotslashzero.javalib.IGenerator */ =
            _jenv->FindClass("net/dotslashzero/javalib/IGenerator");

        if (igeneratorClass == nullptr) {
            throw (JavaLibException(2));
        }

        jmethodID generateIntID = _jenv->GetMethodID(igeneratorClass, "generateInt", "(I)I");

        if (generateIntID == nullptr) {
            throw (JavaLibException(2));
        }

        jint result = _jenv->CallIntMethod(_impl, generateIntID, static_cast<jint>(data));

        if (_jenv->ExceptionCheck() == JNI_TRUE) {
            _jenv->ExceptionClear();
            throw (JavaLibException(2));
        }

        return (static_cast<int>(result));
    }

    std::string JavaLibGeneratorImpl::generateString(int data) const
    {
        if (_jenv == nullptr || _impl == nullptr) {
            throw (JavaLibException(2));
        }

        jclass igeneratorClass /* net.dotslashzero.javalib.IGenerator */ =
            _jenv->FindClass("net/dotslashzero/javalib/IGenerator");

        if (igeneratorClass == nullptr) {
            throw (JavaLibException(2));
        }

        jmethodID generateStringID = _jenv->GetMethodID(igeneratorClass, "generateString", "(I)Ljava/lang/String;");

        if (generateStringID == nullptr) {
            throw (JavaLibException(2));
        }

        jstring result = static_cast<jstring>(
            _jenv->CallObjectMethod(_impl, generateStringID, static_cast<jint>(data))
         );

        if (_jenv->ExceptionCheck() == JNI_TRUE) {
            _jenv->ExceptionClear();
            throw (JavaLibException(2));
        }

        if (result == nullptr)
            return ("");

        jboolean isStringCopy = JNI_FALSE;
        const char* resultBuffer = _jenv->GetStringUTFChars(result, &isStringCopy);

        std::string cxxResult(resultBuffer);

        if (isStringCopy) {
            _jenv->ReleaseStringUTFChars(result, resultBuffer);
        }

        return (cxxResult);
    }
} // namespace JavaLibCore

/*
 * Class:     net_dotslashzero_javalib_JavaLibException
 * Method:    nativeDestroyException
 * Signature: (J)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_JavaLibException_nativeDestroyException(
    JNIEnv*, jclass, jlong exceptionImpl
)
{
    auto* exceptionPtr = GET_CORE_EXCEPTION_PTR(exceptionImpl);

    if (exceptionPtr == nullptr)
        return (-1);

    delete (exceptionPtr);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_JavaLibException
 * Method:    nativeGetMessage
 * Signature: (J[C)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_JavaLibException_nativeGetMessage(
    JNIEnv* jenv, jclass, jlong exceptionImpl, jcharArray message
)
{
    BEGIN_EX_GUARD(jenv);
    auto* exceptionPtr = GET_CORE_EXCEPTION_PTR(exceptionImpl);

    if (exceptionPtr == nullptr)
        return (-1);

    auto exMessage = exceptionPtr->getMessage();

    // put contents of exMessage to message...
    // note: part of this function's pre-condition is that message is at least 512 in size.
    // also: jchar's type is unsigned 16 bit type

    // convert exMessage to UTF-16 string
    std::wstring_convert<JavaLibCore::deletable_facet<std::codecvt<char16_t, char, std::mbstate_t>>, char16_t> conv16;
    auto u16ExMessage = conv16.from_bytes(exMessage.data());

    jenv->SetCharArrayRegion(
        message, 0, static_cast<jsize>(u16ExMessage.size()), reinterpret_cast<const jchar*>(u16ExMessage.data())
    );
    END_EX_GUARD(jenv);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeCreateAddress
 * Signature: (ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lnet/dotslashzero/javalib/Core/AddressType;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Address_nativeCreateAddress(
    JNIEnv* jenv, jclass,
    jint streetNum, jstring street,
    jstring city, jstring province,
    jstring country, jstring zipCode,
    jobject addressImpl
)
{
    BEGIN_EX_GUARD(jenv);

    // Before we create an actual Address, let us evaluate the parameters first so we don't need to do clean up if
    // something fails.

    // streetNum
    int coreStreetNum = static_cast<int>(streetNum);

    const char* jstringCstr = nullptr;
    jboolean isStringCopy = JNI_FALSE;

    // street
    jstringCstr = jenv->GetStringUTFChars(street, &isStringCopy);

    if (jstringCstr == nullptr)
        return (2);

    std::string coreStreet(jstringCstr);

    if (isStringCopy == JNI_TRUE)
        jenv->ReleaseStringUTFChars(street, jstringCstr);

    // city
    jstringCstr = jenv->GetStringUTFChars(city, &isStringCopy);

    if (jstringCstr == nullptr)
        return (2);

    std::string coreCity(jstringCstr);

    if (isStringCopy == JNI_TRUE)
        jenv->ReleaseStringUTFChars(city, jstringCstr);

    // province
    jstringCstr = jenv->GetStringUTFChars(province, &isStringCopy);

    if (jstringCstr == nullptr)
        return (2);

    std::string coreProvince(jstringCstr);

    if (isStringCopy == JNI_TRUE)
        jenv->ReleaseStringUTFChars(province, jstringCstr);

    // country
    jstringCstr = jenv->GetStringUTFChars(country, &isStringCopy);

    if (jstringCstr == nullptr)
        return (2);

    std::string coreCountry(jstringCstr);

    if (isStringCopy == JNI_TRUE)
        jenv->ReleaseStringUTFChars(country, jstringCstr);

    // zipCode
    jstringCstr = jenv->GetStringUTFChars(zipCode, &isStringCopy);

    if (jstringCstr == nullptr)
        return (2);

    std::string coreZipCode(jstringCstr);

    if (isStringCopy == JNI_TRUE)
        jenv->ReleaseStringUTFChars(zipCode, jstringCstr);

    // We want to use a smart pointer in case something happens when we try to invoke methods on addressImpl
    std::unique_ptr<CXXLib::Address> coreAddressPtr(
        new CXXLib::Address(
            coreStreetNum, coreStreet,
            coreCity, coreProvince,
            coreCountry, coreZipCode
        )
    );

    // call addressImpl.setAddress(long impl); - long is actually a jlong in native type

    jclass coreAddressTypeClass /* net.dotslashzero.javalib.Core.AddressType */ =
        jenv->FindClass("net/dotslashzero/javalib/Core$AddressType");

    if (coreAddressTypeClass == nullptr)
        return (1);

    jmethodID setAddressID = jenv->GetMethodID(coreAddressTypeClass, "setAddress", "(J)V");

    if (setAddressID == nullptr)
        return (1);

    jlong impl = reinterpret_cast<jlong>(coreAddressPtr.get());

    jenv->CallVoidMethod(addressImpl, setAddressID, impl);

    // it is important that we release here in case CallVoidMethod fails...
    coreAddressPtr.release();

    END_EX_GUARD(jenv);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeDestroyAddress
 * Signature: (J)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Address_nativeDestroyAddress(
    JNIEnv*, jclass, jlong addressImpl
)
{
    auto* addressPtr = GET_CORE_ADDRESS_PTR(addressImpl);

    if (addressPtr == nullptr)
        return (-1);

    delete (addressPtr);

    return (0);
}


/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetStreetNum
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedInteger;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Address_nativeGetStreetNum(
    JNIEnv* jenv, jclass, jlong addressImpl, jobject streetNum
)
{
    BEGIN_EX_GUARD(jenv);
    auto* addressPtr = GET_CORE_ADDRESS_PTR(addressImpl);

    if (addressPtr == nullptr)
        return (-1);

    int coreStreetNum = addressPtr->getStreetNum();

    jclass WrappedIntegerClass /* net.dotslashzero.javalib.Core.WrappedInteger */ =
        jenv->FindClass("net/dotslashzero/javalib/Core$WrappedInteger");

    if (WrappedIntegerClass == nullptr)
        return (1);

    jmethodID setIntID = jenv->GetMethodID(WrappedIntegerClass, "setInt", "(I)V");

    if (setIntID == nullptr)
        return (1);

    jenv->CallVoidMethod(streetNum, setIntID, static_cast<jint>(coreStreetNum));

    END_EX_GUARD(jenv);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetStreet
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Address_nativeGetStreet(
    JNIEnv* jenv, jclass, jlong addressImpl, jobject street
)
{
    BEGIN_EX_GUARD(jenv);

    auto* addressPtr = GET_CORE_ADDRESS_PTR(addressImpl);

    if (addressPtr == nullptr)
        return (-1);

    std::string coreStreet = addressPtr->getStreet();

    jclass WrappedStringClass /* net.dotslashzero.javalib.Core.WrappedString */ =
        jenv->FindClass("net/dotslashzero/javalib/Core$WrappedString");

    if (WrappedStringClass == nullptr)
        return (1);

    jmethodID setValueID = jenv->GetMethodID(WrappedStringClass, "setString", "(Ljava/lang/String;)V");

    if (setValueID == nullptr)
        return (1);

    jstring streetValue = jenv->NewStringUTF(coreStreet.data());

    jenv->CallVoidMethod(street, setValueID, streetValue);

    END_EX_GUARD(jenv);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetCity
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Address_nativeGetCity(
    JNIEnv* jenv, jclass, jlong addressImpl, jobject city
)
{
    BEGIN_EX_GUARD(jenv);

    auto* addressPtr = GET_CORE_ADDRESS_PTR(addressImpl);

    if (addressPtr == nullptr)
        return (-1);

    std::string coreCity = addressPtr->getCity();

    jclass WrappedStringClass /* net.dotslashzero.javalib.Core.WrappedString */ =
        jenv->FindClass("net/dotslashzero/javalib/Core$WrappedString");

    if (WrappedStringClass == nullptr)
        return (1);

    jmethodID setValueID = jenv->GetMethodID(WrappedStringClass, "setString", "(Ljava/lang/String;)V");

    if (setValueID == nullptr)
        return (1);

    jstring cityValue = jenv->NewStringUTF(coreCity.data());

    jenv->CallVoidMethod(city, setValueID, cityValue);

    END_EX_GUARD(jenv);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetProvince
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Address_nativeGetProvince(
    JNIEnv* jenv, jclass, jlong addressImpl, jobject province
)
{
    BEGIN_EX_GUARD(jenv);

    auto* addressPtr = GET_CORE_ADDRESS_PTR(addressImpl);

    if (addressPtr == nullptr)
        return (-1);

    std::string coreProvince = addressPtr->getProvince();

    jclass WrappedStringClass /* net.dotslashzero.javalib.Core.WrappedString */ =
        jenv->FindClass("net/dotslashzero/javalib/Core$WrappedString");

    if (WrappedStringClass == nullptr)
        return (1);

    jmethodID setValueID = jenv->GetMethodID(WrappedStringClass, "setString", "(Ljava/lang/String;)V");

    if (setValueID == nullptr)
        return (1);

    jstring provinceValue = jenv->NewStringUTF(coreProvince.data());

    jenv->CallVoidMethod(province, setValueID, provinceValue);

    END_EX_GUARD(jenv);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetCountry
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Address_nativeGetCountry(
    JNIEnv* jenv, jclass, jlong addressImpl, jobject country
)
{
    BEGIN_EX_GUARD(jenv);

    auto* addressPtr = GET_CORE_ADDRESS_PTR(addressImpl);

    if (addressPtr == nullptr)
        return (-1);

    std::string coreCountry = addressPtr->getCountry();

    jclass WrappedStringClass /* net.dotslashzero.javalib.Core.WrappedString */ =
        jenv->FindClass("net/dotslashzero/javalib/Core$WrappedString");

    if (WrappedStringClass == nullptr)
        return (1);

    jmethodID setValueID = jenv->GetMethodID(WrappedStringClass, "setString", "(Ljava/lang/String;)V");

    if (setValueID == nullptr)
        return (1);

    jstring countryValue = jenv->NewStringUTF(coreCountry.data());

    jenv->CallVoidMethod(country, setValueID, countryValue);

    END_EX_GUARD(jenv);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetZipCode
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Address_nativeGetZipCode(
    JNIEnv* jenv, jclass, jlong addressImpl, jobject zipCode
)
{
    BEGIN_EX_GUARD(jenv);

    auto* addressPtr = GET_CORE_ADDRESS_PTR(addressImpl);

    if (addressPtr == nullptr)
        return (-1);

    std::string coreZipCode = addressPtr->getZipCode();

    jclass WrappedStringClass /* net.dotslashzero.javalib.Core.WrappedString */ =
        jenv->FindClass("net/dotslashzero/javalib/Core$WrappedString");

    if (WrappedStringClass == nullptr)
        return (1);

    jmethodID setValueID = jenv->GetMethodID(WrappedStringClass, "setString", "(Ljava/lang/String;)V");

    if (setValueID == nullptr)
        return (1);

    jstring zipCodeValue = jenv->NewStringUTF(coreZipCode.data());

    jenv->CallVoidMethod(zipCode, setValueID, zipCodeValue);

    END_EX_GUARD(jenv);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeCreatePerson
 * Signature: (Ljava/lang/String;Ljava/lang/String;IJLnet/dotslashzero/javalib/Core/PersonType;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Person_nativeCreatePerson(
    JNIEnv* jenv, jclass,
    jstring lastName, jstring firstName, jint age,
    jlong addressImplPtr,
    jobject personImpl
)
{
    BEGIN_EX_GUARD(jenv);

    const char* jstringCstr = nullptr;
    jboolean isStringCopy = JNI_FALSE;

    jstringCstr = jenv->GetStringUTFChars(lastName, &isStringCopy);

    if (jstringCstr == nullptr)
        return (2);

    std::string coreLastName(jstringCstr);

    if (isStringCopy == JNI_TRUE)
        jenv->ReleaseStringUTFChars(lastName, jstringCstr);

    jstringCstr = jenv->GetStringUTFChars(firstName, &isStringCopy);

    if (jstringCstr == nullptr)
        return (2);

    std::string coreFirstName(jstringCstr);

    if (isStringCopy == JNI_TRUE)
        jenv->ReleaseStringUTFChars(firstName, jstringCstr);

    int coreAge = static_cast<int>(age);

    auto* coreAddressPtr = GET_CORE_ADDRESS_PTR(addressImplPtr);

    if (coreAddressPtr == nullptr)
        return (2);

    std::unique_ptr<CXXLib::Person> corePersonPtr(
        new CXXLib::Person(
            coreLastName, coreFirstName,
            coreAge, *coreAddressPtr
        )
    );

    // call personImpl.setAddress(long impl); - long is actually a jlong in native type

    jclass corePersonTypeClass /* net.dotslashzero.javalib.Core.PersonType */ =
        jenv->FindClass("net/dotslashzero/javalib/Core$PersonType");

    if (corePersonTypeClass == nullptr)
        return (1);

    jmethodID setAddressID = jenv->GetMethodID(corePersonTypeClass, "setAddress", "(J)V");

    if (setAddressID == nullptr)
        return (1);

    jlong impl = reinterpret_cast<jlong>(corePersonPtr.get());

    jenv->CallVoidMethod(personImpl, setAddressID, impl);

    // it is important that we release here in case CallVoidMethod fails...
    corePersonPtr.release();

    END_EX_GUARD(jenv);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeDestroyPerson
 * Signature: (J)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Person_nativeDestroyPerson(
    JNIEnv*, jclass, jlong personImpl
)
{
    auto* personPtr = GET_CORE_PERSON_PTR(personImpl);

    if (personPtr == nullptr)
        return (-1);

    delete (personPtr);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeGetLastName
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Person_nativeGetLastName(
    JNIEnv* jenv, jclass, jlong personImpl, jobject lastName
)
{
    BEGIN_EX_GUARD(jenv);

    auto* personPtr = GET_CORE_PERSON_PTR(personImpl);

    if (personPtr == nullptr)
        return (-1);

    std::string coreLastName = personPtr->getLastName();

    jclass WrappedStringClass /* net.dotslashzero.javalib.Core.WrappedString */ =
        jenv->FindClass("net/dotslashzero/javalib/Core$WrappedString");

    if (WrappedStringClass == nullptr)
        return (1);

    jmethodID setValueID = jenv->GetMethodID(WrappedStringClass, "setString", "(Ljava/lang/String;)V");

    if (setValueID == nullptr)
        return (1);

    jstring lastNameValue = jenv->NewStringUTF(coreLastName.data());

    jenv->CallVoidMethod(lastName, setValueID, lastNameValue);

    END_EX_GUARD(jenv);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeGetFirstName
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Person_nativeGetFirstName(
    JNIEnv* jenv, jclass, jlong personImpl, jobject firstName
)
{
    BEGIN_EX_GUARD(jenv);

    auto* personPtr = GET_CORE_PERSON_PTR(personImpl);

    if (personPtr == nullptr)
        return (-1);

    std::string coreFirstName = personPtr->getFirstName();

    jclass WrappedStringClass /* net.dotslashzero.javalib.Core.WrappedString */ =
        jenv->FindClass("net/dotslashzero/javalib/Core$WrappedString");

    if (WrappedStringClass == nullptr)
        return (1);

    jmethodID setValueID = jenv->GetMethodID(WrappedStringClass, "setString", "(Ljava/lang/String;)V");

    if (setValueID == nullptr)
        return (1);

    jstring firstNameValue = jenv->NewStringUTF(coreFirstName.data());

    jenv->CallVoidMethod(firstName, setValueID, firstNameValue);

    END_EX_GUARD(jenv);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeGetAge
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedInteger;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Person_nativeGetAge(
    JNIEnv* jenv, jclass, jlong personImpl, jobject age
)
{
    BEGIN_EX_GUARD(jenv);

    auto* personPtr = GET_CORE_PERSON_PTR(personImpl);

    if (personPtr == nullptr)
        return (-1);

    int coreAge = personPtr->getAge();

    jclass WrappedIntegerClass /* net.dotslashzero.javalib.Core.WrappedInteger */ =
        jenv->FindClass("net/dotslashzero/javalib/Core$WrappedInteger");

    if (WrappedIntegerClass == nullptr)
        return (1);

    jmethodID setIntID = jenv->GetMethodID(WrappedIntegerClass, "setInt", "(I)V");

    if (setIntID == nullptr)
        return (1);

    jenv->CallVoidMethod(age, setIntID, static_cast<jint>(coreAge));

    END_EX_GUARD(jenv);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeGetAddress
 * Signature: (JLnet/dotslashzero/javalib/Core/AddressType;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Person_nativeGetAddress(
    JNIEnv* jenv, jclass, jlong personImpl, jobject addressImpl
)
{
    BEGIN_EX_GUARD(jenv);

    auto* personPtr = GET_CORE_PERSON_PTR(personImpl);

    if (personPtr == nullptr)
        return (-1);

    auto coreAddress = personPtr->getAddress();

    std::unique_ptr<CXXLib::Address> coreNewAddressPtr(
        new CXXLib::Address(
            coreAddress.getStreetNum(), coreAddress.getStreet(),
            coreAddress.getCity(), coreAddress.getProvince(),
            coreAddress.getCountry(), coreAddress.getZipCode()
        )
    );

    // call addressImpl.setAddress(long impl); - long is actually a jlong in native type

    jclass coreAddressTypeClass /* net.dotslashzero.javalib.Core.AddressType */ =
        jenv->FindClass("net/dotslashzero/javalib/Core$AddressType");

    if (coreAddressTypeClass == nullptr)
        return (1);

    jmethodID setAddressID = jenv->GetMethodID(coreAddressTypeClass, "setAddress", "(J)V");

    if (setAddressID == nullptr)
        return (1);

    jlong impl = reinterpret_cast<jlong>(coreNewAddressPtr.get());

    jenv->CallVoidMethod(addressImpl, setAddressID, impl);

    // it is important that we release here in case CallVoidMethod fails...
    coreNewAddressPtr.release();

    END_EX_GUARD(jenv);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Printer
 * Method:    nativeCreatePrinter
 * Signature: (Lnet/dotslashzero/javalib/IGenerator;Lnet/dotslashzero/javalib/Core/PrinterType;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Printer_nativeCreatePrinter(
    JNIEnv* jenv, jclass, jobject generatorInstance, jobject printerImpl
)
{
    BEGIN_EX_GUARD(jenv);

    // Create a JavaLibGeneratorImpl
    std::unique_ptr<JavaLibCore::JavaLibGeneratorImpl> generator(
        new JavaLibCore::JavaLibGeneratorImpl(jenv, generatorInstance)
    );
    std::unique_ptr<CXXLib::Printer> corePrinterPtr(new CXXLib::Printer(std::move(generator)));

    // call printerImpl.setAddress(long impl); - long is actually a jlong in native type

    jclass corePrinterTypeClass /* net.dotslashzero.javalib.Core.PrinterType */ =
        jenv->FindClass("net/dotslashzero/javalib/Core$PrinterType");

    if (corePrinterTypeClass == nullptr)
        return (1);

    jmethodID setAddressID = jenv->GetMethodID(corePrinterTypeClass, "setAddress", "(J)V");

    if (setAddressID == nullptr)
        return (1);

    jlong impl = reinterpret_cast<jlong>(corePrinterPtr.get());

    jenv->CallVoidMethod(printerImpl, setAddressID, impl);

    // it is important that we release here in case CallVoidMethod fails...
    corePrinterPtr.release();

    END_EX_GUARD(jenv);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Printer
 * Method:    nativeDestroyPrinter
 * Signature: (J)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Printer_nativeDestroyPrinter(
    JNIEnv*, jclass, jlong printerImpl
)
{
    auto* printerPtr = GET_CORE_PRINTER_PTR(printerImpl);

    if (printerPtr == nullptr)
        return (-1);

    delete (printerPtr);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Printer
 * Method:    nativePrintInt
 * Signature: (J)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Printer_nativePrintInt(
    JNIEnv* jenv, jclass, jlong printerImpl
)
{
    BEGIN_EX_GUARD(jenv);

    auto* printerPtr = GET_CORE_PRINTER_PTR(printerImpl);

    if (printerPtr == nullptr)
        return (-1);

    printerPtr->printInt();

    END_EX_GUARD(jenv);

    return (0);
}

/*
 * Class:     net_dotslashzero_javalib_Printer
 * Method:    nativePrintString
 * Signature: (J)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Printer_nativePrintString(
    JNIEnv* jenv, jclass, jlong printerImpl
)
{
    BEGIN_EX_GUARD(jenv);

    auto* printerPtr = GET_CORE_PRINTER_PTR(printerImpl);

    if (printerPtr == nullptr)
        return (-1);

    printerPtr->printString();

    END_EX_GUARD(jenv);

    return (0);
}
