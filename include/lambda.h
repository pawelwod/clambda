/*
 * This file is provided under a GPLv2 license.
 *
 * Copyright (c) 2018 Pawel Wodkowski <pawelwod 'at' gmail.com>
 */

#ifndef LAMBDA_H_
#define LAMBDA_H_

/**
 * Return pointer to lambda function of \c _ret_type and params \c _params.
 * Function will have their bode defined using \c _body parameter.  Please note
 * that last semicolon in \c _body is added automaticaly so it is required.
 *
 * Usage:
 *
 * Print some text:
 * \code{.c}
 * lambda(void, (const char *str), printf("%s", str) )("Hello World!");
 * \endcode
 *
 * This is equivalent of: *
 * \code{.c}
 * void some_fn(const char *str) {
 *     printf("%s", str);
 * }
 *
 * some_fn("Hello World!")
 * \endcode
 *
 * Using lambda to sort some array:
 * \code{.c}
 * char buf[] = "981532467";
 * qsort(buf, strlen(buf), 1,
 *		lambda(int, (const void *a , const void *b),
 *					const char *_a = a, *_b = b;
 *					return *_a - *_b;
 *			)
 *	);
 * \endcode
 *
 * Using nested lambda to sort some array:
 * \code{.c}
 * char buf[] = "981532467";
 * qsort(buf, strlen(buf), 1,
 *		lambda(int, (const void *a , const void *b),
 *	           return lambda(int, (const char *a , const char *b), return *a - *b )(a, b)
 *	    )
 *	);
 * \endcode
 *
 * \param _ret_type Return type of lambda function
 * \param _params This is the round bracets part of function deffinition. Put
 *                all parameters there.
 * \param _body This is the body of lambda function including any \c return statements.
 */
#define lambda(_ret_type, _params, _body...) ({ \
	_ret_type _lambda_fn _params { _body; }; \
	_lambda_fn; \
})

/**
 * Declare and assign variable holding lambda expression.
 *
 * Usage:
 *
 * Print some text:
 * \code{.c}
 * lambda_var(the_printer, void, (const char *str), printf("%s", str) );
 *
 * the_printer("Hello World!");
 * \endcode
 *
 * Using lambda to sort some array:
 * \code{.c}
 * char buf[] = "981532467";
 * lambda_var(cmp_fn, int, (const char *a , const char *b), return *a - *b )
 * qsort(buf, strlen(buf), 1, lambda(int, (const void *a , const void *b), return cmp_fn(a, b));
 * \endcode
 *
 * \param _lambda_name Name of variable
 * \param _ret_type Return type of lambda function
 * \param _params This is the round bracets part of function deffinition. Put
 *                all parameters there.
 * \param _body This is the body of lambda function including any \c return statements.
 */
#define lambda_var(_lambda_name, _ret_type, _params, _body...) \
	__typeof( lambda(_ret_type, _params, _body) ) _lambda_name = lambda(_ret_type, _params, _body)

/*
 * TODO: macro for array of lambdas.
 */

#endif /* LAMBDA_H_ */
