/*
        CS570 masc1005
        sem.cpp
*/

#include <iostream>
#include "prog1.h"

using namespace std;

int main()
{
        string prompt_startup;
        prog1 start;

        //Ask the user if they would like to run the program
        cout << "Would you like to run the program? Yes/No" << endl;
        cin >> prompt_startup;

        //If the answer is "Yes" (case matters), run the program
        if (prompt_startup == "Yes")
                start.run();

        //If "No" (case matters), close the program
        else if (prompt_startup == "No")
                exit(1);

        //CASE MATTERS!
        else
        {
                cout << "Invalid Responce. Ending Program." << endl;
                exit(1);
        }

        return 0;
}