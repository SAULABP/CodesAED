
//  Grafos y BFS 

//  Convención: vértices numerados de 1 a n -> arreglos de tamaño n+1
//  rillas indexadas desde 0 (fila f, columna c)

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <deque>
#include <map>
#include <algorithm>
using namespace std;

const int INF = 1000000000;

// ============================================================
//  1. LECTURA DEL GRAFO
//  Entrada típica: "n m" y luego m líneas "u v" (o "u v w").
// ============================================================

// Lista de adyacencia, grafo NO dirigido — O(n + m)
vector<vector<int>> leerNoDirigido(int n, int m) {
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    return adj;
}

// Lista de adyacencia, grafo DIRIGIDO u -> v — O(n + m)
vector<vector<int>> leerDirigido(int n, int m) {
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
    }
    return adj;
}

// Lista con pesos: adj[u] guarda pares (vecino, peso) — O(n + m)
vector<vector<pair<int, int>>> leerConPesos(int n, int m, bool dirigido) {
    vector<vector<pair<int, int>>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        if (!dirigido) adj[v].push_back({u, w});
    }
    return adj;
}

// Matriz de adyacencia (solo si n es de pocos miles) — memoria Θ(n²)
vector<vector<int>> leerMatriz(int n, int m, bool dirigido) {
    vector<vector<int>> A(n + 1, vector<int>(n + 1, 0));
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        A[u][v] = 1;
        if (!dirigido) A[v][u] = 1;
    }
    return A;
}

// ============================================================
//  2. OPERACIONES BÁSICAS SOBRE LA LISTA DE ADYACENCIA
// ============================================================

// ¿u y v son adyacentes? — O(deg u)
bool adyacentes(const vector<vector<int>>& adj, int u, int v) {
    for (int x : adj[u])
        if (x == v) return true;
    return false;
}

// Grados de entrada de un grafo dirigido, una sola pasada — O(n + m)
vector<int> gradosEntrada(const vector<vector<int>>& adj, int n) {
    vector<int> indeg(n + 1, 0);
    for (int u = 1; u <= n; u++)
        for (int v : adj[u])
            indeg[v]++;
    return indeg;
}

// Grafo transpuesto (invierte cada arista) — O(n + m)
vector<vector<int>> transpuesto(const vector<vector<int>>& adj, int n) {
    vector<vector<int>> adjT(n + 1);
    for (int u = 1; u <= n; u++)
        for (int v : adj[u])
            adjT[v].push_back(u);
    return adjT;
}

// Quitar arista no dirigida: swap con el último + pop_back — O(deg u + deg v)
void quitarArista(vector<vector<int>>& adj, int u, int v) {
    for (int i = 0; i < (int)adj[u].size(); i++) {
        if (adj[u][i] == v) {
            swap(adj[u][i], adj[u].back());
            adj[u].pop_back();
            break;
        }
    }
    for (int i = 0; i < (int)adj[v].size(); i++) {
        if (adj[v][i] == u) {
            swap(adj[v][i], adj[v].back());
            adj[v].pop_back();
            break;
        }
    }
}

// Vecinos en común con arreglo de marcas — O(deg u + deg v)
// 'marca' debe tener tamaño n+1 y estar todo en false al llamar
int vecinosComunes(const vector<vector<int>>& adj, vector<bool>& marca, int u, int v) {
    int cnt = 0;
    for (int x : adj[u]) marca[x] = true;
    for (int y : adj[v])
        if (marca[y]) cnt++;
    for (int x : adj[u]) marca[x] = false;   // desmarcar solo lo marcado
    return cnt;
}

// Euler (grafo NO dirigido y conexo): cuenta vértices de grado impar
// 0 impares -> "CIRCUITO", 2 impares -> "CAMINO", otro -> "IMPOSIBLE"
string tipoEuler(const vector<vector<int>>& adj, int n) {
    int impares = 0;
    for (int u = 1; u <= n; u++)
        if (adj[u].size() % 2 == 1) impares++;
    if (impares == 0) return "CIRCUITO";
    if (impares == 2) return "CAMINO";
    return "IMPOSIBLE";
}

// ============================================================
//  3. BFS BÁSICO: distancias mínimas (sin pesos) y padres — O(n + m)
//  dist = -1 significa "no descubierto"
// ============================================================
void bfs(int s, int n, const vector<vector<int>>& adj,
         vector<int>& dist, vector<int>& padre) {
    dist.assign(n + 1, -1);
    padre.assign(n + 1, -1);
    queue<int> q;
    dist[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {          // se descubre al ENTRAR a la cola
                dist[v] = dist[u] + 1;
                padre[v] = u;
                q.push(v);
            }
        }
    }
}

