#include <iostream>
#include <Eigen\Eigen>
extern "C" {
	using namespace std;
	using namespace Eigen;
	#include <stdlib.h>
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

	__declspec(dllexport) double *CreateModel(int inputdimension) {
		double* array = new double[inputdimension];

		for (int i = 0; i < inputdimension; i++) {

			double seed = randomDouble(-1, 1);

			array[i] = seed;
		}

		return array;

	}
	__declspec(dllexport) void RemoveLinearModel(double *model) {
		delete[] model;
	}

	__declspec(dllexport) double LinearClassification(double *model, double *input, int inputSize) {
		printf("\n\nClassification lineaire\nValeurs d'entree : ");

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


		double weightedSum = 0;

		// On calcule la somme pondérée des valeurs * poids
		for (int i = 0; i < inputSize; i++) {
			weightedSum += input[i] * model[i];
		}

		printf("\nSomme ponderee : %lf", weightedSum);

		// On ajoute à la somme pondérée le biais (qui sera en * 1 donc pas besoin de calcul)
		weightedSum += model[inputSize];

		printf("\nSomme ponderee apres ajout du biais : %lf", weightedSum);

		// On effectue la fonction signe (si > 0 vaut 1, si < 0 vaut -1, sinon 0)
		double signWS = (weightedSum > 0 ? 1.0 : (weightedSum < 0 ? -1.0 : 0));

		printf("\nSomme ponderee apres fonction signe : %lf", signWS);

		return signWS;
	}
	__declspec(dllexport) double LinearRegression(double *model, double *input, int inputSize) {
		printf("\n\nRégression lineaire\nValeurs d'entree : ");

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


		double weightedSum = 0;

		// On calcule la somme pondérée des valeurs * poids
		for (int i = 0; i < inputSize; i++) {
			weightedSum += input[i] * model[i];
		}

		printf("\nSomme ponderee : %lf", weightedSum);

		// On ajoute à la somme pondérée le biais (qui sera en * 1 donc pas besoin de calcul)
		weightedSum += model[inputSize];

		printf("\nSomme ponderee apres ajout du biais : %lf", weightedSum);

		return weightedSum;
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

	__declspec(dllexport) int FitLinearRegression(double *model, double *arrayInputs, int inputsSize,
		int inputSize, double *outputs, int outputsSize) {
		//on init nos matrice en se basant sur les tableaux de double
		MatrixXd MatX(inputsSize, inputSize + 1);//avec une colone en plus
		for (int i = 0; i < inputsSize; i++) {
			for (int j = 0; j < inputSize; j++) {
				MatX(i, j) = arrayInputs[(i*inputSize) + j];
			}
			MatX(i, inputSize) = 1;//on rajoute le 1 dans la dernière colone
		}
		MatrixXd MatY(inputsSize, 1);
		for (int i = 0; i < inputsSize; i++) {
			MatY(i, 0) = outputs[i];
		}

		//calcule du pdf
		MatrixXd result = ((MatX.transpose()*MatX).inverse()*MatX.transpose())*MatY;

		//on renseigne le format de la matrice de resultat pour pouvoir parser le tableau de double qu'on vas renvoyer en matrice
		int colRetour = result.cols();
		int LineRetour = result.rows();

		//on parse la matrice en tableau de double
		model = new double[colRetour * LineRetour];
		for (int i = 0; i < LineRetour; i++) {
			for (int j = 0; j < colRetour; j++) {
				model[(i * colRetour) + j] = result(i, j);
			}
		}
		return 1;
	}

	__declspec(dllexport) int FitLinearClassification(double *model, double *arrayInputs, int inputsSize,
		int inputSize, double *outputs, int outputsSize, double step, int iterations) {

		double **inputs = SingleArrayToDoubleArray(arrayInputs, inputsSize * inputSize, inputsSize);

		for (int j = 0; j < iterations; j++) {
			for (int i = 0; i < inputsSize; i++) {
				// On récupère la valeur donnée par le perceptron actuellement
				double actualValue = LinearClassification(model, inputs[i], inputSize);

				printf("\nValeur attendue : %lf", outputs[i]);

				for (int l = 0; l < inputSize + 1; l++) {
					// On calcule la différence entre valeur attendue - valeur du perceptron
					double resultDiff = outputs[i] - actualValue;

					// On multiplie par le pas d'apprentissage
					resultDiff *= step;

					// Si on est dans les poids régulier (le biais sera en * 1 donc pas besoin)
					if (l < inputSize) {
						// On multiplie par la valeur de l'exemple correspondant au poid
						resultDiff *= inputs[i][l];
					}

					// On ajoute le calcul au paramètre du modèle
					model[l] = model[l] + resultDiff;
				}
			}
		}

		return 1;
	}

	int main(int argc, char *argv[])
	{
		double* model = CreateModel(3);

		for (int i = 0; i < 3; i++) {
			printf("%lf, ", model[i]);
		}

		double* array = new double[6];

		array[0] = 1;
		array[1] = 2;
		array[2] = 5;
		array[3] = 3;
		array[4] = 6;
		array[5] = 3;

		int res = FitLinearRegression(model, array, 3, 2, new double[3] {0.5, -0.3, -0.6} , 3);

		LinearRegression(model, new double[2]{ 3, 1 }, 2);

		char* temp = new char[50];
		scanf_s("%s", &temp);

	}
}