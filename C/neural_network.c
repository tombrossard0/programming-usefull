#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "data.h"
#include "user_inputs_functions.h"
#include "functions.h"

/* Constants */
#define INPUTS_R 2 // 784
#define INPUTS_C 1

#define HIDDENS_R 10 // 12
#define HIDDENS_C 1

#define OUTPUTS_R 2 // 10
#define OUTPUTS_C 1

#define XOR_DATA "XOR.csv"
#define DIGITS_DATA "TMNIST_Data.csv"

#define SAVE "save"

struct Neural_Network
{
	double inputs[INPUTS_R * INPUTS_C];

	double w1[INPUTS_R * HIDDENS_R];
	double b1[HIDDENS_R];
	double weighted_hiddens[HIDDENS_R];
	double cost_gradient_w1[INPUTS_R * HIDDENS_R];
	double cost_gradient_b1[HIDDENS_R];
	double hiddens[HIDDENS_R * HIDDENS_C];

	double w2[OUTPUTS_R * HIDDENS_R];
	double b2[OUTPUTS_R];
	double weighted_outputs[OUTPUTS_R];
	double cost_gradient_w2[OUTPUTS_R * HIDDENS_R];
	double cost_gradient_b2[OUTPUTS_R];
	double outputs[OUTPUTS_R * OUTPUTS_C];

	/* for each weights and biases, calcul cost derivative */
    double w1_derivative[INPUTS_R * HIDDENS_R];
    double b1_derivative[HIDDENS_R];
    double w2_derivative[OUTPUTS_R * HIDDENS_R];
    double b2_derivative[OUTPUTS_R];
};

/* Save Manager */

void Export_NN(struct Neural_Network * NN, char file[])
{
	FILE *out = fopen(file, "w");

	for (int i = 0; i < INPUTS_R * HIDDENS_R; i++)
		fprintf(out, "%f\n", NN->w1[i]);

	for (int i = 0; i < HIDDENS_R; i++)
		fprintf(out, "%f\n", NN->b1[i]);

	for (int i = 0; i < OUTPUTS_R * HIDDENS_R; i++)
		fprintf(out, "%f\n", NN->w2[i]);

	for (int i = 0; i < OUTPUTS_R; i++)
		fprintf(out, "%f\n", NN->b2[i]);


	fclose(out);
}

void Import_NN(struct Neural_Network * NN, char file[])
{
	FILE * in = fopen(file, "r");

	char line[1000];
	char *rest;

	for (int i = 0; i < INPUTS_R * HIDDENS_R; i++)
	{
		fgets(line, 1000, in);
		NN->w1[i] = strtod(line, &rest);
	}

	for (int i = 0; i < HIDDENS_R; i++)
	{
		fgets(line, 1000, in);
		NN->b1[i] = strtod(line, &rest);
	}

	for (int i = 0; i < OUTPUTS_R * HIDDENS_R; i++)
	{
		fgets(line, 1000, in);
		NN->w2[i] = strtod(line, &rest);
	}

	for (int i = 0; i < OUTPUTS_R; i++)
	{
		fgets(line, 1000, in);
		NN->b2[i] = strtod(line, &rest);
	}

	fclose(in);
}


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

double Calculate_Cost(struct Neural_Network * NN, struct Data data)
{
	double cost = 0;
	
	for (int i = 0; i < INPUTS_R; i++)
	{
		NN->inputs[i] = data.input[i];
	}

	Get_Layers_Outputs(NN);

	for (int i = 0; i < OUTPUTS_R; i++)
	{
		cost += Node_Cost(NN->outputs[i], data.expected_output[i]);
	}

	return cost;
}

double Calculate_Total_Cost(struct Neural_Network NN, struct DataSet data_set)
{
	double total_cost = 0;

	for (int i = 0; i < data_set.length; i++)
	{
		total_cost += Calculate_Cost(&NN, data_set.data_set[i]);
	}

	return total_cost;
}