// Reconstruir el camino s -> t subiendo por padre[] y luego invirtiendo
// Devuelve vacío si t no fue alcanzado (dist[t] == -1)
vector<int> reconstruirCamino(int t, const vector<int>& dist, const vector<int>& padre) {
    vector<int> camino;
    if (dist[t] == -1) return camino;
    for (int v = t; v != -1; v = padre[v])
        camino.push_back(v);
    reverse(camino.begin(), camino.end());
    return camino;
}

// ============================================================
//  4. BFS MULTI-FUENTE ("súper fuente"): todas las fuentes con dist 0
//  Distancia de cada vértice a la fuente MÁS CERCANA — O(n + m)
// ============================================================
vector<int> bfsMultiFuente(const vector<int>& fuentes, int n, const vector<vector<int>>& adj) {
    vector<int> dist(n + 1, -1);
    queue<int> q;
    for (int s : fuentes) {
        if (dist[s] == -1) {
            dist[s] = 0;
            q.push(s);
        }
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
    return dist;
}

// ============================================================
//  5. COMPONENTES CONEXAS: un BFS por cada vértice no visitado — O(n + m)
//  comp[v] = número de componente de v (1, 2, 3, ...)
// ============================================================
int contarComponentes(int n, const vector<vector<int>>& adj, vector<int>& comp) {
    comp.assign(n + 1, 0);
    int c = 0;
    for (int s = 1; s <= n; s++) {
        if (comp[s] != 0) continue;
        c++;
        comp[s] = c;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (comp[v] == 0) {
                    comp[v] = c;
                    q.push(v);
                }
            }
        }
    }
    return c;
}

// ============================================================
//  6. ¿BIPARTITO? Colorear por niveles (0 / 1) — O(n + m)
//  Arista entre dos del mismo color => ciclo impar => NO bipartito
// ============================================================
bool esBipartito(int n, const vector<vector<int>>& adj, vector<int>& color) {
    color.assign(n + 1, -1);
    bool ok = true;
    for (int s = 1; s <= n; s++) {
        if (color[s] != -1) continue;
        color[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : adj[u]) {
                if (color[v] == -1) {
                    color[v] = 1 - color[u];
                    q.push(v);
                } else if (color[v] == color[u]) {
                    ok = false;
                }
            }
        }
    }
    return ok;
}

// ============================================================
//  7. CONTAR CAMINOS MÍNIMOS desde s — O(n + m)
//  cnt[v] = suma de cnt[u] de los vecinos u en la capa anterior
// ============================================================
vector<long long> contarCaminosMinimos(int s, int n, const vector<vector<int>>& adj, long long MOD) {
    vector<int> dist(n + 1, -1);
    vector<long long> cnt(n + 1, 0);
    queue<int> q;
    dist[s] = 0;
    cnt[s] = 1;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : adj[u]) {
            if (dist[v] == -1) {                 // primera vez: nueva capa
                dist[v] = dist[u] + 1;
                cnt[v] = cnt[u] % MOD;
                q.push(v);
            } else if (dist[v] == dist[u] + 1) { // otro camino mínimo
                cnt[v] = (cnt[v] + cnt[u]) % MOD;
            }
        }
    }
    return cnt;
}

// ============================================================
//  8. BFS EN GRILLA (grafo implícito) — O(R·C)
//  Vecinos con df/dc. 'fuentes' puede tener una o varias casillas.
//  dist[f][c] = -1 si es pared o no se alcanza.
// ============================================================
int df[] = {-1, 1, 0, 0};
int dc[] = {0, 0, -1, 1};

vector<vector<int>> bfsGrilla(const vector<string>& g, const vector<pair<int, int>>& fuentes) {
    int R = g.size(), C = g[0].size();
    vector<vector<int>> dist(R, vector<int>(C, -1));
    queue<pair<int, int>> q;
    for (auto p : fuentes) {
        dist[p.first][p.second] = 0;
        q.push(p);
    }
    while (!q.empty()) {
        int f = q.front().first;
        int c = q.front().second;
        q.pop();
        for (int k = 0; k < 4; k++) {
            int nf = f + df[k];
            int nc = c + dc[k];
            if (nf >= 0 && nf < R && nc >= 0 && nc < C &&
                g[nf][nc] != '#' && dist[nf][nc] == -1) {
                dist[nf][nc] = dist[f][c] + 1;
                q.push({nf, nc});
            }
        }
    }
    return dist;
}

