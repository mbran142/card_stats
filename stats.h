#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEFAULT_AMOUNT 0
#define DEFAULT_CONSECUTIVE false
#define DEFAULT_EQUAL true
#define DEFAULT_GREATER true
#define DEFAULT_LESS false
#define DEFAULT_ONLY -1
#define DEFAULT_PRINT false
#define DEFAULT_SAMPLE 0

#define IS_NUM(i) (i >= '0' && i <= '9')

typedef struct Specifications {
	int amount;
	bool consecutive;
	bool equal;
	bool greater;
	bool less;
	int only;
	bool print;
	int sample;
} specs;

typedef struct Sequence_Node {
	int* sequence;
	specs* spec;
	int size;
	struct Sequence_Node* next;
} node;

//Gets dynamically allocated user input (must be freed)
char* get_msg(char);

//Extracts an integer from user input. Returns -1 if unsuccessful
int get_selection(const char*);

//Frees list of sequences
void free_sequences(void);

//Frees one sequence
void free_seq(node*);

//Prints all sequences in the list
bool print_sequences(void);

//Prints specifications for each sequence
void print_spec(specs*);

//Modify the list of sequences
void mod_seqs(void);

//Prints help for sequence modification
void seq_help(void);

//Adds a sequence to a list. Returns true or false based on if input is valid 
int add_seq(char*);

//Deletes a sequence from the list. Returns true or false based on if input is valid
int del_seq(char*);

//Creates a sequence struct with default spec based on an integer array input
void generate_seq(int*, int);

//Sorts an array of integers
void sort(int*, int);

//Modify specifcations in the list
void mod_specs(void);

//Prints help for getting user input for numbers of sequences to choose
void num_help(void);

//Prints help for inputing specifications
void spec_help(void);

//Gets and processes user input for which sequences to modify
int* get_nums(int*);

//Gets specifications from user input
specs* get_specs(void);

//Applies specs to a sequence
void apply_specs(node*, specs*, int);

//Prepares simulation to be run
void simulation(void);

//Runs a simulation
void run_simulation(node*);

//Rotates a part of the sequence
void rotate(int*, int);

//Rotates one time
void rotate_one(int*, int);

//Checks if a sequence fits the spec
int check(int*, specs*, int);

//Reduces and prints a fraction
void reduce_fractions(long long x, long long y);