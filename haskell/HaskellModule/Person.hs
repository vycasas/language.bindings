module HaskellModule.Person
(
    Person,
    LastName,
    FirstName,
    Age,
    create,
    destroy,
    getLastName,
    getFirstName,
    getAge,
    getAddress,
    asString
)
where

import Data.Maybe
import Foreign
import Foreign.C
import Foreign.Ptr

import qualified HaskellModule.Address as HMAddress

type Person = Integer
type LastName = String
type FirstName = String
type Age = Int

foreign import ccall unsafe "c/api.h CLPersonCreate"
    clib_PersonCreate :: CString -> CString -> CInt -> CUIntPtr -> Ptr CUIntPtr -> IO CSize

create :: LastName -> FirstName -> Age -> HMAddress.Address -> IO (Maybe Person, Integer)
create lastName firstName age address = alloca $ \ptrToCUIntPtr -> do
    lastNameCStr <- newCAString lastName
    firstNameCStr <- newCAString firstName
    apiResult <- clib_PersonCreate lastNameCStr firstNameCStr (fromIntegral age) (fromIntegral address) ptrToCUIntPtr
    result <- if apiResult == 0
        then do
            resultPerson <- peek ptrToCUIntPtr
            return (Just (fromIntegral resultPerson) :: Maybe Person)
        else do
            return (Nothing :: Maybe Person)
    return (result, fromIntegral apiResult)

foreign import ccall unsafe "c/api.h CLPersonDestroy"
    clib_PersonDestroy :: CUIntPtr -> IO CSize

destroy :: Person -> IO Integer
destroy person = do
    apiResult <- clib_PersonDestroy $ fromIntegral person
    return (fromIntegral apiResult)

foreign import ccall unsafe "c/api.h CLPersonGetLastName"
    clib_PersonGetLastName :: CUIntPtr -> Ptr CChar -> CSize -> Ptr CSize -> IO CSize

getLastName :: Person -> IO (Maybe LastName, Integer)
getLastName person = alloca $ \ptrToCSize -> do
    allocaBytes 24 $ \ptrToCChar -> do
        apiResult <- clib_PersonGetLastName (fromIntegral person) ptrToCChar 40 ptrToCSize
        result <- if apiResult == 0
            then do
                resultLastName <- peekCString ptrToCChar
                return (Just (resultLastName) :: Maybe LastName)
            else do
                return (Nothing :: Maybe LastName)
        return (result, fromIntegral apiResult)

foreign import ccall unsafe "c/api.h CLPersonGetFirstName"
    clib_PersonGetFirstName :: CUIntPtr -> Ptr CChar -> CSize -> Ptr CSize -> IO CSize

getFirstName :: Person -> IO (Maybe FirstName, Integer)
getFirstName person = alloca $ \ptrToCSize -> do
    allocaBytes 24 $ \ptrToCChar -> do
        apiResult <- clib_PersonGetFirstName (fromIntegral person) ptrToCChar 40 ptrToCSize
        result <- if apiResult == 0
            then do
                resultFirstName <- peekCString ptrToCChar
                return (Just (resultFirstName) :: Maybe FirstName)
            else do
                return (Nothing :: Maybe FirstName)
        return (result, fromIntegral apiResult)

foreign import ccall unsafe "c/api.h CLPersonGetAge"
    clib_PersonGetAge :: CUIntPtr -> Ptr CInt -> IO CSize

getAge :: Person -> IO (Maybe Age, Integer)
getAge person = alloca $ \ptrToCInt -> do
    apiResult <- clib_PersonGetAge (fromIntegral person) ptrToCInt
    result <- if apiResult == 0
        then do
            resultAge <- peek ptrToCInt
            return (Just (fromIntegral resultAge) :: Maybe Age)
        else do
            return (Nothing :: Maybe Age)
    return (result, fromIntegral apiResult)

foreign import ccall unsafe "c/api.h CLPersonGetAddress"
    clib_PersonGetAddress :: CUIntPtr -> Ptr CUIntPtr -> IO CSize

-- This function makes a new copy of address!
getAddress :: Person -> IO (Maybe HMAddress.Address, Integer)
getAddress person = alloca $ \ptrToCUIntPtr -> do
    apiResult <- clib_PersonGetAddress (fromIntegral person) ptrToCUIntPtr
    result <- if apiResult == 0
        then do
            resultAddress <- peek ptrToCUIntPtr
            return (Just (fromIntegral resultAddress) :: Maybe HMAddress.Address)
        else do
            return (Nothing :: Maybe HMAddress.Address)
    return (result, fromIntegral apiResult)

asString :: Person -> IO String
asString person = do
    (maybeLastName, getLastNameResult) <- getLastName person
    (maybeFirstName, getFirstNameResult) <- getFirstName person
    (maybeAge, getAgeResult) <- getAge person
    (maybeAddress, getAddressResult) <- getAddress person
    result <- if getLastNameResult == 0 && getFirstNameResult == 0 && getAgeResult == 0 && getAddressResult == 0
        then do
            addressString <- HMAddress.asString $ fromJust maybeAddress
            let personString = (fromJust maybeLastName) ++ ", " ++ (fromJust maybeFirstName) ++ "\n" ++
                    (show $ fromJust maybeAge) ++ " years old\n" ++ addressString        
            HMAddress.destroy $ fromJust maybeAddress
            return (personString)
        else do
            return ("")
    return (result)
