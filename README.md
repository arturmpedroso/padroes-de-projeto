### Uso de IA
Foi utilizado o Gemini<br>
O texto foi escrito por mim, foi pedido para corrigir minimammente os erros ortográficos e fazer a formatação do README em markdown<br>
O contexto dos problemas são originais (feitos por mim)<br>
O códgio foi feito pelo gemini

# Padrão criacional:
## Builder

Também chamado de Construtor, é um padrão que auxilia na implementação e uso de objetos, separando esse processo em passos. Assim, ele permite a produção de de variações de um objeto tendo o mesmo código como base

### Problema
Ao implementar um objeto complexo, muitos métodos podem ficar explorados pelo código ou possuir assinaturas com muitos argumentos específicos que às vezes não são necessários para a resolução de todos os problemas.

Utilizando como exemplo o projeto em que estou envolvido, estamos implementando um sistema de flashcards. Cada flashcard tem o mesmo comportamento esperado, perguntas, respostas, notas e avaliação de dificuldade pelo usuário, entretanto sua estrutura pode variar muito conforme o tipo de questão a ser elaborada. Portanto, seria inviável criar um genérico método monstruoso com inúmeros parâmetros para a criação de qualquer flashcard.

### Solução
Ao implementar esse padrão de projeto, o código utilizado na construção de um objeto é retirado da própria classe e agrupado sob outro objeto chamado de Builder (Construtor). Dessa forma podemos separar o processo de construção em diferentes partes (construirTextoExplicativo, construirImagem,construirQuestaoMultiplaEscolha, etc), a onde nem todas precisam ser chamadas durante a criação de um novo objeto, permitindo á ele ficar mais adequado ao seu propósito. Sendo assim, também é possível utilizar diferentes builders para a mesma etapa de construção, de modo a garantir que os objetos contenham os mesas interfaces, mas funcionando de modos diferentes.

Diretor - chamar builder após builder de forma espalhado pelo código pode acabar gerando poluição , então, caso seja possível determinar um padrão de construção (flashcard de resposta aberta, flashcard com múltipla escolha de 5 alternativas, flashcard de verdadeiro ou falso com 3 questões, etc), pode-se utilizar objetos chamados de diretores, para chamar etapas da criação em uma ordem específica. Note que não é obrigatório a existência desse objeto, mas ele ajuda muito á reutilizar código e deixar o produto final mais limpo.

### Estrutura:
* **`interface Builder:`** declara as etapas de produção esperadas e comum em qualquer objeto construído pelos builders concretos
* **`class Builder Concreto:`** é onde se encontram as diversas implementações da construção de um objeto.
* **`class Produto:`** são os objetos finais, as classes quais as partes de construção foram retiradas e agrupadas nos Builder Concretos.
* **`class Diretor:`** indica em qual ordem as etapas de construção serão chamadas. Pode receber um builder específico direto na sua criação, ou utilizar métodos de produção que contenham um objeto builder como parâmetro.
* **`class>>Cliente :`** relaciona um objeto builder á um construtor, geralmente isso acontece por meio da passagem de um objeto builder como parâmetro no método construtor de um diretor, de modo á fazer com que ele utilize os métodos daquele objeto nas etapas de construção. Contudo, é possível passar um builder como parámetro em um método de produção do diretor, assim utilizando diferentes builders na construção de um único objeto.

### Referência:
https://refactoring.guru/pt-br/design-patterns/builder

builderExemplo.cpp
```
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
```
# Padrão Estrutural:
## Adapter,Wrapper ou Adaptador

É um padrão de projeto que permite dois objetos com  interfaces diferentes se comunicarem.

### Problema
Por exemplo, um sistema é utilizado  para a formatação de arquivos em formato .odt, entretanto, um cliente especial e com alta demanda necessita deste serviço, mas API utilizada por ele apenas disponibiliza arquivos em formato .docx.

### Solução
Dessa forma, é implementado um objeto adaptador, que irá tratar as informações entre as interfaces para que os objetos conversem e trabalhem entre si. Muitas vezes os objetos encontrados não são notificados da presença do adaptador, pois ele não interfere no funcionamento interno dos objetos.

