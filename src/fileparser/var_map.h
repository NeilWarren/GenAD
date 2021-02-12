
#ifndef MISHRA_WARREN_AUTODIFF_PREPROCESSOR_H
#define MISHRA_WARREN_AUTODIFF_PREPROCESSOR_H

#include "../sys.h"
using namespace std;
bool cmp(pair<string, double>& a,
         pair<string, double>& b) {
    return a.first.length() > b.first.length();
}
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

        vector<pair<string, double> > A;

        for (auto& it : var_in) {
            A.emplace_back(it);
        }

        sort(A.begin(), A.end(), cmp);

        if (!var_in.empty()){
            for (auto &it : A) {
                string orig_string = it.first;
                string new_var = "$" + to_string(var_count);
                var_count++;
                double orig_val = it.second;
                vmap.insert({new_var, orig_string});
                out_inits.insert({new_var, orig_val });

                int index;
                while((index = input.find(orig_string)) != string::npos) {

                    input.replace(index,orig_string.length(), new_var);
                }


            }
        }
        this->out_func = input;

    }

    map<string, double> swap(map<string, double> d){
        map<string, double> return_d;
        for (auto &it : d) {
            string var_n = it.first;
            string var_o = vmap[var_n];
            double val = it.second;
            return_d.insert({ var_o, val});
        }
        return return_d;

    }




};


#endif //MISHRA_WARREN_AUTODIFF_PREPROCESSOR_H
