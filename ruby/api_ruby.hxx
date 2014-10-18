#if !defined(RUBLIB_API_HXX)
#define RUBLIB_API_HXX

#include <ruby.h>

#define RUBYLIB_API __attribute__ ((visibility ("default")))
#if defined(_MSC_VER)
    #undef RUBYLIB_API
    #define RUBYLIB_API __declspec(dllexport)
#endif // defined(_MSC_VER)

#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

RUBYLIB_API
void Init_rubylibnative(void);

RUBYLIB_API
VALUE RLRubyLibErrorGetMessage(VALUE, VALUE);

RUBYLIB_API
VALUE RLLibraryInitialize(VALUE);

RUBYLIB_API
VALUE RLLibraryTerminate(VALUE);

RUBYLIB_API
VALUE RLAddressCreate(VALUE, VALUE);

RUBYLIB_API
VALUE RLAddressDestroy(VALUE, VALUE);

RUBYLIB_API
VALUE RLAddressGetStreetNum(VALUE, VALUE);

RUBYLIB_API
VALUE RLAddressGetStreet(VALUE, VALUE);

RUBYLIB_API
VALUE RLAddressGetCity(VALUE, VALUE);

RUBYLIB_API
VALUE RLAddressGetProvince(VALUE, VALUE);

RUBYLIB_API
VALUE RLAddressGetCountry(VALUE, VALUE);

RUBYLIB_API
VALUE RLAddressGetZipCode(VALUE, VALUE);

RUBYLIB_API
VALUE RLPersonCreate(VALUE, VALUE);

RUBYLIB_API
VALUE RLPersonDestroy(VALUE, VALUE);

RUBYLIB_API
VALUE RLPersonGetLastName(VALUE, VALUE);

RUBYLIB_API
VALUE RLPersonGetFirstName(VALUE, VALUE);

RUBYLIB_API
VALUE RLPersonGetAge(VALUE, VALUE);

RUBYLIB_API
VALUE RLPersonGetAddressImpl(VALUE, VALUE);

RUBYLIB_API
VALUE RLPrinterCreate(VALUE, VALUE);

RUBYLIB_API
VALUE RLPrinterDestroy(VALUE, VALUE);

RUBYLIB_API
VALUE RLPrinterPrintInt(VALUE, VALUE);

RUBYLIB_API
VALUE RLPrinterPrintString(VALUE, VALUE);

#if defined(__cplusplus)
} // extern "C"
#endif // defined(__cplusplus)

#endif // !defined(RUBLIB_API_HXX)
