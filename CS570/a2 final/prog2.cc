/*
        CS570, Spring 2014
        Assignment #2, Page Replacement
        prog2.cc
*/

#include "prog2.h"

using namespace std;

void prog2::read_file(string filename)
{
        ifstream infile;

        //open and read in the file chosen by the user
        infile.open(filename.c_str());

        //if the file doesn't open properly, error and terminate
        if (!infile)
        {
                cerr << "File not opened properly, program terminating." << endl;
                exit(1);
        }

        //using a string stream to extract the contents of the file, store the contents
        //int a string
        stringstream strStream;
        strStream << infile.rdbuf();
        string line = strStream.str();
        string item;
        string strnum;

        //prep the vector
        numbers.clear();
        numbers.erase(numbers.begin(), numbers.end());

        //separate the tokens in the string by spaces
        for (unsigned int i=0; i<line.length(); i++)
        {
                strnum = "";

                if (line[i] == ' ')
                        continue;

                while (line[i] != ' ')
                {
                        strnum += line[i];
                        i++;
                }

                //convert each number token in the file to an integer and insert it
                //into the vector
                int num = atoi(strnum.c_str());
                numbers.push_back(num);
        }
}

void prog2::prompt()
{
        //ask the user to supply a filename and the number of
        //frames to be used for the program to run

        cout << "What file would you like to read?" << endl;
        cin >> filename;

        read_file(filename);

        cout << "How many frames would you like to use?" << endl;
        cin >> num_frames;

/*      if (!isdigit(num_frames))
        {
                cout << "Invalid input. Program terminating." << endl;
                exit(1);
        }
*/
        //run the four paging replacement algorithms
        opt_faults = optimal(num_frames);
        fifo_faults = fifo(num_frames);
        lru_faults = lru(num_frames);
        random_faults = random(num_frames);
        clock_faults = clock(num_frames);


        cout << "Optimal: " << opt_faults << endl;
        cout << "LRU: " << lru_faults << endl;
        cout << "CLOCK: " << clock_faults << endl;
        cout << "RANDOM: " << random_faults << endl;
        cout << "FIFO: " << fifo_faults << endl;


}

int prog2::optimal(int num_frames)
{
        //prep the vector and clear the faults each time before use
        num_faults = 0;
        opt_frames.clear();
        opt_frames.reserve(num_frames);

        //loop through all the numbers in the int vector
        for (unsigned int i=0; i<numbers.size(); i++)
        {
                counter = 0;
                cntaway = 0;
                no_faults = 0;
                already_replaced = 0;

                //find the distance of the next instance of the current number
                //in question (0 if it does not occur again)
                for (unsigned int j=i+1; j<numbers.size(); j++)
                {
                        if (numbers[i] != numbers[j])
                                counter++;

                        if (numbers[i] == numbers[j])
                        {
                                counter++;
                                cntaway = counter;
                                break;
                        }

                }

                for (int j=0; j<num_frames; j++)
                {
                        //if the page already exists, do nothing, not a page fault
                        if(opt_frames[j].page == numbers[i])
                        {
                                no_faults = 1;
                                break;
                        }
                }

                if (no_faults)
                        continue;

                //loop through the frames (using the number provided by the user)
                for (int j=0; j<num_frames; j++)
                {
                        //if the frame is null, insert the page
                        if (opt_frames[j].page == 0)
                        {
                                opt_frames[j].page = numbers[i];
                                opt_frames[j].distance = cntaway;
                                num_faults++;
                                already_replaced = 1;
                                break;
                        }
                }

                if (already_replaced)
                        continue;

                prev = 0;

                for (int j=0; j<num_frames; j++)
                {
                        //compare the distances of the pages to find the furthest away
                        //this will be the place the new page is inserted
                        num1 = opt_frames[j].distance;

                        if (num1 <= 0)
                        {
                                use_frame = j;
                                break;
                        }

                        if (opt_frames[prev].distance > num1)
                                use_frame = prev;
                        else
                        {
                                use_frame = j;
                                prev = j;
                        }
                }

                for (int k=0; k<num_frames; k++)
                {
                        opt_frames[k].distance--;
                }

                //insert the new page and attach the distance
                opt_frames[use_frame].page = numbers[i];
                opt_frames[use_frame].distance = cntaway;
                num_faults++;

        }

        return num_faults;
}

