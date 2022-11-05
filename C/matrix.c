#include <stdio.h>

void print_matrix(char s[], double m[], size_t rows, size_t cols)
{
	printf("%s = \n", s);
	size_t i = 0;
	while (i < rows)
	{
		size_t j = 0;
		while (j < cols)
		{
			printf("%4g", m[i*cols+j]);
			++j;
		}
		printf("\n");
		++i;
	}
}

void transpose(double m[], size_t rows, size_t cols, double r[])
{
	size_t i = 0;
	while (i < rows)
	{
		size_t j = 0;
		while (j < cols)
		{
			r[j*rows+i] = m[i*cols+j];
			++j;
		}
		++i;
	}
}

void add(double m1[], double m2[], size_t rows, size_t cols, double r[])
{
	size_t i = 0;
	while (i < rows*cols)
	{
		r[i] = m1[i] + m2[i];
		++i;
	}
}

void mul(double m1[], double m2[], size_t r1, size_t c1, size_t c2, double r[])
{
	size_t i = 0;
	while (i < r1)
	{
		size_t j = 0;
		while (j < c2)
		{
			size_t z = 0;
			double acc = 0;
			while (z < c1)
			{
				acc += m1[i*c1+z]*m2[z*c2+j];		
				++z;
			}
			r[i*c2+j] = acc;
			++j;
		}
		++i;
	}
}
