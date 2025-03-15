#include <iostream >
#include <vector>

using namespace std;

double function(double x) {
	return x;
	//return x * x;
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

void quadraticSpline(vector<double> x, vector<double> y) {
	int n = x.size() - 1;
	vector<double> a(n);
	vector<double> b(n);
	vector<double> c(n);

	// естественные граничные условия
	a[0] = 0;
	a[n - 1] = 0;

	for (int i = 0; i < x.size(); i++) {
		if (i == 0) {

		}
	}
}

void LU(vector<vector<double>> u) {
	int nRows = u.size();
	int nCols = u[0].size();
	//создаем матрицу L имеющую только 2 диагонали
	vector<vector<double>> l(nRows, vector<double> (2));

	//инициализация матрицы L изначально единичной
	for (int i = 0; i < nRows; i++) {
		l[i][1] = 1;
	}

	for (int i = 1; i < nRows; i++) {
		l[i][0] = u[i][0] / u[i - 1][1];
		for (int j = 0; j < nCols - 2; j++) {
			u[i][j] = u[i][j] - l[i][0] * u[i - 1][j + 1];
		}
	}
	
	// прямой ход
	vector<double> b = { 1, 1, 1, 1, 1, 1 };
	vector<double> y(nRows);

	y[0] = b[0] / l[0][1];
	for (int i = 1; i < nRows; i++) {
		y[i] = (b[i] - l[i][0] * y[i - 1]) / l[i][1];
	}

	// обратный ход
	vector<double> x(nRows);

	x[nRows - 1] = y[nRows - 1] / u[nRows - 1][1];
	x[nRows - 2] = (y[nRows - 2] - u[nRows - 2][2] * x[nRows - 1]) / u[nRows - 2][1];
	for (int i = nRows - 3; i > -1; i--) {
		x[i] = (y[i] - (u[i][2] * x[i + 1] + u[i][3] * x[i + 2])) / u[i][1];
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

int main() {
	setlocale(LC_ALL, "Russian");
	vector<vector<double>> u = { {0, 4, 15, 0}, {1, 5, 11, 0}, {0, 2, 0, 9}, {0, 7, 22, 0}, {1, 2, 12, 0}, {0, 6, 0, 0} };
	LU(u);
	//vector<double> x = { 5, 10, 15, 20, 25, 30 };
	//vector<double> y = { 5, 15, 10, 3, 20, 7 };
	//linearSpline(x, y);
	return 0;
}