int prog2::fifo(int num_frames)
{
        //prep the vector and clear the counter and page faults
        counter = 0;
        num_faults = 0;
        frames.clear();
        frames.reserve(num_frames);

        //loop through all the numbers in the int vector
        for (unsigned int i=0; i<numbers.size(); i++)
        {
                //clear page_exists and already_replaced for next loop around
                no_faults = 0;
                already_replaced = 0;

                //counter cannot exceed the number of frames provided by the user
                //minus one because it is to be used with vectors
                //therefore 0...num_frames-1; reset the round robin at num_frames
                if (counter == num_frames)
                        counter = 0;

                //loop through the frames (using the number provided by the user)
                for (int j=0; j<num_frames; j++)
                {
                        //if the frame is null
                        //if (frames[j] == 0)
                                //break;

                        //if the frame already exists
                        if (numbers[i] == frames[j])
                        {
                                no_faults = 1;
                                break;
                        }
                }

                //continue to the next number/page in the int vector
                if (no_faults)
                        continue;

                for (int j=0; j<num_frames; j++)
                {
                        //if the frame is null
                        if (frames[j] == 0)
                        {
                                frames[j] = numbers[i];
                                already_replaced = 1;
                                num_faults++;
                                break;
                        }
                }

                if (already_replaced)
                        continue;

                //store the new page into the appropriate frame according to round
                //robin placement
                frames[counter] = numbers[i];

                //increment the round robin counter
                counter++;
                num_faults++;

        }

        return num_faults;
}

int prog2::lru(int num_frames)
{
        //prep the vector and clear the page faults count
        num_faults = 0;
        lru_frames.clear();
        lru_frames.reserve(num_frames);

        //loop through all the numbers in the int vector
        for(unsigned int i=0; i<numbers.size(); i++)
        {
                //clear these boolean flags for continual use
                no_faults = 0;
                already_replaced = 0;

                for (int j=0; j<num_frames; j++)
                {
                        //if the page already exists
                        if (numbers[i] == lru_frames[j].page)
                        {
                                //when referenced, the last used count goes back to
                                //starting
                                lru_frames[j].last_use = 0;
                                no_faults = 1;

                                //increment last use to keep count for later checks
                                for (int n=0; n<num_frames; n++)
                                {
                                        if (lru_frames[n].page == lru_frames[j].page)
                                                continue;
                                        lru_frames[n].last_use++;
                                }

                                break;
                        }
                }

                if (no_faults)
                        continue;

                //loop through the frames (using the number provided by the user)
                for (int j=0; j<num_frames; j++)
                {
                        //if the frame is null
                        if (lru_frames[j].page == 0)
                        {
                                //flag that the page has already been replaced
                                //num_faults increment
                                //then insert the new page and increment the faults counter
                                //also initialize the last use count
                                already_replaced = 1;
                                lru_frames[j].page = numbers[i];
                                lru_frames[j].last_use = 0;
                                num_faults++;

                                //increment last use to keep count for later checks
                                for (int n=0; n<num_frames; n++)
                                {
                                        if (lru_frames[n].page == lru_frames[j].page)
                                                continue;
                                        lru_frames[n].last_use++;
                                }

                                break;
                        }
                }

                if (already_replaced)
                        continue;

                int tmp = 0;

                //loop through the frames and find either a null frame, or
                //the page that has the greatest last_use count attached to it
                //meaning it has been sitting the longest
                for (int k=1; k<num_frames; k++)
                {
                        if (lru_frames[k-1].page == 0)
                        {
                                tmp = (k-1);
                                break;
                        }

                        if (lru_frames[k].page == 0)
                        {
                                tmp = k;
                                break;
                        }

                        if (lru_frames[tmp].last_use <= lru_frames[k].last_use)
                        {
                                tmp = k;
                        }
                }

                //insert the new page into the appropriate place via the least recently used count
                lru_frames[tmp].page = numbers[i];
                lru_frames[tmp].last_use = 0;

                //increment last use to keep count for later checks
                for (int n=0; n<num_frames; n++)
                {
                        if (lru_frames[n].page == lru_frames[tmp].page)
                                continue;
                        lru_frames[n].last_use++;
                }

                num_faults++;
        }

        return num_faults;
}

