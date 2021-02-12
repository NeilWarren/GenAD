
#ifndef MISHRA_WARREN_AUTODIFF_PREPROCESSOR_H
#define MISHRA_WARREN_AUTODIFF_PREPROCESSOR_H

#include "../sys.h"
using namespace std;

class var_map {

public:

    std::pair<string, map<std::string, double>> inputs;
    string out_func;
    map<string, double> out_inits;
    map<string, string> vmap;

    var_map(std::pair<string, map<std::string, double>> i){
        this->inputs = i;
        string input = i.first;
        map<std::string, double> var_in = i.second;
        int var_count = 0;

        if (!var_in.empty()){
            for (auto &it : var_in) {
                string orig_string = it.first;
                char n = '0' + var_count;
                var_count++;
                string new_var = "x";
                new_var.push_back(n);
                double orig_val = it.second;
                vmap.insert({orig_string, new_var});
                out_inits.insert({new_var, orig_val });

                int index;
                while((index = input.find(orig_string)) != string::npos) {
                    input.replace(index,new_var.length(), new_var);
                }


            }
        }
        this->out_func = input;

    }



};


#endif //MISHRA_WARREN_AUTODIFF_PREPROCESSOR_H
