/* File: 6-is_prime_number.c */
#include "main.h"

int check_prime(int n, int i);

/**
 * is_prime_number - checks if an integer is a prime number.
 * @n: the number to check
 *
 * Return: 1 if n is prime, 0 otherwise
 */
int is_prime_number(int n)
{
	if (n <= 1)
	{
		return (0);
	}
	return (check_prime(n, 2));
}

/**
 * check_prime - helper function to check divisibility recursively
 * @n: the number to check
 * @i: the current divisor iterator
 *
 * Return: 1 if n is prime, 0 if divisible
 */
int check_prime(int n, int i)
{
	if (i * i > n)
	{
		return (1);
	}
	if (n % i == 0)
	{
		return (0);
	}
	return (check_prime(n, i + 1));
}
