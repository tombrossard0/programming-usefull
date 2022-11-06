#include <stdio.h>
#include "matrix.h"

/* Constants */
#define INPUTS_R 784
#define INPUTS_C 1

#define HIDDENS_R 12
#define HIDDENS_C 1

#define OUTPUTS_R 10
#define OUTPUTS_C 1


struct Neural_Network
{
	double inputs[2];

	double w1[INPUTS_R * HIDDENS_R];
	double b1[HIDDENS_R];
	double hiddens[HIDDENS_R * HIDDENS_C];

	double w2[OUTPUTS_R * HIDDENS_R];
	double b2[OUTPUTS_R];
	double outpus[OUTPUTS_R * OUTPUTS_C];

	/* for each weights and biases, calcul cost derivative */
    double w1_derivative[INPUTS_R * HIDDENS_R];
    double b1_derivative[HIDDENS_R];
    double w2_derivative[OUTPUTS_R * HIDDENS_R];
    double b2_derivative[OUTPUTS_R];
};


/* --------- FUNCTIONS --------- */

double Node_Cost(double output, double desired_output) {
	double error = desired_output - output;
	return error * error;
}

double Node_Cost_Derivative(double output, double desired_output) {
	return 2 * (output - desired_output);
}


int main(void)
{
	printf("Hellow world!");

	return 0;
}