// Grado de cada casilla de tierra '#' y perímetro P = suma de (4 - grado)
// (problema "Perímetro del terreno")
long long perimetroGrilla(const vector<string>& g, vector<long long>& cntGrado) {
    int R = g.size(), C = g[0].size();
    cntGrado.assign(5, 0);
    long long P = 0;
    for (int f = 0; f < R; f++) {
        for (int c = 0; c < C; c++) {
            if (g[f][c] != '#') continue;
            int grado = 0;
            for (int k = 0; k < 4; k++) {
                int nf = f + df[k];
                int nc = c + dc[k];
                if (nf >= 0 && nf < R && nc >= 0 && nc < C && g[nf][nc] == '#')
                    grado++;
            }
            cntGrado[grado]++;
            P += 4 - grado;
        }
    }
    return P;
}

// ============================================================
//  9. BFS SOBRE GRAFO DE ESTADOS (implícito) — estados como string
//  Ejemplo: rompecabezas 2x3 (0 = vacío). Para otro problema,
//  solo cambia cómo se generan los vecinos dentro del for.
// ============================================================
int bfsEstados(const string& inicio, const string& meta, int filas, int cols) {
    map<string, int> dist;
    queue<string> q;
    dist[inicio] = 0;
    q.push(inicio);
    while (!q.empty()) {
        string u = q.front();
        q.pop();
        if (u == meta) return dist[u];
        int pos = u.find('0');
        int f = pos / cols, c = pos % cols;
        for (int k = 0; k < 4; k++) {
            int nf = f + df[k];
            int nc = c + dc[k];
            if (nf >= 0 && nf < filas && nc >= 0 && nc < cols) {
                string t = u;
                swap(t[pos], t[nf * cols + nc]);
                if (dist.find(t) == dist.end()) {
                    dist[t] = dist[u] + 1;
                    q.push(t);
                }
            }
        }
    }
    return -1;
}

// ============================================================
//  10. BFS 0-1 con DEQUE (pesos solo 0 o 1) — O(n + m)
//  peso 0 -> push_front, peso 1 -> push_back
//  Se actualiza si se MEJORA la distancia (un vértice puede entrar 2 veces)
// ============================================================
vector<int> bfs01(int s, int n, const vector<vector<pair<int, int>>>& adj) {
    vector<int> dist(n + 1, INF);
    deque<int> dq;
    dist[s] = 0;
    dq.push_back(s);
    while (!dq.empty()) {
        int u = dq.front();
        dq.pop_front();
        for (auto e : adj[u]) {
            int v = e.first, w = e.second;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                if (w == 0) dq.push_front(v);
                else dq.push_back(v);
            }
        }
    }
    return dist;   // INF = no alcanzable
}

// BFS 0-1 en grilla: entrar a '.' cuesta 0, romper una pared '#' cuesta 1
// Devuelve el mínimo de paredes a romper de (sf,sc) a (ef,ec)
int bfs01Grilla(const vector<string>& g, int sf, int sc, int ef, int ec) {
    int R = g.size(), C = g[0].size();
    vector<vector<int>> dist(R, vector<int>(C, INF));
    deque<pair<int, int>> dq;
    dist[sf][sc] = 0;
    dq.push_back({sf, sc});
    while (!dq.empty()) {
        int f = dq.front().first;
        int c = dq.front().second;
        dq.pop_front();
        for (int k = 0; k < 4; k++) {
            int nf = f + df[k];
            int nc = c + dc[k];
            if (nf < 0 || nf >= R || nc < 0 || nc >= C) continue;
            int w = (g[nf][nc] == '#') ? 1 : 0;
            if (dist[f][c] + w < dist[nf][nc]) {
                dist[nf][nc] = dist[f][c] + w;
                if (w == 0) dq.push_front({nf, nc});
                else dq.push_back({nf, nc});
            }
        }
    }
    return dist[ef][ec];
}

// ============================================================
//  11. (Extra) DFS recursivo — recorrido en profundidad — O(n + m)
// ============================================================
void dfs(int u, const vector<vector<int>>& adj, vector<bool>& visitado, vector<int>& orden) {
    visitado[u] = true;
    orden.push_back(u);
    for (int v : adj[u])
        if (!visitado[v]) dfs(v, adj, visitado, orden);
}

//  MAIN DE PRUEBA (ejemplos de clase). En Codeforces se reemplaza.

