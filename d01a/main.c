#include <stdio.h>

#define NA_MSG "(N/A - no previous measurement)"
#define EQL_MSG "(no change)"
#define INC_MSG "(increased)"
#define DEC_MSG "(decreased)"

int main(void)
{
	/* Previous- & current measurement, count of longer lines. */
	int prev = -1, cur = 0, longer = 0;

	/* Handle first element explicitly. */
	if (1 > scanf("%i", &cur))
		return 0;
	printf("%i " NA_MSG, cur);
	putchar('\n');
	prev = cur;

	/* While available items. */
	while (0 < scanf("%i", &cur)) {
		if (cur > prev) {
			printf("%i " INC_MSG, cur);
			longer++;
		} else if (prev > cur) {
			printf("%i " DEC_MSG, cur);
		} else {
			printf("%i " EQL_MSG, cur);
		}

		prev = cur;
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

