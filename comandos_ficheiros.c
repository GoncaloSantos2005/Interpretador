// comandos_ficheiros.c
// Implementação de comandos para manipulação de ficheiros usando system calls

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

// Função para mostrar conteúdo de um ficheiro
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

// Função para copiar um ficheiro
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

// Função para acrescentar conteúdo de um ficheiro a outro
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

// Função para contar linhas de um ficheiro
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

// Função para apagar um ficheiro
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

// Função para obter informações de um ficheiro
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

// Função para listar conteúdo de uma diretoria
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