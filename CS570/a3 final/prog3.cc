/*
        CS570, Spring 2014
        Assignment #3, Countdown Timer
        prog3.cc
*/

#include "prog3.h"

void prog3::prompt()
{
        //message prompt for user to enter info
        cout << "Please enter the number of seconds you would" << endl;
        cout << "like the countdown timer to start with." << endl;
        cout << "(Or hit ENTER to use the default 6 seconds)" << endl;

        //get what the user enters into the command line
        getline(cin, str_seconds);

        //if the user doesn't enter anything (just hits the ENTER key)
        //use a default of 6 seconds
        if(str_seconds.empty())
                str_seconds = "6";

        //input checking
        for(unsigned int i=0; i<str_seconds.size();i++)
        {
                if(!isdigit(str_seconds[i]))
                {
                        cout << "Input must be positive integers only. ";
                        cout << "Program Terminating..." << endl;
                        exit(0);
                }
        }


        //change the input (seconds) into an int
        seconds = atoi(str_seconds.c_str());

        //fork and execute processes 1 and 2
        begin_processes(seconds);
}

void prog3::begin_processes(int seconds)
{
        int cal_sec = seconds;
        buf= 1;
        pipe(fd);

        //create child process 1
        cpid1 = fork();

        //while in child process 1
        while(cpid1 == 0)
        {
                //while child process 2 has not sent a 0 over the pipe
                while(buf != 0)
                {
                        //get the local time by first using asctime to get
                        //the date and time and then substringing that info
                        //to use the local time only
                        time_t ctt = time(0);
                        string time_only = asctime(localtime(&ctt));
                        time_only = time_only.substr(11,9);
                        cout << "Process 1: " << time_only << endl;

                        //pause the process for one second
                        sleep(1);
                        close(fd[1]);

                        //read the info from the pipe from child process 2
                        bytes = read(fd[0], &buf, sizeof(buf));
                }

                //once child process 2 sends a 0 over the pipe
                //child process 1 will exit
                exit(0);
        }

        //create child process 2
        cpid2 = fork();

        //while in child process 2
        while(cpid2 == 0)
        {
                stringstream sstr;

                //calculate the hours, minutes, and seconds to
                //be displayed on the timer
                int hours = cal_sec/3600;
                int mins = (cal_sec%3600)/60;
                int sec = (cal_sec%3600)%60;

                //build the look of the timer
                sstr << setw(2) << setfill('0') << hours;
                sstr << setw(1) << setfill('0') << ":";
                sstr << setw(2) << setfill('0') << mins;
                sstr << setw(1) << setfill('0') << ":";
                sstr << setw(2) << setfill('0') << sec;
                cout << sstr.str() << endl;

                //decrement the counter, pause the process for one second
                cal_sec--;
                sleep(1);

                close(fd[0]);

                //write the remaining seconds to child process 1
                write(fd[1], &cal_sec, sizeof(int));

                //once the number of seconds remaining hits 0, exit
                if(cal_sec == 0)
                        exit(0);
        }

        //parent process waits until the children finish, then displays a message
        //that the end of the timer has been reached, and terminates the program
        waitpid(cpid1, &status, 0);
        cout << "The Countdown Has Reached 00:00:00, Program Terminating." << endl;
        exit(0);


}