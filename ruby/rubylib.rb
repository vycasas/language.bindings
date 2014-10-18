#!/usr/bin/env ruby

require '%{pwd}/rubylibnative' % { :pwd => Dir.pwd }

module RubyLib

    class RubyLibError < StandardError
        def initialize(impl)
            @impl = impl
        end
        def get_message
            args = {
                'impl' => @impl,
                'message' => nil
            }
            RubyLibNative::RubyLibError::get_message(args)
            return args['message']
        end
    end # class RubyLibError

    class NoImplementationError < StandardError
        def initialize
            super("Method has no implementation.")
        end
    end

    class Library < Object
        def self.initialize
            result = RubyLibNative::Library::initialize
            if result != 0
                raise RubyLibError.new(result)
            end
        end

        def self.terminate
            RubyLibNative::Library::terminate
        end
    end # class Library

    class Address < Object
        def initialize(street_num, street, city, province, country, zip_code, impl = nil)
            if impl != nil
                @impl = impl
            else
                args =  {
                    'street_num' => street_num,
                    'street' => street,
                    'city' => city,
                    'province' => province,
                    'country' => country,
                    'zip_code' => zip_code,
                    'impl' => nil
                }
                result = RubyLibNative::Address::create(args)
                if result != 0
                    raise RubyLibError.new(result)
                end
                @impl = args['impl']
            end
            ObjectSpace::define_finalizer(self, method(:finalize))
        end

        def finalize(object_id)
            args = {
                'impl' => @impl 
            }
            RubyLibNative::Address::destroy(args)
        end

        def street_num
            args = {
                'impl' => @impl,
                'street_num' => nil
            }
            result = RubyLibNative::Address::get_street_num(args)
            if result != 0
                raise RubyLibError.new(result)
            end
            return args['street_num']
        end

        def street
            args = {
                'impl' => @impl,
                'street' => nil
            }
            result = RubyLibNative::Address::get_street(args)
            if result != 0
                raise RubyLibError.new(result)
            end
            return args['street']
        end

        def city
            args = {
                'impl' => @impl,
                'city' => nil
            }
            result = RubyLibNative::Address::get_city(args)
            if result != 0
                raise RubyLibError.new(result)
            end
            return args['city']
        end

        def province
            args = {
                'impl' => @impl,
                'province' => nil
            }
            result = RubyLibNative::Address::get_province(args)
            if result != 0
                raise RubyLibError.new(result)
            end
            return args['province']
        end

        def country
            args = {
                'impl' => @impl,
                'country' => nil
            }
            result = RubyLibNative::Address::get_country(args)
            if result != 0
                raise RubyLibError.new(result)
            end
            return args['country']
        end

        def zip_code
            args = {
                'impl' => @impl,
                'zip_code' => nil
            }
            result = RubyLibNative::Address::get_zip_code(args)
            if result != 0
                raise RubyLibError.new(result)
            end
            return args['zip_code']
        end

        def inspect
            return self.to_s
        end

        def to_s
            '%{street_num} %{street}%{new_line}%{city}, %{province}%{new_line}%{country} %{zip_code}' %  {
                :street_num => self.street_num, :street => self.street,
                :city => self.city, :province => self.province,
                :country => self.country, :zip_code => self.zip_code,
                :new_line => "\n"
            }
        end
    end # class Address

    class Person < Object
        def initialize(last_name, first_name, age, address, impl = nil)
            if impl != nil
                @impl = nil
            else
                args =  {
                    'last_name' => last_name,
                    'first_name' => first_name,
                    'age' => age,
                    'address_impl' => address.instance_variable_get(:@impl),
                    'impl' => nil
                }
                result = RubyLibNative::Person::create(args)
                if result != 0
                    raise RubyLibError.new(result)
                end
                @impl = args['impl']
            end
            ObjectSpace::define_finalizer(self, method(:finalize))
        end

        def finalize(object_id)
            args = {
                'impl' => @impl 
            }
            RubyLibNative::Person::destroy(args)
        end

        def last_name
            args = {
                'impl' => @impl,
                'last_name' => nil
            }
            result = RubyLibNative::Person::get_last_name(args)
            if result != 0
                raise RubyLibError.new(result)
            end
            return args['last_name']
        end

        def first_name
            args = {
                'impl' => @impl,
                'first_name' => nil
            }
            result = RubyLibNative::Person::get_first_name(args)
            if result != 0
                raise RubyLibError.new(result)
            end
            return args['first_name']
        end

        def age
            args = {
                'impl' => @impl,
                'age' => nil
            }
            result = RubyLibNative::Person::get_age(args)
            if result != 0
                raise RubyLibError.new(result)
            end
            return args['age']
        end

        def address
            args = {
                'impl' => @impl,
                'address_impl' => nil
            }
            result = RubyLibNative::Person::get_address_impl(args)
            if result != 0
                raise RubyLibError.new(result)
            end
            return Address.new(
                0, nil, nil, nil, nil, nil,
                args['address_impl']
            )
        end

        def inspect
            return self.to_s
        end

        def to_s
            '%{last_name}, %{first_name}%{new_line}%{age} years old%{new_line}%{address}' % {
                :last_name => self.last_name, :first_name => self.first_name,
                :age => self.age,
                :address => self.address,
                :new_line => "\n"
            }
        end
    end # class Person

    class GeneratorBase < Object
        def initialize
            ObjectSpace::define_finalizer(self, method(:finalize))
        end

        def finalize(object_id)
        end

        def generate_int(data)
            raise NoImplementationError.new
        end

        def generate_string(data)
            raise NoImplementationError.new
        end
    end # class GeneratorBase

    class Printer < Object
        def initialize(generator)
            args = {
                'generator' => generator,
                'impl' => nil
            }
            result = RubyLibNative::Printer::create(args)
            if result != 0
                raise RubyLibError.new(result)
            end
            @impl = args['impl']
            # due to the way GC works in Ruby (via mark & sweep), we need to keep the generator's reference
            # alive for as long as this Printer object is alive.
            @generator = generator
            ObjectSpace::define_finalizer(self, method(:finalize))
        end

        def finalize(object_id)
            args = {
                'impl' => @impl 
            }
            RubyLibNative::Printer::destroy(args)
            @generator = nil
        end

        def print_int
            args = {
                'impl' => @impl 
            }
            result = RubyLibNative::Printer::print_int(args)
            if result != 0
                raise RubyLibError.new(result)
            end
        end

        def print_string
            args = {
                'impl' => @impl 
            }
            result = RubyLibNative::Printer::print_string(args)
            if result != 0
                raise RubyLibError.new(result)
            end
        end
    end # class Printer
end # module RubyLib
