#include <iostream>
#include <Eigen\Eigen>
extern "C" {
	using namespace std;
	using namespace Eigen;
	#include<stdlib.h>
	#include<stdio.h>
	#include<math.h>
	#include<ctime>

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
	///X : la matrice des points avec X et Z
	///Y : la matrice des points avec Y
	///W : le model resultant de l'opération du pdf
	///impossile de retourner un objet MatrixXd donc je parse la matrice en tableau de double avec colRetour et lineRetour pour préciser la taille des colonnes et ligne
	__declspec(dllexport) int LinearFirstRegression(double *X, int XCol, int XLin, double* Y,int YLin,double *W,int* colRetour,int* LineRetour) {
		//on init nos matrice en se basant sur les tableaux de double
		MatrixXd MatX(XLin, XCol+1);//avec une colone en plus
		for (int i = 0; i < XLin; i++) {
			for (int j = 0; j < XCol; j++) {
				MatX(i, j) = X[(i*XCol) + j];				
			}
			MatX(i, XCol) = 1;//on rajoute le 1 dans la dernière colone
		}
		MatrixXd MatY(YLin, 1);
		for (int i = 0; i< YLin; i++) {
			MatY(i, 0) = Y[i];
		}

		//calcule du pdf
		MatrixXd result = ((MatX.transpose()*MatX).inverse()*MatX.transpose())*MatY;

		//on renseigne le format de la matrice de resultat pour pouvoir parser le tableau de double qu'on vas renvoyer en matrice
		*colRetour = result.cols();
		*LineRetour = result.rows();

		//on parse la matrice en tableau de double
		W = new double[*colRetour * *LineRetour];
		for (int i = 0; i < *LineRetour; i++) {
			for (int j = 0; j < *colRetour; j++) {
				W[(i * *colRetour) + j] = result(i, j);
			}
		}
		return 1;
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

				printf("\nValeur attendue : %lf", outputs[i]);

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

	int main(int argc, char *argv[])
	{

		//double* model = CreateModel(3);

		double* fakeY = new double[9];

		fakeY[0] = 1;
		fakeY[1] = 2;
		fakeY[2] = 5;

		double *fakeX = new double[9];
	

		fakeX[0] = 0;
		fakeX[1] = 4;
		fakeX[2] = 5;
		fakeX[3] = 9;
		fakeX[4] = 7;
		fakeX[5] = 2;
		fakeX[6] = 1;
		fakeX[7] = 3;
		fakeX[8] = 0;

		double* resultat = new double[9];
		int *ligne=new int(), *col=new int();
 		LinearFirstRegression(fakeX, 3, 3, fakeY, 3, resultat, col, ligne);

		//int res = FitRosenblatt(model, array, 3, 2, new double[3] {1, -1, -1} , 3, 0.01, 500);

		//LinearClassification(model, new double[2]{ 3, 1 }, 2);
		//LinearClassification(model, new double[2]{ 1, 5 }, 2);

		//char* temp = new char[50];
		//scanf_s("%s", &temp);


	}
}