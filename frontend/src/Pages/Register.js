import {useState} from 'react';
import Sidebar from '../Components/Sidebar.js';
import "./Register.css";
import {Button} from 'react-bootstrap';
import 'bootstrap/dist/css/bootstrap.min.css';
import axios from 'axios';
 
export default function  Register ()  {
	const  [rfid, setRfid] =  useState('');
    const  [item, setItem] =  useState('');
    const  [studentNo, setStudentNo] =  useState('');
    const  [studentName, setStudentName] =  useState('');
    const  [rfidNo, setRfidNo] =  useState('');


    const handleChangeRfid = async () => {
        try {
            const response = await fetch('http://127.0.0.1:8000/api/GetRFID', {
                method: 'GET',
                headers: {
                    'Content-Type': 'application/json'
                    // Add any other headers if needed
                }
            });
            const data = await response.json();
            // Do something with the data, such as updating state
            console.log(data);
        } catch (error) {
            console.error('Error fetching data:', error);
        }
    };

    const  handleChangeItem = (event) => {
		setItem(event.target.value);
	};
    const  handleChangeStudentName = (event) => {
		setStudentName(event.target.value);
	};
    const  handleChangeStudentNo = (event) => {
		setStudentNo(event.target.value);
	};
    const  handleChangeRfidNo = (event) => {
		setRfidNo(event.target.value);
	};

    const handleSubmit1 = (event) => {
        event.preventDefault();

        const formData = {
            item: item,
            rfid: rfid
        };

        axios.post('http://127.0.0.1:8000/api/register', formData)
            .then(response => {
                console.log('Item registered:', response.data);
            })
            .catch(error => {
                console.error('Error registering item:', error);
            });

        // Clear form fields after submission
        setRfid('');
        setItem('');
    }
    const handleSubmit2 = (event) => {
        event.preventDefault();

        const formData2 = {
            studentName: studentName,
            studentNo: studentNo,
            rfidNo: rfidNo
        };

        axios.post('http://127.0.0.1:8000/api/register2', formData2)
            .then(response => {
                console.log('student registered:', response.data);
            })
            .catch(error => {
                console.error('Error registering student:', error);
            });

        // Clear form fields after submission
        setStudentName('');
        setStudentNo('');
        setRfidNo('');
    }

return  (
    <div id = "dashboard">
        <Sidebar/>
        <div>
        <form id = "registerform" onSubmit={handleSubmit1} >
            <div className='registerform'>
                <h4>Register Item</h4>
                <label>RFID No:
                    <>   </>
                    <input  readOnly type="Integer"  value={rfid} onChange={handleChangeRfid} />
                    <button  onClick={handleChangeRfid} >Read</button>
                </label> 
                <label>Item Name:
                    <>   </>
                    <input  type="text"  value={item} onChange={handleChangeItem} />
                </label>

               <Button  type = "submit" className='submit_button'>Submit</Button>
            </div>
           
        </form>
        <form id = "registerform" onSubmit={handleSubmit2}>
            <div className='registerform'>
                <h4>Register Student</h4>
                <label>Student No:
                    <>   </>
                    <input  type="Integer"  value={studentNo} onChange={handleChangeStudentNo} />
                </label> 
                <label>Student Name:
                    <>   </>
                    <input  type="text"  value={studentName} onChange={handleChangeStudentName} />
                </label>
                <label>RFID No:
                    <>   </>
                    <input  type="Integer"  value={rfidNo} onChange={handleChangeRfidNo} />
                </label> 
               <Button type = "submit" className='submit_button'>Submit</Button>
            </div>
           
        </form>
        </div>
    </div>

)};