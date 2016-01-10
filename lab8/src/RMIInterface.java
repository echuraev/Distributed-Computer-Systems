import java.rmi.Remote;
import java.rmi.RemoteException;

public interface RMIInterface extends Remote {
	boolean checkName(String name) throws RemoteException;
	boolean checkInVisitor(String entryName, String userName) throws RemoteException;
	boolean checkOutVisitor(String entryName, String userName) throws RemoteException;
}
