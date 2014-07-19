/*
    CS 576
*/

import java.io.*;
import java.net.*;
import java.util.*;

public class Server {

  private static ServerSocket serverSocket = null;
  private static Socket clientSocket = null;
  private static final int max = 10;
  private static int count = 0;
  private static final clientThread[] threads = new clientThread[max];

  public static void main(String args[]) {
    //port number is declared instead of being taken in as a paramenter
    int portNumber = 4444;

    try {
      //create the Server Socket with the port specified above
      serverSocket = new ServerSocket(portNumber);
    } catch (IOException e) {
      System.out.println(e);
    }

    //Do forever
    while (true) {
      try {

        //wait and accept client connections
        clientSocket = serverSocket.accept();

        //Start a thread for each client connected
        for (int i = 0; i < max; i++) {
          if (threads[i] == null) {
            count++;
            (threads[i] = new clientThread(clientSocket, count, threads)).start();
            break;
          }
        }

        //if there are too may clients trying to enter the server, send an 
        //error message
        if (count > max) {
          PrintStream os = new PrintStream(clientSocket.getOutputStream());
          os.println("Server too busy. Try later.");
          os.close();
          clientSocket.close();
        }
      } catch (IOException e) {
        System.out.println(e);
      }
    }
  }
}

class clientThread extends Thread {

  private String clientName = null;
  private Socket clientSocket = null;
  private final clientThread[] threads;
  private int max;
  private int netCount;
  private int timesRun = 0;

  private FileInputStream fin;
  private BufferedInputStream bin;
  private BufferedOutputStream bout;

  //Constructor
  public clientThread(Socket clientSocket, int count, clientThread[] threads) {
    this.clientSocket = clientSocket;
    this.threads = threads;
    max = threads.length;
    netCount = count;
  }

  public void run() {
    int max = this.max;
    clientThread[] threads = this.threads;

    try {
      //Assingn a name to each network joined
      String networkName = "Network " + netCount;

      //also assign a client name for use later
      synchronized (this) {
        for (int i = 0; i < max; i++) {
          //as long as the client is not already in the system, create
          //a client name by adding the @ symbol to the network name
          //created above
          if (threads[i] != null && threads[i] == this) {
            clientName = "@" + networkName;
            this.wait(2000);
            break;
          }
        }
      }

      //All the variables necessary to picking the client at random and
      //the file at random; Also, variables that will be needed for the 
      //transferring of the file chosen
      Random ranNum = new Random();
      int[] wakeSingals = new int[10];
      int wakeNum, num, pickClient, msgSize;
      boolean bitExists, wokeNum = false;;
      File file;
      int pickFile = ranNum.nextInt(20);

      while(true){
      //pick a File to be sent to the client at random
      if(pickFile < 10)
          file = new File("FileA.txt");
      else
          file = new File("FileB.txt");

      //Create io streams for use in file transferring
      fin = new FileInputStream(file);
      bin = new BufferedInputStream(fin);
      bout = new BufferedOutputStream(clientSocket.getOutputStream());
      long length = file.length();
      byte[] bytes = new byte[(int)length];

      //let the uer know how many networks above joined
      System.out.println("Number of Networks Joined: " + netCount);
    
      //pause the program so that there is time for enough clients to join
      if (netCount < 3){
        sleep(5000);
      }

      if (netCount > 2){
        synchronized(this) {
          for (int i = 0; i < netCount; i++){
            bitExists = false;
            //create a random number for a wake signal for the clients
            num = ranNum.nextInt(100);

            //if the wake signal number is already in use...
            for (int j = 0; j < netCount; j++){
              if (wakeSingals[j] == num){ 
                bitExists = true;
                break;
              }
            } 

            //...then decrement i since it will increment when you continue
            //and retry the process to get a signal number
            if (bitExists){
              i--;
              continue;
            }     

            //insert the random wake signal number into the array
            wakeSingals[i] = num;
          }

          //while 
          while(!wokeNum){
            //create a random number as the wake number
            wakeNum = ranNum.nextInt(100);

            //try to find the wake number inside the wake signals
            //array.  If found, notify the thread to wake it up
            for (int i = 0; i < netCount; i++){
              if (wakeSingals[i] == wakeNum){
                synchronized(threads[i]){
                  threads[i].notify();

                  //pick a random client in the list
                  pickClient = ranNum.nextInt(netCount+1);

                  //If the client is equal to the one currently in question, change
                  //the pick client random number
                  while (pickClient == i){
                    pickClient = ranNum.nextInt(netCount+1);
                  }

                    //Make sure the client/thread picked is valid and notify the user (server
                    //window) where the file is being sent to
                    if (threads[pickClient] != null && threads[pickClient] != threads[i] 
                    && threads[i].clientName != null) {
                        System.out.println("Sending File to " + threads[pickClient].clientName);

                        //send the message
                        while((msgSize = bin.read(bytes)) != -1){
                          bout.write(bytes, 0, msgSize);
                        }
                    }
                  }

                //already woke up the thread so break
                wokeNum = true;
                break;

              }
            }
          }
          timesRun++;
          if(timesRun > 20){
            break;
          }
        }
      }
    }
          //flush the buffer and close the streams and socket
          bout.flush();
          bout.close();
          fin.close();
          bin.close();
 //         clientSocket.close();
    } catch (IOException e) {
    } catch (InterruptedException e) {
    }
  }
}
