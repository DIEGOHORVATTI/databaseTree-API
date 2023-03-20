#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "./conn.c"

#define MAX_rand 10000

int main() {
    MYSQL* CONN = conn();

    char* queries[MAX_rand];
    for (int i = 0; i < MAX_rand; i++) {
        char query[500];
        snprintf(
            query,
            500,
            "INSERT INTO vendas_sem_indices (id_venda, data_venda, id_produto, id_cliente, valor_total, bitmap_idx_produtos, bitmap_idx_clientes) VALUES (%d, '2022-01-01', %d, %d, %f, 0x01, 0x01);", i + 1, i + 1, i + 1, (i + 1) * 100.0);
        queries[i] = strdup(query);
    }

    printf("Inserindo %d linhas...\n", MAX_rand);
    for (int i = 0; i < MAX_rand; i++) {
        if (mysql_query(CONN, queries[i]) != 0) {
            fprintf(stderr, "Erro ao inserir os dados na linha %d: %s\n", i + 1, mysql_error(CONN));
            mysql_close(CONN);
            exit(1);
        }
    }

    printf("Inserção de dados com sucesso\n");

    mysql_close(CONN);
    return 0;
}
