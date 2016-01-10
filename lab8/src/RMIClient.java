import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.rmi.Naming;

public class RMIClient {
	
	private static RMIInterface obj = null;
	public static void main(String[] args) throws IOException {
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		String name;
		System.out.print("Enter name of entry: ");
		name = br.readLine();
		
		try {
			obj = (RMIInterface)Naming.lookup("//127.0.0.1/RMIServer");
			if (obj.checkName(name) == false) {
				System.err.println("Error! Unknown name of entry!");
				System.exit(1);
			}
			while (true)
			{
				System.out.print("Enter name of visitor (in name or out name or exit): ");
				String user = br.readLine();
				if (user.equals("exit"))
					break;
				String [] checkUser = user.split(" ");
				if (checkUser.length < 2) {
					System.out.println("Error! Invalid argument!");
					continue;
				}
				if (checkUser[0].equals("in"))
					if (obj.checkInVisitor(name, checkUser[1]))
						System.out.println("Welcome " + checkUser[1] + "!");
					else
						System.out.println("Access denied!");
				else if (checkUser[0].equals("out"))
					if (obj.checkOutVisitor(name, checkUser[1]))
						System.out.println("Goodbye " + checkUser[1] + "!");
					else
						System.out.println("Access denied!");
				else {
					System.out.println("Error! Invalid argument!");
					continue;
				}
			}
		} catch (Exception e) {
			System.err.println("Client exception: " + e.toString());
			e.printStackTrace();
	    }
	}

}
