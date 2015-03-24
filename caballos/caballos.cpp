#include <utility>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <queue>

using namespace std;

// Varios typedefs
typedef pair<int, int> Coord;
struct CoordEsp {
    
    int cant;
    Coord c;

    CoordEsp(int n, Coord d) : cant(n), c(d) { }

    bool operator<(const struct CoordEsp& other) const {
        //Como ordena el MaxHeap
        return cant < other.cant;
    }
};
typedef vector<int> Vec;
typedef vector<Vec> Tablero;

// Prototipado de funciones
bool agregarCaballo(Tablero& p, int n, int f, int c);
int simAgregarCaballo(Tablero& p, int n, int f, int c);
vector<Coord> coordenadasAmenazadas(int n, int f, int c);
bool amenazada(Tablero& p, int n, Coord c);
bool algunRepetido(const Vec &v);
bool valido(Tablero& p, int n, Coord& c, int k);

int resolver(Tablero& p, int n);
pair<int, Tablero> resolverAux(Tablero& p, int n, Coord anterior);

void mostrar(Tablero& p, int n);
int check(Tablero& p, int n);

int main() {
	int n;
	int k;

    cin >> n >> k;
    if(cin.eof()){
        return 0;
    }
    
    Tablero p(n, Vec(n, 0));
    bool tableroValido = true;
    for(int i = 0; i < k; i++) {
        int f, c;
        cin >> f >> c;
        tableroValido = tableroValido && agregarCaballo(p, n, f, c);
    }

    if (tableroValido) {
        resolver(p,n);
    }

	return 0;
}

bool agregarCaballo(Tablero& p, int n, int f, int c) {
    if (p[f][c] == 2)
        return true;
    if (f < 0 || f >= n || c < 0 || c >= n)
        return false;
    p[f][c] = 2;
    vector<Coord> cam = coordenadasAmenazadas(n, f, c);
    for (int i = 0; i < cam.size(); i++)
        // si cam[i] == 1 entonces no hace falta cambiarlo, y si cam[i] == 2 entonces estoy sacando un caballo !!
        if (p[cam[i].first][cam[i].second] == 0) {
            p[cam[i].first][cam[i].second] = 1;
        }
    return true;
}

// simula agregar un caballo, y me devuelve la cantidad de casillas nuevas amenazadas
// precondicion: se puede agregar un caballo en la posicion (f,c)
int simAgregarCaballo(Tablero& p, int n, int f, int c) {
    //p[f][c] = 2;
    int count = 0;
    vector<Coord> cam = coordenadasAmenazadas(n, f, c);
    for (int i = 0; i < cam.size(); i++) {
        if (p[cam[i].first][cam[i].second] == 0) {
            count = count + 1;
        }
    }
    return count;
}
vector<Coord> coordenadasAmenazadas(int n, int f, int c) {
    vector<Coord> solution;
    solution.reserve(8);
    Coord offsets[] = {make_pair(-1, 2),make_pair(2, -1),
        make_pair(-1, -2),make_pair(-2, -1),
        make_pair(2, 1),make_pair(1, 2),
        make_pair(-2, 1),make_pair(1, -2)};
    for (int i = 0; i < 8; i++) {
        int x = offsets[i].first;
        int y = offsets[i].second;
        if (0<=f+x && f+x<n && 0<=c+y && c+y<n)
            solution.push_back(make_pair(f+x, c+y));
    }
    return solution;
}

int resolver(Tablero& p, int n) {
    mostrar(p, n);
    cout << endl;
    Coord principio(0, 0);
    pair<int, Tablero> k = resolverAux(p, n, principio);
    mostrar(k.second, n);
    cout << endl;
    return 0;
}

