/*
        prog3
        CS530, Spring 2014

        sicxe_asm.cpp
*/

#include "sicxe_asm.h"
#include <stdexcept>

using namespace std;

//ctor
//creates the opcode table and the symbol table
sicxe_asm::sicxe_asm(){
        current_token = 0;
        current_line = 1;
        location_counter = 0;
        base = -1;
        opcode_table = new opcodetab();
        symbol_table = new symtab();
}

//dtor
sicxe_asm::~sicxe_asm(){}

void sicxe_asm::run(string file){

        try{
        while(true){

                //parse the file
                try{ parse_file(file); }
                catch (file_parse_exception e){
                        cout << e.getMessage() << endl;
                        break;
                }

                //initiate pass one
                if(!pass_one()) break;

                if(!pass_two()) break;

                //make the lis file
                output_list_file(file);
                output_obj_file(file);
                break;
        }
        }
        catch(std::length_error e){
                cout << "Error:" << e.what() << endl;
        }
}

//*******************************************************
//                      PRIVATE METHODS
//*******************************************************
//                      PASS METHODS
//*******************************************************

bool sicxe_asm::pass_one(){
        try{
                //do the following while there are still lines in the asm file
                 do{
                        list_record record;

                        //first token consists of the label
                        first_token(next_token(),record);

                        //mid token consists of the opcode and the operand
                        if(!mid_token(next_token(),record)){
                                list_file_vec.push_back(record);
                                check_after_end();
                                break;
                        }

                         //push the contents into the lis file
                         list_file_vec.push_back(record);
                 }
                 while(next_line());
        }
        catch(opcode_error_exception e){
                cout << e.getMessage() << " Error on line " << current_line << endl;
                return false;
        }

        return true;
}

bool sicxe_asm::pass_two(){
        list_record record;

        //this boolean determines whether pc or base will be used
        pc_worked = true;

        //loop through the listing file
        for(unsigned int i = 0; i < list_file_vec.size();i++){
                record = list_file_vec.at(i);
                record.flags.n = 0;
                record.flags.i = 0;
                record.flags.x = 0;
                record.flags.b = 0;
                record.flags.p = 0;
                record.flags.e = 0;

                //look for forward references (including simple and
                //complex forward references)
                if(!check_forward_reference(record))
                        return false;

                //get the machine code for the line in question
                if(!get_machine_code(record, pc_worked))
                        return false;

                //edit the list file
                list_file_vec.at(i) = record;

        }

        return true;
}

//*******************************************************
//                      OTHER METHODS
//*******************************************************


void sicxe_asm::output_list_file(string f){

        string list_name = convert_filename(f);
        ofstream list(list_name.c_str());

        print_list_header(list);
        print_list_contents(list);
}

void sicxe_asm::parse_file(string s){

        //Parse .asm file
        parser = new file_parser(s);
        parser->read_file();
}

string sicxe_asm::convert_filename(string f){
        string name;

        //get the filename up until the period
        for(unsigned int i = 0; i < f.size();i++){
                if(f[i] == '.'){
                        break;
                }

                name += f[i];
        }

        //concatinate the extension onto the filename
        name = name + ".lis";
        return name;
}

void sicxe_asm::print_list_header(ofstream &list){
        stringstream sstr;
        int list_space = 14;

        //setw handles the spacing, and the words are filled with blank space
        //to before passing in the titles to create the header
        sstr << setw(list_space) << setfill(' ') << "Line#";
        sstr << setw(list_space) << setfill(' ') << "Address";
        sstr << setw(list_space) << setfill(' ') << "Label";
        sstr << setw(list_space) << setfill(' ') << "Opcode";
        sstr << setw(list_space) << setfill(' ') << "Operand";
        sstr << setw(list_space) << setfill(' ') << "Machine Code" << endl;

        sstr << setw(list_space) << setfill(' ') << "=====";
        sstr << setw(list_space) << setfill(' ') << "=======";
        sstr << setw(list_space) << setfill(' ') << "=====";
        sstr << setw(list_space) << setfill(' ') << "======";
        sstr << setw(list_space) << setfill(' ') << "=======";
        sstr << setw(list_space) << setfill(' ') << "============" << endl;

        list << sstr.str();
}

void sicxe_asm::print_list_contents(ofstream &list){
        stringstream sstr;
        int list_space = 14;

        //loop through and print the contents of the listing file
        for(unsigned int i = 0;i < list_file_vec.size();i++){
                sstr << setw(list_space) << setfill(' ') << list_file_vec.at(i).line;
                sstr << setw(list_space) << setfill(' ') << list_file_vec.at(i).address;
                sstr << setw(list_space) << setfill(' ') << list_file_vec.at(i).label;
                sstr << setw(list_space) << setfill(' ') << list_file_vec.at(i).opcode;
                sstr << setw(list_space) << setfill(' ') << list_file_vec.at(i).operand;
                sstr << setw(list_space) << setfill(' ') << list_file_vec.at(i).machine_code << endl;
                list << sstr.str();
                sstr.clear();
                sstr.str(string());
        }

}

//*******************************************************
//              EXTRA PASS ONE METHODS
//*******************************************************

string sicxe_asm::next_token(){
        try{
                //returns the token following the one currently in question
                //example: the operand after the opcode
                string tmp = parser->get_token(current_line - 1,current_token);
                current_token++;
                return tmp;
        }
        catch(file_parse_exception e){
                //This function should not be called more than 4 times
                return "";
        }
}

bool sicxe_asm::next_line(){
        try{
                //checks for stored tokens to see if there is a next line
                current_token = 0;
                current_line++;
                parser->get_token(current_line-1,current_token);
        }
        catch(file_parse_exception e){
                return false;
        }
        return true;
}

void sicxe_asm::first_token(string t,list_record &r){

        r.line = current_line;
        r.address = int_to_hex(location_counter,6);
        r.label = t;

        //make opcode uppercase after getting that token
        //label in question has its value set as long as the opcode following is not EQU
        if(t != "" && (to_uppercase(parser->get_token(current_line-1,current_token+1)) != "EQU")) {
                symbol_table->set_value(to_uppercase(t),int_to_hex(location_counter,6));
                //cout << "hello" << endl;
        }
}

