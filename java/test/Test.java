import net.dotslashzero.javalib.Address;
import net.dotslashzero.javalib.Library;
import net.dotslashzero.javalib.Person;

public class Test
{
    public static void main(String[] args)
    {
        try {
            Library.initialize();

            System.out.println("Creating a new address...");
            Address addr = new Address(9898, "Corner St.", "Gotham", "CA", "Antartica", "4132");
            System.out.println("New address created!");
            System.out.println("Address:");
            System.out.println(addr.toString());

            System.out.println("Creating a new person...");
            Person person = new Person("Wayne", "Bruce", 25, addr);
            System.out.println("New person created!");
            System.out.println("Person:");
            System.out.println(person.toString());

            Library.terminate();
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

        return;
    }
}
