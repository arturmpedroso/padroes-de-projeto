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
* **<<interface>> Builder:** declara as etapas de produção esperadas e comum em qualquer objeto construído pelos builders concretos
* **<<class>> Builder Concreto:** é onde se encontram as diversas implementações da construção de um objeto.
* **<<class>> Produto:** são os objetos finais, as classes quais as partes de construção foram retiradas e agrupadas nos Builder Concretos.
* **<<class>> Diretor:** indica em qual ordem as etapas de construção serão chamadas. Pode receber um builder específico direto na sua criação, ou utilizar métodos de produção que contenham um objeto builder como parâmetro.
* **<<class>>Cliente :** relaciona um objeto builder á um construtor, geralmente isso acontece por meio da passagem de um objeto builder como parâmetro no método construtor de um diretor, de modo á fazer com que ele utilize os métodos daquele objeto nas etapas de construção. Contudo, é possível passar um builder como parámetro em um método de produção do diretor, assim utilizando diferentes builders na construção de um único objeto.

### Referência:
https://refactoring.guru/pt-br/design-patterns/builder


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
* **<<class>> Client:** responsável por manter as ordens de negócio da aplicação;
* **<<interface>> Client Interface:** descreve o comportamento esperado de outras classes para que estas sejam capazes de interagir com o código do cliente.
* **<<class>> Service:** é uma classe útil, a qual será utilizada para realizar alguma função. Está é a classe que o cliente não consegue usar por causa da interface divergente.
* **<<class>> Adaptador:** é o ponto de ligação entre o cliente e o serviço, nela fica a implementação que encobre o objeto de Serviço. Todas as chamadas feitas através da interface do Cliente são convertidas para chamadas nas quais o Serviço consiga entender.

### Referência:
https://refactoring.guru/pt-br/design-patterns/adapter


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
* **<<class>> Context:** deve possuir um atributo de referência para um objeto concreto de  estado, além de conseguir passar todas as chamadas para esses objetos a partir da mesma interface.
* **<<interface>> State:** contém assinaturas de métodos comuns entre os diversos estados. É importante que eles sejam aplicáveis a todos o estados, a fim de evitar a existência de métodos inutilizados por algum estado
* **<<clas>> Concrete State:** mantém a implementação dos métodos interfaces relativos à funcionalidade daquele estado. É possível que objetos de estado armazenem referências para seu objeto de contexto origem, para garantir a possibilidade de buscar qualquer informação necessária, ou realizar transições de estado. 

OBS: ambos os objetos de estado e contexto podem iniciar uma transição de estado e fazer a substituição da referência presente no contexto.

### Referência:
https://refactoring.guru/pt-br/design-patterns/state