bool sicxe_asm::mid_token(string t,list_record &r){


        r.opcode = t;
        t = to_uppercase(t);

        //check to see if token is in the list of asm directives
        if(is_asm_directive(t)){
                string next = next_token();

                //make sure token is all uppercase and check for the start of the program
                //if there is code before the start of the program that is not the a
                //comment or blank line throw an error
                if (to_uppercase(t) == "START" && location_counter != 0)
                        throw opcode_error_exception("Only comments and blank lines may be before START directive.");

                //handle the asm directive
                int i = asm_directive(t,next);

                if(i == -1){
                        r.operand = next;
                        return false;
                }

                //move forward in the program and store the operand
                location_counter += i;
                r.operand = next;
                return true;
        }

        else{
                //move the location counter forward by the opcode's instruction size
                //this will be used for the address
                if(t != ""){
                        location_counter += opcode_table->get_instruction_size(t);
                }

                r.operand = next_token();
        }

        return true;
}

bool sicxe_asm::is_asm_directive(string s){
        s = to_uppercase(s);

        //loop through the list of directives to find the string in question
        for(unsigned int i = 0; i < sizeof(asm_directives);i++){
                if(asm_directives[i].compare(s) == 0){
                        return true;
                }
        }
        return false;
}

void sicxe_asm::check_after_end(){

        //check to see if there is any code after the END directive that is not a comment
        //or a blank line
        while(next_line()){
                for(int i = 0;i<3;i++){
                        if(parser->get_token(current_line-1,i) != "")
                                throw opcode_error_exception("Code found after END directive. Are you drunk?");
                }
        }
}

//*******************************************************
//              EXTRA PASS TWO METHODS
//*******************************************************

bool sicxe_asm::check_forward_reference(list_record r){
        //if no label or prog label then get next line
        if(r.label == "" || r.label == "prog" || r.label == "PROG") return true;

        //check label value in symbol table for another label
        if(isalpha((symbol_table->get_value(to_uppercase(r.label))).at(0))){

                //if not EQU directive then throw error
                if(r.opcode != "EQU"){
                        cout << "Error on line " << r.line << ". Label " << r.label << " not defined." << endl;
                        return false;
                }

                //try to retrieve the operand's(another label) value in the symbol table,
                //if retrieved set that value to the label
                try{
                        symbol_table->set_value(to_uppercase(r.label),symbol_table->get_value(to_uppercase(r.operand)));
                }
                catch(symtab_exception e){      //operand(label) did not exist
                        cout << "Error on line " << r.line << ". Label " << r.operand << " is not a valid reference" << endl;
                        return false;
                }
        }
        string sym_val = symbol_table->get_value(to_uppercase(r.label));        //store the new value label equals

        //check if the new value is another label
        if(isalpha(sym_val.at(0))){
                //run recursive function, if it fails return false
                if(!check_again(r,sym_val)) return false;
        }
        return true;
}

bool sicxe_asm::check_again(list_record r, string val){
        //try to retrieve the operand's(another label) value in the symbol table,
        //if retrieved set that value to the label
        try{
                symbol_table->set_value(to_uppercase(r.label),symbol_table->get_value(val));
        }
        catch(symtab_exception e){      //operand(label) does not exist
                cout << "Error in forward reference chain. Chain starts on line " << r.line << "." << endl;
                return false;
        }
        string sym_val = symbol_table->get_value(to_uppercase(r.label));        //store the new value label equals

        //check if the new value is another label
        if(isalpha(sym_val.at(0))){
                //use recursion
                 check_again(r,sym_val);
        }
        return true;
}

//flags the e bit if format four is being used
void sicxe_asm::check_format_four(list_record &record){
        string code = record.opcode;

        if(code[0] == '+'){
                record.flags.e = 1;

                if(record.flags.p == 1)
                        record.flags.p = 0;
        }

        else
                record.flags.e = 0;
}

//flags the n bit if it is being used (the @ symbol)
void sicxe_asm::check_indirect(list_record &record){
        string oprnd = record.operand;

        if(oprnd[0] == '@')
                record.flags.n = 1;
        else
                record.flags.n = 0;
}

//flags the i bit if it is being used (the # symbol)
void sicxe_asm::check_immediate(list_record &record){
        string oprnd = record.operand;

        if(oprnd[0] == '#')
                record.flags.i = 1;
        else
                record.flags.i = 0;
}

//flags the x bit if the x register is being used
void sicxe_asm::check_xbit(list_record &record){
        string oprnd = record.operand;
        oprnd = to_uppercase(oprnd);

        int strSize = oprnd.length();

        if((oprnd[strSize-2] == ',') && (oprnd[strSize-1] == 'X'))
                record.flags.x = 1;

        else if ((oprnd[0] == 'X') && (oprnd[1] == ','))
                record.flags.x = 1;
        else
                record.flags.x = 0;
}

//flags the base bit and clears the p bit if base is needed
void sicxe_asm::check_base_bit(list_record &record){
                record.flags.p = 0;
                record.flags.b = 1;
}

//flags the pc bit if needed
void sicxe_asm::check_pc_bit(list_record &record){
        string oprnd = to_uppercase(record.operand);

        //move past the @ symbol if the operand starts with it
        if(oprnd[0] == '@')
                oprnd = oprnd.substr(1);

        //loop through the operand, seeing if it includes an X,
        //or a ,X for the x register
        for(unsigned int i=0; i<oprnd.length(); i++){
                if((oprnd[0] == 'X') && (oprnd[1] == ',')){
                        oprnd = oprnd.substr(2);
                        break;
                }

                if(oprnd[i] == ','){
                        oprnd = oprnd.substr(0,i);
                }
        }

        //if the operand is in the symbol table then flag the p bit
        if(symbol_table->contains_symbol(oprnd))
                record.flags.p = 1;

        //if the operand is an address flag the p bit
        else if(oprnd[0] == '$')
                record.flags.p = 1;

        else
                record.flags.p = 0;
}

