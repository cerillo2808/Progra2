#include <iostream>
using namespace std;

int main(){
    int numero;
    int respuesta;
    int anterior = 1;
    int anterior_anterior = 0;

    printf("Ingrese el número que quiere meterle a Fibonacci: ");

    cin>>numero;

    if (numero==0){
        printf("0");
    }

    if (numero==1){
        printf("1");
    }

    while(true){
        respuesta = anterior + anterior_anterior;
        if ((anterior+1)==numero){
            cout << respuesta;
            break;

        }
        else{
            anterior++;
            anterior_anterior++;
        }
    }

    


}