// Compile with C++ compiler

#include "api_java.hxx"

#include <codecvt>
#include <cstdint>
#include <locale>

#include <cxx/api.hxx>

// helper macros
#define GET_CORE_CLASS_PTR(impl, T) \
    reinterpret_cast<T*>(static_cast<std::uintptr_t>(impl))

#define GET_CORE_EXCEPTION_PTR(impl) GET_CORE_CLASS_PTR(impl, CXXLib::Exception)
#define GET_CORE_ADDRESS_PTR(impl) GET_CORE_CLASS_PTR(impl, CXXLib::Address)
#define GET_CORE_PERSON_PTR(impl) GET_CORE_CLASS_PTR(impl, CXXLib::Person)

#define BEGIN_EX_GUARD(jenv) \
    try {
#define END_EX_GUARD(jenv) \
    } \
    catch (CXXLib::Exception& e) { \
        jthrowable jniEx = MakeJNIException(jenv, e); \
        if (jniEx == nullptr) \
            return (2); \
        jenv->Throw(jniEx); \
    } \
    catch (std::exception& e) { \
        jthrowable jniEx = MakeJNIException(jenv, CXXLib::Exception(e)); \
        if (jniEx == nullptr) \
            return (2); \
        jenv->Throw(jniEx); \
    }

namespace
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

        jmethodID constructorID = jenv->GetMethodID(javaLibExceptionClass, "<init>", "(L)V");

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
}

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
    auto* exceptionPtr = GET_CORE_EXCEPTION_PTR(exceptionImpl);

    if (exceptionPtr == nullptr)
        return (-1);

    auto exMessage = exceptionPtr->getMessage();

    // put contents of exMessage to message...
    // note: part of this function's pre-condition is that message is at least 512 in size.
    // also: jchar's type is unsigned 16 bit type

    // convert exMessage to UTF-16 string
    std::wstring_convert<deletable_facet<std::codecvt<char16_t, char, std::mbstate_t>>, char16_t> conv16;
    auto u16ExMessage = conv16.from_bytes(exMessage.data());

    jenv->SetCharArrayRegion(message, 0, u16ExMessage.size(), reinterpret_cast<const jchar*>(u16ExMessage.data()));

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

    return (0);
}
