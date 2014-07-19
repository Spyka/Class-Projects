/*
    CS 576
*/

import java.io.*;
import java.net.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import java.util.*;

public class Server extends JFrame
{
	//Global Variables
	private JTextArea displayWindow;
	private ServerSocket server;
	private Socket clientSocket;
	private static int clientCount;
	private static int count = 0;
	private final threadHandler[] threads = new threadHandler[10];

	public static void main(String[] args)
	{
		//The server will request an input of how many clients will be run
		//and the number will be the max amount of clients that can run
		//properly in this program
		System.out.println("How many clients are you going to have?");
		Scanner scan = new Scanner(System.in);
		int clientCount = scan.nextInt();

		Server server = new Server();

		//The server program will end when the gui box is closed
		server.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		//start the program
		server.startRunning(clientCount);
	}

	//Constructor
	public Server()
	{
		super("Server");
		displayWindow = new JTextArea();
		add(new JScrollPane(displayWindow));
		setSize(300,150);
		setVisible(true);
	}

	public void startRunning(int clientCount)
	{
		try{
			server = new ServerSocket(4444, 100);
			while(true)
			{
				try
				{
					//Wait for a connection
					System.out.println("Waiting for Client(s) to connect... \n");
					clientSocket = server.accept();

					//While the number of clients specified by the user at the server's 
					//beginning prompt has not been met, create more threads to do work
					//for the clients that connect to the server
					for(int i=0; i<clientCount; i++){
						if(threads[i] == null){
							count++;
							(threads[i] = new threadHandler(clientSocket, count, threads, displayWindow)).start();
							break;
						}
					}

				}catch(EOFException eofException){
					System.out.println("\n Disconnected...");
				}
			}

		}catch(IOException ioException){
			ioException.printStackTrace();
		}
	}
}

class threadHandler extends Thread{

	//Global Variables
	private ObjectOutputStream output;
	private ObjectInputStream input;
	private Socket clientSocket = null;
	private final threadHandler[] threads;
	private int max, clientCount;
	private JTextArea displayWindow;

	//Constructor
	public threadHandler(Socket clientSocket, int count, threadHandler[] threads, JTextArea displayWindow){
		this.clientSocket = clientSocket;
		this.threads = threads;
		this.displayWindow = displayWindow;
		max = threads.length;
		clientCount = count;
	}

	public void run(){
		try{
			//create the streams to pass information back and forth between the server
			//and the client
			output = new ObjectOutputStream(clientSocket.getOutputStream());
			output.flush();
			input = new ObjectInputStream(clientSocket.getInputStream());

			int max = this.max;
			threadHandler[] threads = this.threads;

		}catch(IOException ioException){
		}

		//Variables to be used
		int ranNumbers[] = new int[1000];
		Random ranNum = new Random();
		String sendNums, message = "";
		int k, j=0, n=0;

		//Create a list of 1000 random numbers ranging from 0-9
		for(int i=0; i<1000; i++){
			int num = ranNum.nextInt(10);
			ranNumbers[i] = num;
		}

		//Split the list of 1000 random numbers into 100 lists of 10 numbers
		//Do while not all 100 lists have been sent
		do{
			k=0;
			sendNums = "";
			while(k<10){
				//create sublists of 10 numbers
				sendNums += String.valueOf(ranNumbers[n]);
				n++;
				k++;
			}

			//send the 10 number list
			sendMessage(sendNums);
			j++;
			
		}while(j<100);

		//Always receive messages from the client
		do
		{
			try
			{
				//read the input from the client and display the message
				message = (String) input.readObject();
				showMessage(message);
			}catch(ClassNotFoundException classNotFoundException){
			}catch(IOException ioException){
			}

		}while(true);
	}

/*	private void close()
	{
		showMessage("\n Closing connections... \n");
		try
		{
			output.close();
			input.close();
			clientSocket.close();
		}catch(IOException ioException){
			ioException.printStackTrace();
		}
	}
*/
	public void sendMessage(String message)
	{
		try
		{
			//Write the message to the stream that sends info
			//to the client, then flush everything out of that 
			//stream so it can be used again 
			output.writeObject(message);
			output.flush();
		}catch(IOException ioException){
		}
	}

	public void showMessage(final String text)
	{
		SwingUtilities.invokeLater(
			new Runnable()
			{
				public void run()
				{
					//Add text to the display box
					displayWindow.append(text);
				}
			}
		);
	}
}