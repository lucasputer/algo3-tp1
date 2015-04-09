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

    int n_max = 3000;
    int muestras = 1;
    std::vector<double> tiempos = std::vector<double>(n_max, 0);

    for (int m = 1; m <= muestras; m++) {

        Vec entradas = Vec(n_max);

        int c = 1 + n_max; // 1< c < 10002
        int i = rand() % 1000 + 1; // 1< i < 10001
        int f = rand() % 1000 + i*2 + n_max; // i< f <10002

         entradas[0]=(Signal(1, c, i, f));

        char saux[32];
        sprintf(saux, echo_lines, entradas[0].costo, entradas[0].principio, entradas[0].fin);
        strcat(program, saux);     
        
        system(program);
        std::chrono::time_point<std::chrono::high_resolution_clock> t1 = std::chrono::high_resolution_clock::now();
        system("echo '1'");
        std::chrono::time_point<std::chrono::high_resolution_clock> t2 = std::chrono::high_resolution_clock::now();    

        for (int n = 2; n <= n_max; n++) {


            char program[100000];
            sprintf(program, echo_base, n);

            int c = c - 1 // 1< c < 10002
            int i = i + 1; // 1< i < 10001
            int f = f - 1; // i< f <10002

            entradas[n-1]=(Signal(n, c, i, f));

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

            tiempos[n-1] += double(std::chrono::duration_cast<std::chrono::nanoseconds>(t2-t1).count());

            cout << m << " " << n << endl;
        }
    }

    cout << endl;
    for (int i = 0; i < n_max; i++) {
        double aux = tiempos[i] / muestras;
        fprintf(file, "%d %7.8f\n", i+1, aux);
    }

    fclose(file);

    return 0;
}
