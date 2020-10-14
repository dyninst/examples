#include <stdlib.h>
#include <stdio.h>

static int interestInt;
static int noninterestInt;

void InterestingProcedure()
{
	interestInt++;
	printf("Greetings from intereting procedure %d !\n", interestInt);
}

void justAnotherProcedure()
{
	noninterestInt++;
        printf("Greetings from just another procedure %d !\n", noninterestInt);

}

int main()
{
	interestInt = 0;
	noninterestInt = 0;

	for (int i = 0; i < 5; i++) {
		InterestingProcedure();
		justAnotherProcedure();
	}
}