int prog2::clock(int num_frames)
{
        //prep the vector and clear the faults and the counter for use
        num_faults = 0;
        clock_frames.clear();
        clock_frames.reserve(num_frames);
        counter = 0;
        tracker = 0;

        //loop through all the numbers in the int vector
        for(unsigned int i=0; i<numbers.size(); i++)
        {
                //clear these bool bits for later use
                no_faults = 0;
                already_replaced = 0;

                //the counter cannot exceed the number of frames provided
                //will wrap around to the first frame when the last frame is reached

                for(;;)
                {
                        //if the page already exists
                        if (numbers[i] == clock_frames[counter].page)
                        {
                                //flag the set_bit because the page was referenced
                                //no page faults occur because the page already
                                //exists; move forward to the next frame with the
                                //counter
                                clock_frames[counter].set_bit = 1;
                                no_faults = 1;
                                counter++;
                                tracker = counter;
                                break;
                        }

                        counter++;
                        if (counter == num_frames)
                                counter = 0;

                        if (counter == tracker)
                                break;

                }

                if (counter == num_frames)
                {
                        counter = 0;
                        tracker = counter;
                }

                if (no_faults)
                        continue;

                //loop through all the frames (using the number provided by the user)
                for (;;)
                {
                        //if the frame is empty
                        if (clock_frames[counter].set_bit == 0)
                        {
                                //insert the new page and flag the set_bit for later
                                //checking, also, flag that the page has been replaced
                                //the later incrementation of num_faults
                                //move the counter forward to the next frame
                                clock_frames[counter].page = numbers[i];
                                clock_frames[counter].set_bit = 1;
                                num_faults++;
                                already_replaced = 1;
                                counter++;
                                tracker = counter;
                                break;
                        }

                        clock_frames[counter].set_bit = 0;
                        counter++;
                        tracker++;

                        if (counter == num_frames)
                        {
                                counter = 0;
                                tracker = 0;
                        }

                }

                if (counter == num_frames)
                        counter = 0;
                tracker = counter;

        }

        return num_faults;
}

int prog2::random(int num_frames)
{
        //prep the vector and clear the count of page faults
        num_faults = 0;
        rand_frames.clear();
        rand_frames.reserve(num_frames);

        //loop through all the numbers in the int vector
        for (unsigned int i=0; i<numbers.size(); i++)
        {
                //clear no_faults and already_replaced for next loop around
                no_faults = 0;
                already_replaced = false;

                //loop through the frames (using the number provided by the user)
                for (int j=0; j<num_frames; j++)
                {
                        //if the frame already exists
                        if (numbers[i] == rand_frames[j])
                        {
                                no_faults = 1;
                                break;
                        }
                }

                //continue to the next number/page in the int vector
                if (no_faults)
                        continue;

                for (int j=0; j<num_frames; j++)
                {
                        //if the frame is null
                        if (rand_frames[j] == 0)
                        {
                                rand_frames[j] = numbers[i];
                                already_replaced = true;
                                num_faults++;
                                break;
                        }
                }

                if (!already_replaced)
                {
                //store the new page into a random frame number
                ran = rand() % num_frames;
                rand_frames[ran] = numbers[i];

                num_faults++;
                }

        }

        return num_faults;
}