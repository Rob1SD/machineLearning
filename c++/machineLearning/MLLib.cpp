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
	double calulateX_j_l(int j, int l, double **X, double ***W, int *d) {
		double result = 0;
		for (int i = 0; i <= d[l-1]; ++i) {
			result += W[l][i][j]*X[l-1][i] ;
		}
		return tanh(result);

	}
	double calculate_delta_j_LastCouche_classification(int j, int l, double **X, double *Y) {
		return (1 - pow(X[l][j], 2))*(X[l][j]-Y[j]);
	}
	double calculate_delta_i_l(int i, int l, double **X, double ***W, int *d, double *Y, double **delta) {
		double sum = 0;
		for (int j = 1; j <= d[l]; ++j) {
			sum += W[l][i][j] * delta[l][j];
		}
		return (1 - pow(X[l-1][i], 2))*sum;
	}
	void updateW(int i, int j, int l, double ***W, double **X, double **delta, double biais) {
		W[l][i][j] = W[l][i][j] - biais * X[l - 1][i] * delta[l][j];
	}
	double calculate_delta_j_LastCouche_regression(int j, int l, double **X, double *Y) {
		return X[l][j]-Y[j];
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
		printf("\n\nRegression lineaire\nValeurs d'entree : ");

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

	void DebugMatrix(MatrixXd matrix, const char* txt) {
		printf("\n%s\n", txt);
		for (int i = 0; i < matrix.rows(); i++) {
			for (int j = 0; j < matrix.cols(); j++) {
				printf("%lf,", matrix(i, j));
			}
			printf("\n");
		}
	}

	__declspec(dllexport) int FitLinearRegression(double *model, double *arrayInputs, int inputsSize,
		int inputSize, double *outputs, int outputsSize) {
		//on init nos matrice en se basant sur les tableaux de double
		MatrixXd MatX(inputsSize, inputSize + 1);//avec une colone en plus
		for (int i = 0; i < inputsSize; i++) {
			for (int j = 1; j < inputSize + 1; j++) {
				MatX(i, j) = arrayInputs[(i*inputSize) + (j - 1)];
			}
			MatX(i, 0) = 1;//on rajoute le 1 dans la première colone
		}

		DebugMatrix(MatX, "matrice X");

		MatrixXd MatY(inputsSize, 1);
		for (int i = 0; i < inputsSize; i++) {
			MatY(i, 0) = outputs[i];
		}

		DebugMatrix(MatY, "matrice Y");

		MatrixXd XTransposed = MatX.transpose();

		DebugMatrix(XTransposed, "X^T");

		MatrixXd XTransposedXMatX = XTransposed * MatX;

		DebugMatrix(XTransposedXMatX, "(X^T)*X");

		MatrixXd InverseXTransposedXMatX = XTransposedXMatX.inverse();

		DebugMatrix(InverseXTransposedXMatX, "((X^T)*X)^-1");

		MatrixXd InverseXTransposedXMatXxTransposedXMat = InverseXTransposedXMatX * XTransposed;

		DebugMatrix(InverseXTransposedXMatXxTransposedXMat, "((X^T)*X)^-1 * (X^T)");

		MatrixXd result = InverseXTransposedXMatXxTransposedXMat * MatY;

		DebugMatrix(result, "(((X^T)*X)^-1 * (X^T)) * Y");

		//on renseigne le format de la matrice de resultat pour pouvoir parser le tableau de double qu'on vas renvoyer en matrice
		int colRetour = result.cols();
		int LineRetour = result.rows();

		//on parse la matrice en tableau de double
		for (int i = 0; i < LineRetour; i++) {
			for (int j = 0; j < colRetour; j++) {
				model[(i * colRetour) + j] = result(i, j);
			}
		}
		auto tmp = model[0];

		for (int i = 0; i < (LineRetour * colRetour - 1); i++) {
			model[i] = model[i + 1];
		}

		model[(LineRetour * colRetour) - 1] = tmp;
/*
		double temp = model[(LineRetour * colRetour) - 1];

		model[(LineRetour * colRetour) - 1] = model[0];

		model[0] = temp;*/

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
		double* model = CreateModel(5);

		double* array = new double[8];

		array[0] = 1;
		array[1] = 2;
		array[2] = 5;
		array[3] = 3;
		array[4] = 6;
		array[5] = 3;
		array[6] = 3;
		array[7] = 3;

		int res = FitLinearRegression(model, array, 4, 2, new double[4]{ 0.12, -0.9, -0.75, 0.5 }, 4);

		LinearRegression(model, new double[2]{ 3, 1 }, 2);

		char* temp = new char[50];
		scanf_s("%s", &temp);

	}
}