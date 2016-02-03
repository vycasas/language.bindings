#if !defined(NODEJSLIB_API_HXX)
#define NODEJSLIB_API_HXX

#include <node.h>

#if !defined(__cplusplus)
    #error This library requires a C++ compiler to build.
#endif // !defined(__cplusplus)

void NJLInitialize(v8::Local<v8::Object> exports);

NODE_MODULE(nodejslib, NJLInitialize);

#endif // !defined(NODEJSLIB_API_HXX)
