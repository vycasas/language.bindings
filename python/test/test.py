#!/usr/bin/env python3

# To run this test:
# export PYTHONPATH=/path/to/pythonlib_native.so/parent/directory
# python3 pythonlib_test.py

import sys
import pythonlib

class MyGenerator(pythonlib.GeneratorBase):
    def generate_int(
        self,
        data):
        return data * data

    def generate_string(
        self,
        data):
        return f'{data}'

def main(args):
    try:
        init_ok = pythonlib.Library.initialize()
        if not init_ok:
            sys.stderr.write('Failed to initialize library.')
            return -1

        sys.stdout.write(f'Library initialized... version {pythonlib.Library.get_version_string()}\n')

        sys.stdout.write('Creating a new address...\n')
        address = pythonlib.Address(
            street_num=9898,
            street='Corner St.',
            city='Gotham',
            province='CA',
            zip_code='4132',
            country='Antartica')
        sys.stdout.write('New address created!\n')

        sys.stdout.write(f'Address:\n{address}\n')

        sys.stdout.write('Creating a new person...\n')
        person = pythonlib.Person(
            last_name='Wayne',
            first_name='Bruce',
            age=25,
            address=address)
        sys.stdout.write('New person created!\n')

        sys.stdout.write(f'Person:\n{person}\n')

        sys.stdout.write('Creating a new generator...\n')
        generator = MyGenerator()
        sys.stdout.write('New generator created!\n')

        sys.stdout.write('Creating a new printer...\n')
        printer = pythonlib.Printer(generator=generator)
        sys.stdout.write('New printer created!\n')

        sys.stdout.write('Performing printer actions...\n')
        printer.print_int()
        printer.print_string()

        return 0
    except Exception as e:
        sys.stderr.write(f'An error has occurred: {e}\n')
        return -1
    finally:
        pythonlib.Library.uninitialize()

if __name__ == '__main__':
    sys.exit(main(sys.argv))
