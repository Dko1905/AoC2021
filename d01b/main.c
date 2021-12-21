#include <stdio.h>

#define NA_MSG "(N/A - no previous measurement)"
#define EQL_MSG "(no change)"
#define INC_MSG "(increased)"
#define DEC_MSG "(decreased)"

int main(void)
{
	int a = 0, b = 0, c = 0;
	int sum = 0, cur = 0, prev = 0, longer = 0;

	/* Handle first 3 elements explicitly. */
	if (1 > scanf("%i", &a))
		return 0;
	if (1 > scanf("%i", &b))
		return 1;
	if (1 > scanf("%i", &c))
		return 1;
	cur = a;
	prev = a + b + c;
	printf("%i " NA_MSG, cur);
	putchar('\n');

	/* While available items. */
	while (0 < scanf("%i", &cur)) {
		a = b;
		b = c;
		c = cur;
		sum = a + b + c;

		if (sum > prev) {
			printf("%i " INC_MSG, a);
			longer++;
		} else if (prev > sum) {
			printf("%i " DEC_MSG, a);
		} else {
			printf("%i " EQL_MSG, a);
		}

		prev = sum;
		putchar('\n');
	}

	/* Print summery. */
	if (longer > 1 || longer == 0)
		printf("%i measurements are longer than previous", longer);
	else
		printf("%i measurement is longer than previous", longer);
	putchar('\n');

	return 0;
}

