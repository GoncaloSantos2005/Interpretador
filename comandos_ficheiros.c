/**
 * @file comandos_ficheiros.c
 * @brief Implementação de comandos para manipulação de ficheiros usando system calls.
 * 
 * Este ficheiro contém funções para mostrar, copiar, acrescentar, contar linhas,
 * apagar, informar e listar ficheiros e diretórios, utilizando chamadas de sistema POSIX.
 * 
 * @author Gonçalo e Rodrigo
 * @date 2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <pwd.h>

/// @brief Mostra o conteúdo de um ficheiro no terminal.
/// @author Gonçalo 
/// @param filename Nome do ficheiro a ser mostrado.
/// @return 0 em caso de sucesso, 1 em caso de erro.
/// @details
/// Abre o ficheiro especificado em modo leitura e escreve o seu conteúdo no STDOUT.
/// Utiliza as variáveis:
/// - fd: descritor do ficheiro aberto
/// - n: número de bytes lidos
/// - buffer: buffer temporário para leitura
int mostra(const char *filename) {
    int fd, n;
    char buffer[4096];
    
    // Abrir o ficheiro para leitura
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Erro: O ficheiro '%s' não existe ou não pode ser aberto.\n", filename);
        return 1;
    }
    
    // Ler e mostrar o conteúdo do ficheiro
    while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, n);
    }
    
    // Fechar o ficheiro
    close(fd);
    return 0;
}

/// @brief Copia um ficheiro para um novo ficheiro com extensão ".copia".
/// @author Rodrigo
/// @param filename Nome do ficheiro de origem.
/// @return 0 em caso de sucesso, 1 em caso de erro.
/// @details
/// Cria um novo ficheiro com o mesmo nome acrescido de ".copia" e copia o conteúdo.
/// Variáveis:
/// - fd_src: descritor do ficheiro de origem
/// - fd_dest: descritor do ficheiro de destino
/// - n: número de bytes lidos
/// - buffer: buffer temporário
/// - dest_filename: nome do ficheiro de destino
int copia(const char *filename) {
    int fd_src, fd_dest, n;
    char buffer[4096];
    char dest_filename[1024];
    
    // Verificar se o ficheiro de origem existe
    if (access(filename, F_OK) != 0) {
        fprintf(stderr, "Erro: O ficheiro '%s' não existe.\n", filename);
        return 1;
    }

    // Construir nome do ficheiro de destino
    snprintf(dest_filename, sizeof(dest_filename), "%s.copia", filename);
    
    // Abrir o ficheiro de origem
    fd_src = open(filename, O_RDONLY);
    if (fd_src == -1) {
        fprintf(stderr, "Erro: O ficheiro '%s' não existe ou não pode ser aberto.\n", filename);
        return 1;
    }
    
    // Criar ficheiro de destino
    fd_dest = open(dest_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_dest == -1) {
        fprintf(stderr, "Erro: Não foi possível criar o ficheiro '%s'.\n", dest_filename);
        close(fd_src);
        return 1;
    }
    
    // Copiar conteúdo
    while ((n = read(fd_src, buffer, sizeof(buffer))) > 0) {
        write(fd_dest, buffer, n);
    }
    
    // Fechar ficheiros
    close(fd_src);
    close(fd_dest);
    
    printf("Ficheiro copiado com sucesso para '%s'.\n", dest_filename);
    return 0;
}

/// @brief Acrescenta o conteúdo de um ficheiro ao final de outro ficheiro.
/// @author Gonçalo
/// @param origem Nome do ficheiro de origem.
/// @param destino Nome do ficheiro de destino.
/// @return 0 em caso de sucesso, 1 em caso de erro.
/// @details
/// Abre ambos os ficheiros, verifica se são diferentes, e acrescenta o conteúdo.
/// Variáveis:
/// - fd_src: descritor do ficheiro de origem
/// - fd_dest: descritor do ficheiro de destino
/// - n: número de bytes lidos
/// - buffer: buffer temporário
/// - stat_src, stat_dest: estruturas stat para verificação de ficheiros
int acrescenta(const char *origem, const char *destino) {
    int fd_src, fd_dest, n;
    char buffer[4096];
    struct stat stat_src, stat_dest;

    // Verificar se ficheiro de origem existe
    if (access(origem, F_OK) != 0) {
        fprintf(stderr, "Erro: O ficheiro de origem '%s' não existe.\n", origem);
        return 1;
    }

    // Verificar se ficheiro de destino existe
    if (access(destino, F_OK) != 0) {
        fprintf(stderr, "Erro: O ficheiro de destino '%s' não existe.\n", destino);
        return 1;
    }

    // Abrir ficheiro de origem
    fd_src = open(origem, O_RDONLY);
    if (fd_src == -1) {
        fprintf(stderr, "Erro: Não foi possível abrir o ficheiro de origem '%s'.\n", origem);
        return 1;
    }

    // Abrir ficheiro de destino para acrescentar
    fd_dest = open(destino, O_WRONLY | O_APPEND);
    if (fd_dest == -1) {
        fprintf(stderr, "Erro: Não foi possível abrir o ficheiro de destino '%s'.\n", destino);
        close(fd_src);
        return 1;
    }

    // Verificar se ficheiros são o mesmo (inode e device)
    if (fstat(fd_src, &stat_src) == -1 || fstat(fd_dest, &stat_dest) == -1) {
        fprintf(stderr, "Erro: Falha ao obter informações dos ficheiros.\n");
        close(fd_src);
        close(fd_dest);
        return 1;
    }

    if (stat_src.st_ino == stat_dest.st_ino && stat_src.st_dev == stat_dest.st_dev) {
        fprintf(stderr, "Erro: Os ficheiros de origem e destino são o mesmo. Operação cancelada.\n");
        close(fd_src);
        close(fd_dest);
        return 1;
    }

    // Copiar conteúdo
    while ((n = read(fd_src, buffer, sizeof(buffer))) > 0) {
        if (write(fd_dest, buffer, n) != n) {
            fprintf(stderr, "Erro: Falha ao escrever no ficheiro de destino.\n");
            close(fd_src);
            close(fd_dest);
            return 1;
        }
    }

    if (n < 0) {
        fprintf(stderr, "Erro: Falha ao ler o ficheiro de origem.\n");
    }

    // Fechar ficheiros
    close(fd_src);
    close(fd_dest);

    printf("Conteúdo de '%s' acrescentado com sucesso a '%s'.\n", origem, destino);
    return 0;
}

/// @brief Conta o número de linhas de um ficheiro.
/// @author Rodrigo
/// @param filename Nome do ficheiro.
/// @return 0 em caso de sucesso, 1 em caso de erro.
/// @details
/// Lê o ficheiro e conta o número de caracteres '\n'.
/// Variáveis:
/// - fd: descritor do ficheiro
/// - n: número de bytes lidos
/// - buffer: buffer temporário
/// - line_count: contador de linhas
int conta(const char *filename) {
    int fd, n;
    char buffer[4096];
    int line_count = 0;
    
    // Abrir o ficheiro
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Erro: O ficheiro '%s' não existe ou não pode ser aberto.\n", filename);
        return 1;
    }
    
    // Ler e contar linhas
    while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < n; i++) {
            if (buffer[i] == '\n') {
                line_count++;
            }
        }
    }

    if (n < 0) {
    fprintf(stderr, "Erro: Falha ao ler o ficheiro '%s'.\n", filename);
    close(fd);
    return 1;
    }
    
    // Fechar ficheiro
    close(fd);
    
    printf("O ficheiro '%s' tem %d linhas.\n", filename, line_count);
    return 0;
}

/// @brief Apaga (remove) um ficheiro do sistema de ficheiros.
/// @author Gonçalo
/// @param filename Nome do ficheiro a remover.
/// @return 0 em caso de sucesso, 1 em caso de erro.
/// @details
/// Verifica se o ficheiro existe e tenta removê-lo.
/// Variáveis:
/// - st: estrutura stat para verificar existência
int apaga(const char *filename) {
    struct stat st;

    // Verificar se o ficheiro existe
    if (stat(filename, &st) != 0) {
        fprintf(stderr, "Erro: O ficheiro '%s' não existe.\n", filename);
        return 1;
    }

    // Tentar apagar o ficheiro
    if (unlink(filename) != 0) {
        fprintf(stderr, "Erro: Não foi possível remover o ficheiro '%s'.\n", filename);
        return 1;
    }
    
    printf("Ficheiro '%s' removido com sucesso.\n", filename);
    return 0;
}

/// @brief Mostra informações detalhadas sobre um ficheiro.
/// @author Rodrigo
/// @param filename Nome do ficheiro.
/// @return 0 em caso de sucesso, 1 em caso de erro.
/// @details
/// Mostra tipo, inode, dono, datas de criação, acesso e modificação.
/// Variáveis:
/// - file_stat: estrutura stat com informações do ficheiro
/// - pw: estrutura passwd para obter nome do dono
/// - time_str: buffer para datas formatadas
int informa(const char *filename) {
    struct stat file_stat;
    struct passwd *pw;
    char time_str[100];
    
    // Verificar se o ficheiro existe
    if (access(filename, F_OK) == -1) {
        fprintf(stderr, "Erro: O ficheiro '%s' não existe.\n", filename);
        return 1;
    }

    if (stat(filename, &file_stat) == -1) {
        fprintf(stderr, "Erro: Não foi possível obter informações do ficheiro '%s'.\n", filename);
        return 1;
    }
    
    // Tipo de ficheiro
    printf("Tipo de ficheiro: ");
    if (S_ISREG(file_stat.st_mode))
        printf("Ficheiro regular\n");
    else if (S_ISDIR(file_stat.st_mode))
        printf("Diretoria\n");
    else if (S_ISLNK(file_stat.st_mode))
        printf("Link simbólico\n");
    else if (S_ISFIFO(file_stat.st_mode))
        printf("FIFO/pipe\n");
    else if (S_ISSOCK(file_stat.st_mode))
        printf("Socket\n");
    else if (S_ISCHR(file_stat.st_mode))
        printf("Dispositivo de caracteres\n");
    else if (S_ISBLK(file_stat.st_mode))
        printf("Dispositivo de blocos\n");
    else
        printf("Tipo desconhecido\n");
    
    // Número i-node
    printf("i-node: %lu\n", (unsigned long)file_stat.st_ino);
    
    // Nome do dono
    pw = getpwuid(file_stat.st_uid);
    printf("Utilizador dono: %s\n", pw ? pw->pw_name : "Desconhecido");
    
    // Data de criação
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&file_stat.st_ctime));
    printf("Data de criação: %s\n", time_str);
    
    // Data de último acesso
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&file_stat.st_atime));
    printf("Data do último acesso: %s\n", time_str);
    
    // Data de última modificação
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&file_stat.st_mtime));
    printf("Data da última modificação: %s\n", time_str);
    
    return 0;
}

/// @brief Lista o conteúdo de uma diretoria, mostrando o tipo de cada entrada.
/// @author Gonçalo
/// @param path Caminho da diretoria (se NULL, usa a atual).
/// @return 0 em caso de sucesso, 1 em caso de erro.
/// @details
/// Percorre a diretoria e mostra o nome e tipo de cada entrada.
/// Variáveis:
/// - dir: ponteiro para a diretoria aberta
/// - entry: entrada lida da diretoria
/// - file_stat: estrutura stat para obter tipo
/// - full_path: caminho completo para cada entrada
int lista(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char full_path[1024];
    
    // Se path é NULL, usar diretoria atual
    if (path == NULL) {
        path = ".";
    }
    
    // Abrir diretoria
    dir = opendir(path);
    if (dir == NULL) {
        fprintf(stderr, "Erro: Não foi possível abrir a diretoria '%s'.\n", path);
        return 1;
    }
    
    printf("Conteúdo da diretoria '%s':\n", path);
    
    // Ler entradas da diretoria
    // Ler cada entrada da diretoria
    while ((entry = readdir(dir)) != NULL) {
        // Ignorar "." e ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Construir caminho completo para obter tipo
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &file_stat) == -1) {
            fprintf(stderr, "Erro: Não foi possível aceder a '%s'.\n", full_path);
            continue;
        }

        // Mostrar nome com tipo textual
        if (S_ISDIR(file_stat.st_mode)) {
            printf("[Diretoria] %s\n", entry->d_name);
        } else if (S_ISREG(file_stat.st_mode)) {
            printf("[Ficheiro]  %s\n", entry->d_name);
        } else {
            printf("[Outro]     %s\n", entry->d_name);
        }
    }
    
    // Fechar diretoria
    closedir(dir);
    return 0;
}