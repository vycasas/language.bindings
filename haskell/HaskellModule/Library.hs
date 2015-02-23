module HaskellModule.Library
(
    initialize,
    terminate,
    getVersionString,
    getVersionMajor,
    getVersionMinor
)
where

import Foreign
import Foreign.C
import Foreign.Ptr

-- Small helper
toIOString :: IO CString -> IO String
toIOString x = do
    cstr <- x
    peekCString cstr

foreign import ccall unsafe "c/api.h CLLibraryInitialize"
    clib_LibraryInitialize :: IO CSize

initialize :: IO Integer
initialize = fmap (\a -> fromIntegral a) clib_LibraryInitialize :: IO Integer

foreign import ccall unsafe "c/api.h CLLibraryTerminate"
    clib_LibraryTerminate :: IO ()

terminate :: IO ()
terminate = clib_LibraryTerminate

foreign import ccall unsafe "c/api.h CLLibraryGetVersionString"
    clib_LibraryGetVersionString :: IO CString

getVersionString :: IO String
getVersionString = toIOString clib_LibraryGetVersionString

foreign import ccall unsafe "c/api.h CLLibraryGetVersionMajor"
    clib_LibraryGetVersionMajor :: IO CSize

getVersionMajor :: IO Integer
getVersionMajor = fmap (\a -> fromIntegral a) clib_LibraryGetVersionMajor :: IO Integer

foreign import ccall unsafe "c/api.h CLLibraryGetVersionMinor"
    clib_LibraryGetVersionMinor :: IO CSize

getVersionMinor :: IO Integer
getVersionMinor = fmap (\a -> fromIntegral a) clib_LibraryGetVersionMinor :: IO Integer
