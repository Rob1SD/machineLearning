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
	double calulateX_j_l(int l, int j, double **X, double ***W, int nbNeuroneCouchePrecedante) {
		double result = 0;
		for (int i = 0; i <= nbNeuroneCouchePrecedante; ++i) {
			result += W[l][j][i] * X[l - 1][i];
		}
		return tanh(result);
	}
	double calulateX_j_l_regression(int l, int j, double **X, double ***W, int nbNeuroneCouchePrecedante) {
		double result = 0;
		for (int i = 0; i <= nbNeuroneCouchePrecedante; ++i) {
			result += W[l][j][i] * X[l - 1][i];
		}
		return result;
	}
	double calculate_delta_i_l(int l, int i, double **X, double ***W, int *d, double **delta) {
		double sum = 0;
		for (int j = 0; j < d[l]; ++j) {
			sum += W[l][i][j] * delta[l][j];
		}
		return (1 - pow(X[l - 1][i], 2))*sum;
	}
	double updateW(int l, int i, int j, double ***W, double **X, double **delta, double alpha) {
		return W[l][i][j] - alpha * X[l - 1][i] * delta[l][j];
	}

	double calculate_delta_j_LastCouche_classification(int l, int j, double **X, double target) {
		return (1 - pow(X[l][j], 2))*(X[l][j] - target);
	}
	double calculate_delta_j_LastCouche_regression(int l, int j, double **X, double Y) {
		return X[l][j] - Y;
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
	double ***W;//les poids des neurones
	double **X;//les valeurs des neurones
	double **delta;//stock les delta
	int* neuroneParCouche;
	int nbCouche;
	bool classifOrRegress;
	__declspec(dllexport) double*  usePCM(double* data, int colSize) {

		for (int j = 0; j < colSize; j++) {
			X[0][j] = data[j];

		}
		for (int j = 0; j < nbCouche; j++) {
		
			if (j == 0) {//si on est sur la première couche caché, la couche précedante n'est pas contenue dans neuroneParCouche mais dans colSizeData
				for (int k = 0; k < neuroneParCouche[j]; k++) {
					if (classifOrRegress == 1) {
						X[j + 1][k] = calulateX_j_l(j + 1, k, X, W, colSize);
					}
					else {
						X[j + 1][k] = calulateX_j_l_regression(j + 1, k, X, W, colSize);

					}

				}
			}
			else {
				for (int k = 0; k < neuroneParCouche[j]; k++) {
					if (classifOrRegress == 1) {
						X[j + 1][k] = calulateX_j_l(j + 1, k, X, W, neuroneParCouche[j - 1]);
					}
					else {
						X[j + 1][k] = calulateX_j_l_regression(j + 1, k, X, W, neuroneParCouche[j - 1]);

					}
				}
			}
		}
		double* result = new double[neuroneParCouche[nbCouche - 1]];
		for (int i = 0; i < neuroneParCouche[nbCouche - 1]; i++) {
			result[i] = X[nbCouche ][i];
		}
		return result;
	}

	void resetNetwork() {
		delete[] W;
		delete[] X;
		delete[] delta;
		delete neuroneParCouche;
		nbCouche = 0;
	}
	__declspec(dllexport) void trainPCM(int *_neuroneParCouche, int _nbCouche, double* data, int colSizeData, int lineCountData,
		double *target, int colSizeTarget, int epoch, double apprentissage, bool _classifOrRegress) {
		neuroneParCouche = _neuroneParCouche;
		nbCouche = _nbCouche;
		classifOrRegress = _classifOrRegress;
		W = new double**[nbCouche + 1];//creer les couches + la première couche avec les data en "brut"

		W[0] = new double*[colSizeData + 1];//creer la première couche "manuellement" 		
		for (int h = 0; h < epoch; h++) {

			for (int i = 0; i < nbCouche; i++) {
				W[i + 1] = new double*[neuroneParCouche[i] + 1];//creer les neurones dans chaque couche + le biais
				for (int j = 0; j < neuroneParCouche[i]; j++) {

					//init tout les poids entre -1 et 1
					if (i == 0) {
						W[i + 1][j] = new double[colSizeData + 1];
						for (int k = 0; k < colSizeData + 1; k++) {
							W[i + 1][j][k] = randomDouble(-1, 1);
						}

					}
					else {
						W[i + 1][j] = new double[neuroneParCouche[i - 1] + 1];
						for (int k = 0; k < neuroneParCouche[i - 1] + 1; k++) {
							W[i + 1][j][k] = randomDouble(-1, 1);
						}
					}
				}

			}
			X = new double*[nbCouche + 1];//+1 rajoute la couche inital qui contient les données
			X[0] = new double[colSizeData + 1];// il y as autant de neurone dans la première couche qu'il y as de données à analyser + le biais
			X[0][colSizeData] = 1;//le biais est ici

			delta = new double*[nbCouche];


			//on met en place le tableaux de X et de delta
			for (int j = 0; j < nbCouche; j++) {//on commence à 1 pour compter la couche qu'on a init avec les données de base

				X[j + 1] = new double[neuroneParCouche[j] + 1];// neuroneparcouche +1 pour le biais et j+1 pour ignorer la première couche qu'on as fais amnuellement
				X[j + 1][neuroneParCouche[j]] = 1; //on met le biais qui se trouve à la fin de la couche à 1

				delta[j] = new double[neuroneParCouche[j]];

			}

			for (int i = 0; i < lineCountData; i++) {// pour chaque ligne de donnée pour entrainer
			//on remplie les neurones de la première couche avec les données de la ligne
				for (int j = 0; j < colSizeData; j++) {
					X[0][j] = data[(i*lineCountData) + j];

				}
				for (int j = 0; j < nbCouche; j++) {
					if (j == 0) {//si on est sur la première couche caché, la couche précedante n'est pas contenue dans neuroneParCouche mais dans colSizeData
						for (int k = 0; k < neuroneParCouche[j]; k++) {
							if (classifOrRegress == 1) {
								X[j + 1][k] = calulateX_j_l(j + 1, k, X, W, colSizeData);
							}
							else {
								X[j + 1][k] = calulateX_j_l_regression(j + 1, k, X, W, colSizeData);

							}

						}
					}
					else {
						for (int k = 0; k < neuroneParCouche[j]; k++) {
							if (classifOrRegress == 1) {
								X[j + 1][k] = calulateX_j_l(j + 1, k, X, W, neuroneParCouche[j - 1]);
							}
							else {
								X[j + 1][k] = calulateX_j_l_regression(j + 1, k, X, W, neuroneParCouche[j - 1]);

							}
						}
					}
				}
				//on determine les dernier delta
				for (int j = 0; j < colSizeTarget; j++) {
					if (classifOrRegress == 1) {

						delta[nbCouche - 1][j] =
							calculate_delta_j_LastCouche_classification(nbCouche,
								neuroneParCouche[nbCouche - 1] - 1,
								X,
								target[(lineCountData*i) + j]);
					}
					else {
						delta[nbCouche - 1][j] =
							calculate_delta_j_LastCouche_regression(nbCouche,
								neuroneParCouche[nbCouche - 1] - 1,
								X,
								target[(lineCountData*i) + j]);

					}
				}
				//on determine les delta des couches cachée
				for (int j = nbCouche - 2; j >= 0; j--) {
					for (int k = 0; k < neuroneParCouche[j]; k++) {
						delta[j][k] = calculate_delta_i_l(j + 1, k, X, W, neuroneParCouche, delta);

					}
				}
				for (int j = 1; j < nbCouche; j++) {
					for (int k = 0; k < neuroneParCouche[j - 1]; k++) {
						if (j == 1) {
							for (int l = 0; l < colSizeData; l++) {
								W[j][k][l] = updateW(j, k, l, W, X, delta, apprentissage);

							}

						}
						else {
							for (int l = 0; l < neuroneParCouche[j - 1]; l++) {
								updateW(j, k, l, W, X, delta, apprentissage);
							}

						}
					}
				}
			}
			printf("epoch %d OK\n", h + 1);
		}

		//les x[0] représente les resultat des neurone de la couche 0
		//x[0][1] représente le resultat du neurone 1 de la couche 0
		//pour determiner x[1][2] par exemple, fait la tanh de la somme ponderé de tout les x[0], multiplié par les poids de x[1][2]
		//quand on as ateint le fond du reseau, sur les x[3] si on as 4 couche, on fais la backpropagation
		//les y[n] sont les target attendu. il y as autant de n que de colones attendu en réponse, aka le nombre de neurone de sortie

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
		//double* model = CreateModel(5);

		//double* array = new double[8];

		//array[0] = 1;
		//array[1] = 2;
		//array[2] = 5;
		//array[3] = 3;
		//array[4] = 6;
		//array[5] = 3;
		//array[6] = 3;
		//array[7] = 3;

		//int res = FitLinearRegression(model, array, 4, 2, new double[4]{ 0.12, -0.9, -0.75, 0.5 }, 4);

		//LinearRegression(model, new double[2]{ 3, 1 }, 2);

		//char* temp = new char[50];
		//scanf_s("%s", &temp);
		int* neuronesTest = new int[4];
		neuronesTest[0] = 2;
		neuronesTest[1] = 5;
		neuronesTest[2] = 5;
		neuronesTest[3] = 1;
		double *fakeData = new double[20];
		fakeData[0] = .2;
		fakeData[1] = 1.2;
		fakeData[2] = 3.2;
		fakeData[3] = 4.2;
		fakeData[4] = 5.2;
		fakeData[5] = 9.2;
		fakeData[6] = 12.2;
		fakeData[7] = 5.2;
		fakeData[8] = .2;
		fakeData[9] = 4.2;

		fakeData[10] = .2;
		fakeData[11] = 1.2;
		fakeData[12] = 3.2;
		fakeData[13] = 4.2;
		fakeData[14] = 5.2;
		fakeData[15] = 9.2;
		fakeData[16] = 12.2;
		fakeData[17] = 5.2;
		fakeData[18] = 0.2;
		fakeData[19] = 4.2;

		double* fakeTarget = new double[2];
		fakeTarget[0] = 1.2;
		fakeTarget[1] = 7.2;

		int* neuronebouboule = new int[1];
		neuronebouboule[0] = 3;

		double* fakedatabouboule = new double[6];
		fakedatabouboule[0] = 1;
		fakedatabouboule[0] = -6.05;
		fakedatabouboule[0] = 7;
		fakedatabouboule[0] = 5;
		fakedatabouboule[0] = -5.98;
		fakedatabouboule[0] = 5;

		double *faketargetbouboule = new double[9];
		faketargetbouboule[0] = 0;
		faketargetbouboule[1] = 0;
		faketargetbouboule[2] = 1;
		faketargetbouboule[3] = 0;
		faketargetbouboule[4] = 1;
		faketargetbouboule[5] = 0;
		faketargetbouboule[6] = 1;
		faketargetbouboule[7] = 0;
		faketargetbouboule[8] = 0;

		//trainPCM(neuronesTest, 4, fakeData, 2, 2, fakeTarget, 1, 5, .1, 0);
		trainPCM(neuronebouboule, 1, fakedatabouboule, 2, 3, fakeTarget, 3, 10, .01, 1);
		double *test = new double[2];
		test[0] = 9;
		test[1] = 7;
		double* result = usePCM(test, 2);
		printf("compilation OK. appuyez sur ENTREE pour fermer...\n");
		getchar();
	}
}