/**
 * @file interpretador.c
 * @brief Interpretador de Linha de Comandos em C
 *
 * @mainpage Projeto de Sistemas Operativos - Interpretador de Linha de Comandos
 *
 * Este projeto tem como objetivo desenvolver um interpretador de linha de comandos simples,
 * capaz de executar comandos personalizados (como `mostra` e `lista`) e comandos do sistema Unix/Linux.
 * 
 * O interpretador funciona em ciclo contínuo até ser introduzido o comando `termina`.
 * É responsável por interpretar a linha de entrada, separar argumentos, tratar comandos inválidos,
 * executar processos (com fork e execvp) e reportar o código de saída de cada comando.
 * 
 * Funcionalidades principais:
 * - Execução de comandos personalizados
 * - Execução de comandos do sistema
 * - Gestão de processos com fork e waitpid
 * - Tratamento de erros e mensagens de ajuda
 * - Separação modular do código para facilitar manutenção e extensibilidade
 *
 * O projeto foi desenvolvido no contexto da unidade curricular de Sistemas Operativos do curso de
 * Engenharia de Sistemas Informáticos no Instituto Politécnico do Cávado e do Ave (IPCA).
 *
 * @author
 * Gonçalo Santos e Rodrigo Cruz
 * 
 * @date Maio de 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "comandos_ficheiros.h"

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 64

/**
 * @brief Analisa uma linha de comando e separa em argumentos.
 * @param cmd String com o comando a analisar (modificada pela função).
 * @param args Array de strings onde serão colocados os argumentos.
 * @return Número de argumentos encontrados.
 */
int parse_command(char *cmd, char *args[]) {
    int i = 0;
    char *token;
    
    // Remover o caractere de nova linha do final, se existir
    size_t len = strlen(cmd);
    if (len > 0 && cmd[len-1] == '\n') {
        cmd[len-1] = '\0';
    }
    
    // Tokenizar a string por espaços
    token = strtok(cmd, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;  // O último argumento deve ser NULL para exec
    
    return i;  // Retorna o número de argumentos
}

/**
 * @brief Executa um comando personalizado, se existir.
 * @param args Array de argumentos do comando.
 * @return Código de retorno do comando executado, ou -1 se não for personalizado.
 */
int execute_custom_command(char *args[]) {
    if (args[0] == NULL) {
        return 0;
    }
    
    // Verificar qual comando foi solicitado
    if (strcmp(args[0], "mostra") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "Erro: O comando 'mostra' requer um nome de ficheiro.\n");
            return 1;
        }
        return mostra(args[1]);
    }
    else if (strcmp(args[0], "copia") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "Erro: O comando 'copia' requer um nome de ficheiro.\n");
            return 1;
        }
        return copia(args[1]);
    }
    else if (strcmp(args[0], "acrescenta") == 0) {
        if (args[1] == NULL || args[2] == NULL) {
            fprintf(stderr, "Erro: O comando 'acrescenta' requer dois nomes de ficheiros.\n");
            return 1;
        }
        return acrescenta(args[1], args[2]);
    }
    else if (strcmp(args[0], "conta") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "Erro: O comando 'conta' requer um nome de ficheiro.\n");
            return 1;
        }
        return conta(args[1]);
    }
    else if (strcmp(args[0], "apaga") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "Erro: O comando 'apaga' requer um nome de ficheiro.\n");
            return 1;
        }
        return apaga(args[1]);
    }
    else if (strcmp(args[0], "informa") == 0) {
        if (args[1] == NULL) {
            fprintf(stderr, "Erro: O comando 'informa' requer um nome de ficheiro.\n");
            return 1;
        }
        return informa(args[1]);
    }
    else if (strcmp(args[0], "lista") == 0) {
        return lista(args[1]);  // args[1] pode ser NULL, a função trata isso
    }
    else {
        // Se não for um comando personalizado, retorna -1
        return -1;
    }
}

/**
 * @brief Função principal do interpretador.
 * @return 0 ao terminar.
 */
int main() {
    char command[MAX_COMMAND_LENGTH];
    char *args[MAX_ARGS];
    pid_t pid;
    int status;
    
    while (1) {
        // Mostrar o prompt
        printf("%% ");
        fflush(stdout);
        
        // Ler o comando
        if (fgets(command, MAX_COMMAND_LENGTH, stdin) == NULL) {
            break;  // Sair se EOF (Ctrl+D)
        }
        
        // Analisar o comando
        parse_command(command, args);
        
        // Verificar se o comando é "termina"
        if (args[0] != NULL && strcmp(args[0], "termina") == 0) {
            break;
        }
        
        // Tentar executar como comando personalizado
        int result = execute_custom_command(args);
        
        if (result >= 0) {
            // Foi um comando personalizado
            printf("Terminou comando %s com código %d\n", args[0], result);
        } else {
            // Não é um comando personalizado, executar como comando do sistema
            pid = fork();
            
            if (pid < 0) {
                fprintf(stderr, "Erro: Falha ao criar um novo processo.\n");
                continue;
            } else if (pid == 0) {
                // Processo filho executa o comando
                execvp(args[0], args);
                
                // Se chegar aqui, houve um erro na execução
                fprintf(stderr, "Erro: Comando '%s' não encontrado. Use 'termina' para sair.\n", args[0]);
                exit(1);
            } else {
                // Processo pai espera pelo filho terminar
                waitpid(pid, &status, 0);
                
                if (WIFEXITED(status)) {
                    printf("Terminou comando %s com código %d\n", args[0], WEXITSTATUS(status));
                } else {
                    printf("Comando %s terminou de forma anormal\n", args[0]);
                }
            }
        }
    }
    
    return 0;
}