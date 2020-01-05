#include "stats.h"

extern node* sequences;
extern int seq_size;

void mod_seqs(void) {

	int valid, i;
	int done = 0;

	do {

		i = 0;

		printf("\tEnter input or \"-h\" for help: ");
		char* input = get_msg('\0');

		while (input[i] == ' ' || input[i] == '\n')
			i++;

		if (input[i] == '-') {

			switch(input[i + 1]) {

				case 'a':
					valid = add_seq(input + i + 2);
					break;

				case 'd':
					free_sequences();
					valid = 1;
					break;

				case 'h':
					seq_help();
					valid = 2;
					break;

				default: valid = 0;
			}
		}

		else if (input[i] == '0' && !input[i + 1])
			valid = 1;

		else if (IS_NUM(input[i]))
			valid = add_seq(input + i);

		else if (((input[i] | ' ') == 'q') 
				|| ((input[i] | ' ') == 'b'))
			valid = 1;

		else
			valid = 0;

		free(input);

		switch(valid) {
			case 0: printf("\t\tInvalid input\n\n"); break;
			case 1: done = 1; break;
			default: ;
		}

	} while (!done); 
}

void seq_help(void) {

	printf("\nCommands\n");
	printf("-a: Add new sequence with positive integers\n");
	printf("\t+ Two integer input (eg. \"9 x 2\" for nine IDs with two copies each\n");
	printf("\t+ Quick list input (eg. \"1[5] 2[3] 3[1]\" for five 1's, three 2's, and one 3\n");
	printf("\t+ Exact list input (eg. \"1, 1, 1, 1, 2\" for four 1's and one 2\n");
	printf("-d: Delete sequence\n");
	printf("-h: Print help\n");
	printf("Enter '0', 'q', or 'b' to go back\n\n");
}

int add_seq(char* input) {

	int valid = 0;
	register int index = 0;
	bool has_mult, has_bracket, has_comma;
	has_mult = has_bracket = has_comma = false;
	
	//checks which type of input this is
	while (input[index]) {

		switch (input[index]) {

			case 'x': 
			case 'X':
			case '*': has_mult = true; break;

			case '[':
			case ']': has_bracket = true; break;

			case ',': has_comma = true; break;

			default: ;
		}

		index++;
	}

	//if more than 1 could be possible, it's invalid
	if (has_mult) valid++;
	if (has_bracket) valid++;
	if (has_comma) valid++;
	if (valid != 1)
		return 0;

	index = 0;
	int size;
	int *seq = NULL;

	//"9 x 2" option
	if (has_mult) {

		int nums = 0;
		int amnt = 0;
		valid = 0;
		
		if (!IS_NUM(input[index]) && input[index] != ' ')
			return 0;

		//skip spaces
		while (input[index] == ' ')
			index++;

		if (!IS_NUM(input[index]))
			return 0;

		//extract integer
		while (IS_NUM(input[index])) {
			nums *= 10;
			nums += input[index] & 0xf;
			index++;
		}

		//skip spaces and check for mult symbol. If there is not exactly one, input is invalid
		while (input[index] == ' ' || (input[index] | ' ') == 'x' || input[index] == '*') {

			if ((input[index] | ' ') == 'x' || input[index] == '*')
				valid++;

			index++;
		}

		if (valid != 1)
			return 0;

		if (!IS_NUM(input[index]))
			return 0;

		//extracts integer
		while (IS_NUM(input[index])) {
			amnt *= 10;
			amnt += input[index] & 0xf;
			index++;
		}

		//if either is 0, input is invalid
		if (!nums || !amnt)
			return 0;

		//creating array of these integers
		size = nums * amnt;
		seq = (int *)malloc(sizeof(int) * size);

		if (!seq) {
			printf("\nA critical error has occured. The program will now exit\n\n");
			exit(1);
		}

		for (int i = 0; i < nums; i++) 
			for (int j = 0; j < amnt; j++)
				seq[(i * amnt) + j] = i + 1;

		generate_seq(seq, size);
	}

	//"1[5] 2[3]..." option
	if (has_bracket) {

		size = 0;
		int num, amnt;

		if (!IS_NUM(input[index]) && input[index] != ' ')
			return 0;

		while (input[index] == ' ')
			index++;

		while (input[index]) {

			while (IS_NUM(input[index]))
				index++;

			if (input[index] != '[')
				return 0;
			index++;

			if (!IS_NUM(input[index]))
				return 0;

			//extract amount of each number
			amnt = 0;
			while (IS_NUM(input[index])) {
				amnt *= 10;
				amnt += input[index] & 0xf;
				index++;
			}
			size += amnt;

			if (input[index] != ']')
				return 0;
			index++;

			while (input[index] == ' ')
				index++;
		}

		seq = (int *)malloc(sizeof(int) * size);

		if (!seq) {
			printf("\nA critical error has occured. The program will now exit\n\n");
			exit(1);
		}

		int seq_index = 0;
		index = 0;

		while (input[index] == ' ')
			index++;

		while (input[index]) {

			num = 0;
			while (IS_NUM(input[index])) {
				num *= 10;
				num += input[index] & 0xf;
				index++;
			}

			//skip '[' bracket
			index++;

			amnt = 0;
			while (IS_NUM(input[index])) {
				amnt *= 10;
				amnt += input[index] & 0xf;
				index++;
			}

			for (int i = 0; i < amnt; i++) {
				seq[seq_index] = num;
				seq_index++;
			}
			
			//skip ']' bracket
			index++;

			while (input[index] == ' ')
				index++;
		}

		generate_seq(seq, size);
	}

	//"1, 1, 1, 2..." option
	if (has_comma) {

		//getting amount of numbers and check for extraneous characters
		size = 1;
		while (input[index]) {

			if (!(input[index] == ' ' || IS_NUM(input[index]))) {

				if (input[index] == ',')
					size++;
				else
					return 0;
			}

			index++;
		}

		seq = (int *)malloc(sizeof(int) * size);

		if (!seq) {
			printf("\nA critical error has occured. The program will now exit\n\n");
			exit(1);
		}

		int seq_index = 0;
		index = 0;
		int num;

		while (input[index]) {

			while (input[index] == ' ')
				index++;

			if (!IS_NUM(input[index])) {
				free(seq);
				return 0;
			}

			num = 0;
			while (IS_NUM(input[index])) {
				num *= 10;
				num += input[index] & 0xf;
				index++;
			}

			seq[seq_index] = num;
			seq_index++;

			while (input[index] == ' ')
				index++;

			if (seq_index == size) {
				if (input[index])
					return 0;
			}

			else if (input[index] != ',') {
				free(seq);
				return 0;
			}

			else index++;
		}

		generate_seq(seq, size);
	}

	seq_size++;
	return 1;
}

void generate_seq(int* seq, int size_in) {

	node* new = (node *)malloc(sizeof(node));

	if (!new) {
		printf("\nA critical error has occured. The program will now exit\n\n");
		exit(1);
	}

	new->sequence = seq;
	new->spec = NULL;
	new->size = size_in;
	new->next = NULL;

	sort(seq, size_in);

	if (sequences == NULL)
		sequences = new;

	else {

		node *p = sequences;

		while (p->next != NULL)
			p = p->next;

		p->next = new;
	}
}

void sort(int* arr, int size) {

	for (int i = 1, j, temp; i < size; i++) {

		temp = arr[i];

		for (j = i - 1; j >= 0 && arr[j] > temp; j--)
			arr[j + 1] =  arr[j];

		arr[j + 1] = temp;
	}
}