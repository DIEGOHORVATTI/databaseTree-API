import express from 'express'
import {
  disconnectFromDatabase,
  connectToDatabase,
  createDatabase,
  dbConfig
} from './db'

const app = express()

app.get('/', async (req, res) => {
  try {
    const connection = await connectToDatabase(dbConfig)
    await createDatabase(connection, dbConfig.db)
    await disconnectFromDatabase(connection)
    res.status(200).send('Database created successfully!')
  } catch (error) {
    console.error('Error creating database: ', error)
    res.status(500).send('Error creating database')
  }
})

console.clear()
console.log('Server is running on port 3000')
app.listen(3000)
