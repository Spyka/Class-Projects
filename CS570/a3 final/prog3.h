/*
        CS570, Spring 2014
        Assignment #3, Countdown Timer
        prog3.h
*/

#ifndef PROG3_H
#define PROG3_H

#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <signal.h>

using namespace std;

class prog3
{
        private:
                //used in prompt
                string str_seconds;

                //used in both methods
                int seconds;

                //used in begin_processes
                int fd[2];
                int bytes;
                int buf;

                //for processes
                int cpid1;
                int cpid2;
                int status;

        public:
                //prompts the user to enter the number of seconds to be used
                //for the countdown, or to just hit enter to use the default
                //6 seconds
                void prompt();

                //forks child processes 1 and 2 and executes both
                //process 1 displays the current local time every second
                //process 2 displays a countdown timer
                //parent process waits until both children finish, then
                //exits with a message saying the timer has reached 00:00:00
                void begin_processes(int);

};

#endif