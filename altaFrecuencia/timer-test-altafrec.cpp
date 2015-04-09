#include <vector>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <set>
using namespace std;

clock_t startTime;
void startTimer() {
    startTime = clock();
}

void endTimer(FILE* file) {
    //cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC << " seconds." << endl;
    double tiempo = double( clock() - startTime ) / (double)CLOCKS_PER_SEC;
    fprintf(file, "%7.8f\n", tiempo);
}

double endTimer() {
    //cout << double( clock() - startTime ) / (double)CLOCKS_PER_SEC << " seconds." << endl;
    double tiempo = double( clock() - startTime ) / (double)CLOCKS_PER_SEC;
    return tiempo;
}

// Varios typedefs
struct Signal {
    Signal() : numero(0), costo(0), principio(0), fin(0) {};
    Signal(const int n,const int c,const int p, const int f) : numero(n), costo(c), principio(p), fin(f) {};

    int numero;
    int costo;
    int principio;
    int fin;
};
typedef vector<Signal> Vec;

// Implementacion
int main() {
    FILE* file = fopen("tiempos.txt","w+");
    srand(time(NULL));
    // "echo '3 0' | ./caballos"
    // "echo '1 1 1 2' | .ej2"
    const char *echo_base = "echo '%d";
    const char *echo_lines = " %d %d %d";

    const char *program_base = "' | ./ej2";;

    int n_max = 1000;
    int muestras = 100;
    std::vector<double> tiempos = std::vector<double>(n_max, 0);

    for (int m = 1; m <= muestras; m++) {

        Vec entradas = Vec(n_max);
        for (int n = 1; n <= n_max; n++) {


            char program[100000];
            sprintf(program, echo_base, n);

            int c = rand() % 1000 + 1; // 1< c < 10002
            int i = rand() % 1000 + 1; // 1< i < 10001
            int f = rand() % 1000 + i + 1; // i< f <10002

            entradas[n-1]=(Signal(n, c, i, f));

            for (vector<Signal>::iterator iter = entradas.begin() ; iter != entradas.end() ; iter++) {
                char saux[32];
                sprintf(saux, echo_lines, iter->costo, iter->principio, iter->fin);
                strcat(program, saux);
            }


            strcat(program,program_base);

            double tiempo =0;
            startTimer();
            system(program);
            tiempo = endTimer();
            cout << endl;
            cout << tiempo;



            tiempos[n-1] += tiempo;
        }
    }

    cout << endl;
    for (int i = 0; i < n_max; i++) {
        double aux = tiempos[i] /  double(muestras);
        fprintf(file, "%d %7.8f\n", i+1, aux);
    }

    fclose(file);

    return 0;
}
