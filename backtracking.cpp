// =============================================================
// Plantilla Backtracking - CS2023 AED (UTEC)
// Autor: Saul Baltazar Palomino (SAULABP)
// Patron de todo: caso base -> for(opciones) { if(valida) { hacer; explorar; deshacer; } }
// =============================================================
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void imprimir(const vector<int>& v) {
    cout << "{ ";
    for (int x : v) cout << x << " ";
    cout << "}\n";
}

// ---------- 1) Subconjuntos: O(n * 2^n) ----------
// Para cada elemento: NO tomarlo o SI tomarlo
void subconjuntos(int i, const vector<int>& a, vector<int>& elegidos) {
    if (i == (int)a.size()) {                 // ya decidi sobre todos
        imprimir(elegidos);
        return;
    }
    subconjuntos(i + 1, a, elegidos);         // no tomo a[i]
    elegidos.push_back(a[i]);                 // hacer
    subconjuntos(i + 1, a, elegidos);         // explorar
    elegidos.pop_back();                      // deshacer
}

// ---------- 2) Combinaciones C(n,k) de {1..n}, con poda ----------
void combinar(int inicio, int n, int k, vector<int>& actual) {
    if ((int)actual.size() == k) {            // ya tengo k elementos
        imprimir(actual);
        return;
    }
    for (int x = inicio; x <= n; x++) {
        if (n - x + 1 < k - (int)actual.size()) break;   // PODA: no alcanzan los que quedan
        actual.push_back(x);
        combinar(x + 1, n, k, actual);        // los siguientes, mayores que x
        actual.pop_back();
    }
}

// ---------- 3) Permutaciones de {1..n}: O(n * n!) ----------
void permutar(int n, vector<int>& actual, vector<bool>& usado) {
    if ((int)actual.size() == n) {
        imprimir(actual);
        return;
    }
    for (int x = 1; x <= n; x++) {
        if (usado[x]) continue;               // ya esta en la permutacion
        usado[x] = true;  actual.push_back(x);
        permutar(n, actual, usado);
        actual.pop_back(); usado[x] = false;
    }
}

// ---------- 4) N reinas: contar soluciones ----------
// Una reina por fila. Columnas y diagonales marcadas en O(1):
// diagonal  -> fila + col          (0 .. 2n-2)
// antidiag. -> fila - col + n - 1  (0 .. 2n-2)
int N;
vector<bool> colUsada, diag, antiDiag;

int reinas(int fila) {
    if (fila == N) return 1;                  // puse las N reinas
    int total = 0;
    for (int c = 0; c < N; c++) {
        if (colUsada[c] || diag[fila + c] || antiDiag[fila - c + N - 1]) continue;  // PODA: se atacan
        colUsada[c] = diag[fila + c] = antiDiag[fila - c + N - 1] = true;           // hacer
        total += reinas(fila + 1);                                                  // explorar
        colUsada[c] = diag[fila + c] = antiDiag[fila - c + N - 1] = false;          // deshacer
    }
    return total;
}

// ---------- 5) Subconjuntos que suman exactamente "objetivo" ----------
// Requiere el vector ORDENADO y numeros positivos (para poder podar)
void sumaObjetivo(int i, int suma, int objetivo, const vector<int>& a, vector<int>& actual) {
    if (suma == objetivo) {
        imprimir(actual);
        return;
    }
    for (int j = i; j < (int)a.size(); j++) {
        if (suma + a[j] > objetivo) break;    // PODA: ordenado -> los siguientes tambien se pasan
        actual.push_back(a[j]);
        sumaObjetivo(j + 1, suma + a[j], objetivo, a, actual);
        actual.pop_back();
    }
}

// ---------- 6) Hanoi: T(n) = 2T(n-1) + 1 = 2^n - 1 ----------
long long hanoi(int n, char origen, char apoyo, char destino) {
    if (n == 0) return 0;
    long long mov = hanoi(n - 1, origen, destino, apoyo);   // n-1 discos al apoyo
    mov += 1;                                                // disco grande al destino
    mov += hanoi(n - 1, apoyo, origen, destino);            // n-1 discos del apoyo al destino
    return mov;
}

int main() {
    cout << "Subconjuntos de {1,2,3}:\n";
    vector<int> a = {1, 2, 3}, elegidos;
    subconjuntos(0, a, elegidos);

    cout << "\nCombinaciones C(4,2):\n";
    vector<int> actual;
    combinar(1, 4, 2, actual);

    cout << "\nPermutaciones de {1,2,3}:\n";
    vector<int> perm;
    vector<bool> usado(4, false);
    permutar(3, perm, usado);

    N = 8;
    colUsada.assign(N, false);
    diag.assign(2 * N - 1, false);
    antiDiag.assign(2 * N - 1, false);
    cout << "\nSoluciones de 8 reinas: " << reinas(0) << "\n";

    cout << "\nSubconjuntos de {3,5,6,7} que suman 15:\n";
    vector<int> b = {7, 3, 6, 5}, sel;
    sort(b.begin(), b.end());
    sumaObjetivo(0, 0, 15, b, sel);

    cout << "\nMovimientos Hanoi con 3 discos: " << hanoi(3, 'A', 'B', 'C') << "\n";
    return 0;
}
