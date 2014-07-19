/*
    CS 576
*/

import java.net.*;
import java.io.*;

public class Server
{   
    public static void main(String[] args) throws IOException
    {
        //There should only be one argument - the port to be used
        if (args.length != 1)
            throw new IllegalArgumentException("Parameter(s)");
       
        //Take the argument and make it an int for the port
        int servPort = Integer.parseInt(args[0]);
       
        //Create a new Server Socket at the specified port
        ServerSocket servSock = new ServerSocket(servPort);
		System.out.println("Waiting for Client to ask to connect...");
       
        int msgSize;

        //Create an instance of the file that is to be used
        File file = new File("TransferFile.txt");
        long length = file.length();

        //array of bytes the length of the file for transfer
        byte[] bytes = new byte[(int) length];
       
        while (true) //run always
        {   
            //the server socket accepts and creates a copy of the port for the client
            Socket clntSock = servSock.accept();

            SocketAddress clientAddress = clntSock.getRemoteSocketAddress();
            System.out.println("Handling Client at Address: " + clientAddress);

            //Read the file and store the contents in the buffer, then write the contents
            //to the client (next 3 lines and while loop)

            FileInputStream fin = new FileInputStream(file);
            BufferedInputStream bin = new BufferedInputStream(fin);
            BufferedOutputStream bout = new BufferedOutputStream(clntSock.getOutputStream());
           
            while ((msgSize = bin.read(bytes)) != -1)
            {
                bout.write(bytes, 0, msgSize);
            }
           
            //flush the buffers and close the sockets/streams being used
            bout.flush();
            bout.close();
            fin.close();
            bin.close();
            clntSock.close();
        }
    }
}
