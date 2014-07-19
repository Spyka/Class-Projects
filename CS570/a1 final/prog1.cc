/*
        CS570 masc1005
        prog1.cc
*/

#include <iostream>
#include <fstream>
#include <semaphore.h>
#include <pthread.h>
#include "prog1.h"

using namespace std;

sem_t LOCK;     //LOCK must be global to use in method not defined in header

void * do_work(void * thread_id)
{
        int count = 0;
        long id = (long)thread_id;
        ofstream outfile;

        //Go through the 4 threads 10 times
        while (count < 10)
        {
                //don't let another thread go until the current is done
                sem_wait(&LOCK);

                printf("Thread #%ld! is running \r\n", id);

                //write thread IDs to the text file
                outfile.open("PTAB.txt", ios::app);
                outfile << "Thread ID: " << id << "\r\n";
                outfile.close();

                //release the semaphore to the next thread
                sem_post(&LOCK);

                //sleep the current thread for 1 second
                sleep(1);

                count++;        //Don't let fork bunnies happen
        }

        pthread_exit(NULL);
}


void prog1::run()
{
        //Fork a Child Process
        cpid = fork();

        //If the process is indeed the child, do...
        if (cpid == 0)
        {
                cout << "New Child Process Created. Writing To File." << endl;

                //Open the file to write to - PTAB.txt
                outfile.open("PTAB.txt", ios::out);

                //If not able to get the file
                if (!outfile)
                {
                        cout <<"File Not Created Properly, Exiting." << endl;
                        exit(1);
                }

                //Print the child's process ID
                outfile << "Child PID: " << getpid();
                outfile << "\r\n";

                //close the text file
                outfile.close();

                //initialize the semaphore - named LOCK
                sem_init(&LOCK, 0, 1);

                //create 4 threads and make them "do work"
                for (int i=0; i<4; i++)
                {
                        thread_okay = pthread_create(&threads[i], NULL,
                                                        do_work, (void*)i);

                        //Error handle - thread creation
                        if (thread_okay == 1)
                        {
                                cout << "Thread Not Created Properly. Exiting" << endl;
                                exit(1);
                        }

                }

                pthread_exit(NULL);

                //KILL the semaphore
                sem_destroy(&LOCK);
        }

        //Waiting for the child process to finish
        else
        {
                waitpid(cpid, &status, 0);
        }

}