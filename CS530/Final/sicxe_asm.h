/*
        prog3
        CS530, Spring 2014

        sicxe_asm.h
*/

#ifndef SICXE_ASM_H
#define SICXE_ASM_H

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctype.h>
#include <iomanip>
#include "opcodetab.h"
#include "opcode_error_exception.h"
#include "file_parser.h"
#include "file_parse_exception.h"
#include "symtab.h"
#include "symtab_exception.h"

//list of valid assembly directives
const string asm_directives[] = {"START","END","BYTE","WORD","RESB","RESW","BASE",
                                        "NOBASE","EQU"};

const string registers[] = {"A","X","L","B","S","T","PC","SW"};

const string reg_values[] = {"0","1","2","3","4","5","8","9"};

class sicxe_asm{

        public:

                //ctor
                sicxe_asm();

                //dtor
                ~sicxe_asm();

                //Acts as the main function for the assembler
                //input .asm file to assemble into machine code
                //for now, outputs .lis
                void run(string);

        private:

                //Flag object for sicxe_asm flag bits
                //Used when constructing machine code
                struct flag{
                        bool n;
                        bool i;
                        bool x;
                        bool b;
                        bool p;
                        bool e;
                };

                //Object that holds users sicxe_asm data for each line
                struct list_record{
                        int line;
                        string address;
                        string label;
                        string opcode;
                        string operand;
                        string machine_code;
                        flag flags;
                };

                map<string,string> regs;
                map<string,string>::iterator reg_itr;

                //Global Variables
                file_parser *parser;
                opcodetab *opcode_table;
                symtab *symbol_table;
                vector<list_record> list_file_vec;
                int location_counter;
                string base;
                int token_address;
                int contents;
                string prog_name;
                string obj_name;

                string hex_string;
                string dec_string;

                //checks if pc relative addressing was withing range
                //returns true if within range, else false
                bool pc_worked;

                //used for next_token and next_line()
                int current_token;
                int current_line;

                // takes in a .asm file and runs the file_parser
                // if error returns false, otherwise true
                void parse_file(string);

                //Checks for assembler directives,
                //fills the symbol table, and assigns addresses
                //if error returns false, otherwise true
                bool pass_one();

                //Checks labels for forward references, if label
                //is forward reference it fills in the value
                //Then generates appropriate machine code
                bool pass_two();

                //after the first pass is complete this outputs
                //data as a .lis file
                void output_list_file(string);

                //Gets the next token in a line of asm file
                //if none returns blank line
                string next_token();

                //Goes to next line of asm file
                //if end of file returns false
                bool next_line();

                //loads any user symbols in symbol table and assigns address
                void first_token(string,list_record &);

                //validates second and third tokens
                //checks if the opcode is an assembler directive
                bool mid_token(string,list_record &);

                //returns true is string is asm directive keyword
                bool is_asm_directive(string);

                //convert filename.asm to filename.lis
                string convert_filename(string);

                //print .lis file header in .lis file
                void print_list_header(ofstream &);

                //print .lis file contents in .lis file
                void print_list_contents(ofstream &);

                //looks up asm directive and returns changes location in decimal bytes
                int asm_directive(string,string);

                int operand_type(string);

                //performs EQU operations on operand
                //returns address or label of operand
                string equ_directive(string);

                //returns int of bytes allocated by byte directive
                int byte_directive(string);

                //sets label to operand
                void equ_direction(string);

                //checks contents after end, throws exception
                //if code exists
                void check_after_end();

                //Makes sure that all user defined symbols are
                //assigned values
                //if value is a forward reference call check_again
                bool check_forward_reference(list_record);

                //Uses recursion to follow reference chain until
                //a value (address) is found
                bool check_again(list_record,string);

                //Sets the appropriate flags for each line
                void check_format_four(list_record &);
                void check_indirect(list_record &);
                void check_immediate(list_record &);
                void check_xbit(list_record &);
                void check_base_bit(list_record &);
                void check_pc_bit(list_record &);

                //Generates machine code from list record
                bool get_machine_code(list_record &, bool &);

                //Returns displacement from dest to source
                //Dest -(Source+3)
                string get_pc_displacement(string, string);

                //combines all bits of machine cod
                // Operand code + flags + address/displacement
                void combine_mc(list_record &);

                //Gets the address code or displacement
                void get_address_code(list_record &, bool &);

                //Gets the machine code for Word ASM Directive
                void get_mc_word(list_record &);

                //Gets the machine code for Byte ASM Directive
                void get_mc_byte(list_record &);

                //Checks if string is a label
                bool is_label(string);

                //Checks if PC relative addressing overflows
                //Returns true if is overflow
                bool is_reference_overflow(int,int,string);

                //Returns displacement from source to
                //base
                string get_base_relative(list_record &);

                //OBJECT FILE METHODS
                //These methods create the object file and print
                //the header, text, modification, and end records
                //to that file
                void output_obj_file(string);
                string make_obj_file(string);
                void print_obj_contents(ofstream &);
                void print_header_record(ofstream &);
                void print_text_record(ofstream &);
                void print_mod_record(ofstream &);
                void print_end_record(ofstream &);

                // CONVERSION FUNCTIONS

                //Function to convert string to int
                int string_to_int(string s);

                //Function to convert decimal integer to string hexadecimal
                string int_to_hex(int num, int width);

                //Function to convert string hexadecimal to a decimal integer
                int hex_to_int(string s);

                //Function to convert decimal string to an integer
                int dec_to_int(string s);

                //Function to convert string to upper case
                string to_uppercase(string s);

                //No idea what this does
                string format_15(int x);

                //Convert string hexadecimal to string binary
                //Code by WriteGreatCode
                //http://www.cplusplus.com/forum/beginner/41657/
                string hex_to_bin(string);

                //Convert string binary to string hexadecimal
                //Code by qq66921494
                //http://www.cplusplus.com/forum/windows/35848/
                string bin_to_hex(string);

                //Convert boolean to string value
                string bool_to_string(bool);

                //Convert int to string, returns string
                string int_to_string(int);

                //Convert string to ascii hex value
                string string_to_ascii(string);


};

#endif