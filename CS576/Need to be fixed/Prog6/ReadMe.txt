CS 576

This assingment was achieved using the command line terminal on a mac system and a text editor for coding

In order to compile the project please...
	1) Create 4 or more terminal windows and move all three to the proper directory where the files are stored
		-One terminal will be for the Server, the others will be for Clients

	2) On the Server terminal window type...
		javac Server.java

		Then run the Server...
		java Server

	3) On the Client terminal windows type...
		javac Client.java

		Then run the Client...
		java Client 


The program will pick a random file out of two and will send it to a random client

HOWEVER, there are still some problems with the design.
I tried making the program continuously send files but I could not succeed since one of the client sockets always closes after a file is sent.  The program will not send a new file until 3 or more clients are on the network.  For this reason, you will have to run another client to take the spot of the one that just closed.

Since I was trying to fix the formerly mentioned problem, I wasn't able to get to fixing another desing flaw - closing the other two clients that were accepted by the server and continue running.  

Also, because of the loop the server will continuously notify the user of already existing networks saying they have "joined" when they never left in the first place.

To completely end the program you will need to hit ctrl-c on the server window.  This will close all the clients too.

For the most part this program does what was asked in the assignment.  It picks a random file and sends it to a random client.  There are just a few design errors that I would have addressed had I had more time.