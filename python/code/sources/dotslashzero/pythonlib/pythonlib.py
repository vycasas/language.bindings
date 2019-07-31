#!/usr/bin/env python3

import abc
import os
import sys
import pythonlib_native

__version__ = '2.0'
__all__ = [
    'PythonLibError',
    'Library']

class PythonLibError(RuntimeError):
    def __init__(self, message):
        self.message = message

    def getMessage():
        return message

class Library(object):
    __is_initialized = False

    @staticmethod
    def initialize():
        if Library.__is_initialized:
            return

        try:
            pythonlib_native.library_initialize()
        except RuntimeError as e:
            raise PythonLibError(str(e))

        Library.__is_initialized = True

    @staticmethod
    def uninitialize():
        try:
            if Library.__is_initialized:
                pythonlib_native.library_uninitialize()
                Library.__is_initialized = False
            # should the native library be unloaded?
        except:
            return

    @staticmethod
    def get_version_string():
        try:
            return pythonlib_native.library_get_version_string()
        except RuntimeError as e:
            raise PythonLibError(str(e))

    @staticmethod
    def get_version_major():
        try:
            return pythonlib_native.library_get_version_major()
        except RuntimeError as e:
            raise PythonLibError(str(e))

    @staticmethod
    def get_version_minor():
        try:
            return pythonlib_native.library_get_version_minor()
        except RuntimeError as e:
            raise PythonLibError(str(e))

    @staticmethod
    def get_version_patch():
        try:
            return pythonlib_native.library_get_version_patch()
        except RuntimeError as e:
            raise PythonLibError(str(e))

    @staticmethod
    def get_version_extra():
        try:
            return pythonlib_native.library_get_version_extra()
        except RuntimeError as e:
            raise PythonLibError(str(e))

class Address(object):
    def __init__(
        self,
        *args,
        **kwargs):
        try:
            (address_impl, street_num, street, city, province, zip_code, country) = Address.__get_init_args(kwargs)
            if address_impl is not None:
                self.__impl = address_impl
            else:
                self.__impl = pythonlib_native.address_create(
                    street_num=street_num,
                    street=street,
                    city=city,
                    province=province,
                    zip_code=zip_code,
                    country=country)
        except RuntimeError as e:
            raise PythonLibError(str(e))

    def __del__(self):
        try:
            pythonlib_native.address_destroy(impl=self.__impl)
        except:
            return

    @staticmethod
    def __get_init_args(kwargs):
        assert kwargs is not None

        address_impl = kwargs.get('address_impl', None)
        if address_impl is not None:
            return (address_impl, None, None, None, None, None, None)

        street_num = kwargs.get('street_num', None)
        if street_num is None:
            raise PythonLibError('Missing street_num')

        street = kwargs.get('street', None)
        if street is None:
            raise PythonLibError('Missing street')

        city = kwargs.get('city', None)
        if city is None:
            raise PythonLibError('Missing city')

        province = kwargs.get('province', None)
        if province is None:
            raise PythonLibError('Missing province')

        zip_code = kwargs.get('zip_code', None)
        if zip_code is None:
            raise PythonLibError('Missing zip_code')

        country = kwargs.get('country', None)
        if country is None:
            raise PythonLibError('Missing country')

        return (None, street_num, street, city, province, zip_code, country)

    @classmethod
    def _from_impl(addressClass, address_impl):
        return addressClass(address_impl=address_impl)

    @property
    def street_num(self):
        try:
            return pythonlib_native.addres_get_street_num(impl=self.__impl)
        except RuntimeError as e:
            raise PythonLibError(str(e))

    @property
    def street(self):
        try:
            return pythonlib_native.addres_get_street(impl=self.__impl)
        except RuntimeError as e:
            raise PythonLibError(str(e))

    @property
    def city(self):
        try:
            return pythonlib_native.addres_get_city(impl=self.__impl)
        except RuntimeError as e:
            raise PythonLibError(str(e))

    @property
    def province(self):
        try:
            return pythonlib_native.addres_get_province(impl=self.__impl)
        except RuntimeError as e:
            raise PythonLibError(str(e))

    @property
    def zip_code(self):
        try:
            return pythonlib_native.addres_get_zip_code(impl=self.__impl)
        except RuntimeError as e:
            raise PythonLibError(str(e))

    @property
    def country(self):
        try:
            return pythonlib_native.addres_get_country(impl=self.__impl)
        except RuntimeError as e:
            raise PythonLibError(str(e))

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        try:
            return pythonlib_native.address_to_string(impl=self.__impl)
        except RuntimeError as e:
            raise PythonLibError(str(e))

    def _get_impl(self):
        return self.__impl

class Person(object):
    def __init__(
        self,
        *args,
        **kwargs):
        try:
            (last_name, first_name, age, address) = Person.__get_init_args(kwargs)
            self.__impl = pythonlib_native.person_create(
                last_name,
                first_name,
                age,
                address._get_impl())
        except RuntimeError as e:
            raise PythonLibError(str(e))

    def __del__(self):
        try:
            pythonlib_native.person_destroy(impl=self.__impl)
        except:
            return

    @staticmethod
    def __get_init_args(kwargs):
        assert kwargs is not None

        last_name = kwargs.get('last_name', None)
        if last_name is None:
            raise PythonLibError('Missing last_name')

        first_name = kwargs.get('first_name', None)
        if first_name is None:
            raise PythonLibError('Missing first_name')

        age = kwargs.get('age', None)
        if age is None:
            raise PythonLibError('Missing age')

        address = kwargs.get('address', None)
        if address is None:
            raise PythonLibError('Missing address')

        return (last_name, first_name, age, address)

    @property
    def last_name(self):
        try:
            return pythonlib_native.person_get_last_name(impl=self.__impl)
        except RuntimeError as e:
            raise PythonLibError(str(e))

    @property
    def first_name(self):
        try:
            return pythonlib_native.person_get_first_name(impl=self.__impl)
        except RuntimeError as e:
            raise PythonLibError(str(e))

    @property
    def age(self):
        try:
            return pythonlib_native.person_get_age(impl=self.__impl)
        except RuntimeError as e:
            raise PythonLibError(str(e))

    @property
    def address(self):
        try:
            address_impl = pythonlib_native.person_get_address(impl=self.__impl)
            return Address._from_impl(address_impl=address_impl)
        except RuntimeError as e:
            raise PythonLibError(str(e))

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        try:
            return pythonlib_native.person_to_string(impl=self.__impl)
        except RuntimeError as e:
            raise PythonLibError(str(e))

class GeneratorBase(metaclass=abc.ABCMeta):
    @abc.abstractmethod
    def generate_int(
        self,
        data):
        pass

    @abc.abstractmethod
    def generate_string(
        self,
        data):
        pass

class Printer(object):
    def __init__(
        self,
        *args,
        **kwargs):
        self.__generator = kwargs.get('generator', None)
        if self.__generator is None:
            raise PythonLibError('Missing generator')

        try:
            self.__impl = pythonlib_native.printer_create()
        except RuntimeError as e:
            raise PythonLibError(str(e))

    def __del__(self):
        try:
            pythonlib_native.printer_destroy(impl=self.__impl)
        except:
            return

    def print_int(self):
        try:
            pythonlib_native.printer_print_int(
                impl=self.__impl,
                generator=self.__generator)
        except RuntimeError as e:
            raise PythonLibError(str(e))

    def print_string(self):
        try:
            pythonlib_native.printer_print_string(
                impl=self.__impl,
                generator=self.__generator)
        except RuntimeError as e:
            raise PythonLibError(str(e))
