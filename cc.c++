#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class CubicSpline {
private:
    vector<double> x, y;
    vector<double> a, b, c, d;
    int n;
    
public:
    CubicSpline(const vector<double>& x_points, const vector<double>& y_points) {
        x = x_points;
        y = y_points;
        n = x.size() - 1;
        
        a.resize(n + 1);
        b.resize(n);
        c.resize(n + 1);
        d.resize(n);
        
        // Шаги между точками
        vector<double> h(n);
        for (int i = 0; i < n; i++) {
            h[i] = x[i + 1] - x[i];
        }
        
        // Решение системы для коэффициентов c
        vector<double> alpha(n);
        for (int i = 1; i < n; i++) {
            alpha[i] = 3.0 * (y[i + 1] - y[i]) / h[i] - 3.0 * (y[i] - y[i - 1]) / h[i - 1];
        }
        
        vector<double> l(n + 1), mu(n + 1), z(n + 1);
        l[0] = 1.0; mu[0] = 0.0; z[0] = 0.0;
        
        for (int i = 1; i < n; i++) {
            l[i] = 2.0 * (x[i + 1] - x[i - 1]) - h[i - 1] * mu[i - 1];
            mu[i] = h[i] / l[i];
            z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
        }
        
        l[n] = 1.0; z[n] = 0.0; c[n] = 0.0;
        
        for (int i = n - 1; i >= 0; i--) {
            c[i] = z[i] - mu[i] * c[i + 1];
            a[i] = y[i];
            b[i] = (y[i + 1] - y[i]) / h[i] - h[i] * (c[i + 1] + 2.0 * c[i]) / 3.0;
            d[i] = (c[i + 1] - c[i]) / (3.0 * h[i]);
        }
        a[n] = y[n];
    }
    
    double interpolate(double x_val) {
        // Поиск нужного интервала
        int i = 0;
        while (i < n && x_val > x[i + 1]) i++;
        
        double dx = x_val - x[i];
        return a[i] + b[i] * dx + c[i] * dx * dx + d[i] * dx * dx * dx;
    }
};

int main() {
    int N;
    cout << "Введите количество точек: ";
    cin >> N;
    
    vector<double> x(N), y(N);
    
    cout << "Введите точки (x y):" << endl;
    for (int i = 0; i < N; i++) {
        cin >> x[i] >> y[i];
    }
    
    // Создаем сплайн
    CubicSpline spline(x, y);
    
    // Вводим границы и шаг
    double xmin, xmax, step;
    cout << "Введите xmin, xmax, step: ";
    cin >> xmin >> xmax >> step;
    
    // Выводим результаты
    cout << "\nРезультаты интерполяции:" << endl;
    cout << "x\t\ty" << endl;
    
    for (double x = xmin; x <= xmax; x += step) {
        double y_val = spline.interpolate(x);
        cout << x << "\t\t" << y_val << endl;
    }
    
    return 0;
}
