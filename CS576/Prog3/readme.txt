CS 576

How to Compile:
	This program will require three terminal windows.  One for the Server, one for Client, and one for Client2.  
	The Server will be compiled in its terminal window:
		javac Server.java
	Then the two Clients will be compiled using their respective windows
		javac Client.java
		javac Client2.java

How to Run:
	The Server must be run first and requires a port number as a command line argument.  For example:
		java Server 4444

	Then the two Clients will be run in their respective windows the same way.  Both clients require a server and a port to be specified as command line arguments.  For example:
		java Client 127.0.0.1 4444
		java Client2 127.0.0.1 4444
	The port may be changed but the loopback must be used for this assignment

What the Program Does:
	The Server will send a 1MB file to the client after connection.  Threads are used to allow the Clients to run concurrently.  A new thread is created per connection and proceeds to transfer the file.  Information about the connections and transfer process will be outputted in the server's and clients' terminal windows.  After both Client and Client2 have connected and received the file the server will close (because it only allows two connections to be made before closing).  

	Note:
		If you would like to be able to run the program more than twice please edit the while loop to indicate the number of times you would like itto run before the server closes or simply put "true" to make the server run forever (in which case you would need to hit control-c in order to exit)