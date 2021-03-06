#include "simulation.h"
#include "csv.h"
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <unistd.h>

using namespace std;

int
main (int argc, char **argv)
{
    // defaults
    double longProb = 0.05;
    double deadProb = 0.35;
    double seedProb = 0.0055;
    int rows = 700;
    int columns = 700;
    srand (time (NULL));

    bool exportCSV = false;
    bool calcFatRate = false;

    // having less than of cells 1/3 healthy means death
    double terminalState = 1.0f / 3.0;

    // parse command line configuration
    int c;
    while ((c = getopt (argc, argv, "l:d:r:c:s:t:of")) != -1) {
        switch (c) {
            case 'l':
                longProb = strtod (optarg, NULL);
                break;
            case 'd':
                deadProb = strtod (optarg, NULL);
                break;
            case 't':
                terminalState = strtod (optarg, NULL);
                break;
            case 's':
                seedProb = strtod (optarg, NULL);
                break;
            case 'r':
                rows = strtol (optarg, NULL, 0);
                break;
            case 'c':
                columns = strtol (optarg, NULL, 0);
                break;
            case 'o':
                exportCSV = true;
                break;
            case 'f':
                calcFatRate = true;
                break;
        }
    }
    if (1 - longProb - deadProb < 0) {
        cerr << "Error: l + d > 1" << endl;
        return 1;
    }

    // prepare CSV header
    CSV csv (",");
    vector<string> header;
    header.push_back ("time");
    header.push_back ("healthy");
    header.push_back ("infected");
    header.push_back ("dead");
    csv.writeHeader (header);

    int runCount = 1;

    if (calcFatRate) {
        runCount = 100;
    }

    std::string filename ("img/file");

    int deadCount = 0;

    for (int i = 0; i < runCount; i++) {

        // instantiate celluar automaton
        CA ca (rows, columns, longProb, deadProb, terminalState);

        // place infected cells
        int seedCount = rows * columns * seedProb;
        ca.seed (seedCount == 0 ? 1 : seedCount);

        // run simulation while CA is not completely dead or empty
        while (ca.status () == INFECTED) {

            // cout << ca.dump () << endl;

            // perform step
            ca.randomStep ();

            if (exportCSV) {
                vector<string> row;
                row.push_back (std::to_string (ca.getDays ()));
                row.push_back (std::to_string (ca.numHealthy));
                row.push_back (std::to_string (ca.numInfected));
                row.push_back (std::to_string (ca.numDead));
                csv.writeRow (row);
            }
            // usleep (75000);

            // ca.saveToFile (filename + to_string (ca.getDays ()) + ".bmp");
        }

        if (ca.status () == DEAD) {
            cout << "DEAD: " << ca.getDays () << endl;
            deadCount++;
        } else {
            cout << "ALIVE: " << ca.getDays () << endl;
        }
    }

    // calculate the fatality rate
    if (calcFatRate) {
        cout << "Fatality rate is " << fixed << setprecision (2) << deadCount * 100.0f / runCount
             << "%" << endl;
    }

    if (exportCSV) {
        csv.writeToFile ("simulation.csv");
    }

    return 0;
}
