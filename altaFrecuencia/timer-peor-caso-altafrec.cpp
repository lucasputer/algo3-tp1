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

    const char *program_base = "' | ./ej2-timer";;

    int n_max = 200;
    int muestras = 1;
    std::vector<long long int> tiempos = std::vector<long long int>(n_max, 0);

    for (int m = 1; m <= muestras; m++) {

        Vec entradas;
        entradas.reserve(n_max);

        int c = 1;
        int i = 1; // 1< i < 10001
        int f = 2; // i< f <10002
 

        for (int n = 1; n <= n_max; n++) {


            char program[100000];
            sprintf(program, echo_base, n);

            //c--; // 1< c < 10002
            i++; // 1< i < 10001
            f++; // i< f <10002

            entradas.push_back(Signal(n, c, i, f));

            for (vector<Signal>::iterator iter = entradas.begin() ; iter != entradas.end() ; iter++) {
                char saux[32];
                sprintf(saux, echo_lines, iter->costo, iter->principio, iter->fin);
                strcat(program, saux);
            }

            strcat(program,program_base);

            system(program);
            std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();
            system("echo '1'");
            std::chrono::time_point<std::chrono::high_resolution_clock> t2 = std::chrono::high_resolution_clock::now();

            tiempos[n-1] = std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count();

            cout << m << " " << n << endl;
        }
    }

    cout << endl;
    for (int i = 0; i < n_max; i++) {
        long long int aux = tiempos[i] / muestras;
        fprintf(file, "%d %lld\n", i+1, aux);
    }

    fclose(file);

    return 0;
}
