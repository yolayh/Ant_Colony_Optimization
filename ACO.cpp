#include "ACO.hpp"


int main(int argc, char *argv[]){

    const string data = argv[1];
    const int ants = atoi(argv[2]);
    const double alpha = atof(argv[3]);
    const double beta = atof(argv[4]);
    const double Q = atof(argv[5]);
    const double rho = atof(argv[6]);

    ACO aco;
    for (int i = 0; i < 10; i++){
        aco.excecution(data, ants, alpha, beta, Q, rho);
        aco.solution.clear();
        aco.result = 10000;
    }

    return 0;
}

