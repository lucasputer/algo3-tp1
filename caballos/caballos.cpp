#include <utility>
#include <vector>
#include <iostream>
#include <stdlib.h>

using namespace std;

// Varios typedefs
typedef pair<int, int> Coord;
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
int resolverAux(Tablero& p, int n, Coord anterior);

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
    resolverAux(p, n, principio);
    mostrar(p, n);
    cout << endl;
    return 0;
}

int resolverAux(Tablero& p, int n, Coord c) {
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
    if (!encontroActual)
        return true;

    // me fijo cuales son los casilleros que podrian amenazar a la celda c
    vector<Coord> podrian_amenazar = coordenadasAmenazadas(n, c.first, c.second);
    // cuento cuantos casilleros vacios poniendo un caballo nuevo en cada una de las posibilidades (maximo 9 contando c)
    int nuevos_poner_en_c = simAgregarCaballo(p, n, c.first, c.second);
    Vec nuevos_podrian_amenazar = Vec(podrian_amenazar.size(), 0);
    for (int i = 0; i < podrian_amenazar.size(); i++) {
        nuevos_podrian_amenazar[i] = simAgregarCaballo(p, n, podrian_amenazar[i].first, podrian_amenazar[i].first);
    }

    // busco el maximo de las 9 posibilidades
    int max = nuevos_poner_en_c;
    Coord max_coord = c;
    for (int i = 0; i < podrian_amenazar.size(); i++) {
        if (nuevos_podrian_amenazar[i] > max) {
            max = nuevos_podrian_amenazar[i];
            max_coord = podrian_amenazar[i];
        }
    }

    agregarCaballo(p, n, max_coord.first, max_coord.second);
    //mostrar(p, n);
    //cout << endl;
    // vuelvo a pasar c, porque puede haber posiciones entre c y max_coord que no estan rellenadas
    resolverAux(p, n, c);
    return 0;
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
