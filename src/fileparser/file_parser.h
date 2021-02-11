#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include "../sys.h"
#include "../token/Token.h"
#include "../auto_diff.h"

using namespace std;

class fileparser {

public:
    std::pair<string, map<std::string, double>> autodiff_file_reader(std::string filename);


};


#endif //FILE_PARSER_H