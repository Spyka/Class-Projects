CS576

This program was completed on a mac laptop using the terminal window for compilation.

When compiling the interaction...
1)First compile the Server: javac Server.java

2)Then compile the Client: javac Client.java

3)Run the Server before the Client, including a port as the command
	line argument: java Server 4444

4)Then run the Client, including a server, port, and number of times you wish to run the
	interaction in the command line: java Client 127.0.0.1 4444 1 (or ...4444 2 or...3 etc)

The Server will always remain open, waiting for client connection
The Client may be re-run repeatedly on the same port