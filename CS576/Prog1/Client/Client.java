/*
    CS 576
*/

import java.io.*;
import java.net.*;

public class Client 
{
    //buffer size will be 1024 bytes
   private static final int BUFSIZE = 1024;
    
	public static void main(String[] args) throws IOException 
	{
        //There should be two arguments - the server and the port to be used
        if (args.length != 2)
            throw new IllegalArgumentException("Parameter(s)");
         
        //Take the arguments entered and store them into their respective
        //variables - server and port
        String server = args[0];
        int port = Integer.parseInt(args[1]);

        //Create a new socket using the server and port specified
        //so that the client may connect
        Socket socket = new Socket(server, port);
        System.out.println("Connected to server...");

        //For receiving the contents being submitted by the server
        InputStream in = socket.getInputStream();

        //array of bytes of the length of the buffer
        byte[] bytes = new byte[BUFSIZE];

        //Write the contents received to a text file named TransferText
        //(rest of code)
        FileOutputStream fout = new FileOutputStream("TransferText.txt");
        BufferedOutputStream bout = new BufferedOutputStream(fout);

        int bytesRcvd;

        while ((bytesRcvd = in.read(bytes)) > 0){
                bout.write(bytes, 0, bytesRcvd);
            }
        System.out.println("Received");

        //flush the buffers and close the sockets/streams being used
        bout.flush();
        bout.close();
        in.close();
        socket.close();
	}

}
