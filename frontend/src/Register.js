import {useState} from 'react';
import Sidebar from './Components/Sidebar.js'
import "./Register.css";
 
export default function  Register ()  {
	const  [rfid, setRfid] =  useState('');
    const  [item, setItem] =  useState('');

	const  handleChangeRfid = (event) => {
		setRfid(event.target.value);
	};
    const  handleChangeItem = (event) => {
		setItem(event.target.value);
	};

return  (
    <div id = "dashboard">
        <Sidebar/>
        <form id = "registerform">
            <label>RFID No:
                <input  type="number"  value={rfid} onChange={handleChangeRfid} />
            </label> 
            <label>Item name:
                <input  type="text"  value={item} onChange={handleChangeItem} />
            </label>
        </form>
    </div>

)};