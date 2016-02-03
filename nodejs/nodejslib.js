'use strict';

const os = require('os');
const nodejslib_native = require('./nodejslib_native');

class Library {
  constructor() {
    throw new Error('Invalid function call. This function cannot be invoked.');
  }

  static initialize() {
    nodejslib_native.LibraryInitialize();
  }

  static terminate() {
    nodejslib_native.LibraryTerminate();
  }

  static getVersionString() {
    return (nodejslib_native.LibraryGetVersionString());
  }

  static getVersionMajor() {
    return (nodejslib_native.LibraryGetVersionMajor());
  }

  static getVersionMinor() {
    return (nodejslib_native.LibraryGetVersionMinor());
  }
};

class Address {
  constructor(streetNum, street, city, province, contry, zipCode) {
    this._impl = new nodejslib_native.Address(streetNum, street, city, province, contry, zipCode);
  }

  toString() {
    return (this._impl.toString());
  }

  get streetNum() {
    return (this._impl.getStreetNum());
  }

  get street() {
    return (this._impl.getStreet());
  }

  get city() {
    return (this._impl.getCity());
  }

  get province() {
    return (this._impl.getProvince());
  }

  get country() {
    return (this._impl.getCountry());
  }

  get zipCode() {
    return (this._impl.zipCode());
  }
};

class Person {
  constructor(lastName, firstName, age, address) {
    this._impl = new nodejslib_native.Person(lastName, firstName, age, address._impl);
  }

  toString() {
    return (this._impl.toString());
  }

  // get lastName() {
  //   return (this._impl.getLastName());
  // }

  // get firstName() {
  //   return (this._impl.getFirstName());
  // }

  // get age() {
  //   return (this._impl.getAge());
  // }

  // get address() {
  //   return (this._impl.getAddress());
  // }
};

/*
class Printer {
  constructor() {

  }
}
*/

module.exports.Library = Library;
module.exports.Address = Address;
module.exports.Person = Person;
