CS 576

This assignment was achieved using the command line terminal on a mac system.

In order to compile the project please...
1) Create two terminal windows and move both to the proper directory where the files are stored
	- one will go into the Server file and the other will go into the Client file

2) On the Server terminal window type...
	javac Server.java

	Then run the Server...
	java Server 4444 (or whatever port you would wish to use)

3) On the Client terminal window type...
	javac Client.java

	Then run the Client...
	java Client 127.0.0.1 4444 (for the server and the port, Note: the port must be the same
								as previously used)

Note: The Server must be run before the Client
Note: If you wish to run the program again just repeat step #3