// =============================================================
// Plantilla AVL - CS2023 AED (UTEC)
// Autor: Saul Baltazar Palomino (SAULABP)
// Convencion de Racso: altura(nullptr) = -1, hoja = 0
// (Si el enunciado usa nullptr = 0, cambia las 2 lineas marcadas con [CONVENCION])
// =============================================================
#include <iostream>
#include <algorithm>
using namespace std;

struct Node {
    int key;
    int h = 0;                 // [CONVENCION] altura de un nodo nuevo (hoja). Con nullptr=0 seria 1
    Node* left = nullptr;
    Node* right = nullptr;
    Node(int k) : key(k) {}
};

// ---------- Funciones auxiliares (todas O(1)) ----------
int h(Node* n) { return n ? n->h : -1; }      // [CONVENCION] con nullptr=0 seria: return n ? n->h : 0;

void update(Node* n) {                         // recalcula la altura guardada
    n->h = 1 + max(h(n->left), h(n->right));
}

int fb(Node* n) {                              // factor de balance
    return h(n->left) - h(n->right);
}

// ---------- Rotaciones (O(1)): devuelven la NUEVA raiz del subarbol ----------
Node* rotateRight(Node* y) {                   // caso LL
    Node* x = y->left;
    y->left = x->right;                        // T2 se muda: de hijo der. de x a hijo izq. de y
    x->right = y;                              // y baja
    update(y);                                 // primero y, porque ahora esta ABAJO
    update(x);
    return x;                                  // x sube
}

Node* rotateLeft(Node* x) {                    // caso RR (espejo)
    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    update(x);
    update(y);
    return y;
}

// ---------- Rebalanceo: los 4 casos ----------
Node* rebalance(Node* n) {
    update(n);
    if (fb(n) > 1) {                           // cargado a la IZQUIERDA
        if (fb(n->left) < 0)                   // zigzag "<" -> caso LR
            n->left = rotateLeft(n->left);
        return rotateRight(n);                 // LL (o 2do paso de LR)
    }
    if (fb(n) < -1) {                          // cargado a la DERECHA
        if (fb(n->right) > 0)                  // zigzag ">" -> caso RL
            n->right = rotateRight(n->right);
        return rotateLeft(n);                  // RR (o 2do paso de RL)
    }
    return n;                                  // ya estaba balanceado
}

// ---------- Insertar (O(log n)) ----------
Node* insert(Node* n, int key) {
    if (n == nullptr) return new Node(key);    // subarbol vacio: aqui va el nuevo
    if (key < n->key)      n->left  = insert(n->left, key);
    else if (key > n->key) n->right = insert(n->right, key);
    else                   return n;           // clave repetida: no se inserta
    return rebalance(n);                       // en la VUELTA: actualizar y rotar si hace falta
}

// ---------- Eliminar (O(log n)) ----------
Node* minNode(Node* n) {
    while (n->left != nullptr) n = n->left;
    return n;
}

Node* erase(Node* n, int key) {
    if (n == nullptr) return nullptr;          // no existe
    if (key < n->key)      n->left  = erase(n->left, key);
    else if (key > n->key) n->right = erase(n->right, key);
    else {
        if (n->left == nullptr || n->right == nullptr) {   // 0 o 1 hijo
            Node* hijo = (n->left != nullptr) ? n->left : n->right;
            delete n;
            return hijo;                       // puede ser nullptr
        }
        Node* s = minNode(n->right);           // 2 hijos: sucesor inorden
        n->key = s->key;
        n->right = erase(n->right, s->key);
    }
    return rebalance(n);
}

// ---------- Buscar (O(log n)) ----------
bool contains(Node* n, int key) {
    while (n != nullptr) {
        if (key == n->key) return true;
        n = (key < n->key) ? n->left : n->right;
    }
    return false;
}

// ---------- Recorridos y limpieza ----------
void inorder(Node* n) {                        // imprime las claves ordenadas
    if (n == nullptr) return;
    inorder(n->left);
    cout << n->key << " ";
    inorder(n->right);
}

void preorder(Node* n) {
    if (n == nullptr) return;
    cout << n->key << " ";
    preorder(n->left);
    preorder(n->right);
}

void destroy(Node* n) {
    if (n == nullptr) return;
    destroy(n->left);
    destroy(n->right);
    delete n;
}

// =============================================================
// main de ejemplo: lee n claves y, despues de cada insercion,
// imprime la altura del arbol y la clave de la raiz.
// Adaptar al formato exacto del enunciado.
// =============================================================
int main() {
    int n;
    cin >> n;
    Node* raiz = nullptr;
    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        raiz = insert(raiz, x);
        cout << h(raiz) << " " << raiz->key << "\n";
    }
    destroy(raiz);
    return 0;
}