bool sicxe_asm::get_machine_code(list_record &record, bool &pc_worked){

        //check to see if n needs to be set
        check_indirect(record);

        //check to see if i needs to be set
        check_immediate(record);

        //check to see if x needs to be set
        check_xbit(record);

        //check to see if p needs to be set
        check_pc_bit(record);

        //check to see if e needs to be set
        check_format_four(record);

        //if n and i are both not set (equal to 0), then make both
        //equal to 1
        if((record.flags.i == 0) && (record.flags.n == 0))
        {
                record.flags.i = 1;
                record.flags.n = 1;
        }

        try{
                //get the first half of the machine code for the line
                combine_mc(record);
        }
        catch(opcode_error_exception e){
                cout << e.getMessage() << endl;
                return false;
        }

        //get the second half of the machine code for the line
        get_address_code(record, pc_worked);

        if(is_asm_directive(to_uppercase(record.opcode))){
                if(to_uppercase(record.opcode) == "WORD")
                        get_mc_word(record);

                if(to_uppercase(record.opcode) == "BYTE")
                        get_mc_byte(record);
        }

        return true;
}

void sicxe_asm::get_mc_word(list_record &record){
        string oprnd = record.operand;

        int tmp = string_to_int(oprnd);
        string mc_code = int_to_hex(tmp,6);

        record.machine_code = mc_code;
}

void sicxe_asm::get_mc_byte(list_record &record){
        string oprnd = record.operand;
        string code, tmp_str, starting;
        int tmp;

        starting = oprnd[0];
        starting = to_uppercase(starting);

        if(starting == "X"){
                int i=2;
                while(oprnd[i] != '\''){
                        code += oprnd[i];
                        i++;
                }

                code = to_uppercase(code);
        }

        else if(starting == "C"){
                int i=2;
                while(oprnd[i] != '\''){
                        tmp_str += oprnd[i];
                        i++;
                }

                code = string_to_ascii(tmp_str);
        }

        else{
                tmp = string_to_int(oprnd);
                 code = int_to_hex(tmp,6);
        }

        record.machine_code = code;
}

