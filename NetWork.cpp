#include "NetWork.h"
void NetWork::Init(data_NetWork data) {
	actFunc.set();
	//srand(time(NULL));
	L = data.L;
	size = new int[L];
	for (int i = 0; i < L; i++)
		size[i] = data.size[i];

	weights = new Matrix[L - 1];
	bios = new double* [L - 1];
	for (int i = 0; i < L - 1; i++) {
		weights[i].Init(size[i + 1], size[i]);
		bios[i] = new double[size[i + 1]];
		weights[i].Rand();
		for (int j = 0; j < size[i + 1]; j++) {
			bios[i][j] = ((rand() % 50)) * 0.06 / (size[i] + 15);
		}
	}
	neurons_val = new double* [L]; neurons_err = new double* [L]; _neurons_err = new double* [L];
	for (int i = 0; i < L; i++) {
		neurons_val[i] = new double[size[i]]; neurons_err[i] = new double[size[i]]; _neurons_err[i] = new double[size[i]];
	}
	neurons_bios_val = new double[L - 1];
	for (int i = 0; i < L - 1; i++)
		neurons_bios_val[i] = 1;

	// Новое
	ut = new Matrix[L - 1];
	m = new Matrix[L - 1];
	v = new Matrix[L - 1];
	for (unsigned int i = 0; i < L - 1; i++) {
		ut[i].Init(size[i + 1], size[i]);
		m[i].Init(size[i + 1], size[i]);
		v[i].Init(size[i + 1], size[i]);
	}
	utbios = new double* [L - 1];
	mbios = new double* [L - 1];
	vbios = new double* [L - 1];
	for (int i = 0; i < L - 1; i++) {
		utbios[i] = new double[size[i + 1]];
		mbios[i] = new double[size[i + 1]];
		vbios[i] = new double[size[i + 1]];
		for (int j = 0; j < size[i + 1]; j++) {
			utbios[i][j] = 0;
			mbios[i][j] = 0;
			vbios[i][j] = 0;
		}
	}
}
void NetWork::PrintConfig() {
	cout << "***********************************************************\n";
	cout << "NetWork has " << L << " layers\nSIZE[]: ";
	for (int i = 0; i < L; i++) {
		cout << size[i] << " ";
	}
	cout << "\n***********************************************************\n\n";
}
void NetWork::SetInput(double* values) {
	for (int i = 0; i < size[0]; i++) {
		neurons_val[0][i] = values[i];
	}
}

