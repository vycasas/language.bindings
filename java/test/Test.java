import net.dotslashzero.javalib.Address;
import net.dotslashzero.javalib.IGenerator;
import net.dotslashzero.javalib.Library;
import net.dotslashzero.javalib.Person;
import net.dotslashzero.javalib.Printer;

public final class Test
{
    public static final class MyGenerator implements IGenerator
    {
        public int generateInt(int data)
        {
            return (data * data);
        }

        public String generateString(int data)
        {
            return ("" + data);
        }
    }

    public static void main(String[] args)
    {
        try {
            boolean initOk = Library.initialize();
            if (!initOk)
                return;

            System.out.print("Library initialized... version ");
            System.out.println(Library.getVersionString());

            System.out.println("Creating a new address...");
            Address addr = new Address(9898, "Corner St.", "Gotham", "CA", "4132", "Antartica");
            System.out.println("New address created!");

            System.out.println("Address:");
            System.out.println(addr.toString());

            System.out.println("Creating a new person...");
            Person person = new Person("Wayne", "Bruce", 25, addr);
            System.out.println("New person created!");

            System.out.println("Person:");
            System.out.println(person.toString());

            System.out.println("Creating a generator...");
            MyGenerator generator = new MyGenerator();
            System.out.println("New generator created!");

            System.out.println("Creating a printer...");
            Printer printer = new Printer(generator);
            System.out.println("New printer created!");

            System.out.println("Performing printer actions...");
            printer.printInt();
            printer.printString();
        }
        catch (Exception e) {
            System.err.print("Exception: ");
            System.err.println(e.getMessage());
            e.printStackTrace(System.err);
        }
        catch (Error e) {
            System.err.print("Error: ");
            System.err.println(e.getMessage());
            e.printStackTrace(System.err);
        }
        finally {
            Library.uninitialize();
        }

        return;
    }
}
