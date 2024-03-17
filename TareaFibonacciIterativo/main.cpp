#include <iostream>
using namespace std;

int fibonacci(int posicion);

int main(){

    int posicion;

    printf ("Ingrese la posicion de Fibonacci que quiere ver: ");

    cin>>posicion;

    cout<<fibonacci(posicion)<<endl;
    return 0;

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