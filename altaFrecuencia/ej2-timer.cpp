#include <utility>
#include <vector>
#include <iostream>
#include <time.h>
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
Vec mergeSort(vector<Signal>& vec);
Vec merge(Vec &vec, Vec& izq,  Vec& der);
void mergeAux(Vec& menor, Vec& mayor, int& i, int& j,Vec& result);
void mostrar(const Vec& v);
void mostrarPrueba(const Vec& v);

// Implementación
int main() {
    int n;

    while(true) {
        cin >> n;
        if(cin.eof()) {
            break;
        }

        Vec l(n);
        for(int i = 0; i < n; ++i) {
            int j=0;
            int values [3];
            while(j<3) {
                cin >> values[j];
                j++;
            }
            l[i] = Signal(i+1,values[0],values[1],values[2]);
        }

            clock_t timer;
            timer = clock();

        l = mergeSort(l);
        timer = clock() - timer;
        //cout << n << endl;
        cout << timer << endl;
    }

    return 0;
}

Vec mergeSort(vector<Signal>& vec) {
    if(vec.size() == 1) {
        return vec;
    }

    Vec::iterator medio = vec.begin() + (vec.size() / 2);

    Vec izq(vec.begin(), medio);
    Vec der(medio, vec.end());

    // mostrarPrueba( izq);
    // cout << ">>>>>>>>>>>";
    // mostrarPrueba( der);

    // cout << "------------";

    izq = mergeSort(izq);
    der = mergeSort(der);

    return merge(vec,izq, der);
}

Vec merge(Vec &vec, Vec& izq,  Vec& der) {
    Vec result;
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
    vec = result;
    // cout << endl;
    // cout << "***********";
    // mostrarPrueba(vec);
    // cout << "***********";
    return vec;
}

void mergeAux(Vec& menor, Vec& mayor, int& i, int& j,Vec& result) {
    bool flag = false;
    if(menor[i].costo <= mayor[j].costo) {
        if(mayor[j].principio <= menor[i].fin) {
            mayor[j].principio = menor[i].fin;
        }
        if(mayor[j].principio >= mayor[j].fin) {
            j++;
            flag = true;
        }
        if(!flag) {
            result.push_back(menor[i]);
            i++;
        } else {
            flag = false;
        }
    } else {
        int aux = menor[i].fin;
        if(mayor[j].principio <= menor[i].fin) {
            menor[i].fin = mayor[j].principio;
        }
        if(menor[i].fin > menor[i].principio) {
            result.push_back(menor[i]);
        }
        if(mayor[j].fin < aux) {
            menor[i].principio = mayor[j].fin;
            menor[i].fin = aux;
        } else {
            i++;
        }
    }
}

/**
 * Recibe un vector y lo muestra por pantalla
 */
void mostrar(const Vec& v) {
    cout << endl;
    int n = v.size();
    int finalCost = 0;
    for(int i = 0; i < n; ++i) {
        finalCost += v[i].costo * (v[i].fin - v[i].principio);
    }
    cout << finalCost;
    cout << endl;

    for(int i = 0; i < n; ++i) {
        cout << v[i].numero;
        cout << " " << v[i].principio;
        cout << " " << v[i].fin;
        cout << endl;
    }
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
