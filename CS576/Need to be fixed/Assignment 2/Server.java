/*
	CS576
*/
import java.net.*; // for Socket, ServerSocket, and InetAddress
import java.io.*; // for IOException and Input/OutputStream
import java.util.*; //for random (to gen random numbers or bits in this prog)
import java.lang.*;

public class Server {

	// Start Transmission
	public static final byte STX = 0x01;

	public static void main(String[] args) throws IOException {

		if (args.length != 1) // Test for correct # of args
			throw new IllegalArgumentException("Parameter(s): <Port>");

		int servPort = Integer.parseInt(args[0]);

		// Create a server socket to accept client connection requests
		ServerSocket servSock = new ServerSocket(servPort);

		while (true) { // Run forever, accepting and servicing connections
			Socket clntSock = servSock.accept(); // Get client connection

			SocketAddress clientAddress = clntSock.getRemoteSocketAddress();
			System.out.println("Handling client at " + clientAddress);
			InputStream in = clntSock.getInputStream();
			OutputStream out = clntSock.getOutputStream();
			//DataInputStream din = new DataInputStream(in);

			//Whenever the client sends the Start tansmission byte
			//Generate the random byte, duplicate it 4 times,
			//make up to two errors, pack that data
			//send it to the client
			while (in.read() == STX) {
				//Make some random bits.
				boolean[] madeBits = makeRandomBits(8);
				//Duplicate those random bits 4 times (or 3 depending on how you think)
				String stringBits = booleanArrayToString(madeBits);

				String[] duplicateBits = {stringBits, stringBits, stringBits, stringBits};
				//Display the original byte
				System.out.println(stringBits);
				//Make some random errors
				String[] erroredString = makeRandomErrors(duplicateBits);

				//Pack the boolean representations into a byte array
				byte[] packedBytes = packByte(erroredString);
				//Print out the bytes you are going to send to the client (with errors)
				for (int i=0; i<packedBytes.length; i++)
				{
					System.out.print(packedBytes[i] + " ");
				}

				System.out.print("\n");
				System.out.print("\n");

				//Send data to client
				for (int i=0; i<packedBytes.length; i++)
				{
					out.write(packedBytes[i]);
				}

			}
			//Keep the path open to run another client if desired
				clntSock.close(); // Close the socket. We are done with this client!
				in.close();
				out.close();
				System.out.println("Client Done: " + clientAddress);

		}
	}

	//Make some random bits stored in a boolean array
	public static boolean[] makeRandomBits(int size) {
		Random ranBit = new Random();
		int[] intArr = new int[size];
		boolean[] boolArr = new boolean[size];
		
		//make an int array of 0s and 1s for the 8 bits
		for (int i=0; i<size; i++)
		{
			int bit = ranBit.nextInt(2);
			intArr[i] = bit;
		}

		//convert the int array to trues and falses
		for (int i=0; i<size; i++)
		{
			if (intArr[i] == 1)
				boolArr[i] = true;
			else
				boolArr[i] = false;
		}

		return boolArr;
	}


	//Generate are most 2 random errors and flip two random bits.
	public static String[] makeRandomErrors(String[] data) {
		Random ranNum = new Random();
		String[] errStr = {"0","0","0","0"};

		for (int i=0; i<data.length; i++)
		{
			char[] currentBits = data[i].toCharArray();
	
			int changeNum = ranNum.nextInt(101);
			int changeNum2 = ranNum.nextInt(101);
			int ranSpot = ranNum.nextInt(8);
			int ranSpot2 = ranNum.nextInt(8);

			//There is a 1% chance there will be a bit error in one of the
			//bytes to be sent
			if (changeNum == 55)
			{
				if (currentBits[ranSpot] == '1')
					currentBits[ranSpot] = '0';
				else
					currentBits[ranSpot] = '1';	
			}

			//There is a 1% chance there will be another bit error
			//in one of the bytes to be sent
			if (changeNum2 == 5)
			{
				if (currentBits[ranSpot2] == '1')
					currentBits[ranSpot2] = '0';
				else
					currentBits[ranSpot2] = '1';
			}

			errStr[i] = new String(currentBits);
		}

		return errStr;
	}
	//Pack a boolean array into a byte.
	//This should do the exact opposite of unpackByte(byte) function in the client.
	//Be careful of ordering!
	public static byte[] packByte(String[] s) {
		byte currentbyte = 0;
		byte[] combinedBytes = {0,0,0,0};

		//convert the string to a character array and shift the bits according
		//to the current array in question until byte is created
		for (int i=0; i<s.length; i++)
		{
			currentbyte = 0;
			char[] currentString = s[i].toCharArray();

			for (int j=0; j<currentString.length; j++)
			{
				currentbyte <<=1;

				if(currentString[j] == '1')
					currentbyte |=1;
			}

			combinedBytes[i] = currentbyte;
		}

		return combinedBytes;
	}

	
	//Converts a boolean array to a string of ones and zeros.
	//This function will let you print your boolean[] version of your bytes
	public static String booleanArrayToString(boolean[] data)
	{
		String boolStr = "";

		for (int i=0; i<data.length; i++)
		{
			if (data[i] == true)
				boolStr += "1";
			else
				boolStr += "0";
		}

		return boolStr;
	}
	
}