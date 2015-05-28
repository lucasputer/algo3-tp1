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
    FILE* file = fopen("tiempos-peores.txt","w+");
    srand(time(NULL));
    // "echo '3 0' | ./caballos"
    // "echo '1 1 1 2' | .ej2"
    const char *echo_base = "echo '%d";
    const char *echo_lines = " %d %d %d";

    const char *program_base = "' | ./ej2-timer";;

    int n_max = 3000;
    int muestras = 1;
    std::vector<long long int> tiempos = std::vector<long long int>(n_max, 0);

    for (int m = 1; m <= muestras; m++) {

        Vec entradas;
        entradas.reserve(n_max);

        int c = n_max + 1; // 1< c < 10002
        int b = 2; // 1< i < 10001
        int e = 2*n_max  + 1; // i< f <10002   

        for (int n = 1; n <= n_max; n++) {


            char program[100000];
            sprintf(program, echo_base, n);

            c--;
            b++;
            e--;

            entradas.push_back(Signal(n, c, b, e));

            for (vector<Signal>::iterator iter = entradas.begin() ; iter != entradas.end() ; iter++) {
                char saux[32];
                sprintf(saux, echo_lines, iter->costo, iter->principio, iter->fin);
                strcat(program, saux);
            }
            

            // for (int x = 0; x < n; x++) {
            //     int c = rand() % 100; // 1< c < 10002
            //     int i = rand() % 1000; // 1< i < 10001
            //     int f = rand() % 1000 + i + 1; // i< f <10002
            //     char saux[32];
            //     sprintf(saux, echo_lines, c, i, f);
            //     strcat(program, saux);
            // }

            strcat(program,program_base);

            std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();
            system(program);
            std::chrono::time_point<std::chrono::high_resolution_clock> t2 = std::chrono::high_resolution_clock::now();

            long long int tiempo = std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count();

            tiempos[n-1] += tiempo;

            //cout << tiempos[n-1] << endl;
            cout << m << " " << n << endl;
            //cout  << tiempo << endl;
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
