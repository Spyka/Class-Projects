/*
        CS570, Spring 2014
        Assignment #2, Page Replacement
        prog2.h
*/

#ifndef PROG2_H
#define PROG2_H

#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <map>

using namespace std;

class prog2
{
        private:
                //variables used in multiple places
                int num_faults;
                int counter;
                int tracker;

                //first used in prompt
                string filename;
                int num_frames;
                void read_file(string);
                int fifo_faults;
                int opt_faults;
                int lru_faults;
                int clock_faults;
                int random_faults;

                //first used in read_file
                string item;
                vector<int> numbers;

                //used in fifo
                vector<int> frames;
                bool already_replaced;

                //used in optimal
                struct optimal
                {
                        int page;
                        int distance;
                };
                vector<optimal> opt_frames;
                int cntaway;
                int prev;
                int num1;
                int use_frame;

                //used in lru
                struct lru
                {
                        int page;
                        int last_use;
                };
                vector<lru> lru_frames;

                //used in clock
                struct clock
                {
                        int page;
                        bool set_bit;
                };
                vector<clock> clock_frames;
                bool no_faults;

                //used in random
                int ran;
                vector<int> rand_frames;

        public:
                //prompts the user for imput - the filename and the
                //number of frames
                void prompt();

                //All algorithms return the number of page faults that occur

                //starts the page replacement algorithm - Optimal
                int optimal(int);

                //starts the page replacement algorithm - First In First Out
                int fifo(int);

                //starts the page replacement algorithm - Least Recently Used
                int lru(int);

                //starts the page replacement algorithm - CLOCK
                int clock(int);

                //starts the page replacement algorithm - Random
                int random(int);
};

#endif