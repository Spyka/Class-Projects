/*
        CS570 masc1005
        prog1.h
*/

#ifndef PROG1_H
#define PROG1_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <semaphore.h>
#include <pthread.h>

using namespace std;

//The header file holds private variables and the public run method needed
//for the driver file (sem)

class prog1
{
        private:
                string response;

                int thread_okay;
                int status;

                ofstream outfile;

                pid_t cpid;
                pthread_t threads[4];


        public:
                void run();

};

#endif