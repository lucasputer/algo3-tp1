#include <utility>
#include <vector>
#include <iostream>
using namespace std;

// Varios typedefs
struct Signal {
    Signal() : numero(0), costo(0), principio(0), fin(0) {};
    Signal(const int n,const int c,const int p, const int f) :
    	numero(n), costo(c), principio(p), fin(f) {};

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

// Implementacion
int main() {
    int n;
    cin >> n;

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

    mergeSort(l);
    mostrar(l);

    return 0;
}

void mergeSort(vector<Signal>& vec) {
    if(vec.size() == 1) {
        return;
    }

    Vec::iterator medio = vec.begin() + (vec.size() / 2);

    Vec izq(vec.begin(), medio);
    Vec der(medio, vec.end());
    Vec result;

    /**
    * Se reservan para resultado (2*n)-1 lugares, ya que en peor caso mergeSort
    * devuelve un vector del doble del original menos uno, esto se debe a que se
    * necesitan en peor caso 3 frecuencias en resultado al comparar dos frecuencias.
     */
    result.reserve(2*(izq.size()+der.size())-1); // O(n)

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
}

/**
* Compara dos frecuencias donde primera tiene <= tiempo de inicio que segunda.
* Si es necesario agrega alguna al resultado y aumenta izq_it y der_it.
 */
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
    int n = v.size();
    int finalCost = 0;
    for(int i = 0; i < n; ++i) {
        finalCost += v[i].costo * (v[i].fin - v[i].principio);
    }
    cout << finalCost << endl;

    for(int i = 0; i < n; ++i) {
        cout << v[i].principio;
        cout << " " << v[i].fin;
        cout << " " << v[i].numero;
        cout << endl;
    }
    cout << -1 << endl;
}