#include "stats.h"

extern node* sequences;
extern int seq_size;

void mod_specs(void) {

	putchar('\n');
	if (!print_sequences())
		return;

	int size;
	int* to_modify = get_nums(&size);

	if (to_modify == NULL)
		return;

	specs* new_spec = get_specs();

	if (new_spec == NULL) {
		free(to_modify);
		return;
	}

	putchar('\n');

	int prev = 1;
	node* p = sequences;
 
 	//applying input specifications to each selected sequence
	for (int i = 0; i < size; i++) {

		for (int j = 0; j < to_modify[i] - prev; j++)
			p = p->next;

		apply_specs(p, new_spec, to_modify[i]);

		prev = to_modify[i];
	}

	if (new_spec->equal && new_spec->greater && new_spec->less)
		printf("\tWarning: \'-e\' Requested equality [=, <, >] will yield 100%% check rate\n");

	else if (!new_spec->equal && !new_spec->greater && !new_spec->less)
		printf("\tWarning: \'-e\' Requested equality will yield 0%% check rate\n");

	free(to_modify);
	free(new_spec);
}

void apply_specs(node* p, specs* new_spec, int seq_index) {

	free(p->spec);
	p->spec = (specs *)malloc(sizeof(specs));

	if (!p->spec) {
		printf("\nA critical error has occured. The program will now exit\n\n");
		exit(1);
	}

	//amount
	int temp = 1;
	int max = 0;

	for (int i = 1; i < p->size; i++) {

		if (p->sequence[i - 1] == p->sequence[i])
			temp++;

		else {

			if (temp > max)
				max = temp;

			temp = 0;
		}
	}

	if (temp > max)
		max = temp;

	if (new_spec->amount == DEFAULT_AMOUNT)
		p->spec->amount = max;

	else {

		if (new_spec->amount > max)
			printf("\tWarning: \'-a\' Requested amount greater than sequence #%d\'s max amount\n", seq_index);

		p->spec->amount = new_spec->amount;
	}

	//consecutive
	p->spec->consecutive = new_spec->consecutive;

	//three equality modifers
	p->spec->equal = new_spec->equal;
	p->spec->greater = new_spec->greater;
	p->spec->less = new_spec->less;

	//only
	temp = 0;

	if (new_spec->only == DEFAULT_ONLY)
		p->spec->only = DEFAULT_ONLY;

	else {

		for (int i = 0; i < p->size; i++) 
			if (p->sequence[i] == new_spec->only) {
				temp = 1;
				break;
			}

		if (!temp)
			printf("\tWarning: \'-o\' Requested number for only does not exist in sequence #%d\n", seq_index);

	p->spec->only = new_spec->only;

	}

	//print
	p->spec->print = new_spec->print;

	//sample
	if (new_spec->sample == DEFAULT_SAMPLE)
		p->spec->sample = p->size;

	else if (new_spec->sample > p->size) {
		printf("\tWarning: \'-s\' Requested sample greater than sequence #%d\'s size. -s set to %d\n", seq_index, p->size);
		p->spec->sample = p->size;
	}

	else
		p->spec->sample = new_spec->sample;
}

specs* get_specs() {

	char* input;
	int valid, i, temp;
	bool done = false;
	specs* new_spec = (specs *)malloc(sizeof(specs));

	if (!new_spec) {
		printf("\nA critical error has occured. The program will now exit\n\n");
		exit(1);
	}

	printf("Add new specifications [enter \"-h\" for help]\n\n");

	do {

		i = 0;
		valid = 1;

		//default specifications if they're not explicitly changed by user
		new_spec->amount = DEFAULT_AMOUNT; //sentinel value for max amount of one value
		new_spec->consecutive = DEFAULT_CONSECUTIVE;
		new_spec->equal = DEFAULT_EQUAL;
		new_spec->greater = DEFAULT_GREATER;
		new_spec->less = DEFAULT_LESS;
		new_spec->only = DEFAULT_ONLY; // sentinel value for no only
		new_spec->print = DEFAULT_PRINT;
		new_spec->sample = DEFAULT_SAMPLE; //sentinel value for sample size = sequence->size

		printf("\tEnter input: ");
		input = get_msg('\0');

		while (input[i] == ' ' || input[i] == '\n')
			i++;

		if ((input[i] == '0' && !input[i + 1])
			|| ((input[i] | ' ') == 'q') 
			|| ((input[i] | ' ') == 'b'))
			valid = 2;

		else while (input[i]) {

			if (input[i] == '-') {

				i++;

				switch(input[i]) {

					case 'a':

						while (input[i] == ' ')
							i++;

						new_spec->amount = 0;

						i++;
						while (IS_NUM(input[i])) {
							new_spec->amount *= 10;
							new_spec->amount += input[i] & 0xf;
							i++;
						}
						i--;

						if (!new_spec->amount)
							valid = 0;

						break;

					case 'c':
						new_spec->consecutive = true;
						break;

					case 'e': 

						while (input[i] == ' ')
							i++;

						new_spec->equal = false;
						new_spec->greater = false;
						new_spec->less = false;

						i++;

						while (input[i] == '=' || input[i] == '>' || input[i] == '<') {

							if (input[i] == '=') new_spec->equal = true;
							else if (input[i] == '>') new_spec->greater = true;
							else new_spec->less = true;

							i++;
						}
						i--;

						break;

					case 'h': 
						spec_help();
						valid = 3;
						break;

					case 'o':

						while (input[i] == ' ')
							i++;

						temp = i;
						new_spec->only = 0;
						i++;

						while (IS_NUM(input[i])) {
							new_spec->only *= 10;
							new_spec->only += input[i] & 0xf;
							i++;
						}

						if (temp == i)
							valid = 0;

						i--;

						break;

					case 'p':
						new_spec->print = true;
						break;

					case 's':

						while (input[i] == ' ')
							i++;

						new_spec->sample = 0;

						i++;
						while (IS_NUM(input[i])) {
							new_spec->sample *= 10;
							new_spec->sample += input[i] & 0xf;
							i++;
						}
						i--;

						if (!new_spec->sample)
							valid = 0;

						break;

					default: valid = 0;
				}
			}

			else {
				valid = 0;
				break;
			}

			i++;

			if (input[i] != ' ' && input[i] != ',' && input[i]) {
				valid = 0;
				break;
			}

			if (valid != 1) break;

			while (input[i] == ' ' || input[i] == ',' ||input[i] == '\n')
				i++;
		}

		free(input);

		switch (valid) {
			case 0: printf("\t\tInvalid input; please enter specifications\n\n"); break;
			case 1: done = true; break;
			case 2:
				done = true;
				free(new_spec);
				new_spec = NULL;
				break;

			default: ; 
		}

	} while (!done);

	return new_spec;
}

