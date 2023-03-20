#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mysql/mysql.h>
#include "./conn.c"

int Conn() {
    MYSQL* CONN = conn();
    mysql_query(CONN, "CREATE TABLE IF NOT EXISTS vendas_com_indices \
      ( \
          id_venda INT PRIMARY KEY, \
          data_venda DATE, \
          id_produto INT, \
          id_cliente INT, \
          valor_total DECIMAL(10, 2), \
          bitmap_idx_produtos VARBINARY(1024), \
          bitmap_idx_clientes VARBINARY(1024) \
      ); \
  ");
    mysql_query(CONN, "CREATE TABLE IF NOT EXISTS vendas_sem_indices \
      ( \
          id_venda INT, \
          data_venda DATE, \
          id_produto INT, \
          id_cliente INT, \
          valor_total DECIMAL(10, 2), \
          bitmap_idx_produtos VARBINARY(1024), \
          bitmap_idx_clientes VARBINARY(1024) \
      ); \
  ");
    mysql_query(CONN, "CREATE INDEX IF NOT EXISTS idx_bitmap_produtos ON vendas_com_indices(bitmap_idx_produtos);");
    mysql_query(CONN, "CREATE INDEX IF NOT EXISTS bitmap_idx_clientes ON vendas_com_indices(bitmap_idx_clientes);");

    mysql_close(CONN);
    return 0;
}