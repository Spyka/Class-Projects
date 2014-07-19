/*
        file_parser.cc
        CS530, Spring 2014

        Parses each line of a .txt file into 4 tokens based on the SCI/SCIXE
        architecture.
*/

#include "file_parser.h"
#include "file_parse_exception.h"

using namespace std;

file_parser::file_parser(string f){ text_file = f; }

file_parser::~file_parser(){}

void file_parser::read_file(){
        open_file();

        //initialize useful variables
        vector<string>::iterator v_iter;
        file_parser::CodeLine word_temp;
        std::vector<char>::iterator char_iter;
        std::vector<char> *char_contents;
        int count = 0;
        int line_number = 1;

        //loop through each line of contents (the file being read) and find the label, opcode, operand, and comment of each if
        //applicable.  Will throw errors if does not meet criteria
        for(v_iter = contents.begin(); v_iter != contents.end(); advance(v_iter,1)){ // iterate through each lines
                ostringstream convert;
                char_contents = new vector<char>((*v_iter).begin(),(*v_iter).end()); // iterate through each character of each line
                char_iter = char_contents->begin();

                //initialize count and convert
                count = 0;
                convert << line_number;

                //clear word_temp
                word_temp.col_one = "";
                word_temp.col_two = "";
                word_temp.col_three = "";
                word_temp.col_four = "";


                //Check for blankline or endline
                if(char_iter == char_contents->end()){
                        tokens.push_back(word_temp);
                        line_number++;
                        continue;
                }


                //check first char for label, validate, if its there grab first 8 characters
                if(isalnum(*char_iter)){
                        if(!isalpha(*char_iter) && *char_iter != '.'){
                                throw file_parse_exception("Label must begin with a letter. Error on line " + convert.str());
                        }

                        //The Label will only take the first eight characters entered. Anything else, including invalids will be
                        //tossed out past the eight character marker
                        for(int i=0;i<9;i++){
                                if(isspace(*char_iter) || char_iter == char_contents->end()){ break; }
                                if(i == 8)
                                        throw file_parse_exception("Label must be no longer than 8 alpha-numerics. Error on line " + convert.str());
                                if(!isalnum(*char_iter))
                                        throw file_parse_exception("Label must only be alpha-numeric.  Error on line " + convert.str());

                                word_temp.col_one += *char_iter; //store the first eight characters in column one
                                advance(char_iter,1);
                        }
                }

                //if the first character of the label is not an alpha numeric and it is also not a space or a period
                else if(!isspace(*char_iter) && *char_iter != '.')
                        throw file_parse_exception("Label must begin with a letter.  Error on line " + convert.str());

                //iterate until end of word if past 8 characters
                while(!isspace(*char_iter) && *char_iter != '.'){
                        if(char_iter == char_contents->end()){ break; }
                        advance(char_iter,1);
                }

                //if the end of the line is reached, push the tokens and continue onto the next line
                if(char_iter == char_contents->end()){
                        tokens.push_back(word_temp);
                        line_number++;
                        continue;
                }

                //Check for next word, if so add to tokens, else break
                while(true){
                        //iterate until next appropriate character
                        while(isspace(*char_iter) && char_iter != char_contents->end()){ advance(char_iter,1); }

                        //if the end of the line is reached, break out of the while loop and move onto pushing
                        //the tokens and moving onto the next line
                        if(char_iter == char_contents->end()){
                                break;
                        }

                        //check for comment
                        if(*char_iter == '.'){
                                word_temp.col_four =(*v_iter).substr(distance(char_contents->begin(),char_iter),1000);
                                break;
                        }

                        //check for next word
                        else if(isprint(*char_iter)){
                                while(isprint(*char_iter) && !isspace(*char_iter) && char_iter != char_contents->end()){
                                        if(*char_iter == '\''){
                                                do{
                                                        switch(count){
                                                                case 0  :       throw file_parse_exception("\' Not allowed in Opcode. Error on line " + convert.str());
                                                                                break;
                                                                case 1  :       word_temp.col_three += *char_iter;
                                                                                advance(char_iter,1);
                                                                                break;
                                                                default :       throw file_parse_exception("Too many words on line " + convert.str());
                                                                                break;
                                                        }

                                                        //make sure there is a closing single quote
                                                        if(char_iter == char_contents->end()) throw file_parse_exception("\' Expected on line " + convert.str());

                                                }while(*char_iter != '\'');
                                        }

                                        //store the characters of the opcode or the operand in their proper columns
                                        if(isprint(*char_iter) && !isspace(*char_iter)){
                                                switch(count){
                                                        case 0  :       word_temp.col_two += *char_iter;
                                                                        advance(char_iter,1);
                                                                        break;
                                                        case 1  :       word_temp.col_three += *char_iter;
                                                                        advance(char_iter,1);
                                                                        break;
                                                        default :       throw file_parse_exception("Too many words on line " + convert.str());
                                                                        break;
                                                }
                                        }
                                }
                        }
                        else{ break; }

                        count++;
                }

                tokens.push_back(word_temp);
                line_number++;
        }
}

string file_parser::get_token(unsigned int x, unsigned int y){
        ostringstream convert;
        convert << x << ',' << y;

        //find the row (x) and column (y) the specified token is in and return it
        if(x < tokens.size()){
                CodeLine l = tokens.at(x);
                switch(y) {
                        case 0  : return l.col_one;
                        case 1  : return l.col_two;
                        case 2  : return l.col_three;
                        case 3  : return l.col_four;
                        default : throw file_parse_exception("Error. Token (" + convert.str() + ") does not exist");
                                  break;
                }
        }

        //the token does not exist, the request is out of bounds
        else{
                throw file_parse_exception("Error. Token (" + convert.str() + ") does not exist");
        }
}

void file_parser::print_file(){
        vector<CodeLine>::iterator v_iter;

        //print the tokens of the file in their appropraite columns
        for(v_iter = tokens.begin(); v_iter != tokens.end(); advance(v_iter,1)){
                cout << v_iter->col_one << "\t" << v_iter->col_two << "\t";
                cout << v_iter->col_three << "\t" << v_iter->col_four << endl;
        }
}

int file_parser::size(){ return (int) contents.size(); }

void file_parser::open_file(){

        //open the file and read it into the program as a character string
        //get each line and push it into "contents"

        infile.open(text_file.c_str());
        string line;

        if(!infile)
                throw file_parse_exception("Error: 404. Text file not found.");
        while(!infile.eof()){
                getline(infile, line);
                contents.push_back(line);
        }
        infile.close();
}