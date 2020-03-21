#include <iostream>
#include <list>
#include <omp.h>
#include <cmath>
#include "../include/cxxopts.hpp"
#include "argument_parsing.cpp"

double calculate_quadratic_formula(double a, double b, double c, double x) {
    return a * pow(x, 2) + b * x + c;
}

void rectangular_method(int n, double a, double b, double c, double starting_point, double ending_point) {
    printf("Calculating using rectangular method\n");
    double calculated_integral = 0.0;
    double dx = (ending_point - starting_point) / n;
    double partial_value;
    double start_time = omp_get_wtime();
#pragma omp parallel for default(none) shared(a, b, c, starting_point, dx, n) private(partial_value) reduction(+:calculated_integral)
    for (int i = 0; i <= n; i++) {
        partial_value = calculate_quadratic_formula(a, b, c, starting_point + i * dx);
        calculated_integral += partial_value * dx;
    }
    double end_time = omp_get_wtime();
    printf("Calculation took %f seconds\n", end_time - start_time);
    printf("Calculated integral for limits from %f to %f is: %f\n", starting_point, ending_point, calculated_integral);
}

void trapezoidal_rule(int n, double a, double b, double c, double starting_point, double ending_point) {
    printf("Calculating using trapezoidal rule\n");
    double calculated_integral = 0.0;
    double dx = (ending_point - starting_point) / n;
    double partial_value;
    double start_time = omp_get_wtime();
    double value_for_start = calculate_quadratic_formula(a, b, c, starting_point);
    double value_for_end = calculate_quadratic_formula(a, b, c, ending_point);
    double average = (value_for_start + value_for_end);
    double point;
#pragma omp parallel for default(none) shared(a, b, c, starting_point, dx, n) private(partial_value, average, point) reduction(+:calculated_integral)
    for (int i = 0; i <= n; i++) {
        calculated_integral += calculate_quadratic_formula(a, b, c, starting_point + (i * dx));;
    }
    calculated_integral += average;
    calculated_integral *= dx;
    double end_time = omp_get_wtime();
    printf("Calculation took %f seconds\n", end_time - start_time);
    printf("Calculated integral for limits from %f to %f is: %f\n", starting_point, ending_point, calculated_integral);
}


int main(int argc, char *argv[]) {
    auto result = parse_arguments(argc, argv);
    int n = result["n"].as<int>();
    double a = result["a"].as<double>();
    double b = result["b"].as<double>();
    double c = result["c"].as<double>();
    double start = result["s"].as<double>();
    double end = result["e"].as<double>();
    int processes = result["p"].as<int>();
    omp_set_num_threads(processes);
    rectangular_method(n, a, b, c, start, end);
    trapezoidal_rule(n, a, b, c, start, end);
}
