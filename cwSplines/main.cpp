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
	// return 3;
	// return 2 * x;
	// return x * x;
	 return x * x * x + 2 * x + 6;
	// return sin(x);
}

void err(vector<double> coef, vector<double> x, vector<double> y, string filename, bool isLinear) {
	ofstream file(filename);
	const double EPS = 1e-10;
	double pogr;
	double err;

	switch (isLinear)
	{
	case true:
		for (int i = 0, j = 0; i < x.size() - 1; i++, j++) {
			double a = x[i], b = x[i + 1];
			double h = (b - a) / 5;
			file << "Отрезок: (" << a << ", " << b << ")\n\n";
			//a += h;
			double sum = 0;
			int k = 0;
			while (a <= b + EPS) {
				pogr = abs(function(a) - (coef[j] * a + coef[j + 1]));
				file << " Точка: " << a << "   Погрешность: " << pogr << endl;
				sum += pogr * pogr;
				a += h;
				k++;
			}
			j++;
			err = sqrt(sum / k);
			file << "\nСреднеквадратичное отклонение: " << err << "\n\n";
		}
		break;
	case false:
		for (int i = 0, j = 0; i < x.size() - 1; i++, j++) {
			double a = x[i], b = x[i + 1];
			double h = (b - a) / 5;
			file << "Отрезок: (" << a << ", " << b << ")\n\n";
			double sum = 0;
			int k = 0;
			while (a <= b + EPS) {
				pogr = abs(function(a) - (coef[j] + coef[j + 1] * a + coef[j + 2] * a * a));
				file << " Точка: " << a << "   Погрешность: " << pogr << endl;
				sum += pogr * pogr;
				a += h;
				k++;
			}
			j += 2;
			err = sqrt(sum / k);
			file << "\nСреднеквадратичное отклонение: " << err << "\n\n";
		}
		break;
	default:
		break;
	}
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
	vector<double> coefs(2 * x.size() - 2);
	for (int i = 0, j = 0; i < 2 * x.size() - 2; i++, j++) {
		coefs[i] = ((y[j + 1] - y[j]) / (x[j + 1] - x[j]));
		coefs[i + 1] = (y[j] - coefs[i] * x[j]);
		i++;
	}

	output("lSpline.txt", coefs);
	err(coefs, x, y, "lErr.txt", true);
}

void quadraticSpline(vector<double> x, vector<double> y) {
	int n = 3 * x.size() - 3;

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

	err(LU(A, B), x, y, "qErr.txt", false);
}

void quadraticSplinev2(vector<double> x, vector<double> y) {
	int n = x.size();

	vector<double> coef(3 * n - 3);
	vector<double> beta(n);

	beta[0] = (y[1] - y[0]) / (x[1] - x[0]);

	for (int i = 0, j = 0; i < x.size() - 1; i++, j++) {
		coef[j + 2] = (y[i + 1] - y[i]) / (x[i + 1] - x[i]) * (x[i + 1] - x[i]) - beta[i] / (x[i + 1] - x[i]);
		coef[j + 1] = beta[i] - 2 * coef[j + 2] * x[i];
		coef[j] = y[i] - beta[i] * x[i] + coef[j + 2] * x[i] * x[i];
		beta[i + 1] = coef[j + 1] + 2 * coef[j + 2] * x[i + 1];
		j += 2;
	}
	output("qSplinev2.txt", coef);
	err(coef, x, y, "qErrv2.txt", false);
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
	quadraticSplinev2(x, y);
	return 0;
}