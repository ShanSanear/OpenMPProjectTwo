#include <iostream>
#include <omp.h>
#include <cmath>
#include "argument_parsing.cpp"

class IntegralCalculator {
private:
    long double a, b, c;
    long double starting_point, ending_point;
    long double dx;
    int n;

    [[nodiscard]] long double calculate_formula(long double x) const;

public:
    IntegralCalculator(long double a, long double b, long double c, int n, long double starting_point,
                       long double ending_point);

    void calculate_rectangular_method();

    void calculate_trapezoidal_rule();

};


IntegralCalculator::IntegralCalculator(long double a, long double b, long double c, int n, long double starting_point,
                                       long double ending_point) {
    this->a = a;
    this->b = b;
    this->c = c;
    this->starting_point = starting_point;
    this->ending_point = ending_point;
    this->n = n;
    this->dx = (ending_point - starting_point) / n;
}

long double IntegralCalculator::calculate_formula(long double x) const {
    return a * powl(x, 2) + b * x + c;
}

void IntegralCalculator::calculate_rectangular_method() {
    printf("Calculating using rectangular method\n");
    long double calculated_integral = 0.0;
    long double start_time = omp_get_wtime();
#pragma omp parallel for default(none) shared(a, b, c, starting_point, dx, n) reduction(+:calculated_integral)
    for (int i = 1; i < n; i++) {
        calculated_integral += calculate_formula(starting_point + i * dx) * dx;
    }
    long double end_time = omp_get_wtime();
    printf("Calculation took %Lf seconds\n", end_time - start_time);
    printf("Calculated integral for limits from %Lf to %Lf is: %.15Lf\n", starting_point, ending_point,
           calculated_integral);
}

void IntegralCalculator::calculate_trapezoidal_rule() {
    printf("Calculating using trapezoidal rule\n");
    long double calculated_integral = 0.0;
    long double start_time = omp_get_wtime();
    long double value_for_start = calculate_formula(starting_point);
    long double value_for_end = calculate_formula(ending_point);
    long double average = (value_for_start + value_for_end) / 2;
#pragma omp parallel for default(none) shared(a, b, c, starting_point, dx, n) reduction(+:calculated_integral)
    for (int i = 1; i < n; i++) {
        calculated_integral += (calculate_formula(starting_point + (i * dx)));
    }
    calculated_integral += average;
    calculated_integral *= dx;
    long double end_time = omp_get_wtime();
    printf("Calculation took %Lf seconds\n", end_time - start_time);
    printf("Calculated integral for limits from %Lf to %Lf is: %.15Lf\n", starting_point, ending_point,
           calculated_integral);
}


int main(int argc, char *argv[]) {
    auto result = parse_arguments(argc, argv);
    int n = result["n"].as<int>();
    long double a = result["a"].as<long double>();
    long double b = result["b"].as<long double>();
    long double c = result["c"].as<long double>();
    long double start = result["s"].as<long double>();
    long double end = result["e"].as<long double>();
    int processes = result["p"].as<int>();
    std::string method = result["m"].as<std::string>();
    IntegralCalculator calculator(a, b, c, n, start, end);
    omp_set_num_threads(processes);
    if (method == "both" || method == "rec") {
        calculator.calculate_rectangular_method();
    }
    if (method == "both" || method == "trap") {
        calculator.calculate_trapezoidal_rule();
    }
}
