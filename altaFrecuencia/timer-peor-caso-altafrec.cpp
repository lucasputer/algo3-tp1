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
    FILE* file = fopen("peoresTiempos.txt","w+");
    srand(time(NULL));
    // "echo '3 0' | ./caballos"
    // "echo '1 1 1 2' | .ej2"
    const char *echo_base = "echo '%d";
    const char *echo_lines = " %d %d %d";

    const char *program_base = "' | ./ej2";;

    int n_max = 100;
    int muestras = 100;
    std::vector<double> tiempos = std::vector<double>(n_max, 0);

    for (int m = 1; m <= muestras; m++) {

        Vec entradas = Vec(n_max);

        char program[100000];
            sprintf(program, echo_base, 1);

            int c = rand() % 1001; // 1< c < 10002
            int i = rand() % 1000 + 1; // 1< i < 10001
            int f = rand() % 1000 + i + n_max; // i< f <10002

            entradas[0]=(Signal(1, c, i, f));


            char saux[32];
            sprintf(saux, echo_lines, entradas[0].costo, entradas[0].principio, entradas[0].fin);
            strcat(program, saux);
            


            strcat(program,program_base);

            double tiempo =0;
            startTimer();
            system(program);
            tiempo = endTimer();
            cout << endl;
            cout << tiempo;

            tiempos[0] += tiempo;


        for (int n = 2; n <= n_max; n++) {


            char program2[100000];
            sprintf(program2, echo_base, n);

            c = c-1; // 1< c < 10002
            i = i+1; // 1< i < 10001
            f = f-1; // i< f <10002

            entradas[n-1]=(Signal(n, c, i, f));


            char saux[32];
            sprintf(saux, echo_lines, entradas[0].costo, entradas[0].principio, entradas[0].fin);
            strcat(program2, saux);

            for (vector<Signal>::iterator iter = entradas.begin() ; iter != entradas.end() ; iter++) {
                sprintf(saux, echo_lines, iter->costo, iter->principio, iter->fin);
                strcat(program2, saux);
            }


            strcat(program,program_base);

            double tiempo2 =0;
            startTimer();
            system(program2);
            tiempo2 = endTimer();
            cout << endl;
            cout << tiempo;



            tiempos[n-1] += tiempo2;
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
