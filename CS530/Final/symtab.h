/*
        CS 530, Spring 2014

        symtab.h
*/

#ifndef SYMTAB_H
#define SYMTAB_H

#include <string>
#include <map>
#include <utility>
#include <cstdlib>
#include <iostream>
#include <sstream>

using namespace std;

class symtab {
    public:
        // ctor
        symtab();

        //Inserts a label and its value (address or immediate) into the symbol
        //table.  Throws an exception if the label is already in the symbol table
        void insert_label(string ,string);

        //Returns the value of the label passed through if it contains one
        string get_value(string);

        //sets the value of the label if the label is already
        //defined or inserts the label if not
        void set_value(string,string);

        //prints the contents of the symbol table - the label and the value
        void print_symtab();

        //checks to see if the label is in the symbol table
        bool contains_symbol(string);

    private:
        //map containing the label and its value
        //and iterators used to access the map
        map<string, string > symbol;
        map<string, string >::iterator sym_itr;
        map<string, string >::iterator print_itr;

        string sym_value;

};


#endif