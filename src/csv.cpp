#include "csv.h"
#include <ofstream>

CSV::CSV(string delimiter) : delimiter(delimiter) {   
}

void CSV::writeHeader(vector<string> columns) {
    columnsPerRow = columns.size();

    for (size_t i = 0; i < columns.size(); i++) {
    if (i != 0)
        output << delimiter;
        output << columns[i];
    }

    output << endl;
}

bool CSV::writeRow(vector<string> columns) {
    if (columns.size() != columnsPerRow) {
        return false;
    }

    for (size_t i = 0; i < columns.size(); i++) {
    if (i != 0)
        output << delimiter;
        output << columns[i];
    }

    output << endl;

    return true;
}

bool CSV::writeToFile(string filename) {
    ofstream f;
    f.open(filename);

    if (!f.is_open()) {
        return false;
    }

    f << output.rdbuf();

    f.close();

    return true;
}