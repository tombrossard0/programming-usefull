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

	double 
}
