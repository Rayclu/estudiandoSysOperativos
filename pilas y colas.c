#include <stdio.h>
#define MAX 100

int stack[MAX];
int top = -1;  // Indica el tope de la pila

void push(int value) {
    if (top == MAX - 1) {
        printf("Error: Pila llena.\n");
    } else {
        stack[++top] = value;  // Añade el valor al tope
    }
}

int pop() {
    if (top == -1) {
        printf("Error: Pila vacía.\n");
        return -1;
    } else {
        return stack[top--];  // Devuelve y elimina el elemento superior
    }
}

void display() {
    if (top == -1) {
        printf("Pila vacía.\n");
    } else {
        for (int i = top; i >= 0; i--) {
            printf("%d ", stack[i]);
        }
        printf("\n");
    }
}

int main() {
    push(10);
    push(20);
    push(30);
    display();
    printf("Elemento eliminado: %d\n", pop());
    display();
    return 0;
}
