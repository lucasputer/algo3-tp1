#include <utility>
#include <vector>
#include <iostream>
#include <queue>

using namespace std;

// Varios typedefs
typedef vector<int> Vec;
typedef vector<Vec> Tablero;
typedef pair<int, int> Coord;
struct CoordEspecial {
    int nuevas_amenazadas;
    Coord c;

    CoordEspecial(int n, Coord d) : nuevas_amenazadas(n), c(d) { }

    bool operator<(const struct CoordEspecial& other) const {
        //Como ordena el MaxHeap
        return nuevas_amenazadas < other.nuevas_amenazadas;
    }
};

// Prototipado de funciones
int resolver(Tablero& p, int n, int cant_caballos);
int aux_resolver(Tablero& p, int n, Coord anterior, int cant_caballos, Tablero& t_optimo, int c_optimo, Tablero& original);
bool agregar_caballo(Tablero& p, int n, int f, int c);
int cota_goloso(Tablero& p, int n, int caballos);
vector<Coord> coordenadas_amenazadas(Tablero& p, int n, int f, int c);
vector<Coord> nuevas_coordenadas_amenazadas(Tablero& p, int n, int f, int c);
vector<Coord> aux_coordenadas_amenazadas(Tablero& p, int n, int f, int c, bool dame_todas);
Coord proxima_coordenada(Tablero& p, int n, Coord actual);
void mostrar(const Tablero& m);

// Implementacion
int main() {
    int n;
    int k;

    while(true) {
        cin >> n;
        if(cin.eof()){
            break;
        }
        cin >> k;
        if(cin.eof()){
            break;
        }

        Tablero p(n, Vec(n, 0));
        int cant_caballos = 0;
        for(int i = 0; i < k; i++){
            int f, c;
            cin >> f >> c;
            if (agregar_caballo(p, n, f, c) == true)
                cant_caballos++;
        }

        int sol = resolver(p,n,cant_caballos);
        cout << endl << "El tablero optimo tiene " << sol << " caballos." << endl;
    }

    return 0;
}

/**
 ** Agrega un caballo al Tablero dado.
 **
 ** Codigos de celdas:
 ** -----------------
 ** 0 : no tiene caballo ni esta amenazada
 ** 1 : tiene un caballo
 ** 2 : no tiene un caballo y esta amenazada
 **/
bool agregar_caballo(Tablero& p, int n, int f, int c) {
    //Check si (f,c) invalida o ya tiene un caballo
    if (f < 0 || f >= n || c < 0 || c >= n || p[f][c] == 1)
        return false;
    //Agrego un caballo a (f,c)
    p[f][c] = 1;
    //Marco las posiciones amenazadas
    vector<Coord> cam = coordenadas_amenazadas(p, n, f, c);
    for (int i = 0; i < cam.size(); i++) {
        Coord crd = cam[i];
        if (p[crd.first][crd.second] != 1) // si no tiene un caballo
            p[crd.first][crd.second] = 2; // la marco como amenazada
    }
    return true;
}

/**
 ** Devuelve todas las coordenadas que pueden ser amenazadas desde un casillero.
 **/
vector<Coord> coordenadas_amenazadas(Tablero& p, int n, int f, int c) {
    return aux_coordenadas_amenazadas(p,n,f,c,true);
}

/**
 ** Devuelve todas las coordenadas que pueden ser amenazadas desde un casillero,
 ** y no están ya amenazadas o tienen un caballo.
 **/
vector<Coord> nuevas_coordenadas_amenazadas(Tablero& p, int n, int f, int c) {
    return aux_coordenadas_amenazadas(p,n,f,c,false);
}

/**
 ** Funcion auxiliar que calcula coordenadas amenazadas.
 **/
