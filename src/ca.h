#ifndef CA_H
#define CA_H

#include "cell.h"
#include "cstdlib"
#include <string>
#include <vector>

class CA
{
  public:
    const size_t columns;
    const size_t rows;
    const double longProb;
    const double deadProb;
    const double terminalState;

    int numHealthy;
    int numDead;
    int numInfected;

    CA (size_t rows, size_t columns, double longProb, double deadProb, double terminalState);
    ~CA ();

    void step (bool regenerate, bool delay);
    void randomStep ();
    void seed (int amount);
    Cell status ();
    void saveToFile (std::string filename);

    int getDays () const;

  protected:
    std::vector<Cell **> generations;
    Cell **generation = NULL;
    int counter = 0;
    Cell nextState (size_t row, size_t column, bool regenerate, bool delay);
    int infectedNeighbours (int row, int column);
    Cell **allocateMatrix ();
};

#endif
