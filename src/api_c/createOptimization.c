#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mysql/mysql.h>
#include "./conn.c"

int otimizar() {
  MYSQL* CONN = conn();

  mysql_query(CONN, "delimiter $$ \
    create function fnContagemPaletesPedido(p integer) returns integer deterministic \
    begin \
    declare cont int; \
    (select count(producao.item_completo) into cont from producao inner join pedidos on producao.id_pedido = pedidos.id_pedido where item_completo < > 0 and pedidos.pedido = p); \
    return cont; \
    end \
    $$ delimiter;");

  mysql_query(CONN, "delimiter $$ \
    create function fnSomaPesoPedido(p integer) returns float deterministic \
    begin \
    declare soma float; \
    (select sum(producao.peso_produzido) into soma from producao inner join pedidos on producao.id_pedido = pedidos.id_pedido where pedidos.pedido = p); \
    return soma; \
    end \
    $$ delimiter;");

  mysql_query(CONN, "create or replace view produzidos as select producao.id_producao as id, pedidos.pedido as pedido, pedidos.data_entrada, fnBuscaDiaProducao(turno.id_turno) as data_producao, \
    producao.peso_produzido, producao.qtd_produzida as peças, fnSomaPesoPedido(pedidos.pedido) as peso_pedido, fnContagemPaletesPedido(pedidos.pedido) as paletes_pedido, producao.obs \
    from((producao inner join pedidos on producao.id_pedido = pedidos.id_pedido) inner join turno on producao.id_turno = turno.id_turno) \
    where(pedidos.status = 'Produzido' or pedidos.status = 'Em produção') and producao.id_turno in \
    (select turno.id_turno from turno inner join dias_produtivos on turno.id_dia = dias_produtivos.id_dia where dias_produtivos.id_dia in \
    (select id_dia from dias_produtivos where dia between(subdate(curdate(), interval 1 month)) and curdate())) order by producao.id_producao desc;");

}