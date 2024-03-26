import React from "react";
import Sidebar from "../Components/Sidebar";
import {useState,useEffect} from "react";
import axios from "axios";
import { AgGridReact } from "ag-grid-react";

export default function BorrowList(){
    // Column Definitions: Defines the columns to be displayed.
    const [colDefs, setColDefs] = useState([
      { field: "Item" ,flex: 1},
      { field: "Student" ,flex: 1},
      { field: "Borrowed_date" ,flex: 0.8},
      { field: "Return_date" ,flex: 0.8}
    ]);


    const [rowData, setRowData] = useState([]);
    useEffect(() => {
      axios.get(`${process.env.REACT_APP_BACKEND_API}/api/borrow`)
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
          });
  }, []);
    return(
        <div id = 'dashboard'>
            <Sidebar/>
            <div  className = 'table m-5'>
              <h3 className="m-4">Borrowed Item</h3>
              <div className="ag-theme-quartz" style={{height: 400,width:1000,fontSize:20 }}>
                <AgGridReact rowData={rowData} columnDefs={colDefs}/>
              </div>
            </div>
        </div>
    )};