/*
 * This file is provided under a GPLv2 license.
 *
 * Copyright (c) 2018 Pawel Wodkowski <pawelwod 'at' gmail.com>
 *
 */

/*
 * Tests for lambda C implementation.
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/lambda.h"

#define SORTED_TEXT  "0123456789"
#define SHUFFLED_TEXT "7629451083"

int main(void)
{
	char buf[32];

	/* Env check. */
	assert(sizeof(SORTED_TEXT) == sizeof(SHUFFLED_TEXT));
	assert(sizeof(buf) >= sizeof(SORTED_TEXT));

	/* Check if empty lambda can compile and invoke. */
	lambda(void, (void),
	)();

	/* Check if lambda can accedd local variable. */
	strcpy(buf, SORTED_TEXT);
	lambda(void, (void),
			assert(strcmp(buf, SORTED_TEXT) == 0)
	)();

	/* Check if lambda take parameter. */
	lambda(void, (const char *str),
			assert(strcmp(str, SORTED_TEXT) == 0)
	)(SORTED_TEXT);

	/* Check if we can declare some variable in lambda.*/
	lambda(void, (const char *str),
			char buf[] = SHUFFLED_TEXT;
			assert(strcmp(str, buf) == 0)
	)(SHUFFLED_TEXT);

	/* Basic test to invoke some function from lambda. */
	lambda(void, (const char *str), printf("%s", str) )("Hello World!\n");

	/* Check if lambda can be used as function pointer. */
	strcpy(buf, SHUFFLED_TEXT);
	qsort(buf, strlen(buf), 1,
			lambda(int, (const void *a , const void *b),
					const char *_a = a, *_b = b;
					return *_a - *_b;
			)
	);
	assert(strcmp(buf, SORTED_TEXT) == 0);

	/* Check if lambdas can be nested*/
	strcpy(buf, SHUFFLED_TEXT);
	qsort(buf, strlen(buf), 1,
			lambda(int, (const void *a , const void *b),
					return lambda(int, (const char *a, const char *b),
							return *a - *b
					)(a, b)
			)
	);
	assert(strcmp(buf, SORTED_TEXT) == 0);

	/* Check if we can make variable of lambda and can use it.. */
	lambda_var(cmp1, int, (const char *a , const char *b),
			return *a - *b
	);

	lambda_var(cmp2, int, (const void *a , const void *b),
			return cmp1(a, b)
	);

	strcpy(buf, SHUFFLED_TEXT);
	qsort(buf, strlen(buf), 1,
			lambda(int, (const void *a , const void *b), return cmp2(a, b) )
	);
	assert(strcmp(buf, SORTED_TEXT) == 0);

	printf("CLAMBDA: all tests passed.\n");
	return EXIT_SUCCESS;
}
