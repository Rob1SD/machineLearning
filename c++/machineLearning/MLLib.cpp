extern "C" {
	using namespace std;
	__declspec(dllexport) double *CreateModel(int inputdimension) {
		return new double[inputdimension];
	}
	__declspec(dllexport) void RemoveLinearModel(double *model) {
		delete[] model;  
	}
	__declspec(dllexport) int LinearFirstRegression(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int outputsSize) {
		return 888;
	}
	__declspec(dllexport) int FitRosenblatt(double *modem, double *inputs, int inputsSize, double *outputs, int outputsSize) {
		return 555;
	}
	__declspec(dllexport) double LinearClassification(double *model, double *input, int inputSize) {
		return .25;
	}
	__declspec(dllexport) double LinearRegression(double *model, double *input, int inputSize) {
		return .5;
	}
}