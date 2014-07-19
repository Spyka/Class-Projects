/*
	CS 560
	Summer 2014
	Assignment 1 - #3
	prog3.cpp

	Note: The word array is put into "" in comments because 
	vectors are used instead of actual arrays
*/

#include "prog3.h"

using namespace std;

void prog3::prompt(){

	//ask the user to enter a number to be used for the size of
	//a later generated "array"
	cout << "Please enter an 'n' to be used: " << endl;
	getline(cin, str_n);

	//if the user does not enter a number, use 15 by default
	if(str_n.empty())
		str_n = "15";

	//make sure what is entered included only positive integers
	//if not, end the program
	for(unsigned int i=0; i<str_n.size(); i++){

		if(!isdigit(str_n[i]))
		{
			cout << "Input must be positive integers only. ";
			cout << "Program Terminating... Please Restart" << endl;
			exit(0);
		}
	}

	//change the input from a string into an integer and generate the
	//"array"
	input_n = atoi(str_n.c_str());
	gen_ran_array(input_n);

	//generate the given "arrays" for the assignment
	cout << endl << "Now Running 6 Specified Arrays" << endl;
	gen_arrays();
}

void prog3::gen_ran_array(int input_n){

	arr_element = 0;
	srand(time(NULL));

	//generate random numbers and put them into an 
	//ordered "array"
	for(unsigned int i=0; i<input_n; i++){
		arr_element = rand() % input_n + arr_element;
		generated_arr.push_back(arr_element);
	}

	cout << endl << "ARRAY GENERATED WITH " << input_n << " ELEMENTS" << endl;

	//print the generated "array"
	itr = generated_arr.begin();
	while(itr != generated_arr.end()){
		cout << *itr << " ";
		advance(itr,1);
	}

	cout << endl;

	//generate a random k to be found in the "array"
	k = gen_ran_k(generated_arr);
	min = 0;
	max = generated_arr.size();
	y = 0;

	//binary search the "array"
	bin_search(generated_arr, k, min, max); 
}

void prog3::gen_arrays(){
	/*
		Now generate array elements A[i] = i, for i=1,2,…, n. Generate 6 such 
		arrays of sizes n = 250, 500, 1000, 2000, 4000, 8000. 
	*/

	//Then for each "array" perform a binary search for a randomly
	//generated k

	int size = 250;
	while (size <= 8000){
		for(unsigned int i=1; i<=size; i++){
			arr.push_back(i);
		}
		cout << size << " ARRAY" << endl;
		k =  gen_ran_k(arr);
		min = 0;
		max = arr.size();
		y = 0;
		bin_search(arr, k, min, max);
		size *= 2;
	}
}

int prog3::gen_ran_k(vector<int> vec){
/*
	In each case 
	let  K = rand(1, 2*n) where the rand produces a random number 
	between 1 and 2*n.
*/

	n = vec.size();
	srand(time(NULL));
	k = rand() % (2*n) + 1;  	// adding 1 makes it between 1 and 2n
	return k;
}

int prog3::bin_search(vector<int> vec, int k, int min, int max){

	//y is the number of comparisons done
	y++;
	vec_max = vec.size() - 1;

	//if k is greater than the largest value in the "array", state that and
	//state the largest value in the "array"
	if (k > vec[vec_max]){
		cout << "k: " << k << " is greater than max: " << vec[vec_max] << endl;
		cout << "Therefore, the closest element is: " << vec[vec_max] << endl;
		return vec[vec_max];
	}

	//if k is not found in the "array" find the element closest to k
	if(y>1){
		if ((vec[mid_point] < k) && (k < vec[mid_point+1])){
			cout << "k not found, but..." << endl;

			//find whether the midpoint element or the following element
			//is closer to k
			int btm_val = k - vec[mid_point];
			int top_val = (vec[mid_point+1]) - k;

			//display which elemnt is closest to k
			if(btm_val < top_val){
				cout << "element: " << vec[mid_point] << " closest to k: " << k << endl;
				return vec[mid_point];
			}
			else{
				cout << "element: " << vec[mid_point+1] << " closest to k: " << k << endl;
				return vec[mid_point+1];
			}
		}
	}

	if (max < min){
		cout << "error" << endl;
		return 0;
	}

	else{

		//find the midpoint for each pass through bin search
		//it must be added to the min in order to get the proper
		//position
		mid_point = (((max-min)/2) + min);

		//if k is a smaller value than the midpoint element
		//search from that midpoint down
		if(k < vec[mid_point]){
			return bin_search(vec, k, min, mid_point);
		}

		//if k is a larger value than the midpoint element
		//search from that midpoint up
		else if(k > vec[mid_point]){
			return bin_search(vec, k, mid_point, max);
		}

		//if k is found return y
		else{
				cout << "K: " << k << " found." << endl;
				cout << "Y: " << y << endl;
				return vec[mid_point];
		}
	}
}

int main(){
	prog3 start;
	start.prompt();
	exit(1);
}