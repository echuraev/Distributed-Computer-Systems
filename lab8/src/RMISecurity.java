import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;
import java.util.List;


public class RMISecurity extends UnicastRemoteObject implements RMISecurityInterface {

	private static final long serialVersionUID = 20130105143200L;
	
	private static List<String> history = new ArrayList<String>();
	
	protected RMISecurity() throws RemoteException {
		super();
	}
	
	public static void main(String[] args) throws IOException {
		try {
			RMISecurity obj = new RMISecurity();
			Naming.rebind("RMISecurity", obj);
			BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
			while (true) {
				System.out.print("Enter command (history or exit): ");
				String cmd = br.readLine();
				if (cmd.equals("exit"))
					System.exit(0);
				if (cmd.equals("history")) {
					System.out.println("\nINPUT\tNAME\tWAY\tSTATUS\n");
					for (String s : history) {
						System.out.println(s);
					}
				}
			}
		}
		catch (Exception e) {
			System.out.println ("Server not connected: " + e);
			System.exit(1);
		}
	}

	@Override
	public void addToHistory(String entryName, String userName, String way, boolean status)
			throws RemoteException {
		if (status)
			history.add(entryName + "\t" + userName + "\t" + way + "\ttrue");
		else
			history.add(entryName + "\t" + userName + "\t" + way + "\tfalse");
	}

	@Override
	public void accessDenied(String entryName, String userName, String way)
			throws RemoteException {
		System.out.println("\nWARNING! ACCESS DENIED! Input: " + entryName + ", User: " + userName + ", Way: " + way);
	}
	
}
