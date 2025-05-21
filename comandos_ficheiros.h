// comandos_ficheiros.h
// Declaração de funções para manipulação de ficheiros

#ifndef COMANDOS_FICHEIROS_H
#define COMANDOS_FICHEIROS_H

// Mostra o conteúdo de um ficheiro
int mostra(const char *filename);

// Copia o conteúdo de um ficheiro para um novo ficheiro com extensão .copia
int copia(const char *filename);

// Acrescenta o conteúdo de um ficheiro no final de outro
int acrescenta(const char *origem, const char *destino);

// Conta o número de linhas de um ficheiro
int conta(const char *filename);

// Apaga um ficheiro
int apaga(const char *filename);

// Apresenta informações de um ficheiro
int informa(const char *filename);

// Lista o conteúdo de uma diretoria
int lista(const char *path);

#endif // COMANDOS_FICHEIROS_H