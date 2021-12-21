#include <stdio.h>
#include <string.h>

#define CMD_MAX_LEN 20
#define CMD_MAX_LEN_STR "20"

int main(void)
{
	char cmd[CMD_MAX_LEN] = {0};
	/* x is horizontal position, y is depth/vertical position. */
	int x = 0, y = 0, aim = 0, c = 0;

	/* Scan each input. */
	while (1 < scanf("%" CMD_MAX_LEN_STR "s %i", cmd, &c)) {
		if (0 == strcmp("up", cmd)) {
			aim -= c;
		} else if (0 == strcmp("down", cmd)) {
			aim += c;
		} else if (0 == strcmp("forward", cmd)) {
			x += c;
			y -= aim * c;
		} else {
			fprintf(stderr, "Unknown command \"%s\"\n", cmd);
			return 1;
		}
	}

	/* Print summary. */
	printf("x: %i\ty: %i\taim: %i\n", x, y, aim);
	printf("hpos: %i\tdepth: %i\n", x, -y);
	printf("result: %i\n", x * -y);

	return 0;
}

