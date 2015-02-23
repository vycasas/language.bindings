module HaskellModule.Address
(
    Address,
    StreetNumber,
    Street,
    City,
    Province,
    Country,
    ZipCode,
    create,
    destroy,
    getStreetNumber,
    getStreet,
    getCity,
    getProvince,
    getCountry,
    getZipCode,
    asString
)
where

import Data.Maybe
import Foreign
import Foreign.C
import Foreign.Ptr

type Address = Integer
type StreetNumber = Int
type Street = String
type City = String
type Province = String
type Country = String
type ZipCode = String

foreign import ccall unsafe "c/api.h CLAddressCreate"
    clib_AddressCreate :: CInt -> CString -> CString -> CString -> CString -> CString -> Ptr CUIntPtr -> IO CSize

create :: StreetNumber -> Street -> City -> Province -> Country -> ZipCode -> IO (Maybe Address, Integer)
create streetNum street city province country zipCode = alloca $ \ptrToCUIntPtr -> do
    streetCStr <- newCAString street
    cityCStr <- newCAString city
    provinceCStr <- newCAString province
    countryCStr <- newCAString country
    zipCodeCStr <- newCAString zipCode
    apiResult <- clib_AddressCreate (fromIntegral streetNum) streetCStr cityCStr provinceCStr countryCStr zipCodeCStr
        ptrToCUIntPtr
    result <- if apiResult == 0
        then do
            resultAddr <- peek ptrToCUIntPtr
            return (Just (fromIntegral resultAddr) :: Maybe Address)
        else do
            return (Nothing :: Maybe Address)
    return (result, fromIntegral apiResult)

foreign import ccall unsafe "c/api.h CLAddressDestroy"
    clib_AddressDestroy :: CUIntPtr -> IO CSize    

destroy :: Address -> IO Integer
destroy addr = do
    apiResult <- clib_AddressDestroy $ fromIntegral addr
    return (fromIntegral apiResult)

foreign import ccall unsafe "c/api.h CLAddressGetStreetNumber"
    clib_AddressGetStreetNumber :: CUIntPtr -> Ptr CInt -> IO CSize

getStreetNumber :: Address -> IO (Maybe StreetNumber, Integer)
getStreetNumber addr = alloca $ \ptrToCInt -> do
    apiResult <- clib_AddressGetStreetNumber (fromIntegral addr) ptrToCInt
    result <- if apiResult == 0
        then do
            resultStreetNum <- peek ptrToCInt
            return (Just (fromIntegral resultStreetNum) :: Maybe StreetNumber)
        else do
            return (Nothing :: Maybe StreetNumber)
    return (result, fromIntegral apiResult)

foreign import ccall unsafe "c/api.h CLAddressGetStreet"
    clib_AddressGetStreet :: CUIntPtr -> Ptr CChar -> CSize -> Ptr CSize -> IO CSize

getStreet :: Address -> IO (Maybe Street, Integer)
getStreet addr = alloca $ \ptrToCSize -> do
    allocaBytes 40 $ \ptrToCChar -> do
        apiResult <- clib_AddressGetStreet (fromIntegral addr) ptrToCChar 40 ptrToCSize
        result <- if apiResult == 0
            then do
                resultStreet <- peekCString ptrToCChar
                return (Just (resultStreet) :: Maybe Street)
            else do
                return (Nothing :: Maybe Street)
        return (result, fromIntegral apiResult)

foreign import ccall unsafe "c/api.h CLAddressGetCity"
    clib_AddressGetCity :: CUIntPtr -> Ptr CChar -> CSize -> Ptr CSize -> IO CSize

getCity :: Address -> IO (Maybe City, Integer)
getCity addr = alloca $ \ptrToCSize -> do
    allocaBytes 16 $ \ptrToCChar -> do
        apiResult <- clib_AddressGetCity (fromIntegral addr) ptrToCChar 16 ptrToCSize
        result <- if apiResult == 0
            then do
                resultCity <- peekCString ptrToCChar
                return (Just (resultCity) :: Maybe City)
            else do
                return (Nothing :: Maybe City)
        return (result, fromIntegral apiResult)

foreign import ccall unsafe "c/api.h CLAddressGetProvince"
    clib_AddressGetProvince :: CUIntPtr -> Ptr CChar -> CSize -> Ptr CSize -> IO CSize

getProvince :: Address -> IO (Maybe Province, Integer)
getProvince addr = alloca $ \ptrToCSize -> do
    allocaBytes 8 $ \ptrToCChar -> do
        apiResult <- clib_AddressGetProvince (fromIntegral addr) ptrToCChar 8 ptrToCSize
        result <- if apiResult == 0
            then do
                resultProvince <- peekCString ptrToCChar
                return (Just (resultProvince) :: Maybe Province)
            else do
                return (Nothing :: Maybe Province)
        return (result, fromIntegral apiResult)

foreign import ccall unsafe "c/api.h CLAddressGetCountry"
    clib_AddressGetCountry :: CUIntPtr -> Ptr CChar -> CSize -> Ptr CSize -> IO CSize

getCountry :: Address -> IO (Maybe Country, Integer)
getCountry addr = alloca $ \ptrToCSize -> do
    allocaBytes 16 $ \ptrToCChar -> do
        apiResult <- clib_AddressGetCountry (fromIntegral addr) ptrToCChar 16 ptrToCSize
        result <- if apiResult == 0
            then do
                resultCountry <- peekCString ptrToCChar
                return (Just (resultCountry) :: Maybe Country)
            else do
                return (Nothing :: Maybe Country)
        return (result, fromIntegral apiResult)

foreign import ccall unsafe "c/api.h CLAddressGetZipCode"
    clib_AddressGetZipCode :: CUIntPtr -> Ptr CChar -> CSize -> Ptr CSize -> IO CSize

getZipCode :: Address -> IO (Maybe ZipCode, Integer)
getZipCode addr = alloca $ \ptrToCSize -> do
    allocaBytes 8 $ \ptrToCChar -> do
        apiResult <- clib_AddressGetZipCode(fromIntegral addr) ptrToCChar 8 ptrToCSize
        result <- if apiResult == 0
            then do
                resultZipCode <- peekCString ptrToCChar
                return (Just (resultZipCode) :: Maybe ZipCode)
            else do
                return (Nothing :: Maybe ZipCode)
        return (result, fromIntegral apiResult)

asString :: Address -> IO String
asString addr = do
    (maybeStreetNumber, getStreetNumberResult) <- getStreetNumber addr
    (maybeStreet, getStreetResult) <- getStreet addr
    (maybeCity, getCityResult) <- getCity addr
    (maybeProvince, getProvinceResult) <- getProvince addr
    (maybeCountry, getCountryResult) <- getCountry addr
    (maybeZipCode, getZipCodeResult) <- getZipCode addr
    result <- if getStreetNumberResult == 0 && getStreetResult == 0 && getCityResult == 0 && getProvinceResult == 0 &&
        getCountryResult == 0 && getZipCodeResult == 0 then do
            return (
                (show $ fromJust maybeStreetNumber) ++ " " ++ (fromJust maybeStreet) ++ "\n" ++
                (fromJust maybeCity) ++ ", " ++ (fromJust maybeProvince) ++ "\n" ++
                (fromJust maybeCountry) ++ " " ++ (fromJust maybeZipCode)
                )
        else do
            return ("")
    return (result)
