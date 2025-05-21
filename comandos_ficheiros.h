/**
 * @file comandos_ficheiros.h
 * @brief Declaração de funções para manipulação de ficheiros e diretórios.
 *
 * Este ficheiro contém as declarações das funções que permitem mostrar, copiar,
 * acrescentar, contar linhas, apagar, informar e listar ficheiros e diretórios.
 *
 * @author Goncalo e Rodrigo
 * @date 2025
 */

#ifndef COMANDOS_FICHEIROS_H
#define COMANDOS_FICHEIROS_H

/**
 * @brief Mostra o conteúdo de um ficheiro no terminal.
 * @param filename Nome do ficheiro a ser mostrado.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int mostra(const char *filename);

/**
 * @brief Copia o conteúdo de um ficheiro para um novo ficheiro com extensão .copia.
 * @param filename Nome do ficheiro de origem.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int copia(const char *filename);

/**
 * @brief Acrescenta o conteúdo de um ficheiro no final de outro.
 * @param origem Nome do ficheiro de origem.
 * @param destino Nome do ficheiro de destino.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int acrescenta(const char *origem, const char *destino);

/**
 * @brief Conta o número de linhas de um ficheiro.
 * @param filename Nome do ficheiro.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int conta(const char *filename);

/**
 * @brief Apaga um ficheiro.
 * @param filename Nome do ficheiro a remover.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int apaga(const char *filename);

/**
 * @brief Apresenta informações de um ficheiro.
 * @param filename Nome do ficheiro.
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int informa(const char *filename);

/**
 * @brief Lista o conteúdo de uma diretoria.
 * @param path Caminho da diretoria (se NULL, usa a atual).
 * @return 0 em caso de sucesso, 1 em caso de erro.
 */
int lista(const char *path);

#endif // COMANDOS_FICHEIROS_H