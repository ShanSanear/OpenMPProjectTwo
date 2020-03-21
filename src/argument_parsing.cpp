//
// Created by PruskiM on 2020-03-21.
//

#include "../include/cxxopts.hpp"
#include <list>
#include <iostream>

void show_help_and_exit(cxxopts::Options options, int exit_code) {
    std::cout << options.help({""}) << std::endl;
    exit(exit_code);
}

auto parse_arguments(int argc, char **argv) {
    try {
        cxxopts::Options options(argv[0]);
        options
                .positional_help("[optional args]")
                .show_positional_help();
        options.add_options()
                ("n", "Number of divisions for specified calculus", cxxopts::value<int>(), "int")
                ("a", "Coefficient for x^2", cxxopts::value<long double>(), "double")
                ("b", "Coefficient for x", cxxopts::value<long double>(), "double")
                ("c", "Y-intercept of the parabola", cxxopts::value<long double>(), "double")
                ("s,start", "Start of the calculation", cxxopts::value<long double>(), "double, X0")
                ("e,end", "End of the calculation", cxxopts::value<long double>(), "double, X1")
                ("p,processes", "Number of processes", cxxopts::value<int>()->default_value("4"), "int")
                ("m,method", "Methods to be run", cxxopts::value<std::string>()->default_value("both"), "string")
                ("help", "Print help");
        auto result = options.parse(argc, argv);
        auto arguments = result.arguments();
        int num_of_arguments = arguments.size();
        if (result.count("help") | (num_of_arguments == 0)) {
            show_help_and_exit(options, 0);
        }
        std::vector<std::string> expected_parameters = {"n", "a", "b", "c", "s", "e"};
        for (const std::string &parameter : expected_parameters) {
            if (result.count(parameter) == 0) {
                printf("Parameter '%s' wasn't provided\n", parameter.c_str());
                show_help_and_exit(options, 1);
            }
            if (result.count(parameter) > 1) {
                printf("Parameter '%s' was provided more than once\n", parameter.c_str());
                show_help_and_exit(options, 2);
            }
        }
        long double start = result["s"].as<long double>();
        long double end = result["e"].as<long double>();
        if (start >= end) {
            printf("'s' ('start') must be greater than 'e' ('end')");
            exit(3);
        }
        return result;
    }
    catch (const cxxopts::OptionException &e) {
        std::cout << "error parsing options: " << e.what() << std::endl;
        exit(1);
    }
}