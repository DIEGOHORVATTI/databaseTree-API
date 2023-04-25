/* #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "./conn.c"

#define MAX_rand 1000000

int main() {
    MYSQL* CONN = conn();

    char* queries[MAX_rand];
    for (int i = 0; i < MAX_rand; i++) {
        char query[500];
        snprintf(
            query,
            500,
            "INSERT INTO vendas_com_indices (id_venda, data_venda, id_produto, id_cliente, valor_total, bitmap_idx_produtos, bitmap_idx_clientes) VALUES (%d, '2022-01-01', %d, %d, %f, 0x01, 0x01);", i + 1, i + 1, i + 1, (i + 1) * 100.0);
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
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mysql/mysql.h>
#include "./conn.c"

#define MAX_rand 1000000

 // Funções auxiliares
void generate_random_time(char* time_str) {
    int hour = rand() % 24;
    int minute = rand() % 60;
    snprintf(time_str, 6, "%02d:%02d", hour, minute);
}

int get_random_dia_id(MYSQL* conn) {
    if (mysql_query(conn, "SELECT id_dia FROM dias_produtivos ORDER BY RAND() LIMIT 1") != 0) {
        fprintf(stderr, "Erro ao selecionar um id_dia aleatório: %s\n", mysql_error(conn));
        return -1;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "Erro ao obter o resultado do id_dia aleatório: %s\n", mysql_error(conn));
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    int id_dia = atoi(row[0]);
    mysql_free_result(result);
    return id_dia;
}

int get_random_condutor_id(MYSQL* conn) {
    if (mysql_query(conn, "SELECT id_condutor FROM condutores ORDER BY RAND() LIMIT 1") != 0) {
        fprintf(stderr, "Erro ao selecionar um id_condutor aleatório: %s\n", mysql_error(conn));
        return -1;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "Erro ao obter o resultado do id_condutor aleatório: %s\n", mysql_error(conn));
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    int id_condutor = atoi(row[0]);
    mysql_free_result(result);
    return id_condutor;
}

int main() {
    MYSQL* CONN = conn();

    srand(time(NULL));

    // Inserir dados randômicos na tabela condutores
    for (int i = 0; i < MAX_rand; i++) {
        char query[128];
        snprintf(query, sizeof(query), "INSERT INTO condutores(nome_condutor) VALUES ('Condutor %d');", rand() % 1000);
        if (mysql_query(CONN, query) != 0) {
            fprintf(stderr, "Erro ao inserir os dados na tabela condutores: %s\n", mysql_error(CONN));
            mysql_close(CONN);
            exit(1);
        }
    }

    // Inserir dados randômicos na tabela clientes
    for (int i = 0; i < MAX_rand; i++) {
        char query[128];
        snprintf(query, sizeof(query), "INSERT INTO clientes(nomecli) VALUES ('Cliente %d');", rand() % 1000);
        if (mysql_query(CONN, query) != 0) {
            fprintf(stderr, "Erro ao inserir os dados na tabela clientes: %s\n", mysql_error(CONN));
            mysql_close(CONN);
            exit(1);
        }
    }

    // Inserir dados randômicos na tabela dias_produtivos
    for (int i = 0; i < MAX_rand; i++) {
        char query[128];
        snprintf(query, sizeof(query), "INSERT INTO dias_produtivos(dia) VALUES ('%s');", random_date());
        if (mysql_query(CONN, query) != 0) {
            fprintf(stderr, "Erro ao inserir os dados na tabela dias_produtivos: %s\n", mysql_error(CONN));
            mysql_close(CONN);
            exit(1);
        }
    }

    // Inserir dados randômicos na tabela turno
    for (int i = 0; i < MAX_rand; i++) {
        int id_dia = get_random_dia_id(conn);
        int id_condutor = get_random_condutor_id(conn);
        char turma = "ABCD"[rand() % 4];

        char inicio_turno[6];
        generate_random_time(inicio_turno);

        char final_turno[6];
        generate_random_time(final_turno);

        char query[256];
        snprintf(query, sizeof(query), "INSERT INTO turno (id_dia, id_condutor, turma, inicio_turno, final_turno) VALUES (%d, %d, '%c', '%s', '%s')", id_dia, id_condutor, turma, inicio_turno, final_turno);

        if (mysql_query(conn, query) != 0) {
            fprintf(stderr, "Erro ao inserir turno: %s\n", mysql_error(conn));
            mysql_close(conn);
            return 1;
        }
    }

    // Inserir dados randômicos na tabela pedidos
    for (int i = 0; i < MAX_rand; i++) {
        char query[512];
        const char* status[] = { "Programado", "Produzido", "Em produção", "Carregado" };
        const char* tipo_papel[] = { "Branco", "Pardo" };
        snprintf(query, sizeof(query), "INSERT INTO pedidos(codcli, pedido, data_entrada, tipo_papel, aba, espessura, comprimento, quantidade, status) VALUES (%d, %d, '%s', '%s', %d, %.2f, %d, %d, '%s');", rand() % MAX_rand + 1, rand() % 1000 + 1, random_date(), tipo_papel[rand() % 2], rand() % 50 + 1, (rand() % 100 + 1) * 0.01, rand() % 500 + 1, rand() % 1000 + 1, status[rand() % 4]);
        if (mysql_query(CONN, query) != 0) {
            fprintf(stderr, "Erro ao inserir os dados na tabela pedidos: %s\n", mysql_error(CONN));
            mysql_close(CONN);
            exit(1);
        }
    }
    // Inserir dados randômicos na tabela producao
    for (int i = 0; i < MAX_rand; i++) {
        char query[512];
        int item_completo = rand() % 2;
        snprintf(query, sizeof(query), "INSERT INTO producao(id_turno, id_pedido, hora_inicio, hora_fim, velocidade_maq, peso_produzido, qtd_produzida, item_completo) VALUES (%d, %d, '%s', '%s', %d, %.2f, %d, %d);", rand() % MAX_rand + 1, rand() % MAX_rand + 1, random_time(), random_time(), rand() % 100 + 1, (rand() % 100 + 1) * 0.01, rand() % 1000 + 1, item_completo);
        if (mysql_query(CONN, query) != 0) {
            fprintf(stderr, "Erro ao inserir os dados na tabela producao: %s\n", mysql_error(CONN));
            mysql_close(CONN);
            exit(1);
        }
    }

    // Inserir dados randômicos na tabela unificada
    for (int i = 0; i < MAX_rand; i++) {
        char query[1024];
        char inicio_turno[6];
        generate_random_time(inicio_turno);

        char final_turno[6];
        generate_random_time(final_turno);

        const char* status[] = { "Programado", "Produzido", "Em produção", "Carregado" };
        const char* tipo_papel[] = { "Branco", "Pardo" };

        snprintf(query, sizeof(query), "INSERT INTO tabela_unificada (nome_condutor, nomecli, dia, turma, inicio_turno, final_turno, codcli, pedido, data_entrada, tipo_papel, aba, espessura, comprimento, quantidade, status_producao, hora_inicio, hora_fim, velocidade_maq, peso_produzido, qtd_produzida, item_completo) VALUES ('Condutor %d', 'Cliente %d', '%s', '%c', '%s', '%s', %d, %d, '%s', '%s', %d, %.2f, %d, %d, '%s', '%s', '%s', %d, %.2f, %d, %d);", rand() % 1000, rand() % 1000, random_date(), "ABCD"[rand() % 4], inicio_turno, final_turno, rand() % MAX_rand + 1, rand() % 1000 + 1, random_date(), tipo_papel[rand() % 2], rand() % 50 + 1, (rand() % 100 + 1) * 0.01, rand() % 500 + 1, rand() % 1000 + 1, status[rand() % 4], inicio_turno, final_turno, rand() % 100 + 1, (rand() % 100 + 1) * 0.01, rand() % 1000 + 1, rand() % 2);

        if (mysql_query(CONN, query) != 0) {
            fprintf(stderr, "Erro ao inserir os dados na tabela unificada: %s\n", mysql_error(CONN));
            mysql_close(CONN);
            exit(1);
        }
    }

    printf("Sucesso na inserção de dados.\n");
    mysql_close(CONN);
    return 0;
}









printf("Sucesso na inserção de dados.\n");
mysql_close(CONN);
return 0;
}