void sicxe_asm::get_address_code(list_record &record, bool &pc_worked){
        stringstream sstr, sstream;
        string add,disp;
        string oprnd = to_uppercase(record.operand);
        int size = -1;

        //if there is no opcode or if the opcode is an assembly directive, don't
        //do anything, just return
        if(record.opcode == "" || is_asm_directive(record.opcode)) return;

        try{
                //get the instruction size (or the format) of the opcode
                size = opcode_table->get_instruction_size(record.opcode);
        }
        catch(opcode_error_exception e){
                cout << "Error on line: " << record.line << " " << e.getMessage() << endl;
        }

        //if format 1
        if(size == 1){
                //the lines machine code equals the machine code for the opcode
                record.machine_code = opcode_table->get_machine_code(record.opcode);
        }

        //if format 2
        else if(size == 2){
                string reg1,reg2;
                int comma_mark = 0;
                bool one_reg = false;

                //get the machine code of the opcode
                string machine_code = opcode_table->get_machine_code(record.opcode);

                //the first half of the machine code is the opcode's machine code
                record.machine_code = machine_code;

                //load all the registers and their values
                for(unsigned int i=0; i<(sizeof(registers)/sizeof(*registers)); i++){
                        regs[registers[i]] = reg_values[i];
                }

                //if the operand begins with a digit
                if(isdigit(oprnd[0])){
                        //loop through looking for the end of the operand
                        //or a comma
                        for(unsigned int i=0; i<oprnd.length();i++){
                                //if the next part of the operand equals the
                                //operand length then we have hit the end
                                //theres only one register in the operand
                                if(i+1 == oprnd.length()){
                                        reg1 += oprnd[i];
                                        one_reg = true;
                                        break;
                                }

                                //mark where the comma is and break from the loop
                                if(oprnd[i] == ','){
                                        comma_mark = i;
                                        break;
                                }

                                //keep adding the parts of the operand to the register
                                //in case it has more than one index before the comma
                                //ex: 13
                                reg1 += oprnd[i];
                        }

                        //if there is more than one register used
                        //(if a comma is found before the end of the operand)
                        //substring the operand after the comma mark
                        if(!one_reg)
                                reg2 = oprnd.substr(comma_mark+1);

                        //change the number in register one into
                        //hex and add it to the machine code
                        int tmp = string_to_int(reg1);
                        reg1 = int_to_hex(tmp,1);
                        record.machine_code += reg1;

                        //if only one register is used, place a zero
                        //in the second one
                        if(one_reg)
                                record.machine_code += "0";
                        else{
                                //if the second register's value begins with
                                //an alpha then find that value in the registers map
                                //and return the proper value of the register
                                if(isalpha(reg2[0])){
                                        reg_itr = regs.find(reg2);
                                        record.machine_code += (reg_itr -> second);
                                }

                                //if the second register's value begins with
                                //a digit add that digit to the machine code
                                if(isdigit(reg2[0]))
                                        record.machine_code += reg2;
                        }
                }

                //if the operand begins with an alpha
                if(isalpha(oprnd[0])){
                        for(unsigned int i=0; i<oprnd.length();i++){
                                //if the next part of the operand equals the
                                //operand length then we have hit the end
                                //theres only one register in the operand
                                if(i+1 == oprnd.length()){
                                        reg1 += oprnd[i];
                                        one_reg = true;
                                        break;
                                }

                                //mark where the comma is and break from the loop
                                if(oprnd[i] == ','){
                                        comma_mark = i;
                                        break;
                                }

                                reg1 += oprnd[i];
                        }

                        //if there is more than one register used
                        //(if a comma is found before the end of the operand)
                        //substring the operand after the comma mark
                        if(!one_reg)
                                reg2 = oprnd.substr(comma_mark+1);

                        //find the string in the first register in the register's map
                        //and add that proper value onto the end of the machine code
                        reg_itr = regs.find(reg1);
                        record.machine_code += (reg_itr -> second);

                        //if only one register is used, place a zero
                        //in the second one
                        if(one_reg)
                                record.machine_code += "0";
                        else{
                                //if the second register's value begins with
                                //an alpha then find that value in the registers map
                                //and return the proper value of the register
                                if(isalpha(reg2[0])){
                                        reg_itr = regs.find(reg2);
                                        record.machine_code += (reg_itr -> second);
                                }

                                //if the second register's value begins with
                                //a digit add that digit to the machine code
                                if(isdigit(reg2[0])){
                                        if((to_uppercase(record.opcode) == "SHIFTR") ||
                                                (to_uppercase(record.opcode) == "SHIFTL")){
                                                int tmp = (string_to_int(reg2)) - 1;

                                                if(tmp < 0)
                                                        tmp = 0;

                                                reg2 = int_to_string(tmp);
                                        }

                                        record.machine_code += reg2;
                                }
                        }
                }
        }

        //if format 3
        else if(size == 3){
                string hex;
                string start_set;
                int num;

                //RSUB is format 3/4 but does not include an operand
                //the second half of the machine code is just zeros
                if(to_uppercase(record.opcode) == "RSUB")
                        record.machine_code += "000";

                //check to see if the operand begins with a #
                if(oprnd[0] == '#'){
                        //if the next part of the operand is a digit change that
                        //number to hex and add it onto the machine code
                        //it must be formated with 3 spaces, therefore...
                        //ex: #3 -> 003
                        if(isdigit(oprnd[1])){
                                num = dec_to_int(oprnd.substr(1));
                                hex = int_to_hex(num, 3);
                                sstream << setw(3) << setfill('0') << hex;
                                record.machine_code += sstream.str();
                        }

                        //if the next part of the operand is a $ sign, then
                        //substring the operand past the $ and add that hex
                        //value onto the end of the machine code, it must be
                        //formated with 3 spaces as above
                        if(oprnd[1] == '$'){
                                hex = oprnd.substr(2);
                                sstream << setw(3) << setfill('0') << hex;
                                record.machine_code += sstream.str();
                        }

                }

                //loop through the operand, seeing if it includes an X,
                //or a ,X for the x register; get the value after the
                //X, or get the value before the ,X  (whichever is necessary)
                for(unsigned int i=0; i<oprnd.length(); i++){
                        if((oprnd[0] == 'X') && (oprnd[1] == ',')){
                                oprnd = oprnd.substr(2);
                                break;
                        }

                        if(oprnd[i] == ','){
                                oprnd = oprnd.substr(0,i);
                        }
                }

                //if the operand contains a label
                if(is_label(oprnd)){
                        //get the pc displacement of the line
                        disp = get_pc_displacement(record.operand,record.address);

                        //if pc doesn't work it will return "BASE" which tells the
                        //assembler to use base instead
                        if(disp == "BASE"){
                                //set the base bit and clear the pc bit
                                check_base_bit(record);

                                //clear the machine code
                                record.machine_code = "";

                                //re-find the first half of the machine code now
                                //using the b bit
                                combine_mc(record);

                                //get the displacement using base relative
                                string base_disp = get_base_relative(record);

                                //add that displacement to the machine code
                                record.machine_code += base_disp;
                        }
                        //if pc works properly add that displacement to the
                        //machine code
                        else
                                record.machine_code += disp;
                }
                else{
                        sstr << int_to_hex(operand_type(record.operand),3);
                        add = sstr.str();
                }
        }

        //if format 4
        else if(size == 4){
                if(to_uppercase(record.opcode) == "+RSUB")
                        record.machine_code += "00000";

                if(record.operand != "")
                {
                        //make sure the operand does not start with any extra symbols
                        if((oprnd[0] != '#') && (oprnd[0] != '@') && (oprnd[0] != '$')){
                                //loop through the operand, seeing if it includes an X,
                                //or a ,X for the x register; get the value after the
                                //X, or get the value before the ,X  (whichever is necessary)
                                for(unsigned int i=0; i<oprnd.length(); i++){
                                        if((oprnd[0] == 'X') && (oprnd[1] == ',')){
                                                oprnd = oprnd.substr(2);
                                                break;
                                        }

                                        if(oprnd[i] == ','){
                                                oprnd = oprnd.substr(0,i);
                                        }
                                }

                        //look for the operand in the symbol table and return
                        //its value, substring the value so that it is the last
                        //5 characters
                        add = symbol_table->get_value(oprnd);
                        add = add.substr(1);
                        }
                        else{
                                //if the operand begins with a $ symbol, skip
                                //past that symbol to use the address for the
                                //second half of the machine code
                                if(oprnd[0] == '$')
                                        add = oprnd.substr(1);

                                //if the operand begins with a # or a @, skip
                                //past that symbol
                                if((oprnd[0] == '#') || (oprnd[0] == '@')){
                                        oprnd = oprnd.substr(1);

                                        //if the rest of the operand is a label find
                                        //it in the symbol table and get its value
                                        //for the second half of the machine code
                                        if(isalpha(oprnd[0]))
                                                add = symbol_table->get_value(oprnd);

                                        //if the rest of the operand starts with a digit
                                        //then change that number to hex to be used for
                                        //the second half of the machine code
                                        if(isdigit(oprnd[0])){
                                                int tmp = string_to_int(oprnd.substr(0));
                                                add = int_to_hex(tmp,5);
                                        }

                                        //if the next part of the operand starts with a
                                        //$ then move past that symbol
                                        if(oprnd[0] == '$'){
                                                oprnd = oprnd.substr(1);

                                                //if the next part is a label find it in the symbol
                                                //table and store its value
                                                if(isalpha(oprnd[0]))
                                                        add = symbol_table->get_value(oprnd);

                                                //if the next part is a digit the following value
                                                //for the second half of the machine code
                                                if(isdigit(oprnd[0]))
                                                        add = oprnd.substr(0);
                                        }
                                }
                                //the length cannot exceed 5
                                //since addresses give 6, substring them
                                if(add.length() == 6)
                                        add = add.substr(1);


                        }
                        //add the appropriate value determined to the end of the machine code
                        record.machine_code += add;
                }
        }
}

void sicxe_asm::combine_mc(list_record &record){
        string mc_first_part,mc_second_part,mc_last_part,opcode_mc;

        //get the opcode
        string code = to_uppercase(record.opcode);

        //if there is no opcode or if the opcode is an assembly
        //directive, do nothing, just return
        if(code == "" || is_asm_directive(code)) return;

        //find the machine code of the opcode in question
        opcode_mc = opcode_table->get_machine_code(code);

        //xxxx xx|xx xxxx  (the format for the first half of the machine code)

        //Grab first nibble in opcode machine code
        mc_first_part = hex_to_bin(opcode_mc.substr(0,1));

        //seperate the second nilbble into two parts
        //Now in binary
        string second_digit = hex_to_bin(opcode_mc.substr(1,1));

        //Combine flags with opcode machine code to make second nibble
        mc_second_part += second_digit.substr(0,2);
        mc_second_part += bool_to_string(record.flags.n);
        mc_second_part += bool_to_string(record.flags.i);

        //Combine flags to make third nibble
        mc_last_part += bool_to_string(record.flags.x);
        mc_last_part += bool_to_string(record.flags.b);
        mc_last_part += bool_to_string(record.flags.p);
        mc_last_part += bool_to_string(record.flags.e);

        //combine all three nibbles together to make the first half of the machine code
        //changing the result from binary to hex
        record.machine_code = mc_first_part + mc_second_part + mc_last_part;
        record.machine_code = bin_to_hex(record.machine_code);
}

