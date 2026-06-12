/*Imaginando um contexto parecido com o primeiro, uma calculadora que recebe três parametros;
Calculadora(A,B,Op)
A e B são dois números, e Op irá indicar a operação a ser feita.
Dessa forma, nossa calculadora pode utilizar int,float,double, etc;
O conteúdo de Op pode ser apenas um char '+' ou uma string "soma", ou até mesmo o ponteiro de uma função.
Imaginando 2 calculadoras Tipo1 e Tipo2:
CalculadoraTipo1(int,int,char)
CalculadoraTipo2(double,double,*char)
Em um caso real as duas seriam chamadas sob demanda para caso específicos, 
mas caso ocorra algum imprevisto é sempre bom ter um Adapter por perto.*/ 

#include <iostream>
#include <cstring> // 

// interface que o cliente espera usar
class AlvoCalculadora {
public:
    virtual ~AlvoCalculadora() {}
    virtual int calcular(int a, int b, char op) = 0;
};


// interface incompatível
class CalculadoraTipo2 {
public:
    double executarCalculo(double a, double b, const char* op) {
        if (std::strcmp(op, "soma") == 0) {
            return a + b;
        }
        if (std::strcmp(op, "subtracao") == 0) {
            return a - b;
        }
        return 0.0;
    }
};

// o adaptador esperando para ser usando.
class AdaptadorCalculadora : public AlvoCalculadora {
private:
    CalculadoraTipo2* calcTipo2; // ponteiro para o serviço a ser utilizado

public:
    // recepção da classe imcompatível
    AdaptadorCalculadora(CalculadoraTipo2* servico) : calcTipo2(servico) {}

    int calcular(int a, int b, char op) override {
        //traduzindo a operação;
        const char* opTraduzido = "invalido";
        if (op == '+') opTraduzido = "soma";
        else if (op == '-') opTraduzido = "subtracao";

        
        // em C++ ocorre a conversão altomática, na hora do contexto estava pensando em C puro...
        double resultadoDouble = calcTipo2->executarCalculo(a, b, opTraduzido);

        // conversão do resultado de volta para int, porque é o tipo dos numeros no parâmetro.
        return static_cast<int>(resultadoDouble);
    }
};

int main() {
    // serviço imcopatível
    CalculadoraTipo2* calculadoraModerna = new CalculadoraTipo2();

    // criação do apatador
    AlvoCalculadora* adaptador = new AdaptadorCalculadora(calculadoraModerna);

    std::cout << "--- Usando a Calculadora Tipo 2 através do Adaptador ---" << std::endl;
    
    // parametros sendo passados pela Calculadora incompatível através dos parametros do método no Adapter
    int resultadoSoma = adaptador->calcular(10, 5, '+');
    int resultadoSub  = adaptador->calcular(10, 5, '-');

    std::cout << "Resultado Adaptado (Soma): " << resultadoSoma << std::endl;
    std::cout << "Resultado Adaptado (Subtracao): " << resultadoSub << std::endl;

    delete adaptador;
    delete calculadoraModerna;

    return 0;
}