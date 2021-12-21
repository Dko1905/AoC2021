#include <stdio.h>
#include <stdint.h>

#define BITS_PER_LINE ((size_t)12)

static int to_decimal(unsigned char bits[BITS_PER_LINE]);

int main(void)
{
	unsigned char gamma[BITS_PER_LINE] = {0}, epsilon[BITS_PER_LINE] = {0};
	int count0[BITS_PER_LINE] = {0}, count1[BITS_PER_LINE] = {0};
	size_t i = 0;
	int c = 0;

	/* Go through input. */
	while ((c = getchar()) != EOF) {
		/* Catch stack overflow. */
		if (i >= BITS_PER_LINE && c != '\n') {
			fprintf(stderr, "More bits than expected %zu/%zu\n",
			        i + 1, BITS_PER_LINE);
			return 1;
		}

		switch (c) {
		case '0':
			count0[i]++;
			break;
		case '1':
			count1[i]++;
			break;
		case '\n':
			i=0;
			continue;
		default:
			fprintf(stderr, "Unknown charecter \'%c\'\n", c);
			return 1;
		}

		i++;
	}

	/* Combine bits into number. */
	for (i = 0; i < BITS_PER_LINE; i++) {
		if (count0[i] > count1[i]) {
			gamma[i] = 0;
			epsilon[i] = 1;
		} else if (count1[i] > count0[i]) {
			gamma[i] = 1;
			epsilon[i] = 0;
		} else {
			fprintf(stderr, "Even amount of bits\n");
			return 1;
		}
	}

	/* Print results. */
	printf("Gamma: %i\tEpsilon: %i\n", to_decimal(gamma), to_decimal(epsilon));
	printf("Power consumption: %i\n", to_decimal(gamma) * to_decimal(epsilon));

	return 0;
}

static int to_decimal(unsigned char bits[BITS_PER_LINE]) {
	int num = 0, base = 1;

	for (size_t i = 0; i < BITS_PER_LINE; i++) {
		num = num + bits[BITS_PER_LINE - i - 1] * base;
		base = base * 2;
	}

	return num;
}
