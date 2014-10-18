#!/usr/bin/env ruby

require '%{pwd}/rubylib' % { :pwd => Dir.pwd }

class MyGenerator < RubyLib::GeneratorBase
    def initialize
    end

    def generate_int(data)
        return data * data
    end

    def generate_string(data)
        return '%{data}' % { :data => data }
    end
end

begin
    RubyLib::Library::initialize

    puts('Creating new address...')
    address = RubyLib::Address.new(9898, 'Corner St.', 'Gotham', 'CA', 'Antartica', '4132')
    puts('New address created!')
    puts('Address:')
    puts(address)

    puts('Creating new person...')
    person = RubyLib::Person.new('Wayne', 'Bruce', 28, address)
    puts('New person created!')
    puts('Person:')
    puts(person)

    puts('Creating a new generator...')
    generator = MyGenerator.new
    puts('New generator created!')
    puts('Creating a new printer...')
    printer = RubyLib::Printer.new(generator)
    puts('New printer created!')
    puts('Performing printer actions...')
    printer.print_int
    printer.print_string

    RubyLib::Library::terminate
rescue RubyLib::RubyLibError => e
    STDERR.puts('An error has occurred: %{err_message}' % { :err_message => e.get_message })
rescue Exception => e
    STDERR.puts('An error has occurred: %{error}' % { :error => e })
end
