#include <iostream>

// interface padrão para os estados;
class EstadoCalculadora {
public:
    virtual ~EstadoCalculadora() {}
    virtual void calcular(double a, double b) = 0;
};
// Estado de "trava"
class EstadoDesligado : public EstadoCalculadora {
public:
    void calcular(double a, double b) override {
        std::cout << "[Erro] Calculadora esta DESLIGADA. Ligue-a primeiro!" << std::endl;
    }
};

// Estado de funcionamento (stand-by);
class EstadoLigadoSafe : public EstadoCalculadora {
public:
    void calcular(double a, double b) override {
        std::cout << "[Aviso] Modo Seguro: Selecione uma operacao (+ ou -) antes de passar valores." << std::endl;
    }
};

// Estado de adição
class EstadoAdicao : public EstadoCalculadora {
public:
    void calcular(double a, double b) override {
        std::cout << "Resultado da Soma: " << (a + b) << std::endl;
    }
};

// Estado de subtração
class EstadoSubtracao : public EstadoCalculadora {
public:
    void calcular(double a, double b) override {
        std::cout << "Resultado da Subtracao: " << (a - b) << std::endl;
    }
};

// Classe que irá referências as classes de estado
class Calculadora {
private:
    EstadoCalculadora* estadoAtual;
    EstadoDesligado  estadoDesligado;
    EstadoLigadoSafe estadoLigadoSafe;
    EstadoAdicao     estadoAdicao;
    EstadoSubtracao  estadoSubtracao;

public:
    Calculadora() {
        estadoAtual = &estadoDesligado;
    }

    // métodos para transição de estados
    void ligar() {
        std::cout << "\n-> Ligando a calculadora (Safe Mode)..." << std::endl;
        estadoAtual = &estadoLigadoSafe;
    }

    void definirAdicao() {
        std::cout << "\n-> Mudando para o modo de ADICAO (+)..." << std::endl;
        estadoAtual = &estadoAdicao;
    }

    void definirSubtracao() {
        std::cout << "\n-> Mudando para o modo de SUBTRACAO (-)..." << std::endl;
        estadoAtual = &estadoSubtracao;
    }

    void desligar() {
        std::cout << "\n-> Desligando a calculadora..." << std::endl;
        estadoAtual = &estadoDesligado;
    }

    // metodo principal, sem operadores condicionais
    void executarOperacao(double a, double b) {
        estadoAtual->calcular(a, b);
    }
};

int main() {
    Calculadora calc;

    // Tentando calcular com a calculadora desligada
    calc.executarOperacao(10, 5);

    // Ligando a calculadora (stand-by)
    calc.ligar();
    calc.executarOperacao(10, 5);

    // Estado adiçao
    calc.definirAdicao();
    calc.executarOperacao(10, 5);
    calc.executarOperacao(50, 20); 

    // Estado subtração
    calc.definirSubtracao();
    calc.executarOperacao(10, 5);

    // Estado desligado
    calc.desligar();
    calc.executarOperacao(10, 5); // Teste se está desligada

    return 0;
}