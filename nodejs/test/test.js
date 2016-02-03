'use strict';

// TODO: add a commented section about using
// node --expose-gc test.js
// and global.gc(); to verify that v8's garbage collector eventually frees native objects.

const nodejslib = require('./nodejslib');

const main = () => {
  try {
    nodejslib.Library.initialize();

    console.log(`Library initialized... version ${nodejslib.Library.getVersionString()}`);

    console.log('Creating a new address...');
    const address = new nodejslib.Address(9898, 'Corner St.', 'Gotham', 'CA', 'Antartica', '4132');
    console.log('New address created!');
    console.log(`Address: ${address.toString()}`);

    console.log('Creating a new person...');
    const person = new nodejslib.Person('Wayne', 'Bruce', 25, address);
    console.log('New person created!');
    console.log(person.toString());

    nodejslib.Library.terminate();
  }
  catch (error) {
    console.error(`An error has occurred: ${error}`);
    return (-1);
  }
  return (0);
};

process.exit(main());
