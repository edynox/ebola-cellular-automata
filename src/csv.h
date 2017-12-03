#ifndef CSV_H
#define CSV_H

#include <vector>
#include <string>
#include <sstream>

using namespace std;

class CSV {
    public:
    CSV::CSV(string delimiter);
    void writeHeader(vector<string> columns);
    bool writeRow(vector<string> columns);
    bool writeToFile(string filename);
    private:
    int columnsPerRow;
    string delimiter;
    stringstream output;
};

#endif