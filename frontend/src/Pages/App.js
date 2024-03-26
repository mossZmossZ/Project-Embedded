import { AgGridReact } from 'ag-grid-react'; // AG Grid Component
import "ag-grid-community/styles/ag-grid.css"; // Mandatory CSS required by the grid
import "ag-grid-community/styles/ag-theme-quartz.css"; // Optional Theme applied to the grid
import { useState,useEffect } from 'react';
import Sidebar from '../Components/Sidebar.js'
import "./App.css";
import axios from 'axios';
import DescriptionIcon from '@mui/icons-material/Description';
import { Chart } from "react-google-charts";





function App() {

  const [pieData, setPieData] = useState();
  const [avaData, setAvaData] = useState();
  const [unAvaData, setUnAvaData] = useState();
  const totalData = avaData+unAvaData;
  
  useEffect(() => {
    const interval = setInterval(()=>
      axios.get('http://127.0.0.1:8000/api/ava_data')
          .then(response => {
              const [availableCount, unavailableCount] = response.data;
              setPieData([
                  ['Availability','Count'],
                  [ 'Available', availableCount ],
                  ['Unavailable', unavailableCount ]]
              );
              setAvaData(availableCount)
              setUnAvaData(unavailableCount)
          })
          .catch(error => {
              console.error('Error fetching data: ', error);
          }),500);
          return()=>clearInterval(interval)
  }, []);

  
  const options = {
    chartArea: {'width': '100%', 'height': '80%'},
    colors:['#008170', '#bf1029'],
    backgroundColor: '#D3D3D3',
    'width': 350,
    'height': 400,
    legend: {'position': 'bottom',},
    fontSize: 18,
  };
    // Column Definitions: Defines the columns to be displayed.
     const [colDefs, setColDefs] = useState([
      { field: "Item" ,flex: 1},
      { field: "Student" ,flex: 1},
      { field: "Borrowed_date" ,flex: 0.8},
      { field: "Return_date" ,flex: 0.8}
    ]);


    const [rowData, setRowData] = useState([]);
    useEffect(() => {
      const interval = setInterval(()=>
      axios.get('http://127.0.0.1:8000/api/returnToday')
          .then(response => {
              const mappedData = response.data.map(item => ({
                  Item: item[0],
                  Student: item[1],  
                  Borrowed_date: item[2],
                  Return_date: item[3]
              }));
              setRowData(mappedData);
          })
          .catch(error => {
              console.error('Error fetching data: ', error);
          }),500);
          return()=>clearInterval(interval)
  }, []);

 
  return (
  <div >
    <div id = 'dashboard'>
      <Sidebar/>

      <div className = 'body'>
      <div className="row">
        <div class="col-sm-4">
            <div class="card custom3">
                <div class="card-body">
                    <h2 class="card-title">Total Items :</h2>
                    <h1 class = 'd-flex justify-content-center'>{totalData}</h1>
                </div>
            </div>
        </div>
        <div class="col-sm-4 ">
            <div class="card custom1 text-white">
                <div class="card-body">
                    <h2 class="card-title ">Available Items:</h2>
                    <h1 class = 'd-flex justify-content-center'>{avaData}</h1>
                </div>
            </div>
        </div>
        <div class="col-sm-4 ">
            <div class="card text-white custom2 ;">
                <div class="card-body">
                    <h2 class="card-title">Unavailable Items:</h2>
                    <h1 class = 'd-flex justify-content-center'>{unAvaData}</h1>
                </div>
            </div>
        </div>
        </div>
        <div className="row">
          <div className="col-sm-3 me-4 mt-4">
          
            <Chart chartType="PieChart" data={pieData} options={options}/>  
          </div>  
            <div class="col-sm-8 ps-5 ms-5">
              <div className='head'class = "ps-5">
                <h3>Return today Item</h3>
              </div>
              <div className="ag-theme-quartz shadow-sm" style={{height: 360 }} >
                <AgGridReact rowData={rowData} columnDefs={colDefs} />
              </div>
            </div>
        </div>
      </div> 
    </div>
  </div>


  );
}

export default App;
