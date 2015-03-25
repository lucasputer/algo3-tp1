#include <functional>
#include <queue>
#include <vector>
#include <iostream>

using namespace std;
// Falta definir unos detalles de inputs y un testing intensivo.

struct datoCiudad
{
    int costoRescate;
    int cantSoldados;
    int indiceCiudad;

    datoCiudad(int n1, int n2, int n3) : costoRescate(n1), cantSoldados(n2), indiceCiudad(n3) {}

    bool operator<(const struct datoCiudad& other) const
    {
        //Como ordena el MinHeap
        return costoRescate > other.costoRescate;
    }
};

int main() {
    // El input puede estar vacio?
    // MinHeap
    priority_queue<datoCiudad> infoCiudad;
    // Cantidad de Ciudades
    int n;
    cin >> n;
    // Presupuesto
    int P;
    cin >> P;
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

    // Inicializo el vector con 0. Es necesario?
    for (int i = 0; i < soldadosPorCiudad.size(); i++) {
        soldadosPorCiudad[i] = 0;
    }

    // Tomo los datos del input para generar el MinHeap
    for(int i = 1; i <= n; i++) {
        cin >> z;
        cin >> s;
        cin >> c;
        // indice Ciudad siendo analizada
        int indiceActual = i;
        // cantActual = parte alta de z /10 - s (soldados necesarios para salvar ciudad)
        int cantActual = ((z % minSoldados) ? z / minSoldados + 1 : z / minSoldados) - s;
        // costoActual = el costo para salvar la ciudad
        int costoActual = cantActual * c;
        // Sino tengo que mandar refuerzo pongo los valores en 0
        if (cantActual < 0) {
            costoActual = 0;
            cantActual = 0;
        } 
        // El MinHeap esta ordenado segun el costo de salvar cada ciudad (menor = mayor prioridad)
        infoCiudad.push(datoCiudad(costoActual,cantActual,indiceActual));
    }

    // Veo cuantas ciudades puedo salvar con el presupuesto asignado
    // P puede ser negativo? VER (me arruina el caso con costoRescate 0)
    while (!infoCiudad.empty()) {
        if (P < infoCiudad.top().costoRescate) {
            break;
        }
        ciudadesSalvadas++;
        soldadosPorCiudad[infoCiudad.top().indiceCiudad - 1] = infoCiudad.top().cantSoldados;
        P = P - infoCiudad.top().costoRescate;
        infoCiudad.pop();
    }
    
    // Genero el output, segun formato solicitado
    cout << ciudadesSalvadas << ' ';
    for (int i = 0; i < soldadosPorCiudad.size(); i++) {
        cout << soldadosPorCiudad[i] << ' ';
    }
    
    cout << endl;
}
