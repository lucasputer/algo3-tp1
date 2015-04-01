#include <functional>
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

struct datoCiudad {

    datoCiudad() : costoRescate(0), cantSoldados(0), indiceCiudad(0) {}

    datoCiudad(int n1, int n2, int n3) : costoRescate(n1), cantSoldados(n2), indiceCiudad(n3) {}

    bool operator<(const struct datoCiudad& other) const {
        //Como se ordena datoCiudad
        return costoRescate <= other.costoRescate;
    }

    int costoRescate;
    int cantSoldados;
    int indiceCiudad;
};

// Implementacion. Contiene el cargado de input más la resolución del ejercicio.
int main() {
    // Cantidad de Ciudades
    int n;
    cin >> n;
    // Presupuesto
    int P;
    cin >> P;
    // Vector que tiene los datoCiudad de cada ciudad
    vector<datoCiudad> infoCiudad(n);
    // Zombies por Ciudad
    int z;
    // Soldados por Ciudad
    int s;
    // Costo de enviar Soldado por Ciudad
    int c;
    // Cantidad de Ciudades Salvadas
    int ciudadesSalvadas = 0;
    // Soldados enviados por Ciudad
    vector<int> soldadosPorCiudad(n);
    // 1 Soldado = 10 zombies
    int minSoldados = 10;

    // Inicializo el vector con 0.
    for (int i = 0; i < n; i++) {
        soldadosPorCiudad[i] = 0;
    }

    // Tomo los datos del input para generar el MinHeap
    for(int i = 1; i <= n; i++) {
        cin >> z;
        cin >> s;
        cin >> c;
        // cantActual = parte alta de z /10 - s (soldados necesarios para salvar ciudad)
        int cantActual = ((z % minSoldados) ? z / minSoldados + 1 : z / minSoldados) - s;
        // costoActual = el costo para salvar la ciudad
        int costoActual = cantActual * c;
        // Sino tengo que mandar refuerzo pongo los valores en 0
        if (cantActual < 0) {
            costoActual = 0;
            cantActual = 0;
        }
        // Completo el vector con los nuevos datos de cada ciudad
        datoCiudad dato(costoActual, cantActual, i);
        infoCiudad[i - 1] = dato;
    }

    // Ordeno el vector infoCiudad
    sort(infoCiudad.begin(), infoCiudad.end());

    // Veo cuantas ciudades puedo salvar con el presupuesto asignado
    for(int i = 1; i <= n; i++) {
        if (P < infoCiudad[i - 1].costoRescate) {
            break;
        }
        ciudadesSalvadas++;
        soldadosPorCiudad[infoCiudad[i - 1].indiceCiudad - 1] = infoCiudad[i - 1].cantSoldados;
        P = P - infoCiudad[i - 1].costoRescate;
    }

    // Genero el output, segun formato solicitado
    /*cout << ciudadesSalvadas << ' ';
    for (int i = 0; i < n; i++) {
        cout << soldadosPorCiudad[i] << ' ';
    }
    cout << endl;*/
    cout << ciudadesSalvadas << endl;
}
