/*
    CS 576
*/

import java.io.*;
import java.net.*;

public class Client {

  private static final int BUFSIZE = 1024;
  
  public static void main(String[] args) throws IOException {

    //The port to be used is set instead of having to enter a paramenter
    int portNumber = 4444;
 
    //create the Client Socket with the specified port and address
    Socket clientSocket = new Socket("127.0.0.1", portNumber);

    //create the io streams needed
    InputStream in = clientSocket.getInputStream();
    FileOutputStream fout = new FileOutputStream("ReceivedFile.txt");
    BufferedOutputStream bout = new BufferedOutputStream(fout); 

    //Write the recieved data to a file called "ReceivedFile.txt" (see above) 
    int bytesRcvd;
    byte[] bytes = new byte[BUFSIZE];
    while((bytesRcvd = in.read(bytes)) > 0){
      bout.write(bytes, 0,  bytesRcvd);
    }

    System.out.println("File Was Received by Intended Network");

    bout.flush();
    bout.close();
    in.close();
    clientSocket.close();

  }
}