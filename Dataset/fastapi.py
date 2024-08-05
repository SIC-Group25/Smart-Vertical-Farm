import uvicorn
from fastapi import FastAPI, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
import joblib
import pandas as pd

app = FastAPI()

# Define CORS settings
origins = ["*"]
app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_methods=["*"],
    allow_headers=["*"],
)

# Load the trained model
model = joblib.load('temperature_forecast_model.pkl')

# Define a Pydantic model for input data
class IoTDataInput(BaseModel):
    humidity: float
    kelembabanTanah: float
    tdsValue: float
    temperature: float

# Define the forecasting route
@app.post("/forecast")
async def forecast_iot_data(data: IoTDataInput):
    try:
        # Convert input data to DataFrame
        input_data = pd.DataFrame([data.dict()])
        
        # Perform the t+1 forecasting
        t_plus_1 = model.predict(input_data)
        
        return {"status_code": 200, "message": "success", "body": {"forecast": t_plus_1[0]}}
    except Exception as e:
        raise HTTPException(status_code=500, detail="Error in forecasting")

if __name__ == "__main__":
    uvicorn.run(app, host="0.0.0.0", port=8000)