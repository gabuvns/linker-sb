# Custom Linker
Linker created for "Basic Software" class at University of Brasilia.
There is also an assembler avaiable at [assembler](https://github.com/gabuvns/assembler-sb) and one [runtime simulator](https://github.com/gabuvns/simulator-sb).

Ligador desenvolvido para a disciplina de Software Básico na Universidade de Brasília.
Existe também um [montador](https://github.com/gabuvns/assembler-sb) disponível e um [simulador](https://github.com/gabuvns/simulator-sb).

## Getting Started
This program executes the code generated with this [assembler](https://github.com/gabuvns/assembler-sb).
Just follow along to get the project running

## Prerequisites
Pretty simple, you only need: 

```
g++ (GCC)
gnu make (opt)
```
(Your version of g++ must support c++17, if your are using gcc 5.0 or newer, you should be good)

## Compiling

#### First option is to simply use make

```
make 
```
#### Or you can compile manually:

```
g++ -o linker ./src/main.cpp ./src/linker.cpp -std=gnu++1z
```

## Usage 
This program takes the name of the files to be linked.

```
./linker <inputFileName> [OPT]<inputFileName2> ... [OPT]<inputFileNameN>
./linker inputFile.obj inputFile2.obj inputFile3.obj
```
## Começando :brazil:
Este ligador suporta as instruções disponíveis na tabela disponível no início do README.
Recebe como entrada o programa de saída do montador.

## Pré-requisitos
Precisamos apenas de:

```
g++ (GCC)
gnu make (opt)
```
(A versão do GCC deve ter suporte para c++17, se sua versão é superior ao gcc 5.0, fique tranquilo.)

## Compilando

#### A primeira opção é simplesmente utilizar o comando make

```
make 
```

#### Ou então pode-se compilar manualmente

```
g++ -o linker ./src/main.cpp ./src/linker.cpp -std=gnu++1z
```

## Uso 
O programa recebe somente o nome do programa a ser executado
```
./linker <inputFileName> [OPT]<inputFileName2> ... [OPT]<inputFileNameN>
./linker inputFile.obj inputFile2.obj inputFile3.obj
```

## Author

* **Carlos Gabriel Vilas Novas Soares** - [Github](https://github.com/gabuvns)
