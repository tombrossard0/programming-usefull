#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>


/* --------- RANDOM --------- */

double random_value() {
	return (double)rand() / (double) RAND_MAX;
}


/* --------- ACTIVATION FUNCTIONS --------- */

double Sigmoid(double x) {
	return 1 / (1 + exp(-x));
}


double Sigmoid_Derivation(double x) {
	return Sigmoid(x) * (1 - Sigmoid(x));
}


double Softmax(double * inputs, int index, int length)
{
    double exp_sum = 0;
    for (int i = 0; i < length; i++)
    {
        exp_sum += exp(inputs[i]);
    }

    double res = exp(inputs[index]) / exp_sum;
    return res;
}

double Softmax_Derivation(double * inputs, int index, int length)
{
    double exp_sum = 0;
    for (int i = 0; i < length; i++)
    {
        exp_sum += exp(inputs[i]);
    }

    double ex = exp(inputs[index]);
    return (ex * exp_sum - ex * ex) / (exp_sum * exp_sum);
}