vector<Coord> aux_coordenadas_amenazadas(Tablero& p, int n, int f, int c, bool dame_todas) {
    vector<Coord> solution;
    solution.reserve(8);
    Coord offsets[] = {	make_pair(-1, 2),make_pair(2, -1),
        make_pair(-1, -2),make_pair(-2, -1),
        make_pair(2, 1),make_pair(1, 2),
        make_pair(-2, 1),make_pair(1, -2)};
    for(int i = 0; i < 8; i++) {
        int x = offsets[i].first;
        int y = offsets[i].second;
        if (0<=f+x && f+x<n && 0<=c+y && c+y<n && (p[f+x][c+y] == 0 || dame_todas))
            solution.push_back(make_pair(f+x, c+y));
    }
    return solution;
}

/**
 ** Dada una coordenada, devuelve la proxima que no vale 1 o una invalida
 **/
Coord proxima_coordenada(Tablero& p, int n, Coord actual) {
    bool encontro_proxima = false;
    while (actual.first < n && !encontro_proxima){
        if (actual.second < n-1){
            actual.second++;
        } else {
            actual.second = 0;
            actual.first++;
        }
        if (actual.first < n && p[actual.first][actual.second] != 1)
            encontro_proxima = true;
    }
    return actual;
}

/**
 ** Imprime un tablero por stdout.
 **/
void mostrar(const Tablero& m) {
    cout << endl;
    int n = m.size();
    for(int i = 0; i < n; ++i) {
        cout << int(m[i][0] == 1);;
        for (int j = 1; j < n; j++) {
            cout << " " << int(m[i][j] == 1);
        }
        cout << endl;
    }
}

/**
 ** Encuentra el tablero optimo que minimiza la cantidad de caballos utilizados.
 **
 ** param Tablero& p: el tablero inicial
 ** param int n: el tamaño del tablero (p.size() = n y p[i].size() == n, con 0<=i<n)
 ** param int cant_caballos: la cantidad de caballos utilizados en el tablero inicial
 **/
int resolver(Tablero& p, int n, int cant_caballos) {
    // calculo una primera cota para el tablero optimo con un algoritmo goloso
    Tablero optimo = p;
    int cota = cota_goloso(optimo, n, cant_caballos);
    cout << "Cota algoritmo goloso: " << cota << endl;
    // guardo el tablero original para saber cuales eran los caballos que venian al principio
    Tablero original = p;
    // empiezo evaluando en el (0,0)
    Coord principio(0, 0);
    int sol = aux_resolver(p, n, principio, cant_caballos, optimo, cota, original);
    cout << "Tablero optimo: " << endl;
    mostrar(optimo);
    return sol;
}

/**
 ** Función auxiliar que evalua un Tablero y se vuelve a llamar de forma recursiva
 ** luego de tomar una decisison con respecto al siguiente casillero a evaluar.
 ** La decision puede ser: dejar el casillero vacio (para amenazarlo más adelante) o
 ** poner un caballo en el casillero.
 **
 ** Casos de poda:
 ** -------------
 ** A : todas las posiciones amenazadas por el actual estan en 1 o 2 y alguna tiene un 1
 ** => tengo que dejar la posicion actual en 2
 ** B : existe alguna posicion amenazada que ya fue procesada, tiene un 0 y solo la puede
 ** amenazar la posicion actual (todas las posiciones que pueden amenazar a la amenazada
 ** ya fueron procesadas y ninguna tiene un caballo) obs: clave para la correctitud
 ** => tengo que poner un 1 en la posicion actual
 ** C : todas las posiciones amenazadas ya fueron procesadas y ninguna tiene un 1 (ej: centro 3x3)
 ** obs: clave para la correctitud
 ** => tengo que poner un 1 en la posicion actual
 ** D : existe una posicion amenazada que tiene caballo y, a la vez, todas sus posiciones
 ** amenazadas que no son la actual tienen caballo. Ademas, no se deben cumplir B o C
 ** => tengo que dejar la posicion actual en 2
 ** G : el primer tablero optimo lo calculamos con un goloso
 ** Z: la solucion que esta siendo calculada tiene >= caballos que la solucion optima actual
 **
 ** param Tablero& p: el tablero a evaluar
 ** param int n: el tamaño del tablero (p.size() = n y p[i].size() == n, con 0<=i<n)
 ** param Coord actual: el último casillero evaluado antes de la llamada recursiva.
 ** param int cant_caballos: la cantidad de caballos utilizados para el tablero dado.
 ** param Tablero& t_optimo: el tablero de la mejor solucion encontrada hasta ahora.
 ** param int c_optimo: la cantidad de caballos de la mejor solucion encontrada hasta ahora.
 ** param Tablero& original: el tablero inicial.
 **/
