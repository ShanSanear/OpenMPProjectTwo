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
                ("n", "Number of divisions for specified calculus", cxxopts::value<int>())
                ("a", "Coefficient for x^2", cxxopts::value<double>())
                ("b", "Coefficient for x", cxxopts::value<double>())
                ("c", "Y-intercept of the parabola", cxxopts::value<double>())
                ("s,start", "Start of the calculation", cxxopts::value<double>())
                ("e,end", "End of the calculation", cxxopts::value<double>())
                ("p,processes", "Number of processes", cxxopts::value<int>()->default_value("4"))
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
        double start = result["s"].as<double>();
        double end = result["e"].as<double>();
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