#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<vector<double>> inputMatrix(string fileName) {
	ifstream file(fileName);

	int rows, cols;
	file >> rows;
	file >> cols;

	vector<vector<double>> matrix(rows, vector<double>(cols));

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			file >> matrix[i][j];
		}
	}
	return matrix;
}

vector<double> inputVector(string fileName) {
	ifstream file(fileName);

	int size;
	file >> size;

	vector<double> B(size);

	for (int i = 0; i < size; i++) {
		file >> B[i];
	}
	return B;
}

double function(double x) {
	//return x;
	return x * x;
}

void LU(vector<vector<double>> u, vector<double> b) {
	int nRows = u.size();
	int nCols = u[0].size();
	// создаем матрицу L имеющую только 2 диагонали
	vector<vector<double>> l(nRows, vector<double>(2));

	// инициализация матрицы L изначально единичной
	for (int i = 0; i < nRows; i++) {
		l[i][1] = 1;
	}
	// вычисление элементов матриц U и L	
	for (int i = 1; i < nRows; i++) {
		l[i][0] = u[i][0] / u[i - 1][1];
		for (int j = 0; j < nCols - 2; j++) {
			u[i][j] = u[i][j] - l[i][0] * u[i - 1][j + 1];
		}
	}

	// прямой ход
	vector<double> y(nRows);

	y[0] = b[0] / l[0][1];
	for (int i = 1; i < nRows; i++) {
		y[i] = b[i] - l[i][0] * y[i - 1];
	}

	// обратный ход
	vector<double> x(nRows);

	x[nRows - 1] = y[nRows - 1] / u[nRows - 1][1];
	x[nRows - 2] = (y[nRows - 2] - u[nRows - 2][2] * x[nRows - 1]) / u[nRows - 2][1];
	x[nRows - 3] = (y[nRows - 3] - u[nRows - 3][2] * x[nRows - 2] - u[nRows - 3][3] * x[nRows - 1]) / u[nRows - 3][1];
	int sum;
	for (int i = nRows - 4; i > -1; i--) {
		x[i] = (y[i] - u[i][2] * x[i + 1] - u[i][3] * x[i + 2] - u[i][4] * x[i + 3]) / u[i][1];
	}
	//cout << "U" << endl;
	//for (int i = 0; i < nRows; i++) {
	//	for (int j = 0; j < nCols; j++) {
	//		cout << u[i][j] << " ";
	//	}
	//	cout << endl;
	//}
	//cout << endl << "L" << endl;
	//for (int i = 0; i < nRows; i++) {
	//	for (int j = 0; j < 2; j++) {
	//		cout << l[i][j] << " ";
	//	}
	//	cout << endl;
	//}
	//cout << endl << "y" << endl;
	//for (int i = 0; i < nRows; i++) {
	//	cout << y[i] << " ";
	//}
	//cout << endl << "x" << endl;
	//for (int i = 0; i < nRows; i++) {
	//	cout << x[i] << " ";
	//}
}

void linearSpline(vector<double> x, vector<double> y) {
	vector<double> a; // угловой коэффициент
	vector<double> b; // свободный член

	for (int i = 0; i < x.size() - 1; i++) {
		a.push_back((y[i + 1] - y[i]) / (x[i + 1] - x[i]));
		b.push_back(y[i] - a[i] * x[i]);

		cout << "a: " << a[i] << "\tb: " << b[i] << endl;
		cout << "Линейная функция: " << a[i] << "x + " << b[i] << endl << endl;
	}
}

void quadraticSpline(vector<double> x) {
	int n = 3 * x.size() - 3;

	// коэффициенты сплайна
	vector<double> a(n/3);
	vector<double> b(n/3);
	vector<double> c(n/3);

	// вектор правой части
	vector<double> B(n);

	// вычисляем вектор правой части
	int delta = 0; // переменная для расчета f(xi) 
	for (int i = 0; i < n - 2; i++) {
		if ((i + 1) % 3 == 0) {
			B[i] = 0;
		}
		else {
			B[i] = function(x[i - delta]);
			B[i + 1] = function(x[i + 1 - delta]);
			i++;
			delta += 2;
		}
	}
	// граничные условия - вторая производная в x[n] равна значению функции в этой точке
	B[n - 1] = function(x[x.size() - 1]);
	//cout << "B" << endl;
	//for (int i = 0; i < n; i++) {
	//	cout << B[i] << " ";
	//}

	// составляем ленточную матрицу
	vector<vector<double>> A(n, vector<double>(5));
	delta = 0;
	for (int i = 0; i < n - 2; i++) {
		if ((i + 1) % 3 == 0) {
			A[i][0] = 1;
			A[i][1] = 2 * x[i - delta - 1];
			A[i][2] = 0;
			A[i][3] = -1;
			A[i][4] = -2 * x[i - delta - 1];
			delta += 2;
		}
		else {
			for (int j = 0; j < 3; j++) {
				A[i][j + 1] = pow(x[i - delta], j);
				A[i + 1][j] = pow(x[i + 1 - delta], j);
			}
			i++;
		}
	}
	// граничные условия - вторая производная в x[n] равна значению функции в этой точке
	A[n - 1][1] = 2;

	//cout << "A" << endl;
	//for (int i = 0; i < n; i++) {
	//	for (int j = 0; j < 5; j++) {
	//		cout << A[i][j] << " ";
	//	}
	//	cout << endl;
	//}

	LU(A, B);
}

int main() {
	setlocale(LC_ALL, "Russian");
	//LU(inputMatrix("matrix.txt"), inputVector("vector.txt"));
	vector<double> x = { 0, 1, 2, 3 };
	quadraticSpline(x);
	//vector<double> y = { 5, 15, 10, 3, 20, 7 };
	//linearSpline(x, y);
	return 0;
}