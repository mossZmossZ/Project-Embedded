#uvicorn main:app --reload
from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from sqlite3 import Error
import sqlite3
import datetime


time_now = datetime.datetime.now()
time_formatted = time_now.strftime("%d-%m-%Y %H:%M:%S")
print("Current Time :", time_formatted)

lastRFID = None
# Function to create a connection to the SQLite database
def create_connection(db_file):
    conn = None
    try:
        conn = sqlite3.connect(db_file)
    except Error as e:
        print(e)
    return conn



app = FastAPI()

origins = [
    "*"
]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)




@app.get("/api/")
async def root():
    return {"message": "Hello World"}

@app.get("/api/getTime")
async def getTime():
    time_formatted = time_now.strftime("%d-%m-%Y %H:%M:%S")
    return time_formatted

class RFID(BaseModel):
    rfid_id: str

@app.post('/api/SentRFID')
async def read_rfid(request: RFID):
    data_RFID = {'rfid': request.rfid_id}
    rfid_id = data_RFID['rfid'] 
    conn = create_connection("Embedded.db")
    cursor = conn.cursor()
    print("RFID ID:", rfid_id)
    try:
        # Check if rfid_no exists in Students table
        cursor.execute("SELECT student_name FROM Students WHERE rfid_tags = ?", (rfid_id,))
        student_result = cursor.fetchone()
        if student_result:
            student_names = student_result[0]
            return student_names,"Student"

        # Check if rfid_no exists in Items table
        cursor.execute("SELECT item_name FROM Items WHERE rfid_tags = ?", (rfid_id,))
        item_result = cursor.fetchone()
        
        if item_result:
            name_items = item_result[0]
            return name_items,"Items"
        else:
            global lastRFID
            lastRFID = rfid_id 
            return "NotFound","NotFound"
    except Error as e:
        raise HTTPException(status_code=500, detail=f"An error occurred: {e}")
    finally:
        cursor.close()


@app.post('/api/CheckItems')
async def read_rfid(request: RFID):
    data_RFID = {'rfid': request.rfid_id}
    rfid_id = data_RFID['rfid'] 
    print(rfid_id)
    conn = create_connection("Embedded.db")
    cursor = conn.cursor()
    try:
        cursor.execute("""SELECT available
    FROM Items
    WHERE rfid_tags = ? """,(rfid_id,))
        rows = cursor.fetchall()
        rows_Result = rows[0]
        rows_finalResult = rows_Result[0]
        if rows_finalResult == 0:
            return "unavailable"
        elif rows_finalResult == 1:
            return "Available"
        else:
            return "Unknown"
        
        
        
    except Error as e:
        raise HTTPException(status_code=500, detail=f"An error occurred: {e}")
    finally:
        cursor.close()
    
@app.get("/api/GetRFID")
async def get_rfid():
    global lastRFID
    if lastRFID is None:
        raise HTTPException(status_code=404, detail="No RFID data has been posted yet")
    return int(lastRFID)

    
@app.get("/api/borrow")
async def get_borrowed_items():
    # Create a connection to the SQLite database
    conn = create_connection("Embedded.db")
    cursor = conn.cursor()
    try:
        cursor.execute("""SELECT items.item_name, students.student_name, borrow.borrowed_date, borrow.return_date
    FROM borrow
    JOIN items ON borrow.item_id = items.item_id
    JOIN students ON borrow.student_id = students.student_id
""")
        rows = cursor.fetchall()
        return rows
    except Error as e:
        raise HTTPException(status_code=500, detail=f"An error occurred: {e}")
    finally:
        cursor.close()

@app.get("/api/itemdata")
async def get_borrowed_items():
    # Create a connection to the SQLite database
    conn = create_connection("Embedded.db")
    cursor = conn.cursor()
    try:
        cursor.execute("""SELECT item_name,available
    FROM Items
""")
        rows = cursor.fetchall()
        return rows
    except Error as e:
        raise HTTPException(status_code=500, detail=f"An error occurred: {e}")
    finally:
        cursor.close()

class ItemData(BaseModel):
    rfid: int
    item: str

@app.post("/api/register")
async def register_item(item_data: ItemData):
    conn = create_connection("Embedded.db")
    cursor = conn.cursor()
    try:
        cursor.execute("INSERT INTO Items (item_name, rfid_tags, available) VALUES (?, ?, 1)",(item_data.item, item_data.rfid))
        
        conn.commit()
        return {"message": "Item registered successfully"}
    except Error as e:
        raise HTTPException(status_code=500, detail=f"An error occurred: {e}")
    finally:
        cursor.close()

class ItemData2(BaseModel):
    studentNo: int
    studentName: str
    rfidNo: int        

@app.post("/api/register2")
async def register_item2(item_data2: ItemData2):
    conn = create_connection("Embedded.db")
    cursor = conn.cursor()
    try:
        cursor.execute("INSERT INTO Students (student_id, student_name, rfid_tags) VALUES (?, ?, ?)",(item_data2.studentNo, item_data2.studentName,item_data2.rfidNo))
        
        conn.commit()
        #print(item_data2.studentNo, item_data2.studentName,item_data2.rfidNo)
        return {"message": "Item registered successfully"}
    except Error as e:
        raise HTTPException(status_code=500, detail=f"An error occurred: {e}")
    finally:
        cursor.close()

@app.get("/api/ava_data")
async def ava_data():
    conn = create_connection("Embedded.db")
    cursor = conn.cursor()
    try:
        cursor.execute("SELECT COUNT(item_id) FROM Items WHERE available = 1")
        available_count = cursor.fetchone()[0]

        cursor.execute("SELECT COUNT(item_id) FROM Items WHERE available = 0")
        unavailable_count = cursor.fetchone()[0]

        conn.commit()
        return  available_count,  unavailable_count
    except Error as e:
        raise HTTPException(status_code=500, detail=f"An error occurred: {e}")
    finally:
        cursor.close()


