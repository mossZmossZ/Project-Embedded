import React from "react";
import Sidebar from "../Components/Sidebar";
import {useState,useEffect} from "react";
import axios from "axios";
import { AgGridReact } from "ag-grid-react";

export default function ItemList(){
    const [colDefs, setColDefs] = useState([
        { field: "Item" ,flex: 1},
        { field: "Available" ,flex: 1},
      ]);

    const [rowData, setRowData] = useState([]);
    useEffect(() => {
      axios.get('http://127.0.0.1:8000/api/itemdata')
          .then(response => {
              const mappedData = response.data.map(item => ({
                  Item: item[0],
                  Available: item[1] ===1 ? true :false  
              }));
              setRowData(mappedData);
          })
          .catch(error => {
              console.error('Error fetching data: ', error);
          });
  }, []);
    return(
        <div>
            <Sidebar/>
            <div  className = 'table'>
        <h3>Borrowed Item</h3>
        <div className="ag-theme-quartz" style={{height: 250 }}>
          <AgGridReact rowData={rowData} columnDefs={colDefs}/>
        </div>
    </div>
        </div>
    )};