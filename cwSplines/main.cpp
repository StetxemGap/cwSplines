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
		//a[i] = (function(x[i + 1]) - function(x[i])) / (x[i + 1] - x[i]);
		//b[i] = function(x[i]) - a[i] * x[i];

		a.push_back((y[i + 1] - y[i]) / (x[i + 1] - x[i]));
		b.push_back(y[i] - a[i] * x[i]);

		cout << "a: " << a[i] << "\tb: " << b[i] << endl;
		cout << "Линейная функция: " << a[i] << "x + " << b[i] << endl << endl;
	}
}

int main() {
	setlocale(LC_ALL, "Russian");

	vector<double> x = {0, 2, 4, 5};
	vector<double> y = {1, 3, 0, 2};
	linearSpline(x, y);
	return 0;
}