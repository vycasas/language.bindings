#!/usr/bin/env python

'''
This file is a bridge module that performs the necessary glue between the client (end-user) code and the native library.
This module provides easy initialization by loading the native modules through initialization methods.
'''

import abc
import imp
import os
import sys

__version__ = '1.0'
__all__ = [
    'PythonLibError',
    'Library',
    'Address',
    'Person',
    'GeneratorBase',
    'Printer'
]

_pythonlib_native = None

class PythonLibError(Exception):
    def __init__(self, message):
        self.message = message

    def getMessage():
        return message

class Library(object):
    is_initialized = False

    @staticmethod
    def initialize():
        global _pythonlib_native
        if Library.is_initialized:
            return

        try:
            (module_file, file_path, module_description) = imp.find_module('pythonlib_native', [os.getcwd()])
            if module_file is None:
                raise Exception('')

            _pythonlib_native = imp.load_module('_pythonlib_native', module_file, file_path, module_description)
            if _pythonlib_native is None:
                raise Exception('')
        except:
            raise PythonLibError('Error loading module "pythonlib_native".')

        _pythonlib_native.library_initialize()
        Library.is_initialized = True

        return

    @staticmethod
    def terminate():
        if Library.is_initialized:
            _pythonlib_native.library_terminate()
            Library.is_initialized = False
        return

    @staticmethod
    def get_version_string():
        return _pythonlib_native.library_get_version_string()

    @staticmethod
    def get_version_major():
        return _pythonlib_native.library_get_version_major()

    @staticmethod
    def get_version_minor():
        return _pythonlib_native.library_get_version_minor()

def _check_if_initialized():
    if not Library.is_initialized:
        raise PythonLibError('PythonLib is not initialized. Please call Library.initialize() first')
    return

class Address(object):
    def __init__(self, street_num, street, city, province, country, zip_code, impl = None):
        _check_if_initialized()
        if impl <> None:
            self.impl = impl
        else:
            self.impl = 0
            self.impl = _pythonlib_native.address_create(
                street_num=street_num,
                street=street,
                city=city,
                province=province,
                country=country,
                zip_code=zip_code
            )
        return

    def __del__(self):
        if self.impl <> 0:
            _pythonlib_native.address_destroy(impl=self.impl)
        return

    def get_street_num(self):
        _check_if_initialized()
        return _pythonlib_native.address_get_street_num(impl=self.impl)

    street_num = property(get_street_num)

    def get_street(self):
        _check_if_initialized()
        return _pythonlib_native.address_get_street(impl=self.impl)

    street = property(get_street)

    def get_city(self):
        _check_if_initialized()
        return _pythonlib_native.address_get_city(impl=self.impl)

    city = property(get_city)

    def get_province(self):
        _check_if_initialized()
        return _pythonlib_native.address_get_province(impl=self.impl)

    province = property(get_province)

    def get_country(self):
        _check_if_initialized()
        return _pythonlib_native.address_get_country(impl=self.impl)

    country = property(get_country)

    def get_zip_code(self):
        _check_if_initialized()
        return _pythonlib_native.address_get_zip_code(impl=self.impl)

    zip_code = property(get_zip_code)

    def __str__(self):
        _check_if_initialized()
        return '{} {}{}{}, {}{}{} {}'.format(
            self.street_num, self.street, os.linesep,
            self.city, self.province, os.linesep,
            self.country, self.zip_code
        )

class Person(object):
    def __init__(self, last_name, first_name, age, address, impl = None):
        _check_if_initialized()
        if impl <> None:
            self.impl = impl
        else:
            self.impl = 0
            self.impl = _pythonlib_native.person_create(
                last_name=last_name,
                first_name=first_name,
                age=age,
                address_impl=address.impl
            )
        return

    def __del__(self):
        if self.impl <> 0:
            _pythonlib_native.person_destroy(impl=self.impl)
        return

    def get_last_name(self):
        _check_if_initialized()
        return _pythonlib_native.person_get_last_name(impl=self.impl)

    last_name = property(get_last_name)

    def get_first_name(self):
        _check_if_initialized()
        return _pythonlib_native.person_get_first_name(impl=self.impl)

    first_name = property(get_first_name)

    def get_age(self):
        _check_if_initialized()
        return _pythonlib_native.person_get_age(impl=self.impl)

    age = property(get_age)

    def get_address(self):
        _check_if_initialized()
        address_impl = _pythonlib_native.person_get_address(impl=self.impl)
        return Address(0, '', '', '', '', '', address_impl)

    address = property(get_address)

    def __str__(self):
        return '{}, {}{}{} years old{}{}'.format(
            self.last_name, self.first_name, os.linesep,
            self.age, os.linesep,
            self.address
        )

class GeneratorBase(object):
    __metaclass__ = abc.ABCMeta

    @abc.abstractmethod
    def generate_int(self, data):
        pass

    @abc.abstractmethod
    def generate_string(self, data):
        pass

class Printer(object):
    def __init__(self, generator):
        _check_if_initialized()
        self.impl = 0
        self.impl = _pythonlib_native.printer_create(generator=generator)
        return

    def __del__(self):
        if self.impl <> 0:
            _pythonlib_native.printer_destroy(impl=self.impl)
        return

    def print_int(self):
        _check_if_initialized()
        _pythonlib_native.printer_print_int(impl=self.impl)
        return

    def print_string(self):
        _check_if_initialized()
        _pythonlib_native.printer_print_string(impl=self.impl)
        return
