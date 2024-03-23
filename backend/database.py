import sqlite3

conn = sqlite3.connect('Embedded.db')
c = conn.cursor()

c.execute(""" CREATE TABLE IF NOT EXISTS Students(
          student_id INTEGER,
          student_name TEXT,
          rfid_tags TEXT
            )""")

c.execute(""" CREATE TABLE IF NOT EXISTS Items(
          item_id INTEGER,
          item_name TEXT,
          rfid_tags TEXT,
          avaliable BOOLEAN
            )""")

c.execute(""" CREATE TABLE IF NOT EXISTS Borrow(
          borrow_id INTEGER,
          item_id INTEGER,
          student_id Integer,
          borrowed_date timestamp,
          return_date timestamp
            )""")

conn.commit()
conn.close()