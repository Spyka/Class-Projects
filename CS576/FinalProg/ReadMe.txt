CS 576

This assingment was achieved using the command line terminal on a mac system and a text editor for coding

In order to compile the project please...
	1) Create 2 or more terminal windows and move all to the proper directory where the files are stored
		-One terminal will be for the Server, the other(s) will be for Client(s)

	2) On the Server terminal window type...
		javac Server.java

		Then run the Server...
		java Server

	--> The Server will ask you to input how many clients you are going to use.  Do this before running the client program.  (Ex: if you have 2 client terminal windows, type 2 and hit enter)
	--> The Server GUI should pop up.  It will be blank until a client connects.  Since all the GUIs will appear in the same place I suggest moving the Server GUI out of the way

	3) On the Client terminal window(s) type...
		javac Client.java

		Then run the Client...
		java Client 

	--> The Client GUI should pop up.  The Server program will then run to completion for that client and the Server and Client GUI windows will populate
	--> If you are going to have more than one Client, the GUI windows will pop up in the same place.  I would suggest moving the Client GUI out of the way

What the Program Does:

The server will create a list of 1000 random numbers for each client.  Then that list will be broken into 100 lists of 10 numbers.  Those 100 lists will be sent one by one to the client.  The client will add these numbers together and send the sum of the 10 numbers back to the server.  It will also add the 100 sums together to get the total sum of the 1000 numbers created by the server and will send that back to the server as well.  

Each client will receive and add its own list of numbers and will send its own information back to the server.


