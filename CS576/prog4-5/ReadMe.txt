CS 576

This assingment was achieved using the command line terminal on a mac system and a text editor for coding

In order to compile the project please...
	1) Create 3 or more terminal windows and move all three to the proper directory where the files are stored
		-One terminal will be for the Server, the others will be for Clients

	2) On the Server terminal window type...
		javac Server.java

		Then run the Server...
		java Server 4444 

		Note: A port number must be specified as a command line argument.  Above I used 4444.

	3) On the Client terminal windows type...
		javac Client.java

		Then run the Client...
		java Client 4444

		Note: A port number must also be specified for the Client and it must be the same as the Server


This will create a chat room for the clients.  The server will be the go-between that handles the messages sent.  The server will display all messages sent and the corresponding client information (who sent the message).  The client terminals will show when another client sends a message and will display the client information along with that message.

If a client wishes to leave they must type "/quit".  This will disconnect the client and close that client's port.  

The server will run continuously waiting for more clients to join, even after all have left.  In order to end the server please use ctrl-C command.