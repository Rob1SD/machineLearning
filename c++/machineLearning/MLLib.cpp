
extern "C" {
	#include<stdlib.h>
	#include<stdio.h>
	#include<math.h>
	#include<ctime>


	using namespace std;

	// Generate a random double number
	double randomDouble(float min, float max)
	{
		double scale = rand() / (double)RAND_MAX; /* [0, 1.0] */
		return min + scale * (max - min);      /* [min, max] */
	}

	__declspec(dllexport) int Return42() {
		return 42;
	}

	__declspec(dllexport) double *CreateModel(int inputdimension) {
		double* array = new double[inputdimension];

		for (int i = 0; i < inputdimension; i++) {
		
			double seed = randomDouble(-1, 1);

			int signRand = 1;

			if (signRand == 0) {
				array[i] = seed * -1;
			}
			else if (signRand == 1) {
				array[i] = seed;
			}
			else {
				array[i] = -12;
			}
		}

		return array;

	}
	__declspec(dllexport) void RemoveLinearModel(double *model) {
		delete[] model;
	}
	__declspec(dllexport) int LinearFirstRegression(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int outputsSize) {
		return 999;
	}

	__declspec(dllexport) double LinearClassification(double *model, double *input, int inputSize) {
		/*printf("\n\nClassification lineaire\nValeurs d'entree : ");

		for (int i = 0; i < inputSize; i++) {
			if (i == inputSize - 1) {
				printf("%lf", input[i]);
			}
			else {
				printf("%lf,", input[i]);
			}
		}

		printf("\nValeurs des poids : ");

		for (int i = 0; i < inputSize; i++) {
			if (i == inputSize - 1) {
				printf("%lf", model[i]);
			}
			else {
				printf("%lf,", model[i]);
			}
		}

		printf("\nValeur du biais : %lf", model[inputSize]);
		*/

		double weightedSum = 0;

		// On calcule la somme pondérée des valeurs * poids
		for (int i = 0; i < inputSize; i++) {
			weightedSum += input[i] * model[i];
		}

		//printf("\nSomme ponderee : %lf", weightedSum);

		// On ajoute à la somme pondérée le biais (qui sera en * 1 donc pas besoin de calcul)
		weightedSum += model[inputSize];

		//printf("\nSomme ponderee apres ajout du biais : %lf", weightedSum);

		// On effectue la fonction signe (si > 0 vaut 1, si < 0 vaut -1, sinon 0)
		double signWS = (weightedSum > 0 ? 1.0 : (weightedSum < 0 ? -1.0 : 0));

		//printf("\nSomme ponderee apres fonction signe : %lf", signWS);

		return signWS;
	}
	__declspec(dllexport) double LinearRegression(double *model, double *input, int inputSize) {
		return .5;
	}

	double **SingleArrayToDoubleArray(double *arrayInputs, int arraySize, int inputsSize) {
		int sizeOneInput = arraySize / inputsSize;

		double** array = new double*[inputsSize];

		int index = 0;

		for (int i = 0; i < inputsSize; i++) {
			double* temp = new double[sizeOneInput];

			for (int j = 0; j < sizeOneInput; j++) {
				temp[j] = arrayInputs[index];
				index += 1;
			}
			array[i] = temp;
		}

		return array;
	}

	__declspec(dllexport) int FitRosenblatt(double *model, double *arrayInputs, int inputsSize,
		int inputSize, double *outputs, int outputsSize, double step, int iterations) {

		double **inputs = SingleArrayToDoubleArray(arrayInputs, inputsSize * inputSize, inputsSize);

		for (int j = 0; j < iterations; j++) {
			for (int i = 0; i < inputsSize; i++) {
				// On récupère la valeur donnée par le perceptron actuellement
				double actualValue = LinearClassification(model, inputs[i], inputSize);

				for (int j = 0; j < inputSize + 1; j++) {
					// On calcule la différence entre valeur attendue - valeur du perceptron
					double resultDiff = outputs[i] - actualValue;

					// On multiplie par le pas d'apprentissage
					resultDiff *= step;

					// Si on est dans les poids régulier (le biais sera en * 1 donc pas besoin)
					if (j < inputSize) {
						// On multiplie par la valeur de l'exemple correspondant au poid
						resultDiff *= inputs[i][j];
					}

					// On ajoute le calcul au paramètre du modèle
					model[j] = model[j] + resultDiff;
				}
			}
		}

		return 1;
	}
	/*
	int main(int argc, char *argv[])
	{
		double* model = CreateModel(3);
		double **myarray = new double*[2];

		myarray[0] = new double[2];
		myarray[1] = new double[2];

		myarray[0][0] = 2;
		myarray[0][1] = 2;

		myarray[1][0] = 13;
		myarray[1][1] = 27;

		double *outputs = new double[2];
		outputs[0] = 1;
		outputs[1] = -1;

		//int res = FitRosenblatt(model, myarray, 2, 2, outputs, 2, 0.01, 10000);

		//LinearClassification(model, new double[2]{ 10, 2 }, 2);

		double* array = new double[9];

		array[0] = 1;
		array[1] = 2;
		array[2] = 3;
		array[3] = 4;
		array[4] = 5;
		array[5] = 6;
		array[6] = 7;
		array[7] = 8;
		array[8] = 9;

		double** newArray = SingleArrayToDoubleArray(array, 9, 3);

		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < sizeof(newArray[i]); j++) {
				printf("%lf, ", newArray[i][j]);
			}
			printf("\n");
		}

		char* temp = new char[50];
		scanf_s("%s", &temp);
	}
	*/
}