int aux_resolver(Tablero& p, int n, Coord actual, int cant_caballos, Tablero& t_optimo, int c_optimo, Tablero& original) {
    //Speedup para problemas que empiezan con Tablero vacio con n <= 10 :D
    int precomputed_solutions[] = {0,1,4,4,4,5,8,10,12,14,16};
    if (n < 11 && c_optimo == precomputed_solutions[n])
        return c_optimo;

    //Poda caso Z
    if (cant_caballos >= c_optimo)
        return c_optimo;

    //Encuentro proxima coordenada a rellenar
    if (actual.first < n && p[actual.first][actual.second] == 1)
        actual = proxima_coordenada(p, n, actual);

    //Si no encontro un actual, ya no hay mas nada para completar
    if (actual.first == n) {
        if (cant_caballos < c_optimo) {
            t_optimo = p;
            return cant_caballos;
        } else {
            return c_optimo;
        }
    }

    //Sino, veamos...
    vector<Coord> cam = coordenadas_amenazadas(p, n, actual.first, actual.second);
    bool caso_A = true, caso_B = false, caso_C = true, caso_D = false;

    for (int i = 0; i < cam.size() && !caso_B; i++) {
        Coord crd = cam[i];
        //check caso A de poda
        if (p[crd.first][crd.second] == 0) {
            caso_A = false;
        }
        //check caso C de poda
        if (crd > actual) {
            caso_C = false;
        }
        //check caso B de poda
        if (crd < actual && p[crd.first][crd.second] == 0) { // crd ya fue procesada y tiene un 0
            vector<Coord> cam_crd = coordenadas_amenazadas(p, n, crd.first, crd.second);
            bool solo_la_puede_amenazar_actual = true;
            for (int j = 0; j < cam_crd.size() && solo_la_puede_amenazar_actual; j++) {
                Coord crd2 = cam_crd[j];
                if (crd2 > actual)
                    solo_la_puede_amenazar_actual = false;
            }
            if (solo_la_puede_amenazar_actual)
                caso_B = true;
        }
        //check caso D de poda
        if (p[crd.first][crd.second] == 1 && original[crd.first][crd.second] != 1) {
            vector<Coord> cam_crd = coordenadas_amenazadas(p, n, crd.first, crd.second);
            bool todas_menos_actual_la_amenazan = true;
            for (int j = 0; j < cam_crd.size() && todas_menos_actual_la_amenazan; j++) {
                Coord crd2 = cam_crd[j];
                if (crd2 != actual && p[crd2.first][crd2.second] != 1)
                    todas_menos_actual_la_amenazan = false;
            }
            if (todas_menos_actual_la_amenazan)
                caso_D = true;
        }
    }
    //ademas, para los casos A y C de poda, se tiene que cumplir lo siguiente
    caso_A = caso_A && p[actual.first][actual.second] != 0;
    caso_C = caso_C && p[actual.first][actual.second] == 0;

    //Si se cumple alguno de los casos
    if (caso_B || caso_C) {
        // si o si hay que poner un caballo
        int valor_anterior = p[actual.first][actual.second];
        vector<Coord> ncam = nuevas_coordenadas_amenazadas(p, n, actual.first, actual.second);
        p[actual.first][actual.second] = 1;

        // seteo como amenazadas, las coordenadas que estan en 0 y puedo amenazar
        for (int i = 0; i < ncam.size(); i++) {
            p[ncam[i].first][ncam[i].second] = 2;
        }

        int sol = aux_resolver(p, n, proxima_coordenada(p,n,actual), cant_caballos+1, t_optimo, c_optimo, original);

        //vuelvo todo a como estaba antes
        for (int i = 0; i < ncam.size(); i++) {
            p[ncam[i].first][ncam[i].second] = 0;
        }
        p[actual.first][actual.second] = valor_anterior;

        return sol;
    }
    if (caso_A || caso_D) {
        // hay que dejar todo como estaba, obs: el caso D requiere que B y C no se cumplan
        return aux_resolver(p, n, proxima_coordenada(p,n,actual), cant_caballos, t_optimo, c_optimo, original);
    }

    //La coordenada actual puede ser 0 o 2 (porque seguro que no tiene un caballo) y tengo que comparar resoluciones
    vector<Coord> ncam = nuevas_coordenadas_amenazadas(p, n, actual.first, actual.second);
    int valor_anterior = p[actual.first][actual.second];

    //Veo las soluciones dejando la coordenada como estaba
    int sol1 = aux_resolver(p, n, proxima_coordenada(p,n,actual), cant_caballos, t_optimo, c_optimo, original);

    //Veo las soluciones poniendo un caballo en la coordenada
    p[actual.first][actual.second] = 1;
    for (int i = 0; i < ncam.size(); i++) {
        p[ncam[i].first][ncam[i].second] = 2;
    }
    int sol2 = aux_resolver(p, n, proxima_coordenada(p,n,actual), cant_caballos+1, t_optimo, sol1, original);
    //obs: el sol1 aca es clave, porque compara las soluciones de los dos casos

    //vuelvo todo a como estaba
    for (int i = 0; i < ncam.size(); i++) {
        p[ncam[i].first][ncam[i].second] = 0;
    }
    p[actual.first][actual.second] = valor_anterior;

    return sol2;
}

