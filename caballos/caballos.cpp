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
int resolver(Tablero& p, int n);
int resolverAux(Tablero& p, int n, Coord c);
bool amenazada(Tablero& p, int n, Coord c);
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
    for(int i = 0; i < k; i++) {
        int x;
        int y;
        cin >> x >> y;
        p[x][y] = 1;
    }
    //mostrar(p, n);

    resolver(p,n);

	return 0;
}

int resolver(Tablero& p, int n) {
    // busco primera celda vacia no amenazada
    Coord vacia(-1,-1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Coord aux = Coord(i,j);
            if (!amenazada(p, n, aux)) {
                vacia = aux;
                break;
            }
        }
        if (vacia.first != -1 && vacia.second != -1) {
            break;
        }
    }
    if (vacia.first == -1 && vacia.second == -1) {
        cout << "el tablero ya es una solucion" << endl;
        return check(p, n);
    } else {
        return resolverAux(p, n, vacia);
    }
}

int resolverAux(Tablero& p, int n, Coord c) {
    //cout << c.first << c.second;
    // busco primera celda vacia no amenazada
    Coord vacia(-1,-1);
    for (int i = c.first; i < n; i++) {
        for (int j = 0; j < n; j++) {
            Coord aux = Coord(i,j);
            if (!amenazada(p, n, aux)) {
                vacia = aux;
                break;
            }
        }
        if (vacia.first != -1 && vacia.second != -1) {
            break;
        }
    }

    if (vacia.first == -1 && vacia.second == -1) {
        // no hay celdas por evaluar: el tablero es invalido o el tablero es una solucion
        return check(p, n);
    }

    p[vacia.first][vacia.second] = 0;
    int k1 = resolverAux(p, n, vacia);

    p[vacia.first][vacia.second] = 1;
    int k2 = resolverAux(p, n, vacia);
    if (k1 < k2) {
        p[vacia.first][vacia.second] = 0;
        return k1;
    } else {
        p[vacia.first][vacia.second] = 1;
        return k2;
    }
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