int* get_nums(int* size) {

	int valid, i, num_specs;
	bool done = false;
	char* input;
	int* to_modify;
	int* final_modify;

	if (seq_size ==  1) {
		final_modify = (int *)malloc(sizeof(int));
		*size = 1;
		*final_modify = 1;
		return final_modify;
	}

	printf("Choose which sequences to use [enter \"-h\" for help]\n\n");

	do {

		num_specs = 0;
		i = 0;		
		to_modify = NULL;
		final_modify = NULL;

		printf("\tEnter input: ");
		input = get_msg('\0');

		while (input[i] == ' ' || input[i] == '\n')
			i++;

		if (input[i] == '-') {

			switch(input[i + 1]) {

				case 'a':

					final_modify = (int *)malloc(sizeof(int) * seq_size);

					if (!final_modify) {
						printf("\nA critical error has occured. The program will now exit\n\n");
						exit(1);
					}

					num_specs = seq_size;

					for (int i = 0; i < num_specs; i++)
						final_modify[i] = i + 1;

					*size = num_specs;

					valid = 1;
					break;

				case 'h':
					num_help();
					valid = 2;
					break;

				default: valid = 0;
			}
		}

		else if ((input[i] == '0' && !input[i + 1])
			|| ((input[i] | ' ') == 'q') 
			|| ((input[i] | ' ') == 'b'))
			valid = 1;

		else if (IS_NUM(input[i])) {

			valid = 1;
			bool expect_comma = false;

			while (input[i]) {

				if (IS_NUM(input[i])) {

					if (expect_comma) {
						valid = 0;
						break;
					}

					int num = 0;

					while (IS_NUM(input[i])) {
						num *= 10;
						num += input[i] & 0xf;
						i++;
					}
					i--;

					if (num <= 0 || num > seq_size) {
						valid = 0;
						break;
					}

					num_specs++;
					expect_comma = true;
				}

				else if (input[i] == ',') {

					if (!expect_comma) {
						valid = 0;
						break;
					}

					expect_comma = false;
				}

				else if (input[i] != ' ') {
					valid = 0;
					break;
				}

				i++;
			}

			if (valid == 1) {

				to_modify = (int *)malloc(sizeof(int) * num_specs);

				if (!to_modify) {
					printf("\nA critical error has occured. The program will now exit\n\n");
					exit(1);
				}

				i = 0;

				for (int j = 0; input[i]; i++) {

					if (IS_NUM(input[i])) {

						int num = 0;

						while (IS_NUM(input[i])) {
							num *= 10;
							num += input[i] & 0xf;
							i++;
						}
						i--;

						to_modify[j++] = input[i] & 0xf;
					}

				}

				sort(to_modify, num_specs);

				*size = 1;

				for (int i = 1; i < num_specs; i++)
					if (to_modify[i - 1] != to_modify[i])
						*size += 1;

				final_modify = (int *)malloc(sizeof(int) * *size);

				if (!final_modify) {
					printf("\nA critical error has occured. The program will now exit\n\n");
					exit(1);
				}

				final_modify[0] = to_modify[0];

				for (int i = 1, j = 0; i < num_specs; i++)
					if (to_modify[i] != final_modify[j])
						final_modify[++j] = to_modify[i];

				free(to_modify);
			}
		}

		else valid = 0;

		free(input);

		switch (valid) {
			case 0: printf("\t\tInvalid input; please enter sequence ID #s\n\n"); break;
			case 1: done = true; break;
			default: ; 
		}

	} while (!done);

	return final_modify;
}

void num_help(void) {

	printf("\nNumber input\n");
	printf("-a: Select all sequences\n");
	printf("Otherwise, enter a comma-separated list of sequence IDs\n");
	printf("\t+ If an ID that does not exist is entered, input is considered invalid\n");
	printf("\t+ Sequence IDs are indexed starting at 1\n");
	printf("Enter '0', 'q', or 'b' to go back\n\n");
}

void spec_help(void) {

	printf("\nSpecification options\n");
	printf("-a: Amount - The set amount of times a number needs to appear to be checked. [Default = max]\n");
	printf("-c: Consecutive - Requires that numbers must be in order to be counted. [Default = false]\n");
	printf("-e: Equality - Enter \'=\', \'<\', or \'>\' to change check equality condition. [Default = \'=\']\n");
	printf("-o: Only - Will only check the number specified. [Default = false]\n");
	printf("-p: Print - Will print every possibility from the simulation. [Default = false]\n");
	printf("-s: Samples - Only the first sample of numbers will be checked in the sequence. [Default = sequence size]\n\n");
}