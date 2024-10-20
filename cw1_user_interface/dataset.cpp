// COMP2811 Coursework 1: QuakeDataset class

#include <stdexcept>
#include "dataset.hpp"
#include "csv.hpp"


using namespace std;

QuakeDataset::QuakeDataset(const string& filename){
    loadData(filename);
}

