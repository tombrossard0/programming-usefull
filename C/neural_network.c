#include <stdio.h>
#include "matrix.h"
#include "data.h"
#include "user_inputs_functions.h"


/* Constants */
#define INPUTS_R 784 // 784
#define INPUTS_C 1

#define HIDDENS_R 12 // 12
#define HIDDENS_C 1

#define OUTPUTS_R 10 // 10
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

double Calculate_Cost(struct Neural_Network NN, struct Data data)
{
	double cost = 0;
	
	for (int i = 0; i < 784; i++)
	{
		NN.inputs[i] = data.input[i];
	}

	Get_Layers_Outputs(&NN);

	for (int i = 0; i < 10; i++)
	{
		cost += Node_Cost(NN.outputs[i], data.expected_output[i]);
	}

	return cost;
}

double Calculate_Total_Cost(struct Neural_Network NN, struct DataSet data_set)
{
	double total_cost = 0;

	for (int i = 0; i < data_set.length; i++)
	{
		total_cost += Calculate_Cost(NN, data_set.data_set[i]);
	}

	return total_cost;
}


int main(void)
{
	struct DataSet data;
	data.length = 100;
	
	Get_CSV_Data_Image("TMNIST_Data.csv", &data);


	struct Neural_Network NN;
	
	New_Matrix(HIDDENS_R, INPUTS_R, (&NN)->w1);
	New_Matrix(HIDDENS_R, HIDDENS_C, (&NN)->b1);
	New_Matrix(OUTPUTS_R, HIDDENS_R, (&NN)->w2);
	New_Matrix(OUTPUTS_R, OUTPUTS_C, (&NN)->b2);


	for (int epoch = 0; epoch < 50; epoch++)
    {
        Reset_Matrix(INPUTS_R, HIDDENS_R, (&NN)->w1_derivative);
        Reset_Matrix(HIDDENS_R, 1, (&NN)->b1_derivative);     
        Reset_Matrix(OUTPUTS_R, HIDDENS_R, (&NN)->w2_derivative);
        Reset_Matrix(OUTPUTS_R, 1, (&NN)->b2_derivative);

		double delta = 0.1;
        double original_cost = Calculate_Total_Cost(NN, data);

        for (int j = 0; j < INPUTS_R * HIDDENS_R; j++)
        {
            (&NN)->w1[j] += delta;
            double cost = Calculate_Total_Cost(NN, data) - original_cost;
            (&NN)->w1_derivative[j] += cost;
            (&NN)->w1[j] -= delta;
        }

        for (int j = 0; j < HIDDENS_R; j++)
        {
            (&NN)->b1[j] += delta;
            double cost = Calculate_Total_Cost(NN, data) - original_cost;
            (&NN)->b1_derivative[j] += cost;
            (&NN)->b1[j] -= delta;
        }

        for (int j = 0; j < OUTPUTS_R * HIDDENS_R; j++)
        {
            (&NN)->w2[j] += delta;
            double cost = Calculate_Total_Cost(NN, data) - original_cost;
            (&NN)->w2_derivative[j] += cost;
            (&NN)->w2[j] -= delta;
        }

        for (int j = 0; j < OUTPUTS_R; j++)
        {
            (&NN)->b2[j] += delta;
            double cost = Calculate_Total_Cost(NN, data) - original_cost;
            (&NN)->b2_derivative[j] += cost;
            (&NN)->b2[j] -= delta;
        }

        for (int j = 0; j < INPUTS_R * HIDDENS_R; j++)
            (&NN)->w1[j] -= (&NN)->w1_derivative[j];

        for (int j = 0; j < HIDDENS_R; j++)
            (&NN)->b1[j] -= (&NN)->b1_derivative[j];

        for (int j = 0; j < OUTPUTS_R * HIDDENS_R; j++)
            (&NN)->w2[j] -= (&NN)->w2_derivative[j];

        for (int j = 0; j < OUTPUTS_R; j++)
            (&NN)->b2[j] -= (&NN)->b2_derivative[j];
     	
		printf("\n%f\n\n", Calculate_Total_Cost(NN, data));

		for (int i = 0; i < 784; i++)
		{
			NN.inputs[i] = data.data_set[0].input[i];
		}

		Get_Layers_Outputs(&NN);

		printf("Label : %s\n", data.data_set[0].label);
		Print_Matrix("Output", NN.outputs, OUTPUTS_R, OUTPUTS_C);
	}
	
	
	return 0;
}
