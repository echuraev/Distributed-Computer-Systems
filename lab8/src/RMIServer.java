import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.rmi.Naming;
import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class RMIServer extends UnicastRemoteObject implements RMIInterface {

	private static final long serialVersionUID = 20130105120800L;
	private static List<String> entryNames = new ArrayList<String>();
	private static Map <String, List<String>> map = new HashMap<String, List<String>>();
	private static RMISecurityInterface objSecurity = null;

	public RMIServer() throws RemoteException
	{
		super();
	}
	public static void main(String[] args) throws IOException {
		String entryFile = "entry_point.txt";
		if (args.length > 0)
			entryFile = args[0];
		InputStream file = new FileInputStream(entryFile);
		BufferedReader br = new BufferedReader(new InputStreamReader(file));
		String line;
		
		while ((line = br.readLine()) != null) {
			line = line.trim();
			entryNames.add(line);
		}
		br.close();
		for(int i = 0; i < entryNames.size(); ++i) {
			file = new FileInputStream(entryNames.get(i) + ".txt");
			br = new BufferedReader(new InputStreamReader(file));
			List<String> users = new ArrayList<String>();
			while ((line = br.readLine()) != null) {
				line = line.trim();
				users.add(line);
			}
			br.close();
			map.put(entryNames.get(i), users);
		}

		try {
			RMIServer obj = new RMIServer();
			Naming.rebind("RMIServer", obj);
			System.out.println("Server is connected and ready for operation.");
		}
		catch (Exception e) {
			System.out.println ("Server not connected: " + e);
			System.exit(1);
		}
	}

	@Override
	public boolean checkName(String name) throws RemoteException {
		if (entryNames.contains(name)) 
			return true;
		return false;
	}
	@Override
	public boolean checkInVisitor(String entryName, String userName)
			throws RemoteException {
		if (map.get(entryName).contains(userName)) {
			try {
				objSecurity = (RMISecurityInterface)Naming.lookup("//127.0.0.1/RMISecurity");
				objSecurity.addToHistory(entryName, userName, "in", true);
			} catch (Exception e) {
				System.err.println("Client exception: " + e.toString());
				e.printStackTrace();
		    }
			return true;
		}
		try {
			objSecurity = (RMISecurityInterface)Naming.lookup("//127.0.0.1/RMISecurity");
			objSecurity.addToHistory(entryName, userName, "in", false);
			objSecurity.accessDenied(entryName, userName, "in");
		} catch (Exception e) {
			System.err.println("Client exception: " + e.toString());
			e.printStackTrace();
	    }
		return false;
	}
	@Override
	public boolean checkOutVisitor(String entryName, String userName)
			throws RemoteException {
		if (map.get(entryName).contains(userName)) {
			try {
				objSecurity = (RMISecurityInterface)Naming.lookup("//127.0.0.1/RMISecurity");
				objSecurity.addToHistory(entryName, userName, "out", true);
			} catch (Exception e) {
				System.err.println("Client exception: " + e.toString());
				e.printStackTrace();
		    }
			return true;
		}
		try {
			objSecurity = (RMISecurityInterface)Naming.lookup("//127.0.0.1/RMISecurity");
			objSecurity.addToHistory(entryName, userName, "out", false);
			objSecurity.accessDenied(entryName, userName, "out");
		} catch (Exception e) {
			System.err.println("Client exception: " + e.toString());
			e.printStackTrace();
	    }
		return false;
	}

}