int main() {
    // Grafo del slide de BFS (s14): 8 vértices, BFS desde 1
    int n = 8;
    vector<vector<int>> adj(n + 1);
    int aristas[9][2] = {{1,2},{1,3},{2,4},{3,4},{3,5},{4,6},{5,6},{5,8},{6,7}};
    for (int i = 0; i < 9; i++) {
        adj[aristas[i][0]].push_back(aristas[i][1]);
        adj[aristas[i][1]].push_back(aristas[i][0]);
    }

    vector<int> dist, padre;
    bfs(1, n, adj, dist, padre);
    cout << "dist:";
    for (int v = 1; v <= n; v++) cout << " " << dist[v];
    cout << "\n";                                   // 0 1 1 2 2 3 4 3

    vector<int> cam = reconstruirCamino(7, dist, padre);
    cout << "camino 1->7:";
    for (int v : cam) cout << " " << v;
    cout << "\n";                                   // 1 2 4 6 7

    vector<long long> cnt = contarCaminosMinimos(1, n, adj, 1000000007);
    cout << "cnt:";
    for (int v = 1; v <= n; v++) cout << " " << cnt[v];
    cout << "\n";                                   // 1 1 1 2 1 3 3 1

    // Hospitales: pueblos 2 y 7
    int n2 = 8;
    vector<vector<int>> adj2(n2 + 1);
    int ar2[7][2] = {{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},{3,8}};
    for (int i = 0; i < 7; i++) {
        adj2[ar2[i][0]].push_back(ar2[i][1]);
        adj2[ar2[i][1]].push_back(ar2[i][0]);
    }
    vector<int> dh = bfsMultiFuente({2, 7}, n2, adj2);
    cout << "hospitales:";
    for (int v = 1; v <= n2; v++) cout << " " << dh[v];
    cout << "\n";                                   // 1 0 1 2 2 1 0 2

    // Componentes y bipartito (Equipos Rivales, ejemplo 1)
    int n3 = 7;
    vector<vector<int>> adj3(n3 + 1);
    int ar3[4][2] = {{1,2},{2,3},{3,4},{5,6}};
    for (int i = 0; i < 4; i++) {
        adj3[ar3[i][0]].push_back(ar3[i][1]);
        adj3[ar3[i][1]].push_back(ar3[i][0]);
    }
    vector<int> comp, color;
    cout << "componentes: " << contarComponentes(n3, adj3, comp) << "\n";        // 3
    cout << "bipartito: " << (esBipartito(n3, adj3, color) ? "SI" : "NO") << "\n"; // SI

    // Grilla: Laberinto (S en (0,0), E en (4,7)) -> 15
    vector<string> lab = {"S..#....", ".#.#.##.", ".#...#..", ".####.#.", "......#E"};
    vector<vector<int>> dl = bfsGrilla(lab, {{0, 0}});
    cout << "laberinto: " << dl[4][7] << "\n";      // 15

    // Perímetro del terreno -> 34 0 9 3 1 0
    vector<string> ter = {"##..#.", "#...##", "..#...", ".###.#", "....##"};
    vector<long long> cg;
    long long P = perimetroGrilla(ter, cg);
    cout << "perimetro: " << P;
    for (int k = 0; k <= 4; k++) cout << " " << cg[k];
    cout << "\n";

    // Rompecabezas 2x3 -> 1 y -1
    cout << "puzzle1: " << bfsEstados("123405", "123450", 2, 3) << "\n";
    cout << "puzzle2: " << bfsEstados("213450", "123450", 2, 3) << "\n";

    // BFS 0-1: aristas (u, v, peso)
    int n4 = 5;
    vector<vector<pair<int, int>>> adj4(n4 + 1);
    int ar4[6][3] = {{1,2,1},{1,3,0},{3,2,0},{2,4,1},{3,5,1},{4,5,0}};
    for (int i = 0; i < 6; i++) {
        adj4[ar4[i][0]].push_back({ar4[i][1], ar4[i][2]});
        adj4[ar4[i][1]].push_back({ar4[i][0], ar4[i][2]});
    }
    vector<int> d01 = bfs01(1, n4, adj4);
    cout << "bfs01:";
    for (int v = 1; v <= n4; v++) cout << " " << d01[v];
    cout << "\n";                                   // 0 0 0 1 1

    // BFS 0-1 en grilla: romper paredes
    vector<string> gp = {"..#", "###", "#.."};
    cout << "paredes: " << bfs01Grilla(gp, 0, 0, 2, 2) << "\n";   // 1

    // Euler: Königsberg (4 zonas con grado impar) -> IMPOSIBLE
    vector<vector<int>> kb(5);
    int ark[7][2] = {{1,2},{1,2},{1,3},{2,3},{2,4},{2,4},{3,4}};
    for (int i = 0; i < 7; i++) {
        kb[ark[i][0]].push_back(ark[i][1]);
        kb[ark[i][1]].push_back(ark[i][0]);
    }
    cout << "euler: " << tipoEuler(kb, 4) << "\n";

    return 0;
}