//pc displacement = destination - source+3
string sicxe_asm::get_pc_displacement(string dest, string source){
        //Note: dest is the operand, source is the address of the line
        string dest_address;
        dest = to_uppercase(dest);
        try{
                //if the destination (operand) begins with a $ sign, then
                //substring it to get the following address to be used for
                //the displacement
                if(dest[0] == '$')
                        dest_address = dest.substr(1);

                //if the destination (operand) begins with a # sign, or
                //a @ sign, then substring past it (do the same if a $
                //sign follows)
                if((dest[0] == '#') || (dest[0] == '@')){
                        if(dest[1] == '$'){
                                dest = dest.substr(2);
                        }
                        dest = dest.substr(1);
                }

                //check to see if the destination (operand) is using the
                //x register and either substring before if it finds a ,X
                //or substring after if it finds an X,
                for(unsigned int i=0; i<dest.length(); i++){
                        if((dest[0] == 'X') && (dest[1] == ',')){
                                dest = dest.substr(2);
                                break;
                        }

                        if(dest[i] == ','){
                                dest = dest.substr(0,i);
                                break;
                        }
                }

                //if the operand is a label then find it in the symbol
                //table and get its value as the destination address
                if(isalpha(dest[0]))
                        dest_address = symbol_table->get_value(dest);

                //change both the source address and the destination address
                //to ints to be used for arithmetic
                int src = hex_to_int(source);
                int dst = hex_to_int(dest_address);

                //pc displacement = destination address - (source address + 3)
                int disp = dst - (src+3);

                //change the found displacement back to hex
                string pc_disp = int_to_hex(disp,6);

                //if the length of the displacement is greater than 6, then
                //substring it to get the last 6 characters
                if(pc_disp.length() > 6){
                        int lgth = pc_disp.length() - 6;
                        pc_disp = pc_disp.substr(lgth);
                }

                //substr(3) b/c we want the last 3 for the machine code
                pc_disp = pc_disp.substr(3);

                //check to see if the pc displacement found overflows, if so
                //return "BASE"
                if(is_reference_overflow(dst,src,hex_to_bin(pc_disp)))
                        return "BASE";

                //return the displacement found
                return pc_disp;

        }
        catch(symtab_exception e){
                cout << e.getMessage() << endl;
        }

        return "";
}

bool sicxe_asm::is_reference_overflow(int dest, int source, string disp){
        //if the destination is past the source (a forward reference) but
        //the binary of the displacment begins with a one (meaning it is
        //negative) then there is an overflow
        if(dest > source && disp[0] == '1')
                return true;
        return false;
}

//check to see if there is a label past all the
//extra symbols
bool sicxe_asm::is_label(string s){
        if(s[0] == '#'){
                s = s.substr(1);
                return is_label(s);
        }
        if(s[0] == '$'){
                s = s.substr(1);
                return is_label(s);
        }
        if(s[0] == '@'){
                s = s.substr(1);
                return is_label(s);
        }
        return symbol_table->contains_symbol(to_uppercase(s));
}

string sicxe_asm::get_base_relative(list_record &record){
        string op = to_uppercase(record.operand);
        string dest_address, base_disp;
        int base_address = 0;
        int dest = 0;
        int disp = 0;

        try{
                //if the operand begins with a $ sign, then substring
                //it to get the following address to be used for
                //the displacement
                if(op[0] == '$')
                        dest_address = op.substr(1);

                //if the operand begins with a # sign, or
                //a @ sign, then substring past it (do the same if a $
                //sign follows)
                if((op[0] == '#') || (op[0] == '@')){
                        if(op[1] == '$'){
                                op = op.substr(2);
                        }
                        op = op.substr(1);
                }

                //check to see if the operand is using the
                //x register and either substring before if it finds a ,X
                //or substring after if it finds an X,
                for(unsigned int i=0; i<op.length(); i++){
                        if((op[0] == 'X') && (op[1] == ',')){
                                op = op.substr(2);
                                break;
                        }

                        if(op[i] == ','){
                                op = op.substr(0,i);
                                break;
                        }
                }

                //if the operand is a label then find it in the symbol
                //table and get its value as the destination address
                if(isalpha(op[0]))
                        dest_address = symbol_table->get_value(op);

                //change the destination address and the base address to ints
                //for arithmetic
                dest = hex_to_int(dest_address);

                //base is a global variable which is used to find the base
                //address (it is immediately changed from hex to an int)
                base_address = hex_to_int(symbol_table->get_value(base));

                //the displacement is the destination address - base address
                disp = dest - base_address;

                //change the displacement from an int back to the hex
                //(6 chars for address)
                base_disp = int_to_hex(disp,6);

                //use only the last 3 chars for the displacement
                base_disp = base_disp.substr(3);

                //return the base displacement to be used for the second
                //half of the machine code
                return base_disp;
        }
        catch(symtab_exception e){
                cout << "Error on line: " << record.line << ". Reference overflow. Must use base referencing" << endl;
        }

        return "";
}

//*******************************************************
//              ASSEMBLER DIRECTIVES
//*******************************************************

