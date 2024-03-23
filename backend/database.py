import sqlite3
from datetime import datetime, timedelta

conn = sqlite3.connect('./backend/Embedded.db')
c = conn.cursor()

c.execute("""CREATE TABLE IF NOT EXISTS Students(
          student_id INTEGER PRIMARY KEY,
          student_name TEXT,
          rfid_tags TEXT
            )""")

c.execute("""CREATE TABLE IF NOT EXISTS Items(
          item_id INTEGER PRIMARY KEY,
          item_name TEXT,
          rfid_tags TEXT,
          available BOOLEAN
            )""")

c.execute("""CREATE TABLE IF NOT EXISTS Borrow(
          borrow_id INTEGER PRIMARY KEY,
          item_id INTEGER,
          student_id INTEGER,
          borrowed_date TIMESTAMP,
          return_date TIMESTAMP
            )""")
c.execute("INSERT INTO Students (student_id, student_name, rfid_tags) VALUES (6401012630086, 'Narin Sirinapuk', '0005280984')")
c.execute("INSERT INTO Items (item_id,item_name, rfid_tags, available) VALUES (1,'Microscope', 'RFID_101', 1)")
borrowed_date = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
return_date = (datetime.now() + timedelta(days=7)).strftime('%Y-%m-%d %H:%M:%S')
c.execute("INSERT INTO Borrow (item_id, student_id, borrowed_date, return_date) VALUES (1, 6401012630086, ?, ?)", (borrowed_date, return_date))


conn.commit()
conn.close()