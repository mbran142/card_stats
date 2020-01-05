#include "stats.h"

extern node* sequences;
extern int seq_size;

void simulation(void) {

	int size;
	int* to_test = get_nums(&size);
	puts("-~~~~~~~~~~~~~~~-");

	if (to_test == NULL)
		return;

	node* p = sequences;
	int prev = 1;

	for (int i = 0; i < size; i++) {

		for (int j = 0; j < to_test[i] - prev; j++)
			p = p->next;

		if (p->spec == NULL)
			printf("Sequence #%d has no specifications - simulation canceled\n", to_test[i]);
		
		else run_simulation(p);

		puts("-~~~~~~~~~~~~~~~-");

		prev = to_test[i];
	}

	free(to_test); 
}

void run_simulation(node* sequence) {

	int* seq = (int *)malloc(sizeof(int) * sequence->size);
	long long simulations = 0;
	long long passed = 0;
	int temp = seq_size;
	seq_size = sequence->size;

	if (!seq) {
		printf("\nA critical error has occured. The program will now exit\n\n");
		exit(1);
	}

	for (int i = 0; i < sequence->size; i++)
		seq[i] = sequence->sequence[i];

	//creating an array for the amount of required rotations
	int* rotations = (int *)malloc(sizeof(int) * sequence->size);

	if (!rotations) {
		printf("\nA critical error has occured. The program will now exit\n\n");
		exit(1);
	}

	rotations[0] = 1;

	for (int i = 1, rot = 1; i < sequence->size; i++) {

		if (seq[i] != seq[i - 1])
			rot++;

		rotations[i] = rot;
	}

	bool done = false;
	int scope = 0;
	int num = 1;

	//case in which every number in the sequence is the same
	if (rotations[sequence->size - 1] == 1) {
		passed += check(seq, sequence->spec, num);
		simulations++;
		done = true;
	}

	while (!done) {

		//get new scope
		scope = 1;

		while (seq[0] == seq[scope]) 
			scope++;

		if (scope == 1) {
			while (seq[1] == seq[scope])
				scope++;
			scope--;
		}

		for (int i = 0; i <= scope; i++) {
			passed += check(seq, sequence->spec, num++);
			simulations++;
			rotate_one(seq, scope);
		}

		scope++;
		while (!(--rotations[scope])) {

			rotate(seq, scope);
			scope++;

			if (scope == sequence->size) {
				done = true;
				break;
			}
		}

		//reseting scope resolutions
		if (!done) {
			rotate(seq, scope);

			for (int i = 1, rot = 1; i < scope; i++) {

				if (seq[i] != seq[i - 1])
					rot++;

				rotations[i] = rot;
			}
		}
	}

	free(rotations);
	free(seq);

	//printing results
	if (sequence->spec->print)
		putchar('\n');

	printf("Sequence: ");

	for (int i = 0; i < sequence->size; i++)
		printf("%d ", sequence->sequence[i]);

	putchar('\n');

	reduce_fractions(passed, simulations);
	printf("\t\tUnreduced: %lld/%lld\n", passed, simulations);
	printf("Percent: %.2lf%%\n", ((double)passed/(double)simulations) * 100);

	seq_size = temp;
}

int check(int* seq, specs* spec, int num) {

	int count = 0;
	int maxCount = 0;

	if (!spec->consecutive) {

		for (int j = 0; j < (spec->sample - (spec->amount - 1)); j++) {

    		for (int h = j; h < spec->sample; h++) {

        		if (seq[j] == seq[h] && (spec->only == DEFAULT_ONLY || spec->only == seq[j]))
        		    count++;

			}

    maxCount = count > maxCount ? count : maxCount;
    count = 0;
		}
	}

	else {

		count =  1;
		maxCount = 1;

		for (int j = 0; j < spec->sample - 1; j++) {

			if (seq[j] == seq[j + 1] && (spec->only == DEFAULT_ONLY || spec->only == seq[j]))
				count++;
			
			else {
				maxCount = count > maxCount ? count : maxCount;
				count = 1;
			}
		}

		maxCount = count > maxCount ? count : maxCount;
	}

	bool check = (maxCount == spec->amount && spec->equal == 1)
				|| (maxCount < spec->amount && spec->less == 1)
				|| (maxCount > spec->amount && spec->greater == 1);

	if (spec->print == 1) {

		printf("%6d - | ", num);

		for (int i = 0; i < seq_size; i++) {

			printf("%d ", seq[i]);

			if (i == spec->sample - 1)
				printf("| ");
		}

		if (check)
			putchar('<');
		putchar('\n');
	}

return check ? 1 : 0;

}
 
void rotate_one(int* seq, int max) {

		int	temp = seq[max];

		for (int i = max - 1; i >= 0; i--)
			seq[i + 1] = seq[i];

		seq[0] = temp;
}

void rotate(int* seq, int max) {

	int num = 1;
	int temp;

	for (int i = max - 1; seq[i] == seq[max] && i >= 0; i--)
		num++;

	while (num) {

		temp = seq[max];

		for (int i = max - 1; i >= 0; i--)
			seq[i + 1] = seq[i];

		seq[0] = temp;
		num--;
	}
}

void reduce_fractions(long long numerator, long long denominator) {

	if (numerator == 0)
		denominator = 1;

	for (int i = 2; i <= numerator; i++) {
		if (numerator % i == 0 && denominator % i == 0) {
			numerator /= i;
			denominator /= i--;
		}
	}

	printf("Rate: %lld/%lld", numerator, denominator);
}