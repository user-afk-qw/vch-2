#include <iostream>
#include <cmath>
#include <functional>
#include <vector>

using namespace std;

double quadrature_uniform(double a, double b, const function<double(double)>& f) {
    double x[4];
    for (int i = 0; i < 4; ++i)
        x[i] = a + (b - a) * i / 3.0;

    double A[4] = { 1.0 / 8.0, 3.0 / 8.0, 3.0 / 8.0, 1.0 / 8.0 };

    double sum = 0.0;
    for (int i = 0; i < 4; ++i)
        sum += A[i] * f(x[i]);

    return sum * (b - a);
}

// ============================================================
// Пример 1: Интеграл от sin(x) на [0, pi]
// Точное значение = 2
// ============================================================
void example1_sin() {
    auto f = [](double x) { return sin(x); };
    double a = 0.0, b = 3.14;
    double result = quadrature_uniform(a, b, f);
    double exact = 2.0;
    cout << "Пример 1: ∫ sin(x) dx от 0 до π" << endl;
    cout << "Приближённо: " << result << endl;
    cout << "Точно:       " << exact << endl;
    cout << "Ошибка:      " << fabs(result - exact) << endl << endl;
}

// ============================================================
// Пример 2: Интеграл от x^3 на [0, 2]
// Точное значение = 4
// (формула точна для многочленов степени ≤ 3)
// ============================================================
void example2_cubic() {
    auto f = [](double x) { return x * x * x; };
    double a = 0.0, b = 2.0;
    double result = quadrature_uniform(a, b, f);
    double exact = 4.0;
    cout << "Пример 2: ∫ x^3 dx от 0 до 2" << endl;
    cout << "Приближённо: " << result << endl;
    cout << "Точно:       " << exact << endl;
    cout << "Ошибка:      " << fabs(result - exact) << " (должна быть 0)" << endl << endl;
}

// ============================================================
// Пример 3: Интеграл от exp(x) на [-1, 1]
// Точное значение = e - 1/e ≈ 2.350402387
// ============================================================
void example3_exp() {
    auto f = [](double x) { return exp(x); };
    double a = -1.0, b = 1.0;
    double result = quadrature_uniform(a, b, f);
    double exact = exp(1.0) - exp(-1.0);
    cout << "Пример 3: ∫ e^x dx от -1 до 1" << endl;
    cout << "Приближённо: " << result << endl;
    cout << "Точно:       " << exact << endl;
    cout << "Ошибка:      " << fabs(result - exact) << endl << endl;
}


// ============================================================
// Дополнительно: общая формула для любых 4 узлов (неравномерных)
// ============================================================
double quadrature_general(double a, double b, const vector<double>& x_nodes,
    const function<double(double)>& f) {
    if (x_nodes.size() != 4) {
        cerr << "Ошибка: нужно ровно 4 узла" << endl;
        return 0.0;
    }

    // Вычисляем веса A_k = ∫ L_k(x) dx от a до b
    vector<double> A(4, 0.0);

    for (int k = 0; k < 4; ++k) {
        // Подынтегральная функция L_k(x)
        auto Lk = [&](double x) {
            double res = 1.0;
            for (int j = 0; j < 4; ++j) {
                if (j != k)
                    res *= (x - x_nodes[j]) / (x_nodes[k] - x_nodes[j]);
            }
            return res;
        };

        // Численное интегрирование Lk от a до b (простая формула средних точек для простоты)
        int N = 1000;
        double h = (b - a) / N;
        double sum = 0.0;
        for (int i = 0; i < N; ++i) {
            double x_mid = a + (i + 0.5) * h;
            sum += Lk(x_mid);
        }
        A[k] = sum * h;
    }

    double result = 0.0;
    for (int i = 0; i < 4; ++i)
        result += A[i] * f(x_nodes[i]);

    return result;
}


// ============================================================
// main: раскомментируйте нужный вызов
// ============================================================
int main() {
    setlocale(LC_ALL, "Russian");
    // Раскомментируйте один из примеров:

    example1_sin();
    // example2_cubic();
    // example3_exp();


    return 0;
}
