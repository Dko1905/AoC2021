#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#define BITS_PER_LINE ((size_t)12)

/* Convert bits to an unsigned 32-bit int. */
static uint32_t to_decimal(bool bits[BITS_PER_LINE]);

struct Node {
	bool bits[BITS_PER_LINE];
	struct Node *prev, *next;
};

/* Split head into two new lists, the oxygen list contains the majority
 * elements, while co2 contains the minority, according to the rules.
 * Return elements in oxygen list. */
static size_t filter(struct Node *head, struct Node **oxygen,
                     struct Node **co2, size_t pos);

int main(void)
{
	struct Node *head = NULL, *tail = NULL, *cur = NULL;
	struct Node *oxygen_head = NULL, *co2_head = NULL;
	int oxygen_dec = 0, co2_dec = 0;

	/* Allocate head of list. */
	head = calloc(1, sizeof(*head));
	if (head == NULL) {
		fprintf(stderr, "Failed to allocate: %s\n", strerror(errno));
		return 1;
	}
	cur = head;

	/* Read lines of bits into nodes with bits. */
	while (true) {
		int c = 0;

		for (size_t i = 0; (c = getchar()) != EOF; i++) {
			/* Read bits from input into current node. */
			switch (c) {
			case '0':
				cur->bits[i] = 0;
				break;
			case '1':
				cur->bits[i] = 1;
				break;
			case '\n':
				if (i != BITS_PER_LINE) {
					fprintf(stderr, "Not enough bits %zu/%zu\n",
					        i, BITS_PER_LINE);
					return 1;
				}
				i = 0;
				goto break_for;
			default:
				fprintf(stderr, "Invalid input \'%c\'\n", c);
				return 1;
			}
		}

		/* Append node to tail. */
		break_for:
		if (tail != NULL) {
			cur->prev = tail;
			tail->next = cur;
		}
		tail = cur;

		/* Quit of EOF */
		if (c == EOF) {
			break;
		} else if ((c = getchar()) == EOF) {
			break;
		}
		ungetc(c, stdin);

		/* Allocate new current node. */
		cur = calloc(1, sizeof(*cur));
		if (cur == NULL) {
			fprintf(stderr, "Failed to allocate: %s\n",
			        strerror(errno));
			return 1;
		}
	}

	/* Split into oxygen and co2. */
	filter(head, &oxygen_head, &co2_head, 0);
	head = NULL; /* Head is garbage. */

	/* Filter oxygen. */
	for (size_t i = 1; i < BITS_PER_LINE; i++) {
		struct Node *tmp = NULL;

		filter(oxygen_head, &oxygen_head, &tmp, i);
		/* If tmp/co2 is empty, we are done. */
		if (tmp == NULL) {
			break;
		}
		/* Free tmp/co2. */
		cur = tmp;
		for (struct Node *next = NULL; cur != NULL; cur = next) {
			next = cur->next;
			memset(cur, 0, sizeof(*cur));
			free(cur);
		}
	}

	/* Filter co2. */
	for (size_t i = 1; i < BITS_PER_LINE; i++) {
		struct Node *tmp = NULL;

		filter(co2_head, &tmp, &co2_head, i);
		/* If co2_head is null, the result is in tmp. */
		if (co2_head == NULL) {
			co2_head = tmp;
			break;
		}
		cur = tmp;
		for (struct Node *next = NULL; cur != NULL; cur = next) {
			next = cur->next;
			memset(cur, 0, sizeof(*cur));
			free(cur);
		}
	}

	/* Print results. */
	if (oxygen_head->next != NULL) {
		fprintf(stderr, "Invalid input (oxygen), result has more than 1 node\n");
		return 1;
	}
	if (co2_head->next != NULL) {
		fprintf(stderr, "Invalid input (co2), result has more than 1 node\n");
		return 1;
	}
	oxygen_dec = to_decimal(oxygen_head->bits);
	co2_dec = to_decimal(co2_head->bits);
	printf("oxygen: %i\tco2: %i\n", oxygen_dec, co2_dec);
	printf("result: %i\n", oxygen_dec * co2_dec);

	/* Free. */
	free(oxygen_head);
	free(co2_head);
}