/**
 ** Función auxiliar que evalua un Tablero y calcula una primer cota para la
 ** cantidad de caballos necesarios para armar una solución.
 ** La cota calculada se obtiene utilizando un algoritmo goloso que maximiza a
 ** cada paso la cantidad de posiciones amenazadas con el nuevo caballo.
 **
 ** param Tablero& p: el tablero a evaluar
 ** param int n: el tamaño del tablero (p.size() = n y p[i].size() == n, con 0<=i<n)
 ** param int caballos: la cantidad de caballos utilizados para el tablero dado.
 **/
int cota_goloso(Tablero& p, int n, int caballos) {
    // Encuentro proxima coordenada a rellenar
    Coord c = Coord(-1,-1);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (p[i][j] == 0) {
                c = Coord(i,j);
                break;
            }
        }
        if (c.first != -1) {
            break;
        }
    }
    // Si no encontro un actual, ya no hay mas nada para completar
    if (c.first == -1) {
        // devuelvo la cantidad de caballos usados
        return caballos;
    }

    // vamos a ver si conviene poner un caballo en el casillero c,
    // o si conviene amenazarlo desde alguna otra posicion vacia
    vector<Coord> ncam = nuevas_coordenadas_amenazadas(p, n, c.first, c.second);

    // tengo que evaluar como maximo 9 posibilidades (el casillero actual + 8 que la pueden amenazar)
    // voy a usar un heap para ordenar estas posibilidades de mayor a menor
    priority_queue<CoordEspecial> heap;
    heap.push(CoordEspecial(ncam.size(), c));
    for (int i = 0; i < ncam.size(); i++) {
        vector<Coord> ncam_aux = nuevas_coordenadas_amenazadas(p, n, ncam[i].first, ncam[i].second);
        heap.push(CoordEspecial(ncam_aux.size(), ncam[i]));
    }

    // uso la posiblidad que maximiza la cantidad de casilleros vacios amenazados
    CoordEspecial ce = heap.top();
    agregar_caballo(p, n, ce.c.first, ce.c.second);
    return cota_goloso(p, n, caballos + 1);
}
