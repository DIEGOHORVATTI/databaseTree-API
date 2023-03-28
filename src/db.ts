import mysql, { Connection } from 'mysql2'

interface IDbConfig {
  port: number
  db: string
  user: string
  host: string
  password: string
}

export const dbConfig: IDbConfig = {
  port: 3306,
  db: 'IFC',
  host: '127.0.0.1',
  user: 'root',
  password: '20020000'
}

export const connectToDatabase = async (
  config: IDbConfig
): Promise<Connection> => {
  const connection = mysql.createConnection({
    host: config.host,
    port: config.port,
    user: config.user,
    password: config.password
  })
  console.log('Connected to database!')
  return connection
}

export const createDatabase = async (
  connection: Connection,
  dbName: string
): Promise<void> => {
  await connection.execute(`CREATE DATABASE IF NOT EXISTS ${dbName}`)
  console.log('Database created!')
}

export const disconnectFromDatabase = async (
  connection: Connection
): Promise<void> => {
  connection.end()
  console.log('Disconnected from database!')
}