// int main(void)
// {
// 	struct Node *root = NULL, *prev = NULL, *cur = NULL;
// 	struct Node *oxygen = NULL, *co2 = NULL;
// 	int oxygen_dec = 0, co2_dec = 0;
// 	bool run = true;
// 	int c = 0;

// 	/* Init root. */
// 	root = calloc(sizeof(*root), 1);
// 	if (root == NULL) {
// 		fprintf(stderr, "Failed to allocate: %s\n", strerror(errno));
// 		return 1;
// 	}
// 	cur = root;

// 	/* Read all lines into list. */
// 	while (run) {
// 		/* Read one line. */
// 		for (size_t i = 0; (c = getchar()) != EOF; i++) {
// 			switch (c) {
// 			case '0':
// 				cur->bits[i] = 0;
// 				break;
// 			case '1':
// 				cur->bits[i] = 1;
// 				break;
// 			case '\n':
// 				if (i != BITS_PER_LINE) {
// 					fprintf(stderr, "Not enough data %zu/%zu\n",
// 					        i, BITS_PER_LINE);
// 					return 1;
// 				}
// 				goto break_for;
// 			default:
// 				fprintf(stderr, "Invalid input \'%c\'\n", c);
// 				return 1;
// 			}
// 		}
// 		break_for:
// 		/* Stop when if EOF or if next is. */
// 		if (c == EOF)
// 			run = false;
// 		else if ((c = getchar()) == EOF)
// 			run = false;
// 		else
// 			ungetc(c, stdin);
// 		/* Append cur. */
// 		if (prev != NULL) {
// 			cur->prev = prev;
// 			prev->next = cur;
// 		}
// 		/* Allocate new cur. */
// 		if (run) {
// 			prev = cur;
// 			cur = calloc(sizeof(*cur), 1);
// 			if (cur == NULL) {
// 				fprintf(stderr, "Failed to allocate: %s", strerror(errno));
// 				return 1;
// 			}
// 		}
// 	}

// 	/* Filter. */
// 	oxygen = root;
// 	struct Node *oxygen_last = oxygen, *co2_last = co2;

// 	/* Split into two groups. */
// 	co2 = filter(&oxygen, 0);

// 	/* Filter out oxygen. */
// 	for (size_t i = 1; i < BITS_PER_LINE; i++) {
// 		struct Node *tmp;
// 		printf("Input len: %zu\n", get_len(oxygen));
// 		tmp = filter(&oxygen, i);
// 		printf("Output len: %zu %zu = %zu\n", get_len(tmp), get_len(oxygen), get_len(tmp) + get_len(oxygen));
// 		if (tmp == NULL) {
// 			break;
// 		}
// 		/* Free unused list. */
// 		for (cur = tmp; cur != NULL;) {
// 			tmp = cur;
// 			cur = cur->next;
// 			memset(tmp, 0, sizeof(*tmp));
// 			free(tmp);
// 		}
// 	}

// 	/* Filter out co2. */
// 	for (size_t i = 1; i < BITS_PER_LINE; i++) {
// 		struct Node *tmp = co2;
// 		printf("Input len: %zu\n", get_len(tmp));
// 		co2 = filter(&tmp, i);
// 		printf("Output len: %zu %zu = %zu\n", get_len(co2), get_len(tmp), get_len(co2) + get_len(tmp));
// 		if (co2 == NULL) {
// 			co2 = tmp;
// 			break;
// 		}
// 		/* Free unused list. */
// 		for (cur = tmp; cur != NULL;) {
// 			tmp = cur;
// 			cur = cur->next;
// 			memset(tmp, 0, sizeof(*tmp));
// 			free(tmp);
// 		}
// 	}


// 	/* Print and free. */
// 	if (oxygen->next != NULL) {
// 		fprintf(stderr, "BUG: oxygen has more than 1 element\n");
// 		return 1;
// 	}
// 	oxygen_dec = to_decimal(oxygen->bits);

// 	if (co2->next != NULL) {
// 		fprintf(stderr, "BUG: co2 has more than 1 element\n");
// 		return 1;
// 	}
// 	co2_dec = to_decimal(co2->bits);

