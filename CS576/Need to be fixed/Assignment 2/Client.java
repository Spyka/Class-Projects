/*
	CS576
*/
import java.net.Socket;
import java.nio.ByteBuffer;
import java.util.BitSet;
import java.io.*;
import java.lang.*;
import java.util.*;

public class Client {

	// Start Transmission
	public static final byte STX = 0x01;

	//public static final int BUFSIZE = 32;
	
	public static void main(String[] args) throws IOException {

		if (args.length != 3) // Test for correct # of args
			throw new IllegalArgumentException("Parameter(s): <Server> <Port> <RunCount>");

		String server = args[0]; // Server name or IP address
		int servPort = Integer.parseInt(args[1]);
		int numToRun = Integer.parseInt(args[2]);
		// Create socket that is connected to server on specified port
		Socket socket = new Socket(server, servPort);
		System.out.println("Connected to server...");

		InputStream in = socket.getInputStream();
		OutputStream out = socket.getOutputStream();
		//DataInputStream din = new DataInputStream(in);

		byte[] receivedBytes = {0,0,0,0};

		//byte[] bytesBuffed = new byte[];

		//Loop the number of times you want to run
		for(int i=0; i<numToRun; i++)
		{
			//Ask server to start transmitting
			out.write(STX);
			//Get the 4 bytes from the server.
		
			for(int j=0; j<4; j++)
			{
				receivedBytes[j] = (byte) in.read();
			}
		
			//Discover who is the majority
			int eqcnt = majority(receivedBytes);
			//If there is a majority print out the majority
			if (eqcnt >= 3)
			{
				if ((receivedBytes[0] == receivedBytes[1]) | 
					(receivedBytes[0] == receivedBytes[2]) | 
					(receivedBytes[0] == receivedBytes[3]))
					System.out.println(receivedBytes[0]);

				else if ((receivedBytes[1] == receivedBytes[2]) | 
						(receivedBytes[1] == receivedBytes[3]))
					System.out.println(receivedBytes[1]);

				else if (receivedBytes[2] == receivedBytes[3])
					System.out.println(receivedBytes[2]);

			}
			//If there is no consensus on majority print "Unknowable"
			else
				System.out.println("Unknowable");
		}
		//Tell the server you are done.
		System.out.println("Done");
		socket.close(); // Close the socket and its streams
		in.close();
		out.close();
	}
	
	//Unpack a byte into a boolean array
	//This should do the exact opposite of packByte(boolean[]) in the server.
	//Be careful!
	public static boolean[] unpackByte(byte b)
	{
		//format the unpacked byte to be changed into a character array
		String unpackedByte = String.format("%8s", Integer.toBinaryString(b & 0xFF)).replace(' ', '0');

		boolean[] unpackBoolArr = new boolean[8];
		char[] byteChars = unpackedByte.toCharArray();

		//convert the char array into a bool array
		for (int i=0; i<unpackBoolArr.length; i++)
		{
			if (byteChars[i] == '1')
				unpackBoolArr[i] = true;
			else
				unpackBoolArr[i] = false;
		}

		return unpackBoolArr;
	}
	//Converts a boolean array to a string of ones and zeros.
	//This function will let you print your boolean[] version of your bytes
	public static String booleanArrayToString(boolean[] data)
	{
		String boolStr = "";

		for (int i=0; i<data.length; i++)
		{
			if (data[i] == true)
				boolStr += "1 ";
			else
				boolStr += "0 ";
		}

		return boolStr;
	}
	
	//This function will return the index of one of the majority
	//Or it should return an error value if it cannot determine 
	//which bytes are the majority.
	public static int majority(byte[] members)
	{
		//count how many bytes are equal to each other
		int equalCount = 0;

		if (members[0] == members[1])
			equalCount++;

		if (members[0] == members[2])
			equalCount++;

		if (members[0] == members[3])
			equalCount++;

		if (members[1] == members[2])
			equalCount++;

		if (members[1] == members[3])
			equalCount++;

		if (members[2] == members[3])
			equalCount++;

		return equalCount;
	}
	
}