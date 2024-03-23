import {useState} from 'react';
import Sidebar from '../Components/Sidebar.js';
import "./Register.css";
import {Button} from 'react-bootstrap';
import 'bootstrap/dist/css/bootstrap.min.css';
 
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
            <div className='registerform'>
                <label>RFID No:
                    <input  type="number"  value={rfid} onChange={handleChangeRfid} />
                </label> 
                <label>Item name:
                    <input  type="text"  value={item} onChange={handleChangeItem} />
                </label>
                <Button variant="primary" className='submitbutton'>Primary</Button>{' '}
            </div>
           
        </form>
    </div>

)};