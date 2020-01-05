#include "stats.h"

node *sequences;
int seq_size;

int main(void) {

	sequences = NULL;
	seq_size = 0;

	char* input;
	int selection;

	do {

		printf("[1] Add / Delete sequences\n");
		printf("[2] Modify sequence specifiers\n");
		printf("[3] Run simulation\n");
		printf("[0] Exit program\n");
		printf("Enter selection: ");

		input = get_msg('\0');
		selection = get_selection(input);

		switch (selection) {

			case 0: break;

			case 1: mod_seqs(); break;

			case 2: mod_specs(); break;

			case 3: simulation(); break;

			default:
				if ((input[0] | ' ') == 'q')
					selection = 0;
				else
					printf("\tInvalid selection\n");			
		}

		free(input);
		putchar('\n');

	} while (selection);

	free_sequences();

	return 0;
}

bool print_sequences(void) {

	if (sequences == NULL) {
		printf("\tNo sequences have been entered\n");
		return false;
	}

	int printed = 0;
	node *p = sequences;

	while (p != NULL) {

		printed += 1;
		printf("Sequence #%d - Size[%d]\n{ ", printed, p->size);

		for (int i = 0; i < p->size; i++)
			printf("%d ", p->sequence[i]);
		puts("}");

		print_spec(p->spec);

		putchar('\n');

		p = p->next;
	}

	return true;
}

void print_spec(specs* spec) {

	printf("Specifications: ");

	if (spec == NULL) {
		printf("N/A\n");
		return;
	}

	printf("-a%d%s-e", spec->amount, spec->consecutive ? " -c " : " ");

	if (spec->equal) putchar('=');
	if (spec->greater) putchar('>');
	if (spec->less) putchar('<');

	if (spec->only != DEFAULT_ONLY)
		printf(" -o%d", spec->only);

	printf("%s-s%d\n", spec->print ? " -p " : " ", spec->sample);
}

void free_sequences(void) {

	node *p = sequences;

	while (sequences != NULL) {
		sequences = p->next;
		free_seq(p);
		p = sequences;
	}

	sequences = NULL;
}

void free_seq(node* seq) {

	free(seq->sequence);
	free(seq->spec);
	free(seq);

	seq_size--;
}

int get_selection(const char* input) {

	int selection = 0;

	for (int i = 0; input[i]; i++) {

		if (!IS_NUM(input[i]))
			return -1;

		selection *= 10;
		selection += input[i] & 0xf;
	}

	return selection;
}

char* get_msg(char delim) {

	int current_size = 1;
	int index_size = 1;
	char temp;
	char *text = (char *)malloc(sizeof(char) * 2);

	if (text == NULL) {
		printf("\nA critical error has occured. The program will now exit\n\n");
		exit(1);
	}

	*text = getc(stdin);

	do {

		temp = getc(stdin);

		if (temp == '\n' && (!delim || text[index_size - 1] == delim)) {

			temp = '\0';

			if (delim)
				--index_size;
		}

		else {
			current_size++;
			text = (char *)realloc(text, (current_size + 1) * sizeof(char));

			if (text == NULL) {
			printf("\nA critical error has occured. The program will now exit\n\n");
				exit(1);
			}
		}

		text[index_size++] = temp;

	} while (temp);

	return text;
}