Um fluxo de uso de um adaptador geralmente compreende os seguintes passos:
Obter uma interface compatível com um dos objetos; Após isso, o objeto consegue chamar os métodos dentro do adaptador com segurança. Quando isso acontece, o adaptador repassa a chamada para o segundo objeto na ordem e formato que são entendidos pelo segundo. 

Continuando no exemplo, seria possível criar uma adaptador DOCX-para-ODT que transformasse apenas o formato do documento, mas mantendo seu conteúdo, assim garantindo o funcionamento correto da aplicação.

### Estrutura Adaptador de objeto:
* **`class Client:`** responsável por manter as ordens de negócio da aplicação;
* **`interface Client Interface:`** descreve o comportamento esperado de outras classes para que estas sejam capazes de interagir com o código do cliente.
* **`class Service:`** é uma classe útil, a qual será utilizada para realizar alguma função. Está é a classe que o cliente não consegue usar por causa da interface divergente.
* **`class Adaptador:`** é o ponto de ligação entre o cliente e o serviço, nela fica a implementação que encobre o objeto de Serviço. Todas as chamadas feitas através da interface do Cliente são convertidas para chamadas nas quais o Serviço consiga entender.

### Referência:
https://refactoring.guru/pt-br/design-patterns/adapter

adapterExemplo.cpp
```
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
```

# Padrão Comportamental: 
## State (ou Estado)

Esse padrão permite que um objeto troque seu comportamento através de uma classe de estado interno. Quando isso acontece, o objeto parece mudar de classe, mas ocorre apenas a troca de um atributo.

### Problema
Imagine um contexto em um objeto que possa assumir uma quantidade finita de estados. Para cada estado, o objeto deve se comportar de modo diferente, além disso ele precisa trocar de estado instantaneamente. Entretanto, conforme cada estado, não é possível acontecer a troca para algum outro. Logo, as regras de transição também existem de forma finita e são pré estabelecidas pelas regras de negócio.

Pensando em uma classe Pedido que pode assumir os estados: aberto, produção, entrega, fechado. Existindo um método “avançar” ele deve se comportar de modo diferente para cada estado:
* No aberto, podem ser adicionados itens á esse pedido por um atendente;
* Na produção, apenas o pessoal da produção pode atualizar seu estado;
* Na entrega, deve-se saber o entregador e apenas ele pode atualizar o estado;
* No fechado, não acontece nada.

É possível implementar isso apenas com os operadores condicionais, mas com o tempo se torna inviável, mais estados e condições podem aparecer de modo gerar uma grande poluição de código. Portanto, devido a complexidade de abstração do problema, é difícil prever quais são todos os estados e condições de transições possíveis de se existir

### Solução
Esse padrão aconselha a criação de classes para atuar como os estados de um objeto, armazenando a implementação de todos os métodos que satisfaçam o comportamento esperado do objeto naquele estado.
Sendo assim, todos os comportamentos são retirados do objeto original, o qual passa a ser chamado de contexto. Ele apenas guarda uma referência á uma classe de estado, para ser possível identificar seu estado atual. Para fazer a transição de estados, o objeto de estado atual é trocado pelo objeto do novo estado. Vale ressaltar que todos os objetos de estado devem seguir a mesma, para que o contexto possa interagir com os mesmos parâmetros.

### Estrutura
* **`class Context:`** deve possuir um atributo de referência para um objeto concreto de  estado, além de conseguir passar todas as chamadas para esses objetos a partir da mesma interface.
* **`interface State:`** contém assinaturas de métodos comuns entre os diversos estados. É importante que eles sejam aplicáveis a todos o estados, a fim de evitar a existência de métodos inutilizados por algum estado
* **`class Concrete State:`** mantém a implementação dos métodos interfaces relativos à funcionalidade daquele estado. É possível que objetos de estado armazenem referências para seu objeto de contexto origem, para garantir a possibilidade de buscar qualquer informação necessária, ou realizar transições de estado. 

OBS: ambos os objetos de estado e contexto podem iniciar uma transição de estado e fazer a substituição da referência presente no contexto.

### Referência:
https://refactoring.guru/pt-br/design-patterns/state

stateExemplo.cpp
```
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
```
