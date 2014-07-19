/*
    CS 576
*/

import java.io.*;
import java.net.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class Client extends JFrame
{
	//Global Variables
	private JTextArea displayWindow;
	private ObjectOutputStream output;
	private ObjectInputStream input;
	private String message = "";
	private String serverIP;
	private Socket socket;

	public static void main(String[] args)
	{
		Client client = new Client("127.0.0.1");

		//Client program will end when the GUI box is closed
		client.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		//Start the client program
		client.startRunning();
	}

	//constructor
	public Client(String host)
	{
		super("Client");
		serverIP = host;
		displayWindow = new JTextArea();
		add(new JScrollPane(displayWindow));
		setSize(300,150);
		setVisible(true);
	}

	public void startRunning()
	{
		try
		{
			socket = new Socket(InetAddress.getByName(serverIP), 4444);

			//create the streams to pass information back and forth between the server
			//and the client
			output = new ObjectOutputStream(socket.getOutputStream());
			output.flush();
			input = new ObjectInputStream(socket.getInputStream());

			//start the interaction between the server and the client
			interaction();
		}catch(EOFException eofException){
			showMessage("\n Disconnected...");
		}catch(IOException ioException){
			ioException.printStackTrace();
		}
	}

	private void interaction() throws IOException
	{
		//Variables
		int finalNumber, completeAdd=0;
		String listNums;
		char[] charArr;
		int j=0;

		//Do while the client has not yet recieved all 100 lists (of 10 numbers)
		//from the server
		do
		{
			try
			{
				//read the string from the server
				message = (String) input.readObject();

				//change the string into a character array
				charArr = message.toCharArray();

				//variables
				finalNumber=0;
				listNums = "";

				//take each number in the character array and..
				for(int i=0; i<message.length(); i++){
					char tmp = charArr[i];

					//change the character into a string
					String s = String.valueOf(tmp);

					//add the character to the string list of 10 to be redisplayed
					listNums += s;

					//change the character into an integer and add it to the previous 
					//converted characters to get the sum of the ten in the list
					finalNumber += Integer.parseInt(s);
				}

				//add the final numbers found from the 100 lists of 10 together to get
				//the complete addition of all 1000 numbers
				completeAdd += finalNumber;

				//display the original list from the server
				showMessage("List Sent From Server: " + listNums + "\n");

				//display the sum of the 10 numbers form the server
				sendMessage("The Sum of the List (10 nums sent) is: " + String.valueOf(finalNumber) + "\n");
				j++;

			}catch(ClassNotFoundException classNotFoundException){
			}
		}while(j<100);

		//display the sum of all 1000 numbers from the server
		sendMessage("All 1000 Numbers Added Together: " + String.valueOf(completeAdd) + "\n \n");
	}

/*	private void close()
	{
		try
		{
			output.close();
			input.close();
			socket.close();
		}catch(IOException ioException){
			ioException.printStackTrace();
		}
	}
*/
	private void sendMessage(String message)
	{
		try
		{
			//Write the message to the stream that sends info
			//to the server, then flush everything out of that 
			//stream so it can be used again 			
			output.writeObject("Client: " + message);
			output.flush();

			//display what the client sent
			showMessage(message + "\n");
		}catch(IOException ioException){
		}
	}

	private void showMessage(final String printMessage)
	{
		SwingUtilities.invokeLater(
			new Runnable()
			{
				public void run()
				{
					//add text to the display box
					displayWindow.append(printMessage);
				}
			}
		);
	}
}