#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "neuron.h"

struct Neuron *initializingNeuron(int nbSynap)
{
  struct Neuron *neuron = malloc(sizeof(struct Neuron));
  neuron->nbSynap = nbSynap;
  
  neuron->synapses = malloc(nbSynap * sizeof(double));
  for (int i = 0; i < nbSynap; i++)
  {
    neuron->synapses[i] = ((double)rand() / (double)RAND_MAX)/* * pow(-1, rand())*/;
  }
  //neuron->bias = 0;
  neuron->sum = 0;
  neuron->deltaError = 0;
  neuron->out = 0;

  return neuron;
}

double sigmoid(double x)
{
  return 1 / ( 1 + exp(-x));
}

double derivateSig(double x)
{
  return sigmoid(x) * (1 - sigmoid(x)); 
}

void computeOut(struct Neuron *neuron)
{
  neuron->out = sigmoid(neuron->sum /*+ neuron->bias*/);
}