int sicxe_asm::asm_directive(string token,string next_token){
        next_token = to_uppercase(next_token);

        if(token == "START"){
                //sets the location counter
                location_counter = operand_type(next_token);

                //gets the program name
                prog_name = parser->get_token(current_line-1,current_token-3);

                symbol_table->set_value(to_uppercase(parser->get_token(current_line-1,current_token-3)),equ_directive(next_token));
        }
        else if (token == "END"){
                if(next_token != to_uppercase(prog_name) && next_token != "")
                        throw opcode_error_exception("END operand must be program name.");
                return -1;
        }
        else if (token == "BYTE"){
                return byte_directive(next_token);
        }
        else if (token == "WORD"){
                return 3;
        }
        else if (token == "RESB"){
                return operand_type(next_token);
        }
        else if (token == "RESW"){
                return operand_type(next_token) * 3;
        }
        else if (token == "BASE"){
                try{
                        string t = next_token;
                        if(is_label(t))
                                base = t;
                        else
                                base = equ_directive(t);
                }
                catch(symtab_exception e){
                        cout << e.getMessage() << endl;
                        //cout << base << endl;
                }

                return 0;
        }
        else if (token == "NOBASE"){
                base = "0";
                return string_to_int(base);
        }
        else if (token == "EQU"){
                try{
                        //sets the value of the label to the next token as opposed to the
                        //address of the current line
                        symbol_table->set_value(to_uppercase(parser->get_token(current_line-1,current_token-3)),equ_directive(next_token));
                }
                catch(symtab_exception e){
                        cout << e.getMessage() << endl;
                }
        }

        return 0;
}

int sicxe_asm::operand_type(string s){
        string val;
        s = to_uppercase(s);

        //$ mean HEX in this architecture, handle for this
        if (s[0] == '$'){
                s = s.substr(1);
                return hex_to_int(s);
        }

        //# is an immediate value, handle for this
        else if (s[0] == '#'){
                s = s.substr(1);

                if (isalpha(s[0])){
                        try{
                                val = symbol_table->get_value(s);
                                if(is_label(val))
                                        return 0;
                                else
                                        return string_to_int(val);

                                // return symbol_table->get_value(s);
                        }
                        catch(symtab_exception e){
                                return 0;
                        }
                }

                //there can be a $ after the #
                else if(s[0] == '$'){
                        return operand_type(s);
                }

                return dec_to_int(s);
        }

        //This is wrong, needs to go to any address not just label
        else if (s[0] == '@'){
                s = s.substr(1);
                try{
                        val = symbol_table->get_value(s);
                        if(is_label(val))
                                return 0;
                        else
                                return string_to_int(val);
                }
                catch(symtab_exception e){
                        return 0;
                }
        }

        //check for a label in the operand field and get its value
        else if (isalpha(s[0])){
                try{
                        val = symbol_table->get_value(s);

                        if(is_label(val))
                                return 0;
                        else
                        {
                                return string_to_int(val);
                        }
                }
                catch(symtab_exception e){
                        //cout << e.getMessage() << endl;
                        return 0;
                }
        }
        else{
                return dec_to_int(s);
        }
        return 0;
}

string sicxe_asm::equ_directive(string s){
        int tmp = 0;
        //$ mean HEX in this architecture, handle for this
        if (s[0] == '$'){
                s = s.substr(1);
                return s;
        }

        //# is an immediate value, handle for this
        else if (s[0] == '#'){
                s = s.substr(1);

                if (isalpha(s[0])){
                        try{
                                return symbol_table->get_value(s);
                        }
                        catch(symtab_exception e){
                                return s;
                        }
                }

                //there can be a $ after the #
                else if(s[0] == '$'){
                        return equ_directive(s);
                }
                tmp = dec_to_int(s);
                return int_to_hex(tmp,6);
        }

        else if (s[0] == '@'){
                s = s.substr(1);
                try{
                return symbol_table->get_value(s);
                }
                catch(symtab_exception e){
                        return s;
                }
        }

        //check for a label in the operand field and get its value
        else if (isalpha(s[0])){
                try{
                return symbol_table->get_value(s);
                }
                catch(symtab_exception e){
                        return s;
                }
        }
        else{
                tmp = dec_to_int(s);
                s = int_to_hex(tmp,6);
        }
        return s;
}

int sicxe_asm::byte_directive(string s){
        if(s[0] == 'X'){
                if(((s.size() - 3) % 2) != 0){
                        throw opcode_error_exception("Hexadecimal literal " + s + " length must be even.");
                }
                return ( (int)s.size() - 3)/2;
        }
        else if(s[0] == 'C'){
                return (int)s.size()-3;
        }
        else{
                return (int)s.size();
        }
        return -1;
}

//*******************************************************
//                      OBJECT FILE METHODS
//*******************************************************

void sicxe_asm::output_obj_file(string f){

        //make the file in the current directory
        obj_name = make_obj_file(f);
        ofstream object_file(obj_name.c_str());
        print_obj_contents(object_file);
}

string sicxe_asm::make_obj_file(string f){
        string name;

        //get the filename up until the period
        for(unsigned int i = 0; i < f.size();i++){
                if(f[i] == '.'){
                        break;
                }

                name += f[i];
        }

        //concatinate the extension onto the filename
        name = name + ".obj";
        return name;
}

void sicxe_asm::print_obj_contents(ofstream &object_file){

        //prints all records to object file
        print_header_record(object_file);
        print_text_record(object_file);
        print_mod_record(object_file);
        print_end_record(object_file);
}

void sicxe_asm::print_header_record(ofstream &object_file){
        stringstream sstr;
        string start_address;
        string header_prog_name = prog_name;

        //if the program name is less than 6 characters, add spaces to the end
        //of the name
        if(sizeof(prog_name) != 6){
                int add_num_zeros = 6 - sizeof(prog_name);
                for(int i=0; i < add_num_zeros; i++)
                        header_prog_name += " ";
        }

        //get the value of the starting address
        start_address = symbol_table -> get_value(to_uppercase(prog_name));

        //if the starting address is not equal to 6 characters, add zeros in front
        if(start_address.size() != 6){
                int add_zeros = 6 - start_address.size();
                for(int i=0; i < add_zeros; i++)
                        start_address = "0" + start_address;
        }

        //find how long the program is - find the program length
        int end_int = hex_to_int(list_file_vec.at(list_file_vec.size()-1).address);
        int start_int = hex_to_int(start_address);
        int tmp = end_int - start_int;
        string prog_length = int_to_hex(tmp,6);


        //format the what is going to be put into the object file, and put it into
        //the file
        sstr << setw(1) << setfill(' ') << "H";
        sstr << setw(6) << setfill(' ') << to_uppercase(header_prog_name);
        sstr << setw(6) << setfill('0') << start_address;
        sstr << setw(6) << setfill('0') << prog_length;
        sstr << "\n";
        object_file << sstr.str();
        sstr.clear();
        sstr.str(string());
}