void Apply_All_Gradients_NN(struct Neural_Network * NN, double learning_rate)
{
	// Apply all gradients
	for (int i = 0; i < OUTPUTS_R; i++)
	{
		NN->b2[i] -= NN->cost_gradient_b2[i] * learning_rate;

		for (int j = 0; j < HIDDENS_R; j++)
		{
			NN->w2[i * HIDDENS_R + j] -= NN->cost_gradient_w2[i * HIDDENS_R + j] * learning_rate;
		}
	}

	for (int i = 0; i < HIDDENS_R; i++)
	{
		NN->b1[i] -= NN->cost_gradient_b1[i] * learning_rate;

		for (int j = 0; j < INPUTS_R; j++)
		{
			NN->w1[i* INPUTS_R + j] -= NN->cost_gradient_w1[i * INPUTS_R + j] * learning_rate;
		}
	}
}

void Clear_All_Gradients_NN(struct Neural_Network * NN)
{
	// Clear all gradients
	for (int i = 0; i < HIDDENS_R * INPUTS_R; i++)
		NN->cost_gradient_w1[i] = 0;
	
	for (int i = 0; i < HIDDENS_R; i++)
		NN->cost_gradient_b1[i] = 0;

	for (int i = 0; i < OUTPUTS_R * HIDDENS_R; i++)
		NN->cost_gradient_w2[i] = 0;

	for (int i = 0; i < OUTPUTS_R; i++)
		NN->cost_gradient_b2[i] = 0;
}

void Update_All_Gradients_NN(struct Neural_Network * NN, struct Data data)
{
	Get_Layers_Outputs(NN);
	
	double * new_outputs_nodes_values = malloc(sizeof(double) * OUTPUTS_R);
	
	for (int i = 0; i < OUTPUTS_R; i++)
	{
		double cost_derivative = Node_Cost_Derivative(NN->outputs[i], data.expected_output[i]);
		double activation_derivative = Sigmoid_Derivation(NN->weighted_outputs[i]);
	
		new_outputs_nodes_values[i] = cost_derivative * activation_derivative;
	}

	// Update_Gradients_Layer()
	for (int i = 0; i < OUTPUTS_R; i++)
	{
		for (int j = 0; j < HIDDENS_R; j++)
		{
			double partial_derivative_cost_weight = NN->hiddens[j] * new_outputs_nodes_values[i];
			NN->cost_gradient_w2[i*HIDDENS_R+j] += partial_derivative_cost_weight;
		}

		double partial_derivative_cost_bias = 1 * new_outputs_nodes_values[i];
		NN->cost_gradient_b2[i] += partial_derivative_cost_bias;		
	}

	double * new_hiddens_nodes_values = malloc(sizeof(double) * HIDDENS_R);

	for (int i = 0; i < HIDDENS_R; i++)
	{
		double new_node_value = 0;

		for (int j = 0; j < INPUTS_R; j++)
		{
			double weighted_input_derivative = NN->w1[i*INPUTS_R+j];
			new_node_value += weighted_input_derivative * new_outputs_nodes_values[j];
		}

		new_node_value *= Sigmoid_Derivation(NN->weighted_hiddens[i]);
		new_hiddens_nodes_values[i] = new_node_value;
	}

	// Update_Gradients_Layers()
	for (int i = 0; i < HIDDENS_R; i++)
	{
		for (int j = 0; j < INPUTS_R; j++)
		{
			double partial_derivative_cost_weight = NN->inputs[j] * new_hiddens_nodes_values[i];
			NN->cost_gradient_w1[i*INPUTS_R+j] += partial_derivative_cost_weight;
		}

		double partial_derivative_cost_bias = 1 * new_hiddens_nodes_values[i];
		NN->cost_gradient_b1[i] += partial_derivative_cost_bias;
	}

	free(new_outputs_nodes_values);
	free(new_hiddens_nodes_values);
}

void Gradient_Descent(struct Neural_Network * NN, struct DataSet training_data, double learning_rate)
{
	for (int i = 0; i < training_data.length; i++)
    {
        Update_All_Gradients_NN(NN, training_data.data_set[i]);
    }

    Apply_All_Gradients_NN(NN, learning_rate);

    Clear_All_Gradients_NN(NN);
}


