    #include <iostream>
    using namespace std;

    struct Node {
        long long data;      // long long porque los valores llegan hasta 10^9
        Node* next;
    };

    class Lista {
    private:
        Node* head = nullptr;
    public:
        void push_front(long long d) {
            Node* nuevo = new Node;
            nuevo->data = d;
            nuevo->next = head;    // apunta al viejo primero
            head = nuevo;          // head ahora es el nuevo
        }

        void imprimir() {
            Node* actual = head;
            while (actual != nullptr) {
                cout << actual->data;
                if (actual->next != nullptr) cout << " ";  // espacio entre valores
                actual = actual->next;
            }
            cout << endl;
        }
    };

    int main() {
        int n;
        cin >> n;

        Lista lista;
        for (int i = 0; i < n; i++) {
            long long x;
            cin >> x;
            lista.push_front(x);    // ← inserto al inicio: queda invertida sola
        }

        lista.imprimir();
        return 0;
    }
