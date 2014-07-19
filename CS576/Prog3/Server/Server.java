/*
    CS 576
*/

import java.net.*;
import java.io.*;
import java.util.logging.*;
import java.util.concurrent.TimeUnit;

public class Server
{   
    public static void main(String[] args) throws IOException
    {
        int runCount = 0;

        //There should only be one argument - the port to be used
        if (args.length != 1)
            throw new IllegalArgumentException("Parameter(s)");
       
        //Take the argument and make it an int for the port
        int servPort = Integer.parseInt(args[0]);
       
        //Create a new Server Socket at the specified port
        ServerSocket servSock = new ServerSocket(servPort);
		System.out.println("Waiting for Client to ask to connect...");

        Logger logger = Logger.getLogger("practical");
       
        while (runCount < 2)    //run only twice, once for each client
                                //(change to "true" to run forever)
        {   
            //the server socket accepts and creates a copy of the port for the client
            Socket clntSock = servSock.accept();
            runCount++;

            SocketAddress clientAddress = clntSock.getRemoteSocketAddress();
            System.out.println("Handling Client at Address: " + clientAddress);

            Thread thread = new Thread(new ThreadProtocol(clntSock,logger));
            thread.start();
            logger.info("Started Thread " + thread.getName());

            try
            {
              Thread.sleep(1000);
            }
            catch (InterruptedException e){}
        }

        servSock.close();
    }
}

class ThreadProtocol implements Runnable 
{
    private static final int BUFSIZE = 32; //Size (in bytes) of I/O buffer
    private Socket clntSock; // Socket connect to client
    private Logger logger; // Server logger

    //Constructor
    public ThreadProtocol(Socket clntSock, Logger logger) 
    {
        this.clntSock = clntSock;
        this.logger = logger;
    }

    public static void handleEchoClient(Socket clntSock, Logger logger) 
    {
        try 
        {

            //Create an instance of the file that is to be used
            File file = new File("MyFile.txt");
            long length = file.length();

            //array of bytes the length of the file for transfer
            byte[] bytes = new byte[(int) length];

            //Read the file and store the contents in the buffer, then write the contents
            //to the client (next 3 lines and while loop)
            FileInputStream fin = new FileInputStream(file);
            BufferedInputStream bin = new BufferedInputStream(fin);
            BufferedOutputStream bout = new BufferedOutputStream(clntSock.getOutputStream());

            int msgSize; // Size of received message
            int numBytesSent =0;

            // Receive until client closes connection, indicated by -1
            while ((msgSize = bin.read(bytes)) != -1)
            {
                bout.write(bytes, 0, msgSize);
                numBytesSent += msgSize;
            }

        logger.info("Client " + clntSock.getRemoteSocketAddress() + " was transfered file");
        logger.info(numBytesSent + " bytes were transfered to client");

        //flush the buffers and close the sockets/streams being used
        bout.flush();
        bout.close();
        fin.close();
        bin.close();
        } 
        catch (IOException ex) 
        {
            logger.log(Level.WARNING, "Exception in echo protocol", ex);
        } 
   
        finally 
        {
            try 
            {
                clntSock.close();
            } 
            catch (IOException e) {}
        }
     }

    public void run() 
    {
        handleEchoClient(clntSock, logger);
    }
}



