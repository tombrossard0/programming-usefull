#include <stdio.h>
#include "matrix.h"

/* Constants */
#define INPUTS_R 2 // 784
#define INPUTS_C 1

#define HIDDENS_R 4 // 12
#define HIDDENS_C 1

#define OUTPUTS_R 2 // 10
#define OUTPUTS_C 1


struct Neural_Network
{
	double inputs[INPUTS_R * INPUTS_C];

	double w1[INPUTS_R * HIDDENS_R];
	double b1[HIDDENS_R];
	double hiddens[HIDDENS_R * HIDDENS_C];

	double w2[OUTPUTS_R * HIDDENS_R];
	double b2[OUTPUTS_R];
	double outputs[OUTPUTS_R * OUTPUTS_C];

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

void Get_Layers_Outputs(struct Neural_Network * NN)
{
	/* Layer 1 */
	Mult_Matrix(NN->w1, NN->inputs, HIDDENS_R, INPUTS_R, INPUTS_C, NN->hiddens);
	Add_Matrix(NN->hiddens, NN->b1, HIDDENS_R, HIDDENS_C, NN->hiddens);
	Matrix_Sigmoid(NN->hiddens, HIDDENS_R, HIDDENS_C);	

	/* Layer 2 */
	Mult_Matrix(NN->w2, NN->hiddens, OUTPUTS_R, HIDDENS_R, HIDDENS_C, NN->outputs);
    Add_Matrix(NN->outputs, NN->b2, OUTPUTS_R, OUTPUTS_C, NN->outputs);
    Matrix_Sigmoid(NN->outputs, OUTPUTS_R, OUTPUTS_C);
}

int main(void)
{
	struct Neural_Network NN;
	
	NN.inputs[0] = 1;
	NN.inputs[1] = 1;

	New_Matrix(HIDDENS_R, INPUTS_R, (&NN)->w1);
	New_Matrix(HIDDENS_R, HIDDENS_C, (&NN)->b1);
	New_Matrix(OUTPUTS_R, HIDDENS_R, (&NN)->w2);
	New_Matrix(OUTPUTS_R, OUTPUTS_C, (&NN)->b2);

	
	Print_Matrix("inputs", NN.inputs, INPUTS_R, INPUTS_C);
	Print_Matrix("hiddens", NN.hiddens, HIDDENS_R, HIDDENS_C);
	Print_Matrix("output", NN.outputs, OUTPUTS_R, OUTPUTS_C);


	Get_Layers_Outputs(&NN);
	

	Print_Matrix("inputs", NN.inputs, INPUTS_R, INPUTS_C);
	Print_Matrix("hiddens", NN.hiddens, HIDDENS_R, HIDDENS_C);
	Print_Matrix("output", NN.outputs, OUTPUTS_R, OUTPUTS_C);


	return 0;
}
