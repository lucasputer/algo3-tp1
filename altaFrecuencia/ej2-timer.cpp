#include <utility>
#include <vector>
#include <iostream>
using namespace std;

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

// Prototipado de funciones
void mergeSort(vector<Signal>& vec);
void merge(Vec& izq, Vec& der, Vec& result);
void mergeAux(Vec& menor, Vec& mayor, int& i, int& j,Vec& result);
void mostrar(const Vec& v);
void mostrarPrueba(const Vec& v);

// Implementación
int main() {
    int n;

    // while(true) {
        cin >> n;
        // if(cin.eof()) {
        //     break;
        // }

        Vec l;
        l.reserve(n);
        for(int i = 0; i < n; ++i) {
            int j=0;
            int values [3];
            while(j<3) {
                cin >> values[j];
                j++;
            }
            l.push_back(Signal(i+1,values[0],values[1],values[2]));
        }

        clock_t timer;
        timer = clock();

        mergeSort(l);
        //mostrar(l);
    // }

    timer = clock() - timer;
    FILE* file = fopen("tiempos_caca.txt","a+");
    fprintf(file, "%d %ld\n",n, timer);
    fclose(file);

    return 0;
}

void mergeSort(vector<Signal>& vec) {
    if(vec.size() == 1) {
        return;
    }

    Vec::iterator medio = vec.begin() + (vec.size() / 2);

    Vec izq(vec.begin(), medio);
    Vec der(medio, vec.end());

    // mostrarPrueba( izq);
    // cout << ">>>>>>>>>>>";
    // mostrarPrueba( der);

    // cout << "------------";

    Vec result;
    result.reserve(2*(izq.size()+der.size())+1); // O(n)

    mergeSort(izq); //T(n/2)
    mergeSort(der); //T(n/2)
    merge(izq, der, result); // O(n)

    vec = result; // O(n)
}

void merge(Vec& izq, Vec& der, Vec& result) {
    int izq_it = 0, der_it = 0;

    while(izq_it < izq.size() && der_it < der.size()) {
        if(izq[izq_it].principio <= der[der_it].principio) {
            mergeAux(izq,der,izq_it,der_it,result);
        } else {
            mergeAux(der,izq,der_it,izq_it,result);
        }
    }

    while(izq_it < izq.size()) {
        result.push_back(izq[izq_it]);
        izq_it++;
    }

    while(der_it < der.size()) {
        result.push_back(der[der_it]);
        der_it++;
    }
    // cout << endl;
    // cout << "***********";
    // mostrarPrueba(result);
    // cout << "***********";
}

void mergeAux(Vec& primera, Vec& segunda, int& i, int& j,Vec& result) {
    if(primera[i].costo <= segunda[j].costo) {
        if(segunda[j].principio <= primera[i].fin) {
            segunda[j].principio = primera[i].fin;
        }
        if(segunda[j].principio >= segunda[j].fin) {
            j++;
        }else{
            result.push_back(primera[i]);
            i++;
        }
    } else {
        int aux = primera[i].fin;
        if(segunda[j].principio <= primera[i].fin) {
            primera[i].fin = segunda[j].principio;
        }
        if(primera[i].fin > primera[i].principio) {
            result.push_back(primera[i]);
        }
        if(segunda[j].fin < aux) {
            primera[i].principio = segunda[j].fin;
            primera[i].fin = aux;
        } else {
            i++;
        }
    }
}

/**
 * Recibe un vector y lo muestra por pantalla
 */
void mostrar(const Vec& v) {
    //cout << endl;
    int n = v.size();
    int finalCost = 0;
    for(int i = 0; i < n; ++i) {
        finalCost += v[i].costo * (v[i].fin - v[i].principio);
    }
    cout << finalCost;
    //cout << endl;

    // for(int i = 0; i < n; ++i) {
    //     cout << v[i].numero;
    //     cout << " " << v[i].principio;
    //     cout << " " << v[i].fin;
    //     cout << endl;
    // }
}

void mostrarPrueba(const Vec& v) {
    cout << endl;
    int n = v.size();

    for(int i = 0; i < n; ++i) {
        cout << v[i].numero;
        cout << " " << v[i].costo;
        cout << " " << v[i].principio;
        cout << " " << v[i].fin;
        cout << endl;
    }
}
