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
    print("RFID ID:", rfid_id)  
    return {"Susscessful"}

@app.get("/api/GetTypeRFID")
async def TypeRfid(rfid:int):
    #rfid in database
    if (rfid ==0) :
        return {"yes"}
    else:
        #rfid not in database
        return {"Plese Register Member"}
    
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
async def register_item(item_data2: ItemData2):
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