void Learning(struct Neural_Network * NN, struct DataSet data)
{
	for (int epoch = 0; epoch < 10000; epoch++)
    {
        Reset_Matrix(INPUTS_R, HIDDENS_R, NN->w1_derivative);
        Reset_Matrix(HIDDENS_R, 1, NN->b1_derivative);     
        Reset_Matrix(OUTPUTS_R, HIDDENS_R, NN->w2_derivative);
        Reset_Matrix(OUTPUTS_R, 1, NN->b2_derivative);

		double delta = 0.1;
        double original_cost = Calculate_Total_Cost(*NN, data);

        for (int j = 0; j < INPUTS_R * HIDDENS_R; j++)
        {
            NN->w1[j] += delta;
            double cost = Calculate_Total_Cost(*NN, data) - original_cost;
            NN->w1_derivative[j] += cost;
            NN->w1[j] -= delta;
        }

        for (int j = 0; j < HIDDENS_R; j++)
        {
            NN->b1[j] += delta;
            double cost = Calculate_Total_Cost(*NN, data) - original_cost;
            NN->b1_derivative[j] += cost;
            NN->b1[j] -= delta;
        }

        for (int j = 0; j < OUTPUTS_R * HIDDENS_R; j++)
        {
            NN->w2[j] += delta;
            double cost = Calculate_Total_Cost(*NN, data) - original_cost;
            NN->w2_derivative[j] += cost;
            NN->w2[j] -= delta;
        }

        for (int j = 0; j < OUTPUTS_R; j++)
        {
            NN->b2[j] += delta;
            double cost = Calculate_Total_Cost(*NN, data) - original_cost;
            NN->b2_derivative[j] += cost;
            NN->b2[j] -= delta;
        }

        for (int j = 0; j < INPUTS_R * HIDDENS_R; j++)
           NN->w1[j] -= NN->w1_derivative[j];

        for (int j = 0; j < HIDDENS_R; j++)
            NN->b1[j] -= NN->b1_derivative[j];

        for (int j = 0; j < OUTPUTS_R * HIDDENS_R; j++)
            NN->w2[j] -= NN->w2_derivative[j];

        for (int j = 0; j < OUTPUTS_R; j++)
            NN->b2[j] -= NN->b2_derivative[j];
     	
		// Print current cost
		if (epoch > 0)
			printf("\b\b\b\b\b\b\b\b");
		else
			printf("\nCost : ");
		printf("%8f", Calculate_Total_Cost(*NN, data));	
	}
	printf("\n\n");
}


int main(void)
{
	struct DataSet data;
	data.length = 4;

	data.data_set = malloc(sizeof(struct Data) * 500);
	
	Get_CSV_Data_Image(XOR_DATA, &data);


	struct Neural_Network NN;
	
	New_Matrix(HIDDENS_R, INPUTS_R, (&NN)->w1);
	//New_Matrix(HIDDENS_R, HIDDENS_C, (&NN)->b1);
	New_Matrix(OUTPUTS_R, HIDDENS_R, (&NN)->w2);
	//New_Matrix(OUTPUTS_R, OUTPUTS_C, (&NN)->b2);


	int file_exists = Is_File_Exists(SAVE);
	if (file_exists == 1)
	{
		Import_NN(&NN, SAVE);
	}

	/*
	// Quicker method : (doesn't works actually)		
	int epochs = 1000;
	for (int i = 0; i < epochs; i++)
	{
		Gradient_Descent(&NN, data, 0.1);

		printf("\n%f\n\n", Calculate_Total_Cost(NN, data));

		for (int i = 0; i < INPUTS_R; i++)
		{
			NN.inputs[i] = data.data_set[0].input[i];
		}

		Get_Layers_Outputs(&NN);

		//printf("Label : %s\n", data.data_set[0].label);
		//Print_Matrix("Output", NN.outputs, OUTPUTS_R, OUTPUTS_C);

	}
	*/

	// Train our Neural Network :
	if (file_exists == 0)
	{
		Learning(&NN, data);
		Export_NN(&NN, SAVE);
	}

	// Print results
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < INPUTS_R; i++)
		{
			(&NN)->inputs[i] = data.data_set[j].input[i];
			//printf("data : %f\n", data.data_set[j].input[i]);
		}
		
		Get_Layers_Outputs(&NN);

		Print_Matrix("Input", NN.inputs, INPUTS_R, INPUTS_C);
		printf("Label : %s\n", data.data_set[j].label);
		Print_Matrix("Output", NN.outputs, OUTPUTS_R, OUTPUTS_C);
	}
	
	
	free(data.data_set);
	
	return 0;
}
