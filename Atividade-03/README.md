# Atividade 03 — Controle de Saída Digital com ESP32-S3

## Descrição

Implementação do controle de **4 LEDs utilizando GPIOs do ESP32-S3 configuradas como saídas digitais**, desenvolvido no simulador **Wokwi** com o framework **ESP-IDF**.

O projeto possui duas fases de funcionamento: **contador binário de 4 bits** e **sequência de varredura dos LEDs**.

## Objetivo

Desenvolver uma aplicação utilizando as GPIOs do ESP32-S3 para controlar quatro LEDs, demonstrando o funcionamento de saídas digitais e a utilização de temporização no ESP-IDF.

## Componentes

* ESP32-S3
* 4 LEDs
* 4 resistores limitadores de corrente
* Simulador Wokwi

## Funcionamento

### Fase 1 — Contador Binário

Os quatro LEDs representam um contador binário de 4 bits, exibindo valores de:

```text
0000 → 0001 → 0010 → ... → 1110 → 1111
```

O valor é atualizado a cada **500 ms**.

Após `1111`, o contador retorna para `0000`.

O tempo de atualização pode ser configurado por meio de uma diretiva `#define`, por exemplo:

```c
#define DELAY_MS 500
```

### Fase 2 — Sequência de Varredura

Após o contador binário, os LEDs são acionados sequencialmente, seguindo a sequência:

```text
LED1 → LED2 → LED3 → LED4
LED4 → LED3 → LED2 → LED1
```

A sequência é repetida continuamente.

## Inicialização

Todos os LEDs são inicializados **apagados** antes do início das duas fases de funcionamento.

## Organização do Código

O código foi desenvolvido em **C utilizando ESP-IDF**, com a lógica organizada em funções para facilitar a manutenção e a compreensão do programa.

As principais funções são responsáveis por:

* Configuração das GPIOs;
* Controle individual dos LEDs;
* Exibição do contador binário;
* Execução da sequência de varredura;
* Controle do tempo de atualização.

## Estrutura do Projeto

```text
├── Diagrama de Bloco
├── Diagrama Esquemático
├── main
│   └── código-fonte do projeto
├── Foto da Simulação
└── README.md
```

### Diagrama de Bloco

Representa o ESP32-S3 como controlador das quatro saídas digitais conectadas aos LEDs.

### Diagrama Esquemático

Apresenta as conexões entre as GPIOs do ESP32-S3, os resistores e os quatro LEDs, utilizando alimentação de 3,3 V.

### Código

O código principal está localizado na pasta `main` e foi desenvolvido utilizando o framework **ESP-IDF**.

### Simulação

A simulação no Wokwi permite visualizar as duas fases de funcionamento do sistema.

## Simulação no Wokwi

O projeto pode ser acessado diretamente pelo link:

**[Acessar simulação no Wokwi](https://wokwi.com/projects/473241665012797441)**

## Tecnologias Utilizadas

* **ESP32-S3**
* **ESP-IDF**
* **C**
* **Wokwi**

## Resultado

A atividade implementa o controle de quatro LEDs por meio das GPIOs do ESP32-S3, realizando inicialmente a contagem binária de `0` a `15` e, posteriormente, a sequência de varredura dos LEDs.
