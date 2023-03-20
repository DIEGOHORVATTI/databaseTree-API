/* import mysql from 'mysql'
import { conn } from './conn'

const createTables = (conn: mysql.Connection): void => {
  const query1 = `CREATE TABLE IF NOT EXISTS vendas_com_indices
    (
        id_venda INT PRIMARY KEY,
        data_venda DATE,
        id_produto INT,
        id_cliente INT,
        valor_total DECIMAL(10, 2),
        bitmap_idx_produtos VARBINARY(1024),
        bitmap_idx_clientes VARBINARY(1024)
    );
  `

  const query2 = `CREATE TABLE IF NOT EXISTS vendas_sem_indices
    (
        id_venda INT,
        data_venda DATE,
        id_produto INT,
        id_cliente INT,
        valor_total DECIMAL(10, 2),
        bitmap_idx_produtos VARBINARY(1024),
        bitmap_idx_clientes VARBINARY(1024)
    );
  `

  const query3 =
    'CREATE INDEX IF NOT EXISTS idx_bitmap_produtos ON vendas_com_indices(bitmap_idx_produtos);'
  const query4 =
    'CREATE INDEX IF NOT EXISTS bitmap_idx_clientes ON vendas_com_indices(bitmap_idx_clientes);'

  conn.query(query1, (err) => {
    if (err) {
      throw err
    }
    console.log('Tabela "vendas_com_indices" criada com sucesso!')
  })

  conn.query(query2, (err) => {
    if (err) {
      throw err
    }
    console.log('Tabela "vendas_sem_indices" criada com sucesso!')
  })

  conn.query(query3, (err) => {
    if (err) {
      throw err
    }
    console.log('Índice "idx_bitmap_produtos" criado com sucesso!')
  })

  conn.query(query4, (err) => {
    if (err) {
      throw err
    }
    console.log('Índice "bitmap_idx_clientes" criado com sucesso!')
  })
}

const connection = conn()

createTables(connection)

connection.end((err: any) => {
  if (err) {
    throw err
  }
  console.log('Conexão encerrada com sucesso!')
})
 */
