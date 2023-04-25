#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mysql/mysql.h>
#include "./conn.c"

void createDatabaseOtptimization(MYSQL* conn)
{
  mysql_query(conn, "create table if not exists dias_produtivos( \
                id_dia int auto_increment not null, \
                dia date not null unique, \
                primary key(id_dia));");

  mysql_query(conn, "create table if not exists turno( \
                id_turno int auto_increment not null, \
                id_dia int not null, \
                id_condutor int not null, \
                turma enum('A', 'B', 'C', 'D') not null, \
                inicio_turno time not null, \
                final_turno time, \
                obs varchar(256), \
                primary key(id_turno), \
                foreign key(id_dia) references dias_produtivos(id_dia), \
                foreign key(id_condutor) references condutores(id_condutor) \
                );");

  mysql_query(conn, "create table if not exists condutores( \
                id_condutor int auto_increment not null, \
                nome_condutor varchar(30) not null, \
                primary key(id_condutor) \
                );");

  mysql_query(conn, "create table if not exists pedidos( \
                id_pedido int auto_increment not null, \
                codcli int, \
                pedido int not null, \
                data_entrada date not null, \
                tipo_papel enum('Branco', 'Pardo'), \
                aba int not null, \
                espessura float not null, \
                comprimento int not null, \
                quantidade int not null, \
                status enum('Programado', 'Produzido', 'Em produção', 'Carregado'), \
                obs varchar(256), \
                primary key(id_pedido), \
                foreign key(codcli) references clientes(codcli) \
    );");

  mysql_query(conn, "create table if not exists clientes( \
                codcli int auto_increment not null, \
                nomecli varchar(30) not null, \
                primary key(codcli) \
    );");

  mysql_query(conn, "create table if not exists producao( \
      id_producao int auto_increment not null, \
      id_turno int not null, \
      id_pedido int not null, \
      hora_inicio time not null, \
      hora_fim time not null, \
      velocidade_maq int not null, \
      peso_produzido float not null, \
      qtd_produzida int not null, \
      item_completo boolean not null, \
      obs varchar(256), \
      primary key(id_producao), \
      foreign key(id_turno) references turno(id_turno), \
      foreign key(id_pedido) references pedidos(id_pedido) \
    );");
}

void createDatabaseSimple(MYSQL* conn)
{
  mysql_query(conn, "CREATE TABLE IF NOT EXISTS unified_data ( \
                    id INT AUTO_INCREMENT NOT NULL, \
                    dia DATE, \
                    id_turno INT, \
                    id_dia INT, \
                    id_condutor INT, \
                    turma ENUM('A', 'B', 'C', 'D'), \
                    inicio_turno TIME, \
                    final_turno TIME, \
                    obs_turno VARCHAR(256), \
                    nome_condutor VARCHAR(30), \
                    id_pedido INT, \
                    codcli INT, \
                    pedido INT, \
                    data_entrada DATE, \
                    tipo_papel ENUM('Branco', 'Pardo'), \
                    aba INT, \
                    espessura FLOAT, \
                    comprimento INT, \
                    quantidade INT, \
                    status ENUM('Programado', 'Produzido', 'Em produção', 'Carregado'), \
                    obs_pedido VARCHAR(256), \
                    nomecli VARCHAR(30), \
                    id_producao INT, \
                    hora_inicio TIME, \
                    hora_fim TIME, \
                    velocidade_maq INT, \
                    peso_produzido FLOAT, \
                    qtd_produzida INT, \
                    item_completo BOOLEAN, \
                    obs_producao VARCHAR(256) \
                );");
}

int Conn() {
  MYSQL* CONN = conn();

  createDatabaseSimple(CONN);
  createDatabaseOtptimization(CONN);

  mysql_close(CONN);
  return 0;
}