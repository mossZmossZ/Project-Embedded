import './App.css';
import { AgGridReact } from 'ag-grid-react'; // AG Grid Component
import "ag-grid-community/styles/ag-grid.css"; // Mandatory CSS required by the grid
import "ag-grid-community/styles/ag-theme-quartz.css"; // Optional Theme applied to the grid
import { useState } from 'react';

function App() {
    const [rowData, setRowData] = useState([
      { ID: "Tesla", Item: "Model Y", Status: true,Borrowed_by: "Nattavee", Return_date :"13/02" },
      { ID: "Tesla", Item: "Model Y", Status: false,Borrowed_by: "-", Return_date :"13/02" },
      { ID: "Tesla", Item: "Model Y", Status: false,Borrowed_by: "-", Return_date :"13/02" },
      { ID: "Tesla", Item: "Model Y", Status: true,Borrowed_by: "Nattavee", Return_date :"13/02" }
    ]);
    
    // Column Definitions: Defines the columns to be displayed.
    const [colDefs, setColDefs] = useState([
      { field: "ID" ,flex: 1},
      { field: "Item" ,flex: 1},
      { field: "Status",flex: 1 },
      { field: "Borrowed_by" ,flex: 2},
      { field: "Return_date" ,flex: 1}
    ]);
  return (
    <div  style={{padding:'10%'}}>
      Item Status
      <div className="ag-theme-quartz"   // applying the grid theme
    style={{ height: 250 }} // the grid will fill the size of the parent container 
    >
   <AgGridReact
       rowData={rowData}
       columnDefs={colDefs}
   />
</div>
    </div>

  );
}

export default App;
