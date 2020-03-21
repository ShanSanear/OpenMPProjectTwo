#include <iostream>
#include <list>
#include <omp.h>
#include <cmath>
#include "../include/cxxopts.hpp"
#include "argument_parsing.cpp"

long double calculate_quadratic_formula(long double a, long double b, long double c, long double x) {
    return a * powl(x, 2) + b * x + c;
}

void rectangular_method(int n, long double a, long double b, long double c, long double starting_point,
                        long double ending_point) {
    printf("Calculating using rectangular method\n");
    long double calculated_integral = 0.0;
    long double dx = (ending_point - starting_point) / n;
    long double start_time = omp_get_wtime();
#pragma omp parallel for default(none) shared(a, b, c, starting_point, dx, n) reduction(+:calculated_integral)
    for (int i = 1; i < n; i++) {
        calculated_integral += calculate_quadratic_formula(a, b, c, starting_point + i * dx) * dx;
    }
    long double end_time = omp_get_wtime();
    printf("Calculation took %Lf seconds\n", end_time - start_time);
    printf("Calculated integral for limits from %Lf to %Lf is: %.15Lf\n", starting_point, ending_point,
           calculated_integral);
}

void trapezoidal_rule(int n, long double a, long double b, long double c, long double starting_point,
                      long double ending_point) {
    printf("Calculating using trapezoidal rule\n");
    long double calculated_integral = 0.0;
    long double dx = (ending_point - starting_point) / n;
    long double partial_value;
    long double start_time = omp_get_wtime();
    long double value_for_start = calculate_quadratic_formula(a, b, c, starting_point);
    long double value_for_end = calculate_quadratic_formula(a, b, c, ending_point);
    long double average = (value_for_start + value_for_end) / 2;
    long double point;
#pragma omp parallel for default(none) shared(a, b, c, starting_point, dx, n) private(partial_value, average, point) reduction(+:calculated_integral)
    for (int i = 1; i < n; i++) {
        calculated_integral += (calculate_quadratic_formula(a, b, c, starting_point + (i * dx)));
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
    omp_set_num_threads(processes);
    if (method == "both" || method == "rec") {
        rectangular_method(n, a, b, c, start, end);
    }
    if (method == "both" || method == "trap") {
        trapezoidal_rule(n, a, b, c, start, end);
    }
}
