#uvicorn main:app --reload
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
import datetime

time_now = datetime.datetime.now()
time_formatted = time_now.strftime("%d-%m-%Y %H:%M:%S")
print("Current Time :", time_formatted)

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
    
