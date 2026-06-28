#include <stdio.h>

/**
 * main - prints its name, followed by a new line
 * @argc: the number of command line arguments
 * @argv: an array containing the program command line arguments
 *
 * Return: Always 0 (Success)
 */
int main(int argc __attribute__((unused)), char *argv[])
{
	printf("%s\n", argv[0]);
	return (0);
}
