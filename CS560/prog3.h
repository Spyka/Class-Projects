/*
	CS 560
	Summer 2014
	Assignment 1 - #3
	prog3.h

	Note: The word array is put into "" in comments because 
	vectors are used instead of actual arrays
*/

#ifndef PROG_3_H
#define PROG_3_H

#include <string>
#include <vector>
#include <cstdlib>
#include <ctype.h>
#include <stdio.h>
#include <iostream>

using namespace std;

class prog3{

	private:

		//values needed for prompt
		string str_n;
		int input_n;

		int n; 			//size of the array 
		int k;			//random number to find
		int y;			//number of compares
		int arr_element;	//for the elements in the generated "array"

		//vector for "array" generated by the input
		//asked for from the user - input_n
		//and an iterator to print the values
		vector<int> generated_arr;
		vector<int>::iterator itr;		

		//various pre-given "arrays"
		vector<int> arr;

		//needed for binary search
		int vec_max;
		int min;
		int max;
		int mid_point;


	public:

		//asks the user to input a number for the size of the
		//generated "array"
		void prompt();

		//generate an "array" based off the input given by the user
		void gen_ran_array(int);

		//generate pre-given "arrays"
		void gen_arrays();

		//generate a random k to be found inside the "arrays"
		int gen_ran_k(vector<int>);

		//binary search the "array" for the random k
		//if k is not found, give the value closest to k
		//if k is larger than the largest element in the array,
		//state that, and give the largest element in the array
		//if k is in the array, give the number of comparisons 
		//done to reach k
		int bin_search(vector<int>, int, int, int);

};

#endif