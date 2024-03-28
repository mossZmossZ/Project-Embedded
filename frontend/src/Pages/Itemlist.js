/* eslint no-use-before-define: 0 */
import React from "react";
import Sidebar from "../Components/Sidebar";
import {useState,useEffect} from "react";
import axios from "axios";
import { AgGridReact } from "ag-grid-react";
import "./App.css"

export default function ItemList(){
    const [colDefs, setColDefs] = useState([
        { field: "ID",flex:2 },
        { field: "Item",flex:2 },
        { field: "Available" },
      ]);

    const [rowData, setRowData] = useState([]);
    useEffect(() => {
      axios.get(`${process.env.REACT_APP_BACKEND_API}/api/itemdata`)
          .then(response => {
              const mappedData = response.data.map(item => ({
                ID:item[0],
                  Item: item[1],
                  Available: item[2] ===1 ? true :false  
              }));
              setRowData(mappedData);
              console.log(mappedData)
          })
          .catch(error => {
              console.error('Error fetching data: ', error);
          });
  }, []);
    return(
        <div id = 'dashboard'>
            <Sidebar/>
            <div  className = 'table'>
        <h3 className="m-4">ALL Item</h3>
        <div className="ag-theme-quartz" style={{height: 400,width:1000,fontSize:20 }}>
          <AgGridReact rowData={rowData} columnDefs={colDefs}/>
        </div>
    </div>
        </div>
    )};