double* NetWork::ForwardFeed() {
	for (int k = 1; k < L; ++k) {
		Matrix::Multi(weights[k - 1], neurons_val[k - 1], size[k - 1], neurons_val[k]);
		Matrix::SumVector(neurons_val[k], bios[k - 1], size[k]);
		actFunc.use(neurons_val[k], size[k]);
	}
	//int pred = SearchMaxIndex(neurons_val[L - 1]);
	return neurons_val[L - 1];
}
int NetWork::SearchMaxIndex(double* value) {
	double max = value[0];
	int prediction = 0;
	double tmp;
	for (int j = 1; j < size[L - 1]; j++) {
		tmp = value[j];
		if (tmp > max) {
			prediction = j;
			max = tmp;
		}
	}
	return prediction;
}
void NetWork::PrintValues(int L) {
	for (int j = 0; j < size[L]; j++) {
		cout << j << " " << neurons_val[L][j] << endl;
	}
}
void NetWork::SetUt() {
	for (int i = 0; i < L - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			utbios[i][j] = bios[i][j];
			for (int k = 0; k < size[i]; ++k) {
				ut[i](j, k) = weights[i](j, k);
			}
		}
	}
}
void NetWork::BackPropogation(double expect) {
	for (int i = 0; i < size[L - 1]; i++) {
		if (i != int(expect))
			neurons_err[L - 1][i] = -neurons_val[L - 1][i];
		else
			neurons_err[L - 1][i] = (1.0 - neurons_val[L - 1][i]);
	}
	for (int k = L - 2; k > 0; k--) {
		Matrix::Multi_T(weights[k], neurons_err[k + 1], size[k + 1], neurons_err[k]);
		for (int j = 0; j < size[k]; j++)
			neurons_err[k][j] *= actFunc.useDer(neurons_val[k][j]);
	}
}
void NetWork::_BackPropogation(double expect) {
	for (int i = 0; i < size[L - 1]; i++) {
		if (i != int(expect))
			neurons_err[L - 1][i] = -neurons_val[L - 1][i];
		else
			neurons_err[L - 1][i] = (1.0 - neurons_val[L - 1][i]);
		_neurons_err[L - 1][i] = neurons_err[L - 1][i];
	}
	for (int k = L - 2; k > 0; k--) {
		Matrix::Multi_T(weights[k], neurons_err[k + 1], size[k + 1], neurons_err[k]);
		for (int j = 0; j < size[k]; j++) {
			neurons_err[k][j] *= actFunc.useDer(neurons_val[k][j]);
			_neurons_err[k][j] = neurons_err[k][j];
		}
	}
}
void NetWork::WeightsUpdater(double lr) {
	for (int i = 0; i < L - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			bios[i][j] += neurons_err[i + 1][j] * lr;
			for (int k = 0; k < size[i]; ++k) {
				weights[i](j, k) += neurons_val[i][k] * neurons_err[i + 1][j] * lr;
			}
		}
	}
	//for (int i = 0; i < L - 1; i++) {
	//	for (int k = 0; k < size[i + 1]; k++) 
	//	{
	//		bios[i][k] += neurons_err[i + 1][k] * lr;
	//	}
	//}
}
void NetWork::OptWeightsUpdater(double lr) {
	for (int i = 0; i < L - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			if (neurons_err[i + 1][j] < 0 && _neurons_err[i + 1][j]>0 || neurons_err[i + 1][j] > 0 && _neurons_err[i + 1][j] < 0) {
				bios[i][j] = utbios[i][j] + _neurons_err[i + 1][j] * lr / 2.;
			}
			else
				bios[i][j] = bios[i][j] + neurons_err[i + 1][j] * lr;
			for (int k = 0; k < size[i]; ++k) {
				if (neurons_err[i + 1][j] < 0 && _neurons_err[i + 1][j]>0 || neurons_err[i + 1][j] > 0 && _neurons_err[i + 1][j] < 0) {
					weights[i](j, k) = ut[i](j, k) + neurons_val[i][k] * _neurons_err[i + 1][j] * lr / 2.;
				}
				else
					weights[i](j, k) = weights[i](j, k) + neurons_val[i][k] * neurons_err[i + 1][j] * lr;
			}
		}
	}
}
void NetWork::NWeightsUpdater(double lr) {
	for (int i = 0; i < L - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			utbios[i][j] = 0.5 * utbios[i][j] + lr * neurons_err[i + 1][j];
			bios[i][j] += utbios[i][j];
			for (int k = 0; k < size[i]; ++k) {
				ut[i](j, k) = 0.5 * ut[i](j, k) + neurons_val[i][k] * neurons_err[i + 1][j] * lr;
				weights[i](j, k) += ut[i](j, k);
			}
		}
	}
}
void NetWork::AdagradWeightsUpdater(double lr) {
	for (int i = 0; i < L - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			utbios[i][j] += neurons_err[i + 1][j] * neurons_err[i + 1][j];
			bios[i][j] += lr * neurons_err[i + 1][j] / (sqrt(utbios[i][j]) + 0.001);
			for (int k = 0; k < size[i]; ++k) {
				ut[i](j, k) += neurons_val[i][k] * neurons_err[i + 1][j] * neurons_val[i][k] * neurons_err[i + 1][j];
				weights[i](j, k) += lr * neurons_val[i][k] * neurons_err[i + 1][j] / (sqrt(ut[i](j, k)) + 0.001);
			}
		}
	}
}
void NetWork::AdadeltaWeightsUpdater(double lr) {
	double p = 0.9;
	for (int i = 0; i < L - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			utbios[i][j] = p * utbios[i][j] + (1 - p) * neurons_err[i + 1][j] * neurons_err[i + 1][j];
			bios[i][j] += lr * neurons_err[i + 1][j] / (sqrt(utbios[i][j] + 0.01));
			for (int k = 0; k < size[i]; ++k) {
				ut[i](j, k) = p * ut[i](j, k) + (1 - p) * neurons_val[i][k] * neurons_err[i + 1][j] * neurons_val[i][k] * neurons_err[i + 1][j];
				weights[i](j, k) += lr * neurons_val[i][k] * neurons_err[i + 1][j] / (sqrt(ut[i](j, k) + 0.01));
			}
		}
	}
}
void NetWork::AdamWeightsUpdater(double lr, int epoch, int p) {
	double beta1 = 0.9, beta2 = 0.999, eps = 1e-6;
	double m_hat, m_hat1, v_hat, v_hat1;
	for (int i = 0; i < L - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			mbios[i][j] = beta1 * mbios[i][j] - (1 - beta1) * neurons_err[i + 1][j];
			vbios[i][j] = beta2 * vbios[i][j] + (1 - beta2) * neurons_err[i + 1][j] * neurons_err[i + 1][j];
			if (epoch < 10) {
				m_hat = mbios[i][j] / (1 - beta1);
				v_hat = vbios[i][j] / (1 - beta2);
				bios[i][j] += -lr * m_hat / (sqrt(v_hat) + eps);
			}
			else {
				bios[i][j] += -lr * mbios[i][j] / (sqrt(vbios[i][j]) + eps);
			}
			for (int k = 0; k < size[i]; ++k) {
				m[i](j, k) = beta1 * m[i](j, k) - (1 - beta1) * neurons_val[i][k] * neurons_err[i + 1][j];
				v[i](j, k) = beta2 * v[i](j, k) + (1 - beta2) * neurons_val[i][k] * neurons_err[i + 1][j] * neurons_val[i][k] * neurons_err[i + 1][j];
				m_hat1 = m[i](j, k) / (1 - beta1);
				v_hat1 = v[i](j, k) / (1 - beta2);
				if (epoch < 10) {
					m_hat1 = m[i](j, k) / (1 - beta1);
					v_hat1 = v[i](j, k) / (1 - beta2);
					weights[i](j, k) += -lr * m_hat1 / (sqrt(v_hat1) + eps);
				}
				else {
					weights[i](j, k) += -lr * m[i](j, k) / (sqrt(v[i](j, k)) + eps);
				}
			}
		}
	}
}
void NetWork::SaveWeights() {
	ofstream fout;
	fout.open("Weights.txt");
	if (!fout.is_open()) {
		cout << "Error reading the file";
		system("pause");
	}
	for (int i = 0; i < L - 1; ++i)
		fout << weights[i] << " ";

	for (int i = 0; i < L - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			fout << bios[i][j] << " ";
		}
	}
	cout << "Weights saved \n";
	fout.close();
}
void NetWork::ReadWeights() {
	ifstream fin;
	fin.open("Weights.txt");
	if (!fin.is_open()) {
		cout << "Error reading the file";
		system("pause");
	}
	for (int i = 0; i < L - 1; ++i) {
		fin >> weights[i];
	}
	for (int i = 0; i < L - 1; ++i) {
		for (int j = 0; j < size[i + 1]; ++j) {
			fin >> bios[i][j];
		}
	}
	cout << "Weights readed \n";
	fin.close();
}