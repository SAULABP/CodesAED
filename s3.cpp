

#include <iostream>
using namespace std;


//  1. LISTA SIMPLEMENTE ENLAZADA

template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
    };
    Node* head = nullptr;
    Node* tail = nullptr;   // para push_back en O(1)
    int sz = 0;

public:
    // Insertar al inicio — O(1)
    void push_front(T d) {
        Node* nuevo = new Node{d, head};
        head = nuevo;
        if (tail == nullptr) tail = nuevo;   // era vacía
        sz++;
    }

    // Insertar al final — O(1) gracias a tail
    void push_back(T d) {
        Node* nuevo = new Node{d, nullptr};
        if (head == nullptr) head = tail = nuevo;
        else { tail->next = nuevo; tail = nuevo; }
        sz++;
    }

    // Insertar después del nodo con valor 'ref' — O(n)
    void insert_after(T ref, T d) {
        Node* actual = head;
        while (actual != nullptr && actual->data != ref)
            actual = actual->next;
        if (actual == nullptr) return;       // no se encontró
        Node* nuevo = new Node{d, actual->next};
        actual->next = nuevo;
        if (actual == tail) tail = nuevo;    // insertó al final
        sz++;
    }

    // Buscar por valor — O(n). Devuelve true si existe.
    bool search(T d) {
        Node* actual = head;
        while (actual != nullptr) {
            if (actual->data == d) return true;
            actual = actual->next;
        }
        return false;
    }

    // Eliminar la primera aparición de un valor — O(n)
    void remove(T d) {
        if (head == nullptr) return;
        if (head->data == d) {               // caso: es el primero
            Node* borrar = head;
            head = head->next;
            if (head == nullptr) tail = nullptr;
            delete borrar;
            sz--;
            return;
        }
        Node* actual = head;
        while (actual->next != nullptr && actual->next->data != d)
            actual = actual->next;
        if (actual->next == nullptr) return; // no está
        Node* borrar = actual->next;
        actual->next = borrar->next;
        if (borrar == tail) tail = actual;
        delete borrar;
        sz--;
    }

    // Recorrer e imprimir — O(n)
    void print() {
        Node* actual = head;
        while (actual != nullptr) {
            cout << actual->data;
            if (actual->next != nullptr) cout << " ";
            actual = actual->next;
        }
        cout << "\n";
    }

    int size() { return sz; }
    bool empty() { return head == nullptr; }
};

// ============================================================
//  2. PILA (STACK) — LIFO
//  Implementada sobre lista enlazada: push/pop en el frente = O(1)
// ============================================================
template <typename T>
class Stack {
private:
    struct Node { T data; Node* next; };
    Node* topNode = nullptr;
    int sz = 0;

public:
    void push(T d) {                    // meter arriba — O(1)
        topNode = new Node{d, topNode};
        sz++;
    }
    void pop() {                        // sacar arriba — O(1)
        if (topNode == nullptr) return;
        Node* borrar = topNode;
        topNode = topNode->next;
        delete borrar;
        sz--;
    }
    T top() { return topNode->data; }   // ver el de arriba (¡revisa empty antes!)
    bool empty() { return topNode == nullptr; }
    int size() { return sz; }
};

// ============================================================
//  3. COLA (QUEUE) — FIFO
//  Enqueue al final, dequeue del frente. Con punteros front/back = O(1)
// ============================================================
template <typename T>
class Queue {
private:
    struct Node { T data; Node* next; };
    Node* frontNode = nullptr;
    Node* backNode = nullptr;
    int sz = 0;

public:
    void enqueue(T d) {                 // meter al final — O(1)
        Node* nuevo = new Node{d, nullptr};
        if (backNode == nullptr) frontNode = backNode = nuevo;
        else { backNode->next = nuevo; backNode = nuevo; }
        sz++;
    }
    void dequeue() {                    // sacar del frente — O(1)
        if (frontNode == nullptr) return;
        Node* borrar = frontNode;
        frontNode = frontNode->next;
        if (frontNode == nullptr) backNode = nullptr;
        delete borrar;
        sz--;
    }
    T front() { return frontNode->data; }
    bool empty() { return frontNode == nullptr; }
    int size() { return sz; }
};

// ============================================================
//  4. COLA DOBLE (DEQUE) — meter/sacar por ambos extremos
//  Implementada sobre lista DOBLEMENTE enlazada para pop_back en O(1)
// ============================================================
template <typename T>
class Deque {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
    };
    Node* frontNode = nullptr;
    Node* backNode = nullptr;
    int sz = 0;

public:
    void push_front(T d) {              // meter adelante — O(1)
        Node* nuevo = new Node{d, nullptr, frontNode};
        if (frontNode == nullptr) frontNode = backNode = nuevo;
        else { frontNode->prev = nuevo; frontNode = nuevo; }
        sz++;
    }
    void push_back(T d) {               // meter atrás — O(1)
        Node* nuevo = new Node{d, backNode, nullptr};
        if (backNode == nullptr) frontNode = backNode = nuevo;
        else { backNode->next = nuevo; backNode = nuevo; }
        sz++;
    }
    void pop_front() {                  // sacar adelante — O(1)
        if (frontNode == nullptr) return;
        Node* borrar = frontNode;
        frontNode = frontNode->next;
        if (frontNode == nullptr) backNode = nullptr;
        else frontNode->prev = nullptr;
        delete borrar;
        sz--;
    }
    void pop_back() {                   // sacar atrás — O(1) gracias a prev
        if (backNode == nullptr) return;
        Node* borrar = backNode;
        backNode = backNode->prev;
        if (backNode == nullptr) frontNode = nullptr;
        else backNode->next = nullptr;
        delete borrar;
        sz--;
    }
    T front() { return frontNode->data; }
    T back() { return backNode->data; }
    bool empty() { return frontNode == nullptr; }
    int size() { return sz; }
};

// ============================================================
//  EJEMPLO DE USO (borra o comenta esto según el problema)
// ============================================================
int main() {
    // Ejemplo rápido de cada estructura
    Stack<int> pila;
    pila.push(4); pila.push(7); pila.push(2);
    cout << "Pila top: " << pila.top() << "\n";   // 2 (LIFO)

    Queue<int> cola;
    cola.enqueue(4); cola.enqueue(7); cola.enqueue(2);
    cout << "Cola front: " << cola.front() << "\n"; // 4 (FIFO)

    Deque<int> dq;
    dq.push_back(5); dq.push_front(2); dq.push_back(8);
    dq.pop_front();
    cout << "Deque front: " << dq.front() << " back: " << dq.back() << "\n"; // 5 y 8

    return 0;
}
