#!/usr/bin/env python

import os
import sys

import pythonlib

class MyGenerator(pythonlib.GeneratorBase):
    def __init__(self):
        pass

    def generate_int(self, data):
        return data * data

    def generate_string(self, data):
        return '{}'.format(data)

def main(args):
    try:
        pythonlib.Library.initialize()

        print('Library initialized... version {}'.format(pythonlib.Library.get_version_string()))

        print('Creating a new address...')
        address = pythonlib.Address(9898, 'Corner St.', 'Gotham', 'CA', 'Antartica', '4132')
        print('New address created!')
        print('Address:')
        print(address)

        print('Creating a new person...')
        person = pythonlib.Person('Wayne', 'Bruce', 28, address)
        print('New person created!')
        print('Person:')
        print(person)

        print('Creating a new generator...')
        generator = MyGenerator()
        print('New generator created!')
        print('Creating a new printer...')
        printer = pythonlib.Printer(generator)
        print('New printer created!')
        print('Performing printer actions...')
        printer.print_int()
        printer.print_string()

        pythonlib.Library.terminate()
    except pythonlib.PythonLibError as e:
        sys.stderr.write('An error has occurred: {}.{}'.format(e.getMessage(), os.linesep))
        sys.stderr.flush()
        return -1
    except Exception as e:
        sys.stderr.write('An error has occurred: {}.{}'.format(e, os.linesep))
        sys.stderr.flush()
        return -1
    except:
        sys.stderr.write('An unknown error has occurred.{}'.format(os.linesep))
        sys.stderr.flush()
        return -1

    return 0

if __name__ == '__main__':
    sys.exit(main(sys.argv))
