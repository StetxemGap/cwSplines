#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<double> input(string fileName) {
	ifstream file(fileName);

	double h;
	file >> h;

	double a, b;
	file >> a >> b;

	vector<double> x;

	while (a <= b) {
		x.push_back(a);
		a += h;
	}

	return x;
}

void output(string fileName, vector<double> coef) {
	ofstream file(fileName);

	for (int i = 0; i < coef.size(); i++) {
		file << coef[i] << " ";
	}
}

double function(double x) {
	 return 3;
	// return 2 * x;
	// return x * x;
	// return x * x * x;
	// return sin(x);
}

 vector<double> LU(vector<vector<double>> u, vector<double> b) {
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
	output("qSpline.txt", x);
	return x;
}

void linearSpline(vector<double> x, vector<double> y) {
	vector<double> a; // угловой коэффициент
	vector<double> b; // свободный член

	for (int i = 0; i < x.size() - 1; i++) {
		a.push_back((y[i + 1] - y[i]) / (x[i + 1] - x[i]));
		b.push_back(y[i] - a[i] * x[i]);

		output("lSpline_a.txt", a);
		output("lSpline_b.txt", b);
	}
}

void quadraticSpline(vector<double> x, vector<double> y) {
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
			B[i] = y[i - delta];
			B[i + 1] = y[i + 1 - delta];
			i++;
			delta += 2;
		}
	}
	// граничные условия - вторая производная в x[n] равна 0
	B[n - 1] = 0;

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
	// граничные условия - вторая производная в x[n] равна 0
	A[n - 1][1] = 2;

	LU(A, B);
}

void err(vector<double> coef, vector<double> x) {

}

int main() {
	setlocale(LC_ALL, "Russian");
	vector<double> x = input("X.txt");
	int n = x.size();
	vector<double> y;
	for (int i = 0; i < n; i++) {
		y.push_back(function(x[i]));
	}
	quadraticSpline(x, y);
	linearSpline(x, y);
	vector<double> coef(3 * n - 3);
	err(coef, x);
	return 0;
}