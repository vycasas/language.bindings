'use strict';

const nodejslib_native = require('./nodejslib_native');

class Exception extends Error {
  constructor(message) {
    super(message);
    if (Error.captureStackTrace) {
      Error.captureStackTrace(this, Exception);
    }
  }
}
const Library = {
  initialize: () => {
    try {
      return (nodejslib_native.libraryInitialize());
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  },
  uninitialize: () => {
    nodejslib_native.libraryUninitialize();
    return;
  },
  getVersionString: () => {
    try {
      return (nodejslib_native.libraryGetVersionString());
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  },
  getVersionMajor: () => {
    try {
      return (nodejslib_native.libraryGetVersionMajor());
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  },
  getVersionMinor: () => {
    try {
      return (nodejslib_native.libraryGetVersionMinor());
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  },
  getVersionPatch: () => {
    try {
      return (nodejslib_native.libraryGetVersionPatch());
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  },
  getVersionExtra: () => {
    try {
      return (nodejslib_native.libraryGetVersionExtra());
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }
};

class Address {
  constructor(args) {
    try {
      if (args.hasOwnProperty('impl')) {
        this.m_impl = args.impl;
        return;
      }

      this.m_impl = nodejslib_native.addressCreate({
        streetNum: args.streetNum,
        street: args.street,
        city: args.city,
        province: args.province,
        zipCode: args.zipCode,
        country: args.country
      });
      return;
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }

  get streetNum() {
    try {
      return (nodejslib_native.addressGetStreetNum({ impl: this.m_impl }));
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }

  get street() {
    try {
      return (nodejslib_native.addressGetStreet({ impl: this.m_impl }));
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }

  get city() {
    try {
      return (nodejslib_native.addressGetCity({ impl: this.m_impl }));
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }

  get province() {
    try {
      return (nodejslib_native.addressGetProvince({ impl: this.impl }));
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }

  get zipCode() {
    try {
      return (nodejslib_native.addressGetZipCode({ impl: this.m_impl }));
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }

  get country() {
    try {
      return (nodejslib_native.addressGetCountry({ impl: this.m_impl }));
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }

  toString() {
    try {
      return (nodejslib_native.addressToString({ impl: this.m_impl }));
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }
}

class Person {
  constructor(args) {
    try {
      this.m_impl = nodejslib_native.personCreate({
        lastName: args.lastName,
        firstName: args.firstName,
        age: args.age,
        addressImpl: args.address.m_impl
      });
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }

  get lastName() {
    try {
      return (nodejslib_native.personGetLastName({ impl: this.m_impl }));
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }

  get firstName() {
    try {
      return (nodejslib_native.personGetFirstName({ impl: this.m_impl }));
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }

  get age() {
    try {
      return (nodejslib_native.personGetAge({ impl: this.m_impl }));
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }

  get address() {
    try {
      const addressImpl = nodejslib_native.personGetAddress({ impl: this.m_impl });
      return (new Address({ impl: addressImpl }));
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }

  toString() {
    try {
      return (nodejslib_native.personToString({ impl: this.m_impl }));
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }
}

class Generator {
  generateInt(_) {
    throw (new Error('Not implemented'));
  }

  generateString(_) {
    throw (new Error('Not implemented'));
  }
}

class Printer {
  constructor(args) {
    this.m_generator = args.generator;
    try {
      this.m_impl = nodejslib_native.printerCreate();
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }

  printInt() {
    try {
      nodejslib_native.printerPrintInt({
        impl: this.m_impl,
        generator: this.m_generator
      });
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }

  printString() {
    try {
      nodejslib_native.printerPrintString({
        impl: this.m_impl,
        generator: this.m_generator
      });
    }
    catch (error) {
      throw (new Exception(error.message));
    }
  }
}

module.exports.Exception = Exception;
module.exports.Library = Library;
module.exports.Address = Address;
module.exports.Person = Person;
module.exports.Generator = Generator;
module.exports.Printer = Printer;
