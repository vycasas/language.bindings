#pragma once

#include <jni.h>

#if defined(__cplusplus)
extern "C"
{
#endif /* defined(__cplusplus) */

/*
 * Class:     net_dotslashzero_javalib_JavaLibException
 * Method:    nativeGetErrorMessage
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_JavaLibException_nativeGetErrorMessage(
    JNIEnv* pEnv,
    jclass javaLibExceptionClass,
    jlong errorNum,
    jobject errorMessageWrappedString);

/*
 * Class:     net_dotslashzero_javalib_Library
 * Method:    nativeInitialize
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Library_nativeInitialize(
    JNIEnv* pEnv,
    jclass libraryClass);

/*
 * Class:     net_dotslashzero_javalib_Library
 * Method:    nativeUninitialize
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Library_nativeUninitialize(
    JNIEnv* pEnv,
    jclass libraryClass);

/*
 * Class:     net_dotslashzero_javalib_Library
 * Method:    nativeGetVersionString
 * Signature: (Lnet/dotslashzero/javalib/Core/WrappedString;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Library_nativeGetVersionString(
    JNIEnv* pEnv,
    jclass libraryClass,
    jobject versionStringWrappedString);

/*
 * Class:     net_dotslashzero_javalib_Library
 * Method:    nativeGetVersionMajor
 * Signature: (Lnet/dotslashzero/javalib/Core/WrappedLong;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Library_nativeGetVersionMajor(
    JNIEnv* pEnv,
    jclass libraryClass,
    jobject versionMajorWrappedLong);

/*
 * Class:     net_dotslashzero_javalib_Library
 * Method:    nativeGetVersionMinor
 * Signature: (Lnet/dotslashzero/javalib/Core/WrappedLong;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Library_nativeGetVersionMinor(
    JNIEnv* pEnv,
    jclass libraryClass,
    jobject versionMinorWrappedLong);

/*
 * Class:     net_dotslashzero_javalib_Library
 * Method:    nativeGetVersionPatch
 * Signature: (Lnet/dotslashzero/javalib/Core/WrappedLong;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Library_nativeGetVersionPatch(
    JNIEnv* pEnv,
    jclass libraryClass,
    jobject versionPatchWrappedLong);

/*
 * Class:     net_dotslashzero_javalib_Library
 * Method:    nativeGetVersionExtra
 * Signature: (Lnet/dotslashzero/javalib/Core/WrappedString;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Library_nativeGetVersionExtra(
    JNIEnv* pEnv,
    jclass libraryClass,
    jobject versionExtraWrappedString);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeCreateAddress
 * Signature: (ILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lnet/dotslashzero/javalib/Core/AddressType;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeCreateAddress(
    JNIEnv* pEnv,
    jclass addressClass,
    jint streetNum,
    jstring street,
    jstring city,
    jstring province,
    jstring zipCode,
    jstring country,
    jobject addressAddressType);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeDestroyAddress
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeDestroyAddress(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetStreetNum
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedInteger;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeGetStreetNum(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl,
    jobject streetNumWrappedInteger);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetStreet
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeGetStreet(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl,
    jobject streetWrappedString);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetCity
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeGetCity(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl,
    jobject cityWrappedString);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetProvince
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeGetProvince(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl,
    jobject provinceWrappedString);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetZipCode
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeGetZipCode(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl,
    jobject zipCodeWrappedString);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetCountry
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeGetCountry(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl,
    jobject countryWrappedString);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeToString
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Address_nativeToString(
    JNIEnv* pEnv,
    jclass addressClass,
    jlong addressImpl,
    jobject addressStringWrappedString);

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeCreatePerson
 * Signature: (Ljava/lang/String;Ljava/lang/String;IJLnet/dotslashzero/javalib/Core/PersonType;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Person_nativeCreatePerson(
    JNIEnv* pEnv,
    jclass personClass,
    jstring lastName,
    jstring firstName,
    jint age,
    jlong addressImpl,
    jobject personPersonType);

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeDestroyPerson
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Person_nativeDestroyPerson(
    JNIEnv* pEnv,
    jclass personClass,
    jlong personImpl);

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeGetLastName
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Person_nativeGetLastName(
    JNIEnv* pEnv,
    jclass personClass,
    jlong personImpl,
    jobject lastNameWrappedString);

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeGetFirstName
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Person_nativeGetFirstName(
    JNIEnv* pEnv,
    jclass personClass,
    jlong personImpl,
    jobject firstNameWrappedString);

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeGetAge
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedInteger;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Person_nativeGetAge(
    JNIEnv* pEnv,
    jclass personClass,
    jlong personImpl,
    jobject ageWrappedInteger);

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeGetAddress
 * Signature: (JLnet/dotslashzero/javalib/Core/AddressType;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Person_nativeGetAddress(
    JNIEnv* pEnv,
    jclass personClass,
    jlong personImpl,
    jobject addressAddressType);

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeToString
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Person_nativeToString(
    JNIEnv* pEnv,
    jclass personClass,
    jlong personImpl,
    jobject personStringWrappedString);

/*
 * Class:     net_dotslashzero_javalib_Printer
 * Method:    nativeCreatePrinter
 * Signature: (Lnet/dotslashzero/javalib/Core/PrinterType;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Printer_nativeCreatePrinter(
    JNIEnv* pEnv,
    jclass printerClass,
    jobject printerPrinterType);

/*
 * Class:     net_dotslashzero_javalib_Printer
 * Method:    nativeDestroyPrinter
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Printer_nativeDestroyPrinter(
    JNIEnv* pEnv,
    jclass printerClass,
    jlong printerImpl);

/*
 * Class:     net_dotslashzero_javalib_Printer
 * Method:    nativePrintInt
 * Signature: (JLnet/dotslashzero/javalib/IGenerator;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Printer_nativePrintInt(
    JNIEnv* pEnv,
    jclass printerClass,
    jlong printerImpl,
    jobject generatorInstance);

/*
 * Class:     net_dotslashzero_javalib_Printer
 * Method:    nativePrintString
 * Signature: (JLnet/dotslashzero/javalib/IGenerator;)J
 */
JNIEXPORT jlong JNICALL Java_net_dotslashzero_javalib_Printer_nativePrintString(
    JNIEnv* pEnv,
    jclass printerClass,
    jlong printerImpl,
    jobject generatorInstance);

#if defined(__cplusplus)
}
/* extern "C" */
#endif /* defined(__cplusplus) */
