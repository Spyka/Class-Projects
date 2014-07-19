/*
    CS 576
*/

import java.io.*;  
import java.net.*;

public class Client
{
    public static void main(String args[]) throws IOException 
    {
        //There must be at least one parameter when running the program
        //Parse that parameter as the port
        if (args.length != 1) 
            throw new IllegalArgumentException("Must include a port number as the parameter");
        int sendPort = Integer.parseInt(args[0]);
        
        //store the address of the client
        InetAddress sendAddress = InetAddress.getByName("127.0.0.1");

        //start the thread that will send the message
        SendThread send = new SendThread(sendAddress, sendPort);
        send.start();

        //start the thread that will recieve messages
        Thread receive = new Thread( new ReceiveThread(send.getSocket()));
        receive.start();
    }
}

class SendThread extends Thread 
{
 
    private InetAddress sendAddress;
    private int sendPort;
    private DatagramSocket socket;
    private DatagramPacket sendPacket, finalPacket, clientDetails;
    private BufferedReader reader;

 
    public SendThread(InetAddress sendAddress, int sendPort) throws SocketException 
    {
        this.socket = new DatagramSocket();
        this.sendPort = sendPort;
        this.sendAddress = sendAddress;
        socket.connect(sendAddress, sendPort);
    }
 
    public void run() 
    {       
        try { 
            //Notify all about the connection of a new client       
            byte [] message = new byte[1024];
            String msg = "conncecting...";
            message = msg.getBytes();
            clientDetails = new DatagramPacket(message, message.length, sendAddress, sendPort);
            socket.send(clientDetails);
 
            //this will allow the client to read in what the user types
            reader = new BufferedReader(new InputStreamReader(System.in));

            while (true) 
            {
 
                //read in what the user has typed
                String line = reader.readLine();

                //create buffer and get ready to send data
                byte[] sendData = new byte[1024];   
                sendData = line.getBytes();
                sendPacket = new DatagramPacket(sendData, sendData.length, sendAddress, sendPort);

                //if the user type "/quit" they are leaving the chatroom
                //notify all that the client is disconnecting and close that client's socket
                if (line.equals("/quit")) {
                    msg = "<Client> Disconnecting";
                    System.out.println(msg);
                    sendData = msg.getBytes();
                    finalPacket = new DatagramPacket(sendData, sendData.length, sendAddress, sendPort);
                    socket.send(finalPacket); 
                    socket.close();
                    System.exit(0);
                } 
                //send data
                socket.send(sendPacket);
            }
        }
        catch (IOException ex) {

        }
    }

    //This method is needed by the recieve thread to use the same socket
    public DatagramSocket getSocket() 
    {
        return socket;
    }
}   

class ReceiveThread implements Runnable 
{
 
    private DatagramSocket socket;
    private DatagramPacket receivePacket;
 
    public ReceiveThread(DatagramSocket socket) 
    {
        this.socket = socket;
    }
 
    public void run() {
 
        byte[] receiveData = new byte[1024];
 
        while (true) 
        {            
            //this will be what holds the received information
            receivePacket = new DatagramPacket(receiveData, receiveData.length);

            try 
            {
                //receive the message that was sent and print it to the command line
                socket.receive(receivePacket);
                String message =  new String(receivePacket.getData(), 0, receivePacket.getLength());
                System.out.println(message);
            } 
            catch (IOException ex) {}
        }
    }
}