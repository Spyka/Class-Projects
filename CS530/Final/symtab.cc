/*
        CS 530, Spring 2014

        symtab.cc
*/

#include "symtab.h"
#include "symtab_exception.h"

using namespace std;

symtab::symtab(){}

void symtab::insert_label(string label, string value){
        //if the label is not found, insert it
        if (symbol.find(label) == symbol.end())
                symbol[label] = value;

        //if the label is already in the table throw an error
        else
                throw symtab_exception("Label: " + label + " already is defined.");
}

string symtab::get_value(string label){
        //set an iterator pointer the the label after it is found
        sym_itr = symbol.find(label);

        //if the iterator is equal to the end, then the label was not in
        //the symbol table
        if (sym_itr == symbol.end())
                throw symtab_exception("Label: " + label + " not defined.");

        //get the value of the label
        sym_value = sym_itr -> second;

        if (sym_value.empty())
                throw symtab_exception("Label: "  + label + " does not contain a value");

        return sym_value;
}

void symtab::set_value(string label, string value){
        //set an iterator pointer the the label after it is found
        sym_itr = symbol.find(label);

        //if the iterator is equal to the end, then insert the label
        //because it was not found in the symbol table
        if (sym_itr == symbol.end())
                insert_label(label,value);

        //if the label has a value, return it
        if (!value.empty())
                sym_itr->second = value;

}

void symtab::print_symtab(){
        //set an iterator to the beginning of the symbol table
        print_itr = symbol.begin();

        //loop through the table printing the label and value
        while (print_itr != symbol.end()){
                cout << print_itr -> first << " " << print_itr -> second << endl;

                advance(print_itr,1);
        }


}

bool symtab::contains_symbol(string label){
        if (symbol.find(label) != symbol.end())
                return true;
        else
                return false;
}