void sicxe_asm::print_text_record(ofstream &object_file){
        stringstream sstr;
        string start_address, end_address, text_length, mac_code, opcd;
        int end_int, start_int, length_int;
        unsigned int i=0;
        bool not_the_bottom = true, add_i;

        //if the opcode is not "END" - the end of the program, keep going
        while(to_uppercase(list_file_vec.at(i).opcode) != "END"){

                //get the first starting address value
                if(to_uppercase(list_file_vec.at(i).opcode) == "START"){
                        start_address = symbol_table -> get_value(to_uppercase(prog_name));

                        //if the address is not equal to 6 characters, add zeros to the front
                        if(start_address.size() != 6){
                                int add_zeros = 6 - start_address.size();
                                for(int i=0; i < add_zeros; i++)
                                start_address = "0" + start_address;
                        }
                }

                else
                        //get the starting address of each text record after "START"
                        start_address = list_file_vec.at(i).address;

                add_i = false;

                //if these are not the asm directives at the bottom of the file, loop through them
                if(not_the_bottom){
                        while(is_asm_directive(to_uppercase(list_file_vec.at(i).opcode)))
                                i++;

                        not_the_bottom = false;
                }

                while(to_uppercase(list_file_vec.at(i).opcode) != "END") {
                        //as long as the machine code is not null and the opcode is not an asm directive...
                        if((list_file_vec.at(i).machine_code != "") &&
                                (!is_asm_directive(list_file_vec.at(i).opcode))){

                                //if the machine code already taken for the text record and the next machine
                                //code to be taken make the length of the mc part of the text record greater
                                //than 60 characters, don't take the next machine code
                                if((mac_code.size() + (list_file_vec.at(i).machine_code).size()) > 60){
                                        break;
                                }

                                //add the machine code from the list file to the machine code for
                                //the text record
                                mac_code += list_file_vec.at(i).machine_code;
                        }

                        //if the opcode is an asm directive...
                        if(is_asm_directive(list_file_vec.at(i).opcode)){
                                opcd = list_file_vec.at(i).opcode;
                                opcd = to_uppercase(opcd);

                                //if the machine code is equal to to, flag add_i to increment
                                //later when appropriate and break out of the second while loop
                                if(list_file_vec.at(i).machine_code == ""){
                                        add_i = true;
                                        break;
                                }

                                //add the machine code of the asm directive to the machine code
                                //for the text record
                                mac_code += list_file_vec.at(i).machine_code;
                        }

                        i++;
                }

                //get the ending address for the current text record
                end_address = list_file_vec.at(i).address;

                end_int = hex_to_int(end_address);
                start_int = hex_to_int(start_address);

                //find the length of the current text record
                length_int = end_int - start_int;
                text_length = int_to_hex(length_int, 2);

                //as long as the machine code is not equal to null...
                if(mac_code != ""){
                        //if the machine code is not of length 60, add spaces to the end
                        if(mac_code.size() != 60){
                                int add_spaces = 60 - mac_code.size();
                                for(int j=0; j < add_spaces; j++)
                                        mac_code += " ";
                        }

                        //format the what is going to be put into the object file, and put it into
                        //the file
                        sstr << setw(1) << setfill('0') << "T";
                        sstr << setw(6) << setfill('0') << start_address;;
                        sstr << setw(2) << setfill('0') << text_length;
                        sstr << setw(60) << setfill(' ') << mac_code;
                        sstr << "\n";
                        object_file << sstr.str();
                        sstr.clear();
                        sstr.str(string());
                }

                //if i needs to be incremented before going back into the while loop do so
                if(add_i)
                        i++;

                //reset the machine code
                mac_code = "";

        }

}

void sicxe_asm::print_mod_record(ofstream &object_file){
        stringstream sstr;
        string op, start_address;
        int i_mod_add;

        //go through every list file opcode and look for one that is format 4
        for(unsigned int i=0; i<list_file_vec.size()-1; i++){
                op = list_file_vec.at(i).opcode;

                //if the opcode is not an asm directive and it is not equal to null...
                if(!is_asm_directive(to_uppercase(op)) && op != ""){
                        //if the instuction size of the opcode is 4...
                        if((opcode_table -> get_instruction_size(to_uppercase(op))) == 4){
                                //get the starting address
                                start_address = list_file_vec.at(i).address;
                                //add one to the starting address
                                i_mod_add = hex_to_int(start_address) + 1;
                                start_address = int_to_hex(i_mod_add,6);

                                //format the what is going to be put into the object file, and put it into
                                //the file
                                sstr << setw(1) << setfill(' ') << "M";
                                sstr << setw(6) << setfill('0') << start_address;
                                sstr << setw(2) << setfill('0') << "05";
                                sstr << "\n";
                                object_file << sstr.str();
                                sstr.clear();
                                sstr.str(string());
                        }
                }
        }
}

void sicxe_asm::print_end_record(ofstream &object_file){
        stringstream sstr;
        unsigned int i=0;
        string add_first_ex;

        //skip past the beginning asm directives
        while(is_asm_directive(to_uppercase(list_file_vec.at(i).opcode)))
                i++;

        //get the address of the first execution
        add_first_ex = list_file_vec.at(i).address;

        //format the what is going to be put into the object file, and put it into
        //the file
        sstr << setw(1) << setfill(' ') << "E";
        sstr << setw(6) << setfill('0') << add_first_ex;
        sstr << "\n";
        object_file << sstr.str();
        sstr.clear();
        sstr.str(string());
}

//*******************************************************
//                      CONVERSION METHODS
//*******************************************************

//Provided by Professor

int sicxe_asm::string_to_int(string s){
        istringstream instr(s);
        int n;
        instr >> n;
        return n;
}

string sicxe_asm::int_to_hex(int num, int width){
        stringstream out;
        out << setw(width) << setfill('0') << hex << num;
        return to_uppercase(out.str());
}

int sicxe_asm::hex_to_int(string s){
        int value;
        sscanf(s.c_str(),"%x",&value);
        return value;
}

int sicxe_asm::dec_to_int(string s){
        int value;
        sscanf(s.c_str(),"%d",&value);
        return value;
}

string sicxe_asm::to_uppercase(string s){
        transform(s.begin(),s.end(),s.begin(),::toupper);
        return s;
}