// 	printf("oxygen: %i\tco2: %i\n", oxygen_dec, co2_dec);
// 	printf("result: %i\n", oxygen_dec * co2_dec);

// 	free(oxygen);
// 	free()

// 	return 0;
// }

static size_t filter(struct Node *head, struct Node **oxygen,
                     struct Node **co2, size_t pos)
{
	struct Node *oxygen_head = NULL, *oxygen_tail = NULL;
	struct Node *co2_head = NULL, *co2_tail = NULL;
	bool prevelant = 0;
	int count0 = 0, count1 = 0;
	size_t n = 0; /* Elements in oxygen list. */

	/* Count 1's and 0's. */
	for (struct Node *cur = head; cur != NULL; cur = cur->next) {
		if (cur->bits[pos] == 1) {
			count1++;
		} else {
			count0++;
		}
	}

	/* Find most prevelant bit. */
	prevelant = count1 >= count0;

	/* Move all numbers with prevelant bit to oxygen list, and the rest to
	 * the co2 list. */
	for (struct Node *cur = head, *next = NULL; cur != NULL; cur = next) {
		next = cur->next;

		if (cur->bits[pos] != prevelant) {
			struct Node *dup = cur;

			if (co2_tail == NULL) {
				dup->next = NULL;
				dup->prev = NULL;
				co2_tail = dup;
				co2_head = co2_tail;
			} else {
				dup->next = NULL;
				dup->prev = co2_tail;
				co2_tail->next = dup;
				co2_tail = dup;
			}
		} else {
			struct Node *dup = cur;

			if (oxygen_tail == NULL) {
				dup->next = NULL;
				dup->prev = NULL;
				oxygen_tail = dup;
				oxygen_head = oxygen_tail;
			} else {
				dup->next = NULL;
				dup->prev = oxygen_tail;
				oxygen_tail->next = dup;
				oxygen_tail = dup;
			}

			n++;
		}
	}

	*oxygen = oxygen_head;
	*co2 = co2_head;

	return n;
}

static struct Node *filter2(struct Node **root, size_t pos)
{
	struct Node *cur = NULL;
	struct Node *oxygen = NULL, *oxygen_prev = NULL;
	struct Node *co2 = NULL, *co2_prev = NULL;
	int count0 = 0, count1 = 0, most = 0, special = 0;

	/* Count 1st row */
	cur = *root;
	while (cur != NULL) {
		if (cur->bits[pos] == 0) {
			count0++;
		} else {
			count1++;
		}
		cur = cur->next;
	}
	//TODO: This does not qualify
	special = count1 == count0;
	most = count1 >= count0;

	/* Move all correct numbers to oxygen, and all incorrect to co2. */
	cur = *root;
	while (cur != NULL) {
		struct Node *tmp = cur->next;

		if (cur->bits[pos] != most || special) {
			struct Node *cur2 = cur;
			if (special) {
				struct Node *dup = malloc(sizeof(*dup));
				memcpy(dup, cur2, sizeof(*dup));
				cur2 = dup;
			}
			if (co2_prev == NULL) {
				cur2->prev = NULL;
				cur2->next = NULL;
				co2_prev = cur2;
				co2 = co2_prev;
			} else {
				cur2->prev = co2_prev;
				cur2->next = NULL;
				co2_prev->next = cur2;
				co2_prev = cur2;
			}
		}
		if (cur->bits[pos] == most) {
			if (oxygen_prev == NULL) {
				cur->prev = NULL;
				cur->next = NULL;
				oxygen_prev = cur;
				oxygen = oxygen_prev;
			} else {
				cur->prev = oxygen_prev;
				cur->next = NULL;
				oxygen_prev->next = cur;
				oxygen_prev = cur;
			}
		}

		cur = tmp;
	}

	*root = oxygen;
	return co2;
}

static uint32_t to_decimal(bool bits[BITS_PER_LINE]) {
	uint32_t num = 0, base = 1;

	for (size_t i = 0; i < BITS_PER_LINE; i++) {
		num = num + bits[BITS_PER_LINE - i - 1] * base;
		base = base * 2;
	}

	return num;
}
