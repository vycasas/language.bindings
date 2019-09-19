#pragma once

#include <node_api.h>

#define DSZ_NODEJSLIB_MODULE_VISIBILITY __attribute__ ((visibility ("default")))
#if defined(_MSC_VER)
    #undef DSZ_NODEJSLIB_MODULE_VISIBILITY
    #define DSZ_NODEJSLIB_MODULE_VISIBILITY __declspec(dllexport)
#endif /* defined(_MSC_VER) */

#define DSZ_NODEJSLIB_CALLING_CONVENTION
#if defined(_MSC_VER)
    #undef DSZ_NODEJSLIB_CALLING_CONVENTION
    #define DSZ_NODEJSLIB_CALLING_CONVENTION __stdcall
#endif /* defined(_MSC_VER) */

#if defined(__cplusplus)
    #define DSZ_NODEJSLIB_API(rt) \
        extern "C" \
        DSZ_NODEJSLIB_MODULE_VISIBILITY rt DSZ_NODEJSLIB_CALLING_CONVENTION
#else /* defined(__cplusplus) */
    #define DSZ_NODEJSLIB_API(rt) \
        DSZ_NODEJSLIB_MODULE_VISIBILITY rt DSZ_NODEJSLIB_CALLING_CONVENTION
#endif /* defined(__cplusplus) */

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibInitializeModule
    (napi_env env,
    napi_value exports);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibLibraryInitialize(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibLibraryUninitialize(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibLibraryGetVersionString(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibLibraryGetVersionMajor(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibLibraryGetVersionMinor(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibLibraryGetVersionPatch(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibLibraryGetVersionExtra(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressCreate(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressGetStreetNum(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressGetStreet(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressGetCity(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressGetProvince(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressGetZipCode(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressGetCountry(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibAddressToString(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPersonCreate(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPersonGetLastName(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPersonGetFirstName(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPersonGetAge(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPersonGetAddress(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPersonToString(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPrinterCreate(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPrinterPrintInt(
    napi_env env,
    napi_callback_info callbackInfo);

DSZ_NODEJSLIB_API(napi_value) DszNodeJsLibPrinterPrintString(
    napi_env env,
    napi_callback_info callbackInfo);
