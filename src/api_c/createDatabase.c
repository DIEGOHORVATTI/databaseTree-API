#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mysql/mysql.h>
#include "./conn.c"

int Conn() {
  MYSQL* CONN = conn();

  mysql_query(CONN, "create table if not exists dias_produtivos( \
                id_dia int auto_increment not null, \
                dia date not null unique, \
                primary key(id_dia));");

  mysql_query(CONN, "create table if not exists turno( \
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

  mysql_query(CONN, "create table if not exists condutores( \
                id_condutor int auto_increment not null, \
                nome_condutor varchar(30) not null, \
                primary key(id_condutor) \
                );");

  mysql_query(CONN, "create table if not exists pedidos( \
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

  mysql_query(CONN, "create table if not exists clientes( \
                codcli int auto_increment not null, \
                nomecli varchar(30) not null, \
                primary key(codcli) \
    );");

  mysql_query(CONN, "create table if not exists producao( \
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

  mysql_query(CONN, "delimiter $$ \
              create function fnSomaPesoPedido(p integer) returns float deterministic \
              begin \
              declare soma float; \
  (select sum(producao.peso_produzido) into soma from producao inner join pedidos on producao.id_pedido = pedidos.id_pedido where pedidos.pedido = p);
              return soma; \
              end \
              $$ delimiter;");

              mysql_query(CONN, "delimiter $$ \
                          create function fnBuscaDiaProducao(t integer) returns date deterministic \
                          begin \
                          declare dia date; \
  (select dias_produtivos.dia into dia from dias_produtivos inner join turno on dias_produtivos.id_dia = turno.id_dia where turno.id_turno = t); \
  return dia; \
  end \
    $$ delimiter;");

  mysql_query(CONN, "create or replace view produzidos as select producao.id_producao as id, pedidos.pedido as pedido, pedidos.data_entrada, fnBuscaDiaProducao(turno.id_turno) as data_producao, producao.peso_produzido, producao.qtd_produzida as peças, fnSomaPesoPedido(pedidos.pedido) as peso_pedido, fnContagemPaletesPedido(pedidos.pedido) as paletes_pedido, producao.obs \
              from((producao inner join pedidos on producao.id_pedido = pedidos.id_pedido) inner join turno on producao.id_turno = turno.id_turno) \
              where(pedidos.status = 'Produzido' or pedidos.status = 'Em produção') and producao.id_turno in \
  (select turno.id_turno from turno inner join dias_produtivos on turno.id_dia = dias_produtivos.id_dia where dias_produtivos.id_dia in \
  (select id_dia from dias_produtivos where dia between(subdate(curdate(), interval 1 month)) and curdate())) order by producao.id_producao desc;");

  mysql_close(CONN);
  return 0;
}