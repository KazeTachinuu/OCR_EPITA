//#include "perceptron.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>





// To be clear:
// the hidden neuron has 2 nodes. Weight: hiddenWeights Bias: hiddenLayerBias
// the output layer has 1 node. Weight: outputWeights Bias: outputLayerBias
// the input layer has 2 nodes.
// the input layer is the input to the hidden layer
// the hidden layer is the input to the output layer




// Because I use a Stochastic Gradient descent (SGD):
// I need to randomize the inputs the algorithm will take
void shuffle(int *array, size_t n) {
  if (n > 1) {
    size_t i;
    for (i = 0; i < n - 1; i++) {
      size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
      int t = array[j];
      array[j] = array[i];
      array[i] = t;
    }
  }
}

// Learning function
// we set the epoch to 10 000 by default.

int randinputs[] = {0, 1, 2, 3};

//the sigmoid function
double sigmoid(double x) { return 1 / (1 + exp(-x)); }
//and its derivative
double dSigmoid(double x) { return x * (1 - x); }

//random number generator from 0 to 1
double init_weight() { return ((double)rand())/((double)RAND_MAX); }





int main(int argc, const char * argv[]) {

    if (argc > 2)
    {
        printf("Usage: %s <numEpochs> (or by default 10000)\n", argv[0]);
        return 1;
    }

    static const int numInputs = 2;
    static const int numHiddenNodes = 2;
    static const int numOutputs = 1;
    
    const double lr = 0.1f;
    
    double hiddenLayer[numHiddenNodes];
    double outputLayer[numOutputs];
  
    
    double hiddenLayerBias[numHiddenNodes];
    double outputLayerBias[numOutputs];

    double hiddenWeights[numInputs][numHiddenNodes];
    double outputWeights[numHiddenNodes][numOutputs];
    
    static const int numTrainingSets = 4;
    double outputLayerList[numTrainingSets];
    double training_inputs[4][2] = { {0.0f,0.0f},{1.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f} };
    double training_outputs[4][1] = { {0.0f},{1.0f},{1.0f},{0.0f} };

    int numEpochs = 10000;
    if (argc == 2)
    {
        numEpochs = atoi(argv[1]);
    }
    
    for (int i=0; i<numInputs; i++) {
        for (int j=0; j<numHiddenNodes; j++) {
            hiddenWeights[i][j] = init_weight();
        }
    }
    for (int i=0; i<numHiddenNodes; i++) {
        hiddenLayerBias[i] = init_weight();
        for (int j=0; j<numOutputs; j++) {
            outputWeights[i][j] = init_weight();
        }
    }
    for (int i=0; i<numOutputs; i++) {
        outputLayerBias[i] = init_weight();
    }
    
    int trainingSetOrder[] = {0,1,2,3};
    
    for (int n=0; n < numEpochs; n++) {
        if (n % 100 == 0) {
            printf("Epoch = %d\n", n);
        }
        shuffle(trainingSetOrder,numTrainingSets);
        for (int x=0; x<numTrainingSets; x++) {
            
            int i = trainingSetOrder[x];
            
            // Forward pass
            
            for (int j=0; j<numHiddenNodes; j++) {
                double activation=hiddenLayerBias[j];
                 for (int k=0; k<numInputs; k++) {
                    activation+=training_inputs[i][k]*hiddenWeights[k][j];
                }
                hiddenLayer[j] = sigmoid(activation);
            }
            
            for (int j=0; j<numOutputs; j++) {
                double activation=outputLayerBias[j];
                for (int k=0; k<numHiddenNodes; k++) {
                    activation+=hiddenLayer[k]*outputWeights[k][j];
                }
                outputLayer[j] = sigmoid(activation);
            }
            if (n % 100 == 0) {
                outputLayerList[i] = outputLayer[0];
            }
           // Backprop
            
            double deltaOutput[numOutputs];
            for (int j=0; j<numOutputs; j++) {
                double errorOutput = (training_outputs[i][j]-outputLayer[j]);
                deltaOutput[j] = errorOutput*dSigmoid(outputLayer[j]);
            }
            
            double deltaHidden[numHiddenNodes];
            for (int j=0; j<numHiddenNodes; j++) {
                double errorHidden = 0.0f;
                for(int k=0; k<numOutputs; k++) {
                    errorHidden+=deltaOutput[k]*outputWeights[j][k];
                }
                deltaHidden[j] = errorHidden*dSigmoid(hiddenLayer[j]);
            }
            
            for (int j=0; j<numOutputs; j++) {
                outputLayerBias[j] += deltaOutput[j]*lr;
                for (int k=0; k<numHiddenNodes; k++) {
                    outputWeights[k][j]+=hiddenLayer[k]*deltaOutput[j]*lr;
                }
            }
            
            for (int j=0; j<numHiddenNodes; j++) {
                hiddenLayerBias[j] += deltaHidden[j]*lr;
                for(int k=0; k<numInputs; k++) {
                    hiddenWeights[k][j]+=training_inputs[i][k]*deltaHidden[j]*lr;
                }
            }
        }
        if (n % 100 == 0) {
            double error = 0.0f;
            for (int i=0; i<numTrainingSets; i++) {
                for (int j=0; j<numHiddenNodes; j++) {
                    double activation=hiddenLayerBias[j];
                    for (int k=0; k<numInputs; k++) {
                        activation+=training_inputs[i][k]*hiddenWeights[k][j];
                    }
                    hiddenLayer[j] = sigmoid(activation);
                }
                
                for (int j=0; j<numOutputs; j++) {
                    double activation=outputLayerBias[j];
                    for (int k=0; k<numHiddenNodes; k++) {
                        activation+=hiddenLayer[k]*outputWeights[k][j];
                    }
                    outputLayer[j] = sigmoid(activation);
                }
                error += pow((training_outputs[i][0]-outputLayer[0]),2);
            }
            
            
            //print result of epoch
            for (int i=0; i<numTrainingSets; i++) {
                printf("%.1f XOR %.1f = %f (expected %.1f)\n", training_inputs[i][0], training_inputs[i][1], outputLayerList[i], training_outputs[i][0]);
            } 
            //print in red if error is greater than 0.1 and in green if less than 0.1
            if (error > 0.1) {
                printf("\033[0;31m");
            } else {
                printf("\033[0;32m");
            }
            printf("Error = %f\n\n", error);
            printf("\033[0m");

        }


        }
        FILE *fp;
        fp = fopen("xor.nn", "w");
        
        //print the final weights
        fprintf(fp, "Final Hidden Layer Weights:\n");
        for (int i=0; i<numInputs; i++) {
            for (int j=0; j<numHiddenNodes; j++) {
                fprintf(fp, "%f ", hiddenWeights[i][j]);
            }
            fprintf(fp, "\n");
        }
        //print the final biases
        fprintf(fp, "\nFinal Hidden Layer Biases:\n");
        for (int i=0; i<numHiddenNodes; i++) {
            fprintf(fp, "%f ", hiddenLayerBias[i]);
        }
        fprintf(fp, "\n");
        //print the final weights
        fprintf(fp, "\nFinal Output Layer Weights:\n");
        for (int i=0; i<numHiddenNodes; i++) {
            for (int j=0; j<numOutputs; j++) {
                fprintf(fp, "%f ", outputWeights[i][j]);
            }
            fprintf(fp, "\n");
        }
        //print the final biases
        fprintf(fp, "\nFinal Output Layer Biases:\n");
        for (int i=0; i<numOutputs; i++) {
            fprintf(fp, "%f ", outputLayerBias[i]);
        }
        fprintf(fp, "\n");
        fclose(fp);
        //print in green
        printf("\033[0;32m");
        printf("Results saved to xor.nn\n");
        printf("\033[0m");
       



        return EXIT_SUCCESS;
}
  