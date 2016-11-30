#include "neuralNetwork.h"

#define VERBOSE_TRAINXOR 0

void testXOR()
{
	printf(" ===== BEGIN XOR TEST =====\n");
	const int in_count = 4;
	const int in_size = 2;
	//const int out_size = 1;

	//int data[3] = { in_count, in_size, out_size };
	double **inputs = malloc(in_count * sizeof(int*));

	int i = 0;
	while (i < in_count) {
		inputs[i] = malloc(in_size * sizeof(int));
		++i;
	}

	inputs[0][0] = 0; inputs[0][1] = 0;
	inputs[1][0] = 0; inputs[1][1] = 1;
	inputs[2][0] = 1; inputs[2][1] = 0;
	inputs[3][0] = 1; inputs[3][1] = 1;

	double *wonted = malloc(in_count * sizeof(double));
	wonted[0] = 0;
	wonted[1] = 1;
	wonted[2] = 1;
	wonted[3] = 0;

	//NeuralNetwork *net = initNeurNet(data, 3);
	NeuralNetwork *net = load("networks/XOR_trained");

	if (net != NULL)
	{
		do
		{
			switch (time(NULL) % 4)
			{
			case 0:
				printf("\r|");
				break;
			case 1:
				printf("\r/");
				break;
			case 2:
				printf("\r-");
				break;
			case 3:
				printf("\r\\");
				break;
			}
			workoutXOR(net, inputs, wonted, in_count, in_size);
		} while (net->lastNe->dErr > 0.1
			|| net->lastNe->dErr < -0.1);

		printf("\r \r");

		int i = 0;
		while (i < in_count) {
			int j = 0;
			while (j < in_size) {
				net->layArray[0].nArray[j].out = inputs[i][j];
				++j;
			}
			onward(net);
			backwards(net, &wonted[i]);
			int in_A = (int)net->layArray[0].nArray[0].out;
			int in_B = (int)net->layArray[0].nArray[1].out;
			float delta = net->lastNe->dErr;
			float out = net->lastNe->out;
			int wonted = out > 0.5;

			printf("IN[ %d , %d ] \t(%f) \t%f \tOUT[ %d ]\n",
				in_A,
				in_B,
				delta,
				out,
				wonted);
			++i;
		}
	}
	//save(net, "networks/XOR_trained");
	printf(" ===== END XOR TEST =====\n");
}

NeuralNetwork *initNeurNet(int nArray[], int layCount)
{
	NeuralNetwork *net = malloc(sizeof(NeuralNetwork));
	net->layCount = layCount;
	net->layArray = malloc(layCount * sizeof(Layer));

	int i = 0;
	srand(2);	//total random
	while (i < layCount)
	{
		net->layArray[i] = (i == 0) ? *initLay(nArray[i], 0) :
			*initLay(nArray[i], nArray[i - 1]);
		++i;
	}

	net->lastNe = net->layArray[i - 1].nArray;
	net->lastCount = net->layArray[i - 1].nCount;

	return net;
}

double workErr(Neuron *ne, double wonted)
{
	return wonted - ne->out;
}

void onward(NeuralNetwork *net)
{
	int i = 1;
	while (i < net->layCount) {
		workoutTotal(&net->layArray[i - 1], &net->layArray[i]);
		++i;
	}
}

void backwards(NeuralNetwork *net, double *wonted)
{
	int i = 0;
	while (i < net->lastCount) {
		net->lastNe[i].dErr =
			workErr(&net->lastNe[i], wonted[i]);
		++i;
	}

	i = net->layCount - 2;
	while (i > 0) {
		workoutErr(&net->layArray[i], &net->layArray[i + 1]);
		--i;
	}

	i = 1;
	while (i < net->layCount) {
		workoutWeight(&net->layArray[i - 1], &net->layArray[i]);
		++i;
	}
}

void train(NeuralNetwork *net, double **inputs, double *wonted)
{
	int count = 0;
	int j = 0;
	while (j < 10) {
		int i = 0;
		while (i < 10) {
			net->layArray[0].nArray[count].out = inputs[i][j];
			count++;
			++i;
		}
		++j;
	}

	onward(net);
	if (wonted != NULL)
		backwards(net, wonted);
}

void workoutXOR(NeuralNetwork *net, double **inputs,
	double *wonted,
	int inputsCount, int inCount)
{
	int i = 0;
	while (i < inputsCount) {
		int j = 0;
		while (j < inCount) {
			net->layArray[0].nArray[j].out = inputs[i][j];
			++j;
		}
		onward(net);
		backwards(net, &wonted[i]);

		if (VERBOSE_TRAINXOR) {
			int in_A = (int)net->layArray[0].nArray[0].out;
			int in_B = (int)net->layArray[0].nArray[1].out;
			float delta = net->lastNe->dErr;
			float out = net->lastNe->out;
			int wonted = out > 0.5;

			printf("IN[ %d , %d ] \t(%f) \t%f \tOUT[ %d ]\n",
				in_A,
				in_B,
				delta,
				out,
				wonted);
		}
		++i;
	}
}