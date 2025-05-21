# Interpretador de Linha de Comandos em C

Este projeto implementa um interpretador de linha de comandos personalizado em C, que permite executar comandos próprios para manipulação de ficheiros e diretórios, bem como comandos nativos do sistema.

## Funcionalidades

- `mostra <ficheiro>`: Mostra o conteúdo de um ficheiro no terminal.
- `copia <ficheiro>`: Copia o ficheiro para um novo ficheiro com extensão `.copia`.
- `acrescenta <origem> <destino>`: Acrescenta o conteúdo do ficheiro de origem ao final do ficheiro de destino.
- `conta <ficheiro>`: Conta o número de linhas de um ficheiro.
- `apaga <ficheiro>`: Remove um ficheiro.
- `informa <ficheiro>`: Mostra informações detalhadas sobre um ficheiro, como tipo, i-node, dono e datas de criação/modificação/acesso.
- `lista [diretoria]`: Lista o conteúdo de uma diretoria (se não for especificada, usa a atual).
- `termina`: Encerra o interpretador.
- **Comandos do sistema**: Qualquer outro comando é executado como um comando do sistema (por exemplo, `ls`, `cat`, `pwd`, etc.).

## Compilação

```sh
make
```

## Execução

```sh
./interpretador
```

## Exemplos de Utilização

```sh
mostra ./out/texto.txt
copia ./out/texto.txt
acrescenta ./out/texto.txt.copia ./out/texto.txt
conta ./out/texto.txt
apaga ./out/texto.txt.copia
informa ./out/texto.txt
lista
lista /out
termina
```

## Estrutura do Projeto

- `interpretador.c` — Código principal do interpretador
- `comandos_ficheiros.c` — Implementação dos comandos personalizados
- `comandos_ficheiros.h` — Declaração das funções dos comandos
- `Makefile` — Para compilar o projeto
- `README.md` — Este ficheiro

---

Projeto desenvolvido para fins académicos.

**Autores**: Gonçalo & Rodrigo
**Numeros**: 27985 & 27971
