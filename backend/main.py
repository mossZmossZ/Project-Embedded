#uvicorn main:app --reload
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel

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


class RFID(BaseModel):
    rfid_id: str

@app.post('/api/SentRFID')
async def read_rfid(request: RFID):
    data_RFID = {'rfid': request.rfid_id}
    rfid_id = data_RFID['rfid']  
    print("RFID ID:", rfid_id)  
    return {"Susscessful to sent Data"}

@app.get("/api/GetTypeRFID")
async def TypeRfid(rfid:int):
    #rfid in database
    if (rfid ==0) :
        return {"yes"}
    else:
        #rfid not in database
        return {"Plese Register Member"}
    
