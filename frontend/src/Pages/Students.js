/* eslint no-use-before-define: 0 */
import React from "react";
import Sidebar from "../Components/Sidebar";
import {useState,useEffect} from "react";
import axios from "axios";
import { AgGridReact } from "ag-grid-react";

export default function Students(){
    const [colDefs, setColDefs] = useState([
        { field: "Student_NO",flex :2 },
        { field: "Name",flex :2},
        
      ]);

    const [rowData, setRowData] = useState([]);
    useEffect(() => {
      axios.get(`http://45.141.27.250:8000/api/studentsdata`)
          .then(response => {
            
              const mappedData = response.data.map(item => ({
                Student_NO:item[0],
                  Name: item[1],
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
            <div  className = 'table '>
        <h3 className="m-4">Students</h3>
        <div className="ag-theme-quartz" style={{height: 400,width:1000,fontSize:20 }}>
          <AgGridReact rowData={rowData} columnDefs={colDefs}/>
        </div>
    </div>
        </div>
    )};