#pragma once
#include "ActivateFunction.h"
#include "Matrix.h"
#include <fstream>
using namespace std;
struct data_NetWork {
	int L;
	int* size;
};
class NetWork
{
	int L;
	int* size;
	ActivateFunction actFunc;
	Matrix* weights;
	double** bios;
	double** neurons_val, ** neurons_err, ** _neurons_err;
	double* neurons_bios_val;
	//
	Matrix* ut;
	double** utbios;

	Matrix* m, * v;
	double** mbios, ** vbios;
public:
	void Init(data_NetWork data);
	void PrintConfig();
	void SetInput(double* values);

	double* ForwardFeed();
	int SearchMaxIndex(double* value);
	void PrintValues(int L);

	void SetUt();
	void BackPropogation(double expect);
	void _BackPropogation(double expect);
	void WeightsUpdater(double lr);
	void OptWeightsUpdater(double lr);
	void NWeightsUpdater(double lr);
	void AdagradWeightsUpdater(double lr);
	void AdadeltaWeightsUpdater(double lr);
	void AdamWeightsUpdater(double lr, int epoch, int p);

	void SaveWeights();
	void ReadWeights();
};

