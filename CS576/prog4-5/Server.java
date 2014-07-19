/*
    CS 576
*/

import java.net.*;
import java.io.*;
import java.util.*;

public class Server {
    
    public static void main (String args[]) throws IOException {
        
        //There must be at least one parameter when running the program
        //Parse that parameter as the port
        if (args.length != 1)
            throw new IllegalArgumentException("Must include a port number as the parameter");
        int servPort = Integer.parseInt(args[0]);
        
        DatagramSocket socket = new DatagramSocket(servPort);
        DatagramPacket receivePacket;
        
        while(true) {
            //create the structure to hold the message received from the client
            byte[] receiveMessage = new byte[1024];
            receivePacket = new DatagramPacket(receiveMessage, receiveMessage.length);

            //waits until the client sends something, then receives the packet
            socket.receive(receivePacket);
            
            //start a new thread to handle what the client sent/what the server recieved
            Thread thread = new Thread(new ServerThread(socket,receivePacket));
            thread.start();
            
        }
    }
}
    class ServerThread implements Runnable 
    {
    
        private DatagramSocket socket = null;
        private DatagramPacket receivePacket = null;
        private boolean firstThread = true;
        private static ArrayList<Integer> portsList = new ArrayList<Integer>();    

        //construct the thread
        public ServerThread (DatagramSocket socket, DatagramPacket receivePacket) {
            this.socket = socket;
            this.receivePacket = receivePacket;
        }
        
        public void run() {
            try {
                    //get the Client's Address and Port Number
                    InetAddress clientIP = receivePacket.getAddress();
                    int cPort = receivePacket.getPort();

                    if(portsList.contains(cPort) == false)
                        portsList.add(cPort);

                    //extract the message from the packet (trim excess)
                    String message = (new String(receivePacket.getData())).trim();

                    System.out.println("<Client " + cPort + "> Sent: "  + message );
            
                    //store client's info and the message it sent into a string for
                    //sending to other clients
                    String sendMessage = "<Client " + cPort + "> " + message;
            
                    //change the above string into a byte message to be sent 
                    //to the other clients
                    byte msg[] = new byte[1024];
                    msg = sendMessage.getBytes();
            
                    //send the client's message to every other client in the port list
                    for(int i=0; i<portsList.size(); i++){
                        if(portsList.get(i) != cPort ) {
                            DatagramPacket sendPacket = new DatagramPacket(msg, msg.length, clientIP, portsList.get(i));
                            socket.send(sendPacket);
                        }
                    }

                }
            catch (IOException e){}
        }
    }




























