#!/usr/bin/env node
'use strict';

const process = require('process');

const nodejslib = require('./nodejslib');

class MyGenerator extends nodejslib.Generator {
  generateInt(data) {
    return (data * data);
  }

  generateString(data) {
    return (`${data}`);
  }
}

function main(args) {
  return new Promise((resolve, reject) => {
    const initOk = nodejslib.Library.initialize();
    if (!initOk) {
      reject('Failed to initialize library.');
      return;
    }

    try {
      console.log(`Library initialized... version ${nodejslib.Library.getVersionString()}`);

      console.log('Creating a new address...');
      const address = new nodejslib.Address({
        streetNum: 9898,
        street: 'Corner St.',
        city: 'Gotham',
        province: 'CA',
        zipCode: '4132',
        country:'Antartica'
      });
      console.log('New address created!');

      console.log('Address:');
      console.log(`${address}`);

      console.log('Creating a new person...');
      const person = new nodejslib.Person({
        lastName: 'Wayne',
        firstName: 'Bruce',
        age: 25,
        address: address
      });
      console.log('New person created!');

      console.log('Person:');
      console.log(`${person}`);

      console.log('Creating a new generator...');
      const myGenerator = new MyGenerator();
      console.log('New generator created!');

      console.log('Creating a new printer...');
      const printer = new nodejslib.Printer({
        generator: myGenerator
      });
      console.log('New printer created!');

      console.log('Performing printer actions...');
      printer.printInt();
      printer.printString();

      resolve();
    }
    catch (error) {
      reject(error);
    }
    finally {
      nodejslib.Library.uninitialize();
    }
  });
}

main(process.argv).then(() => {
  process.exit(0);
}).catch(error => {
  console.error(`An error has occurred: ${error}`);
  process.exit(-1);
});
