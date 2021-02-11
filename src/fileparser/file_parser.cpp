#include "file_parser.h"
#include<iostream>
#include<fstream>
#include <sstream>

using namespace std;

std::pair<string, map<std::string, double>> autodiff_file_reader(std::string filename) {

    std::pair<string, map<std::string, double>> result;

    string input;
    string function;
    ifstream infile;
    std::string segment;
    std::vector<std::string> variable_list;
    std::vector<double> value_list;
    map<std::string, double> inits;

    infile.open(filename);
    while(!infile.eof()) {// To get you all the lines.
        getline(infile,input); // Saves the line in STRING.
        std::cout<< input << std::endl; // Prints our STRING.
        if (input.find("f(") != std::string::npos && input.find("=") != std::string::npos) {
            std::stringstream test(input);
            
            std::vector<std::string> seglist;

            while(std::getline(test, segment, '=')) {
                seglist.push_back(segment);
            }

            std::cout << seglist[1] << std::endl;

            function = seglist[1];

            string raw_input_var = seglist[0];

            raw_input_var.erase(remove(raw_input_var.begin(), raw_input_var.end(), '('), raw_input_var.end());
            raw_input_var.erase(remove(raw_input_var.begin(), raw_input_var.end(), ')'), raw_input_var.end());
            raw_input_var.erase(remove(raw_input_var.begin(), raw_input_var.end(), 'f'), raw_input_var.end());
            raw_input_var.erase(remove(raw_input_var.begin(), raw_input_var.end(), ' '), raw_input_var.end());

            cout << raw_input_var << std::endl;

            std::stringstream var_stream(raw_input_var);
            while(std::getline(var_stream, segment, ',')) {
                variable_list.push_back(segment);
            }

        } else if (input.find("f(") != std::string::npos) {


            string raw_input_var = input;

            raw_input_var.erase(remove(raw_input_var.begin(), raw_input_var.end(), '('), raw_input_var.end());
            raw_input_var.erase(remove(raw_input_var.begin(), raw_input_var.end(), ')'), raw_input_var.end());
            raw_input_var.erase(remove(raw_input_var.begin(), raw_input_var.end(), 'f'), raw_input_var.end());
            raw_input_var.erase(remove(raw_input_var.begin(), raw_input_var.end(), ' '), raw_input_var.end());

            cout << raw_input_var << std::endl;

            std::stringstream var_stream(raw_input_var);
            while(std::getline(var_stream, segment, ',')) {
                value_list.push_back(stod(segment));
            }
        }

        for(int i = 0; i < value_list.size(); ++i) {
            inits[variable_list[i]] = value_list[i];
        }

    }
    infile.close();


    result.first = function;
    result.second = inits;

    return result;
}