pair<int,Tablero> resolverAux(Tablero& p, int n, Coord c) {
    // Encuentro proxima coordenada a rellenar
    bool encontroActual = p[c.first][c.second] == 0;
    while (c.first < n && !encontroActual) {
        if (c.second < n-1){
            c.second++;
        } else {
            c.second = 0;
            c.first = c.first + 1;
        }
        if (c.first < n && p[c.first][c.second] == 0)
            encontroActual = true;
    }
    //cout << c.first << c.second << endl;
    // Si no encontro un actual, ya no hay mas nada para completar 
    // (es precondicion que el tablero 'p' sea valido)
    if (!encontroActual) {
        // cuento la cantidad de caballos usados
        int count = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (p[i][j] == 2) {
                    count = count + 1;
                }
            }
        }
        return pair<int, Tablero>(count, p);
    }

    // me fijo cuales son los casilleros que podrian amenazar a la celda c
    vector<Coord> podrian_amenazar = coordenadasAmenazadas(n, c.first, c.second);
    // cuento cuantos casilleros vacios poniendo un caballo nuevo en cada una de las posibilidades (maximo 9 contando c)
    int nuevos_poner_en_c = simAgregarCaballo(p, n, c.first, c.second);
    Vec nuevos_podrian_amenazar = Vec(podrian_amenazar.size(), 0);
    for (int i = 0; i < podrian_amenazar.size(); i++) {
        nuevos_podrian_amenazar[i] = simAgregarCaballo(p, n, podrian_amenazar[i].first, podrian_amenazar[i].first);
    }

    // ordeno las 9 posibilidades de mayor a menor usando un heap
    priority_queue<CoordEsp> heap;
    heap.push(CoordEsp(nuevos_poner_en_c, c));
    for (int i = 0; i < podrian_amenazar.size(); i++) {
        heap.push(CoordEsp(nuevos_podrian_amenazar[i], podrian_amenazar[i]));
    }

    // voy probando las posiblidades a ver cual es la que minimiza los caballos usados
    int min_result = n*n;
    Coord min_coord(-1,-1);
    Tablero min_tablero;
    for (int i = 0; i < podrian_amenazar.size() + 1; i++) {
        Tablero aux = p; // deep copy
        CoordEsp ce = heap.top();
        heap.pop();
        agregarCaballo(aux, n, ce.c.first, ce.c.second);
        pair<int, Tablero> k = resolverAux(aux, n, c);
        if (k.first < min_result) {
            min_result = k.first;
            min_tablero = k.second;
            min_coord = ce.c;
        }
    }

    return pair<int, Tablero>(min_result, min_tablero);
}

bool amenazada(Tablero& p, int n, Coord c) {
    int x = c.first;
    int y = c.second;
    if (p[x][y] == 1) {
        return true;
    } else {
        // me fijo si hay posiciones que la amenazan
        bool amenazada = false;
        vector<Coord> posibles = vector<Coord>(8, Coord(-1,-1));
        // arriba a la derecha
        posibles[0] = Coord(x-2, y+1);
        posibles[1] = Coord(x-1, y+2);
        // abajo a la derecha
        posibles[2] = Coord(x+1, y+2);
        posibles[3] = Coord(x+2, y+1);
        // abajo a la izquierda
        posibles[4] = Coord(x+2, y-1);
        posibles[5] = Coord(x+1, y-2);
        // arriba a la izquierda
        posibles[6] = Coord(x-1, y-2);
        posibles[7] = Coord(x-2, y-2);

        for (int i = 0; i < 8; i++) {
            try {
                if (p.at(posibles[i].first).at(posibles[i].second) == 1) {
                        amenazada = true;
                        //cout << x << y << " " << posibles[i].first << posibles[i].second << endl;
                        break;
                }
            } catch (...) {
                // no hago nada
            }
        }
        return amenazada;
    }
}

void mostrar(Tablero& p, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << p[i][j] << " ";
        }
        cout << endl;
    }
}

int check(Tablero& p, int n) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!amenazada(p, n, Coord(i,j))) {
                // hay una celda vacia que no esta amenazada
                return -1;
            } else if (p[i][j] == 1) {
                count = count + 1;
            }
        }
    }
    return count;
}
