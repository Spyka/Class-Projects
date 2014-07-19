/*
        opcodetab.cc
        CS530, Spring 2014
*/

#include "opcodetab.h"
#include "opcode_error_exception.h"

using namespace std;

        // ctor
        // creates a new dictionary structure and loads all of the opcodes for
        // the SIC/XE architecture into the table.  Use the STL map for this.
        opcodetab::opcodetab() {

                for(unsigned int i=0; i<(sizeof(opcodes)/sizeof(*opcodes)); i++)
                {
                        instruction_set[opcodes[i]] = pair<string,int>(hex_value[i],sizes[i]);
                }

        }

        // takes a SIC/XE opcode and returns the machine code
        // equivalent as a two byte string in hexadecimal.
        // Example:  get_machine_code("ADD") returns the value 18
        // Note that opcodes may be prepended with a '+'.
        // throws an opcode_error_exception if the opcode is not
        // found in the table.
        string opcodetab::get_machine_code(string s) {

                //Checks to see if the opcode begins with a +
                //If so move past that plus to only look at the code
                if (s[0] == '+')
                        s = s.substr(1);

                //Change the code to all upper case
                for (unsigned int i=0; i<s.size();i++)
                        s[i] = toupper(s[i]);

                //Look for the opcode in the map created
                m_iter = instruction_set.find(s);

                //If the iterator hits the end of the map then the opcode
                //does not exist, therefore throw an error
                if (m_iter == instruction_set.end())
                        throw opcode_error_exception("Opcode \'" + s + "\' not found");

                //return the hex code for the opcode given
                code_hex_val = m_iter -> second.first;
                return code_hex_val;
        }

        // takes a SIC/XE opcode and returns the number of bytes
        // needed to encode the instruction, which is an int in
        // the range 1..4.
        // NOTE: the opcode must be prepended with a '+' for format 4.
        // throws an opcode_error_exception if the opcode is not
        // found in the table.
        int opcodetab::get_instruction_size(string s) {
                plus_flag = false;

                //check to see if the opcode begins with a +
                //if so, flag it for later use
                if (s[0] == '+')
                {
                        s = s.substr(1);
                        plus_flag = true;
                }

                //Change the code to all upper case
                for (unsigned int i=0; i<s.size();i++)
                        s[i] = toupper(s[i]);

                //Look for the opcode in the map created
                m_iter = instruction_set.find(s);

                //If the iterator hits the end of the map then the opcode
                //does not exist, therefore throw an error
                if (m_iter == instruction_set.end())
                        throw opcode_error_exception("Opcode \'" + s + "\' not found");

                //find the size of the instruction
                code_size = m_iter -> second.second;

                //If the size of the instuction is equal to 3 and the
                //opcode begins with a + then increment the opcode size
                //so that it is now a 4
                if (plus_flag && code_size == 3)
                        code_size++;

                //If the size of the instruction is not a 3 and there is
                //a plus at the beginning of the opcode then there is an
                //error
                else if (plus_flag && code_size != 3)
                        throw opcode_error_exception("+" + s + " is invalid. Only opcodes of size 3 may be extended to 4 with '+'");

                return code_size;
        }