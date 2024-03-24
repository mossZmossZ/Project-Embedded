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
          });
  }, []);

  
  const options = {
    title: "Equipment Availability",
    colors:['#008170', '#bf1029'],
    backgroundColor: '#D3D3D3'
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
      axios.get('http://127.0.0.1:8000/api/borrow')
          .then(response => {
              const mappedData = response.data.map(item => ({
                  Item: item[0],
                  Student: item[1],  // Assuming 'Status' is a placeholder value
                  Borrowed_date: item[2],
                  Return_date: item[3]
              }));
              setRowData(mappedData);
          })
          .catch(error => {
              console.error('Error fetching data: ', error);
          });
  }, []);

 
  return (
  <div >
    <div id = 'dashboard'>
      <Sidebar/>
      <h2>{totalData}</h2>
      <h2>{avaData}</h2>
      <h2>{unAvaData}</h2>
      <Chart
        chartType="PieChart"
        data={pieData}
        options={options}
        width={"100%"}
        height={"400px"}
    />
    <div  className = 'table'>
      
      <div className='head'>
        <h3>Borrowed Item</h3>
        <DescriptionIcon id = 'task' />
      </div>
        <div className="ag-theme-quartz" style={{height: 250 }}>
          <AgGridReact rowData={rowData} columnDefs={colDefs}/>
        </div>
    </div>
    </div>
  </div>


  );
}

export default App;
