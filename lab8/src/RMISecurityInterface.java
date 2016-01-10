import java.rmi.Remote;
import java.rmi.RemoteException;


public interface RMISecurityInterface extends Remote {
	void addToHistory(String entryName, String userName, String way, boolean status) throws RemoteException;
	void accessDenied(String entryName, String userName, String way) throws RemoteException;
}