string sicxe_asm::format_15(int x) {
        stringstream tmp;
        tmp << hex << setw(8) << setfill('0') << x;
        string xx = tmp.str();
        stringstream tmmp;
        tmmp << setw(15) << xx;
        return tmmp.str();
}

string sicxe_asm::hex_to_bin(string hex){
        string sReturn = "";
        for (unsigned int i = 0; i < hex.length (); ++i)
        {
                switch (hex [i])
                {
                        case '0': sReturn.append ("0000"); break;
                        case '1': sReturn.append ("0001"); break;
                        case '2': sReturn.append ("0010"); break;
                        case '3': sReturn.append ("0011"); break;
                        case '4': sReturn.append ("0100"); break;
                        case '5': sReturn.append ("0101"); break;
                        case '6': sReturn.append ("0110"); break;
                        case '7': sReturn.append ("0111"); break;
                        case '8': sReturn.append ("1000"); break;
                        case '9': sReturn.append ("1001"); break;
                        case 'A': sReturn.append ("1010"); break;
                        case 'B': sReturn.append ("1011"); break;
                        case 'C': sReturn.append ("1100"); break;
                        case 'D': sReturn.append ("1101"); break;
                        case 'E': sReturn.append ("1110"); break;
                        case 'F': sReturn.append ("1111"); break;
                }
        }
        return sReturn;
}

string sicxe_asm::bin_to_hex(string binary){
        string rest,tmp;
        for (unsigned int i=0;i<binary.length();i+=4)
        {
                tmp = binary.substr(i,4);
                if (!tmp.compare("0000")) rest = rest + "0";
                else if (!tmp.compare("0001")) rest = rest + "1";
                else if (!tmp.compare("0010")) rest = rest + "2";
                else if (!tmp.compare("0011")) rest = rest + "3";
                else if (!tmp.compare("0100")) rest = rest + "4";
                else if (!tmp.compare("0101")) rest = rest + "5";
                else if (!tmp.compare("0110")) rest = rest + "6";
                else if (!tmp.compare("0111")) rest = rest + "7";
                else if (!tmp.compare("1000")) rest = rest + "8";
                else if (!tmp.compare("1001")) rest = rest + "9";
                else if (!tmp.compare("1010")) rest = rest + "A";
                else if (!tmp.compare("1011")) rest = rest + "B";
                else if (!tmp.compare("1100")) rest = rest + "C";
                else if (!tmp.compare("1101")) rest = rest + "D";
                else if (!tmp.compare("1110")) rest = rest + "E";
                else if (!tmp.compare("1111")) rest = rest + "F";
                else
                {
                        continue;
                }
        }
        return rest;
}

string sicxe_asm::bool_to_string(bool x){
        if(x) return "1";
        return "0";
}

string sicxe_asm::int_to_string(int n){
        stringstream ss;
        ss << n;
        return ss.str();
}

string sicxe_asm::string_to_ascii(string s){
        string ascii_str = "";
        for (unsigned int i = 0; i < s.length (); ++i)
        {
                switch (s[i])
                {
                        case 'a': ascii_str.append ("61"); break;
                        case 'b': ascii_str.append ("62"); break;
                        case 'c': ascii_str.append ("63"); break;
                        case 'd': ascii_str.append ("64"); break;
                        case 'e': ascii_str.append ("65"); break;
                        case 'f': ascii_str.append ("66"); break;
                        case 'g': ascii_str.append ("67"); break;
                        case 'h': ascii_str.append ("68"); break;
                        case 'i': ascii_str.append ("69"); break;
                        case 'j': ascii_str.append ("6A"); break;
                        case 'k': ascii_str.append ("6B"); break;
                        case 'l': ascii_str.append ("6C"); break;
                        case 'm': ascii_str.append ("6D"); break;
                        case 'n': ascii_str.append ("6E"); break;
                        case 'o': ascii_str.append ("6F"); break;
                        case 'p': ascii_str.append ("70"); break;
                        case 'q': ascii_str.append ("71"); break;
                        case 'r': ascii_str.append ("72"); break;
                        case 's': ascii_str.append ("73"); break;
                        case 't': ascii_str.append ("74"); break;
                        case 'u': ascii_str.append ("75"); break;
                        case 'v': ascii_str.append ("76"); break;
                        case 'w': ascii_str.append ("77"); break;
                        case 'x': ascii_str.append ("78"); break;
                        case 'y': ascii_str.append ("79"); break;
                        case 'z': ascii_str.append ("7A"); break;

                        case 'A': ascii_str.append ("41"); break;
                        case 'B': ascii_str.append ("42"); break;
                        case 'C': ascii_str.append ("43"); break;
                        case 'D': ascii_str.append ("44"); break;
                        case 'E': ascii_str.append ("45"); break;
                        case 'F': ascii_str.append ("46"); break;
                        case 'G': ascii_str.append ("47"); break;
                        case 'H': ascii_str.append ("48"); break;
                        case 'I': ascii_str.append ("49"); break;
                        case 'J': ascii_str.append ("4A"); break;
                        case 'K': ascii_str.append ("4B"); break;
                        case 'L': ascii_str.append ("4C"); break;
                        case 'M': ascii_str.append ("4D"); break;
                        case 'N': ascii_str.append ("4E"); break;
                        case 'O': ascii_str.append ("4F"); break;
                        case 'P': ascii_str.append ("50"); break;
                        case 'Q': ascii_str.append ("51"); break;
                        case 'R': ascii_str.append ("52"); break;
                        case 'S': ascii_str.append ("53"); break;
                        case 'T': ascii_str.append ("54"); break;
                        case 'U': ascii_str.append ("55"); break;
                        case 'V': ascii_str.append ("56"); break;
                        case 'W': ascii_str.append ("57"); break;
                        case 'X': ascii_str.append ("58"); break;
                        case 'Y': ascii_str.append ("59"); break;
                        case 'Z': ascii_str.append ("5A"); break;
                }
        }
        return ascii_str;
}

//*******************************************************
//                      MAIN
//*******************************************************

int main(int argc, char* argv[]){
        if(argc < 2){
                cout << "Please enter a filename with .asm extension" << endl;
                return 0;
        }
        else if(argc > 2){
                cout << "Only 1 filename at a time" << endl;
                return 0;
        }

        sicxe_asm *assembler = new sicxe_asm();

        assembler->run(argv[1]);

        return 0;
}