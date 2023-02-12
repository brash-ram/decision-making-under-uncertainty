#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
using namespace std;

typedef vector<vector<float>> Matrix;

struct MatrixWithVector
{
	Matrix matrix;
	vector<float> vectorPriority;
};



long numberOfDigits(double n) {
	std::ostringstream strs;

	strs << n;
	return strs.str().size();
}

void printMatrix(const Matrix& M) {
	long max_len_per_column[M.size()];
	long n = M.size(), m = M[0].size();

	for (long j = 0; j < m; ++j) {
		long max_len {};

		for (long i = 0; i < n; ++i){
			long num_length {numberOfDigits(M[i][j])};
			if (num_length > max_len)
				max_len = num_length;
		}

		max_len_per_column[j] = max_len;
	}

	for (long i = 0; i < n; ++i)
		for (long j = 0; j < m; ++j)
			std::cout << (j == 0 ? "\n| " : "") << std::setw(max_len_per_column[j]) << M[i][j] << (j == m - 1 ? " |" : " ");

	std::cout << '\n';
}

void initializeMatrix(Matrix& mat) {
	for (int i=0; i<mat.size(); i++){
		for (int j=0; j<mat.size(); j++){
			mat[i][j] = 1;
		}
	}
}

float parseNumber(string line) {
	return (float) (line[0]-'0') / (float) (line[2]-'0');
}

Matrix enterMatrix(int numberParams) {
	Matrix matrix(numberParams, vector<float>(numberParams));
	string number;
	initializeMatrix(matrix);
	for (int i=0; i<numberParams-1; i++) {
		for (int j=i+1; j<numberParams; j++) {
			cout << "Relation " << i+1 << "\\" << j+1 << endl;
			cin >> number;
			if (number.size() == 1){	
				matrix[i][j] = stof(number);
				matrix[j][i] = 1/stof(number);
			}
			else {
				float numberFromInput = parseNumber(number);
				matrix[i][j] = numberFromInput;
				matrix[j][i] = 1/numberFromInput;
			}
		}
	}
	return matrix;
}

void createVectorPriority(MatrixWithVector& matrix) {
	vector<float> vectorGeometricMean(matrix.matrix.size());
	
	for (int i = 0; i < matrix.matrix.size(); i++) {
		float geometricMean = 1.0;
		int size =  matrix.matrix[0].size();

		for (int j = 0; j < size; j++) {
			geometricMean *= matrix.matrix[i][j];
		}
		vectorGeometricMean[i] = pow(geometricMean, (1/ (float) size));
	}
	float normalizingFactor = 0.0;
	for (int i = 0; i < vectorGeometricMean.size(); ++i) {
		normalizingFactor += vectorGeometricMean[i];
	}
	matrix.vectorPriority = vector<float>(matrix.matrix.size());
	for (int i = 0; i < matrix.matrix.size(); i++) {
		matrix.vectorPriority[i] = vectorGeometricMean[i]/normalizingFactor;
	}
}

vector<float> synthesisAlternatives(vector<MatrixWithVector>& matrixes) {
	vector<float> result = vector<float>(matrixes[1].vectorPriority.size());
	for (int i = 0; i < matrixes[1].vectorPriority.size(); ++i) {
		result[i] = 0;
		for (int j = 0; j < matrixes[0].vectorPriority.size(); ++j) {
			result[i] += matrixes[0].vectorPriority[j] * matrixes[j+1].vectorPriority[i];
		}
	}
	return result;
}

int main() {
	cout << "Enter number criteria" << endl;
    int numberK; cin >> numberK;
	cout << "Enter number of alternative" << endl;
	int numberA; cin >> numberA;

	vector<MatrixWithVector> listAllMatrix(numberK+1);

	cout << "Enter matrix of criteria" << endl;
	listAllMatrix[0].matrix = enterMatrix(numberK);
	printMatrix(listAllMatrix[0].matrix);
	createVectorPriority(listAllMatrix[0]);
	for (int j = 0; j < listAllMatrix[0].vectorPriority.size(); j++) {
		cout << "q" << j << " - " << listAllMatrix[0].vectorPriority[j] << "\t";
	}
	cout << endl;

	
	for (int i = 1; i < numberK+1; i++) {
		cout << endl << "Enter matrix of alternative on criteria "  << i << endl;
		listAllMatrix[i].matrix = enterMatrix(numberA);
		createVectorPriority(listAllMatrix[i]);
		printMatrix(listAllMatrix[i].matrix);
		for (int j = 0; j < listAllMatrix[i].vectorPriority.size(); j++) {
			cout << "q" << j << " - " << listAllMatrix[i].vectorPriority[j] << "\t";
		}
		cout << endl;
	}

	cout << "Full matrixes" << endl;
	for (int i = 0; i < numberK+1; ++i) {
		printMatrix(listAllMatrix[i].matrix);
		for (int j = 0; j < listAllMatrix[i].vectorPriority.size(); j++) {
			cout << "q" << j << " - " << listAllMatrix[i].vectorPriority[j] << "\t";
		}
		cout << endl;
	}
	vector<float> resultAlternative = synthesisAlternatives(listAllMatrix);
	cout << "Array of priorities of all alternatives" << endl;
	for (int i = 0; i < resultAlternative.size(); i++) {
		cout << "Priority of " << i+1 << " alternative is " << resultAlternative[i] << endl;
	}
	cout << endl << "Max priority is " << *max_element(resultAlternative.begin(), resultAlternative.end()) << endl;

    return 0;
}