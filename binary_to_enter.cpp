// URL de tu repo GitHub aquí
#include <iostream>
using namespace std;

struct Node {
    int bit;           // cada nodo guarda un bit (0 o 1)
    Node* next;
};

class Lista {
private:
    Node* head = nullptr;
    Node* tail = nullptr;   // guardo tail para push_back en O(1)
public:
    void push_back(int b) {
        Node* nuevo = new Node;
        nuevo->bit = b;
        nuevo->next = nullptr;
        if (head == nullptr) {      // lista vacía
            head = tail = nuevo;
        } else {
            tail->next = nuevo;     // engancho al final
            tail = nuevo;           // actualizo tail
        }
    }

    long long aEntero() {
        long long resultado = 0;
        Node* actual = head;
        while (actual != nullptr) {
            resultado = resultado*2 + actual->bit;   el truco de Horner
            actual = actual->next;       // ← HUECO 2: avanzar
        }
        return resultado;
    }
};

int main() {
    int n;
    cin >> n;

    Lista lista;
    for (int i = 0; i < n; i++) {
        int b;
        cin >> b;
        lista.push_back(b);    // inserto al FINAL para mantener el orden original
    }

    cout << lista.aEntero() << endl;
    return 0;
}
