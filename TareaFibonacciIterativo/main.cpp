#include <iostream>
using namespace std;

//Liqing Yosery Zheng Lu - C38680

/*
Asegúrse de tener un compilador C++ instalado en su máquina. Para compilar, seleccione el archivo de este código fuente, y siga
las instrucciones de su compilador para compilarlo y producir un ejecutable. Ejecute el ejecutable.
Este programa seguirá pidiendo números hasta que el usuario ingrese un número negativo.
*/

int fibonacci(int posicion);
int pregunta ();

int main(){

    while (pregunta()==1){
        continue;
    }

}

int pregunta(){

    int posicion;

    printf ("Ingrese la posicion de Fibonacci que quiere ver: ");

    cin>>posicion;

    if (posicion>=0){
        cout<<fibonacci(posicion)<<endl;
        return 1;
    } else{
        cout<<"Ingreso un numero negativo. Hasta luego."<<endl;
        return 0;
    }
}

int fibonacci(int posicion){
    int anterior = 1;
    int anterior_anterior = 0;
    
    if (posicion==0){
        return 0;
    }
    else if (posicion==1){
        return 1;
    }
    else{
        int resultado;
        
        for (int i = 2; i<=posicion; i++){
            resultado = anterior + anterior_anterior;
            anterior_anterior = anterior;
            anterior = resultado;
        }

        return resultado;
    }
}