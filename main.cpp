#include <iostream>
#include <omp.h>
#include <cmath>
#include "include/cxxopts.hpp"

double A = 12.0;
double B = 10.0;
double C = 0;
const int N = 100000000;
// Cin Fail <- check for type

void parse(int argc, char *argv[]) {
    try {
        cxxopts::Options options(argv[0], " - example command line options");
        options.add_options()
                ("n", "N value", cxxopts::value<double>()->default_value("5.0"));
        auto result = options.parse(argc, argv);
        if (result.count("n")) {
            printf("Found n: %f\n", result["n"].as<double>());
        } else {
            printf("Not found n parameter");
        }

    }
    catch (const cxxopts::OptionException &e) {
        std::cout << "error parsing options: " << e.what() << std::endl;
        exit(1);
    }
}

double calculate_quadratic_function(double a, double b, double c, double x) {
    return a * pow(x, 2) + b * x + c;
}

void test_reduction() {
    double starting_point = -2000.0;
    double ending_point = 2000.0;
    double calculated_integral = 0.0;
    double dx = (ending_point - starting_point) / N;
    double partial_value;
    double start_time = omp_get_wtime();
#pragma omp parallel for default(none) shared(A, B, C, starting_point, dx) private(partial_value) reduction(+:calculated_integral)
    for (int i = 0; i <= N; i++) {
        partial_value = calculate_quadratic_function(A, B, C, starting_point + i * dx);
        calculated_integral += partial_value * dx;
    }
    double end_time = omp_get_wtime();
    printf("It took %f seconds\n", end_time - start_time);
    printf("Calculated integral for limits from %f to %f is: %f", starting_point, ending_point, calculated_integral);
}


int main(int argc, char *argv[]) {
    parse(argc, argv);
//    test_reduction();
}
