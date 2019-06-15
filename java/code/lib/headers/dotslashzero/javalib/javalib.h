#if !defined(JAVALIB_API_H)
#define JAVALIB_API_H

#include <jni.h>

#if defined(__cplusplus)
#define JAVALIB_API extern "C"
#endif // defined(__cplusplus)

#undef net_dotslashzero_javalib_JavaLibException_serialVersionUID
#define net_dotslashzero_javalib_JavaLibException_serialVersionUID -3042686055658047285LL
#undef net_dotslashzero_javalib_JavaLibException_serialVersionUID
#define net_dotslashzero_javalib_JavaLibException_serialVersionUID -3387516993124229948LL
/*
 * Class:     net_dotslashzero_javalib_JavaLibException
 * Method:    nativeDestroyException
 * Signature: (J)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_JavaLibException_nativeDestroyException(
    JNIEnv*, jclass, jlong exceptionImpl
);

/*
 * Class:     net_dotslashzero_javalib_JavaLibException
 * Method:    nativeGetMessage
 * Signature: (J[C)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_JavaLibException_nativeGetMessage(
    JNIEnv* jenv, jclass, jlong exceptionImpl, jcharArray message
);

/*
 * Class:     net_dotslashzero_javalib_Library
 * Method:    nativeGetVersionString
 * Signature: (Lnet/dotslashzero/javalib/Core/WrappedString;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Library_nativeGetVersionString(
    JNIEnv* jenv, jclass, jobject versionString
);

/*
 * Class:     net_dotslashzero_javalib_Library
 * Method:    nativeGetVersionMajor
 * Signature: (Lnet/dotslashzero/javalib/Core/WrappedLong;)I
 */
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Library_nativeGetVersionMajor(
    JNIEnv* jenv, jclass, jobject versionMajor
);

/*
 * Class:     net_dotslashzero_javalib_Library
 * Method:    nativeGetVersionMinor
 * Signature: (Lnet/dotslashzero/javalib/Core/WrappedLong;)I
 */
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Library_nativeGetVersionMinor(
    JNIEnv* jenv, jclass, jobject versionMinor
);

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
    jobject addressType
);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeDestroyAddress
 * Signature: (J)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Address_nativeDestroyAddress(
    JNIEnv*, jclass, jlong addressImpl
);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetStreetNum
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedInteger;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Address_nativeGetStreetNum(
    JNIEnv* jenv, jclass, jlong addressImpl, jobject streetNum
);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetStreet
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Address_nativeGetStreet(
    JNIEnv* jenv, jclass, jlong addressImpl, jobject street
);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetCity
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Address_nativeGetCity(
    JNIEnv* jenv, jclass, jlong addressImpl, jobject city
);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetProvince
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Address_nativeGetProvince(
    JNIEnv* jenv, jclass, jlong addressImpl, jobject province
);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetCountry
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Address_nativeGetCountry(
    JNIEnv* jenv, jclass, jlong addressImpl, jobject country
);

/*
 * Class:     net_dotslashzero_javalib_Address
 * Method:    nativeGetZipCode
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Address_nativeGetZipCode(
    JNIEnv* jenv, jclass, jlong addressImpl, jobject zipCode
);

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
);

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeDestroyPerson
 * Signature: (J)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Person_nativeDestroyPerson(
    JNIEnv*, jclass, jlong personImpl
);

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeGetLastName
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Person_nativeGetLastName(
    JNIEnv* jenv, jclass, jlong personImpl, jobject lastName
);

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeGetFirstName
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedString;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Person_nativeGetFirstName(
    JNIEnv* jenv, jclass, jlong personImpl, jobject firstName
);

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeGetAge
 * Signature: (JLnet/dotslashzero/javalib/Core/WrappedInteger;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Person_nativeGetAge(
    JNIEnv* jenv, jclass, jlong personImpl, jobject age
);

/*
 * Class:     net_dotslashzero_javalib_Person
 * Method:    nativeGetAddress
 * Signature: (JLnet/dotslashzero/javalib/Core/AddressType;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Person_nativeGetAddress(
    JNIEnv* jenv, jclass, jlong personImpl, jobject addressImpl
);

/*
 * Class:     net_dotslashzero_javalib_Printer
 * Method:    nativeCreatePrinter
 * Signature: (Lnet/dotslashzero/javalib/IGenerator;Lnet/dotslashzero/javalib/Core/PrinterType;)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Printer_nativeCreatePrinter(
    JNIEnv* jenv, jclass, jobject generatorInstance, jobject printerImpl
);

/*
 * Class:     net_dotslashzero_javalib_Printer
 * Method:    nativeDestroyPrinter
 * Signature: (J)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Printer_nativeDestroyPrinter(
    JNIEnv*, jclass, jlong printerImpl
);

/*
 * Class:     net_dotslashzero_javalib_Printer
 * Method:    nativePrintInt
 * Signature: (J)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Printer_nativePrintInt(
    JNIEnv* jenv, jclass, jlong printerImpl
);

/*
 * Class:     net_dotslashzero_javalib_Printer
 * Method:    nativePrintString
 * Signature: (J)I
 */
JAVALIB_API
JNIEXPORT jint JNICALL Java_net_dotslashzero_javalib_Printer_nativePrintString(
    JNIEnv* jenv, jclass, jlong printerImpl
);

#endif // !defined(JAVALIB_API_H)
