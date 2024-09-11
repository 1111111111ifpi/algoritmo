/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/





#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TITULO 100
#define MAX_AUTOR 50
#define MAX_LIVROS 100
#define ARQUIVO_BIBLIOTECA "biblioteca.txt"

typedef struct {
    int id;
    char titulo[MAX_TITULO];
    char autor[MAX_AUTOR];
    int emprestado;  // 0 - disponível, 1 - emprestado
} Livro;

Livro biblioteca[MAX_LIVROS];
int totalLivros = 0;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void salvarBiblioteca() {
    FILE *arquivo = fopen(ARQUIVO_BIBLIOTECA, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    for (int i = 0; i < totalLivros; i++) {
        fprintf(arquivo, "%d\n%s\n%s\n%d\n", biblioteca[i].id, biblioteca[i].titulo, biblioteca[i].autor, biblioteca[i].emprestado);
    }

    fclose(arquivo);
    printf("Biblioteca salva com sucesso!\n\n");
}

void carregarBiblioteca() {
    FILE *arquivo = fopen(ARQUIVO_BIBLIOTECA, "r");
    if (arquivo == NULL) {
        printf("Nenhum arquivo de biblioteca encontrado. Carregando livros famosos.\n\n");

        Livro livrosFamosos[] = {
            {1, "1984", "George Orwell", 0},
            {2, "O Senhor dos Anéis", "J.R.R. Tolkien", 0},
            {3, "Dom Quixote", "Miguel de Cervantes", 0},
            {4, "Orgulho e Preconceito", "Jane Austen", 0},
            {5, "O Pequeno Príncipe", "Antoine de Saint-Exupéry", 0}
        };

        int numLivrosFamosos = sizeof(livrosFamosos) / sizeof(livrosFamosos[0]);
        for (int i = 0; i < numLivrosFamosos; i++) {
            biblioteca[totalLivros] = livrosFamosos[i];
            totalLivros++;
        }

        salvarBiblioteca();
    } else {
        while (fscanf(arquivo, "%d\n", &biblioteca[totalLivros].id) != EOF) {
            fgets(biblioteca[totalLivros].titulo, MAX_TITULO, arquivo);
            biblioteca[totalLivros].titulo[strcspn(biblioteca[totalLivros].titulo, "\n")] = '\0';

            fgets(biblioteca[totalLivros].autor, MAX_AUTOR, arquivo);
            biblioteca[totalLivros].autor[strcspn(biblioteca[totalLivros].autor, "\n")] = '\0';

            fscanf(arquivo, "%d\n", &biblioteca[totalLivros].emprestado);

            totalLivros++;
        }
        fclose(arquivo);
        printf("Biblioteca carregada com sucesso!\n\n");
    }
}

// Função para verificar se o livro está disponível
void verificarDisponibilidadeDeLivro() {
    int id;
    printf("Digite o ID do livro para verificar a disponibilidade: ");
    scanf("%d", &id);

    if (id <= totalLivros && id > 0) {
        if (biblioteca[id - 1].emprestado == 0) {
            printf("O livro \"%s\" está disponível para empréstimo.\n\n", biblioteca[id - 1].titulo);
        } else {
            printf("O livro \"%s\" está emprestado.\n\n", biblioteca[id - 1].titulo);
        }
    } else {
        printf("ID inválido!\n\n");
    }
}

// Função para emprestar um livro
void emprestarLivro() {
    int id;
    printf("Digite o ID do livro para emprestar: ");
    scanf("%d", &id);

    if (id <= totalLivros && id > 0 && biblioteca[id - 1].emprestado == 0) {
        biblioteca[id - 1].emprestado = 1;
        printf("Livro \"%s\" emprestado com sucesso!\n\n", biblioteca[id - 1].titulo);
        salvarBiblioteca();
    } else {
        printf("Livro não disponível ou ID inválido!\n\n");
    }
}

// Função para devolver um livro
void devolverLivro() {
    int id;
    printf("Digite o ID do livro para devolver: ");
    scanf("%d", &id);

    if (id <= totalLivros && id > 0 && biblioteca[id - 1].emprestado == 1) {
        biblioteca[id - 1].emprestado = 0;
        printf("Livro \"%s\" devolvido com sucesso!\n\n", biblioteca[id - 1].titulo);
        salvarBiblioteca();
    } else {
        printf("Livro não encontrado ou já devolvido!\n\n");
    }
}

void exibirLivros() {
    if (totalLivros > 0) {
        printf("Livros na Biblioteca:\n");
        for (int i = 0; i < totalLivros; i++) {
            printf("ID: %d | Título: %s | Autor: %s | Emprestado: %s\n", 
                   biblioteca[i].id, 
                   biblioteca[i].titulo, 
                   biblioteca[i].autor, 
                   biblioteca[i].emprestado ? "Sim" : "Não");
        }
        printf("\n");
    } else {
        printf("Nenhum livro na biblioteca.\n\n");
    }
}

void adicionarLivro() {
    if (totalLivros < MAX_LIVROS) {
        Livro novoLivro;
        novoLivro.id = totalLivros + 1;
        novoLivro.emprestado = 0;

        printf("Digite o título do livro: ");
        limparBuffer();
        fgets(novoLivro.titulo, MAX_TITULO, stdin);
        novoLivro.titulo[strcspn(novoLivro.titulo, "\n")] = '\0';

        printf("Digite o autor do livro: ");
        fgets(novoLivro.autor, MAX_AUTOR, stdin);
        novoLivro.autor[strcspn(novoLivro.autor, "\n")] = '\0';

        biblioteca[totalLivros] = novoLivro;
        totalLivros++;

        printf("Livro adicionado com sucesso!\n\n");
        salvarBiblioteca();
    } else {
        printf("A biblioteca está cheia!\n\n");
    }
}

void removerLivro() {
    int id;
    printf("Digite o ID do livro para remover: ");
    scanf("%d", &id);

    int encontrado = 0;
    for (int i = 0; i < totalLivros; i++) {
        if (biblioteca[i].id == id) {
            for (int j = i; j < totalLivros - 1; j++) {
                biblioteca[j] = biblioteca[j + 1];
            }
            totalLivros--;
            encontrado = 1;
            printf("Livro removido com sucesso!\n\n");
            salvarBiblioteca();
            break;
        }
    }

    if (!encontrado) {
        printf("Livro não encontrado.\n\n");
    }
}

void menu() {
    int opcao;

    do {
        printf("Sistema de Biblioteca\n");
        printf("1. Adicionar Livro\n");
        printf("2. Exibir Livros\n");
        printf("3. Emprestar Livro\n");
        printf("4. Devolver Livro\n");
        printf("5. Verificar Disponibilidade de Livro\n");
        printf("6. Remover Livro\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionarLivro();
                break;
            case 2:
                exibirLivros();
                break;
            case 3:
                emprestarLivro();
                break;
            case 4:
                devolverLivro();
                break;
            case 5:
                verificarDisponibilidadeDeLivro();
                break;
            case 6:
                removerLivro();
                break;
            case 0:
                printf("Saindo do sistema...\n");
                break;
            default:
                printf("Opção inválida!\n\n");
        }
    } while (opcao != 0);
}

int main() {
    carregarBiblioteca();
    menu();
    return 0;

}