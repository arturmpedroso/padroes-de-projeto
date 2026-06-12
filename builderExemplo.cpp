/*Imaginando uma classe calculadora, pode vir com métodos de operações implementados de forma diferente, 
por exemplo:
- uma divisão pode ser feita por meio de subtrações sucessivas ou pelo próprio perador
- uma potenciação pode ser feita pelo operador ou por um laço de repetição.

Será possível chamar contrutores diferentes, para implementar cada método dessa classe */

#include <iostream>
#include <cmath>

// funções utilizando as operações "nativas"
double divNativa(double a, double b) { return a / b; }
double potNativa(double a, double b) { return std::pow(a, b); }

// funções das operações em "força bruta"
double divBruta(double a, double b) {
    double resultado = 0;
    double acumulado = a;
    while (acumulado >= b) {
        acumulado -= b;
        resultado++;
    }
    return resultado;
}

double potBruta(double a, double b) {
    double resultado = 1;
    for (int i = 0; i < b; ++i) resultado *= a;
    return resultado;
}

// Classe calculadora que irá conter as referências para as operações
class Calculadora {
public:
    double (*operacaoDivisao)(double, double) = nullptr;
    double (*operacaoPotencia)(double, double) = nullptr;

    double dividir(double a, double b) {
        if (b == 0) {
            std::cerr << "Erro: Divisao por zero!" << std::endl;
            return 0;
        }
        if (operacaoDivisao) return operacaoDivisao(a, b);
        return 0;
    }

    double potenciar(double a, double b) {
        if (operacaoPotencia) return operacaoPotencia(a, b);
        return 0;
    }
};

// a interface padrão do Builder
class CalculadoraBuilder {
public:
    virtual ~CalculadoraBuilder() {}
    virtual void reset() = 0;
    virtual void construirDivisao() = 0;
    virtual void construirPotencia() = 0;
    virtual Calculadora* obterResultado() = 0; 
};

// Builder concreto com as implementações dos métodos "nativos"
class BuilderNativo : public CalculadoraBuilder {
private:
    Calculadora* calculadora;
public:
    BuilderNativo() { reset(); }
    
    void reset() override { calculadora = new Calculadora(); } // Alocação Bruta
    
    void construirDivisao() override { calculadora->operacaoDivisao = divNativa; }
    void construirPotencia() override { calculadora->operacaoPotencia = potNativa; }
    
    Calculadora* obterResultado() override {
        Calculadora* produto = calculadora;
        reset(); 
        return produto;
    }
};

// Builder Concreto com a implementação dos metódos em "força bruta"
class BuilderBruta : public CalculadoraBuilder {
private:
    Calculadora* calculadora;
public:
    BuilderBruta() { reset(); }
    
    void reset() override { calculadora = new Calculadora(); }
    
    void construirDivisao() override { calculadora->operacaoDivisao = divBruta; }
    void construirPotencia() override { calculadora->operacaoPotencia = potBruta; }
    
    Calculadora* obterResultado() override {
        Calculadora* produto = calculadora;
        reset();
        return produto;
    }
};

// Diretor, que ira construir os métodos conforme o metodo no parametro
class Diretor {
public:
    void criarCalculadoraCompleta(CalculadoraBuilder& builder) {
        builder.construirDivisao(); //
        builder.construirPotencia(); //
    }
};

int main() {
    Diretor diretor;

    // Calculadora com métodos "nativos"
    BuilderNativo builderNativo;
    diretor.criarCalculadoraCompleta(builderNativo);
    Calculadora* calcNativa = builderNativo.obterResultado(); // Recebe o ponteiro bruto

    std::cout << "--- Calculadora Nativa ---" << std::endl;
    std::cout << "9 / 3 = " << calcNativa->dividir(9, 3) << std::endl;

    // Calculadora com métodos "força bruta"
    BuilderBruta builderBruta;
    diretor.criarCalculadoraCompleta(builderBruta);
    Calculadora* calcBruta = builderBruta.obterResultado();

    std::cout << "\n--- Calculadora Bruta ---" << std::endl;
    std::cout << "2^3 = " << calcBruta->potenciar(2, 3) << std::endl;

    delete calcNativa;
    delete calcBruta;

    return 0;
}