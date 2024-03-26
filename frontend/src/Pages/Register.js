import {useState} from 'react';
import Sidebar from '../Components/Sidebar.js';
import "./Register.css";
import {Button} from 'react-bootstrap';
import 'bootstrap/dist/css/bootstrap.min.css';
import axios from 'axios';
import Swal from 'sweetalert2'
 
export default function  Register ()  {
	const  [rfid, setRfid] =  useState('');
    const  [item, setItem] =  useState('');
    const  [studentNo, setStudentNo] =  useState('');
    const  [studentName, setStudentName] =  useState('');
    const  [rfidNo, setRfidNo] =  useState('');


    const handleChangeRfid = async (event) => {
        event.preventDefault();
        try {
            const response = await axios.get('http://127.0.0.1:8000/api/GetRFID');
            const data = response.data;
            setRfid(data);
        } catch (error) {
            console.error('Error fetching data:', error);
        }
    };
    const handleClearRfid = (event) => {
        event.preventDefault();
        setRfid('');
    };
    const handleClearRfidNo = (event) => {
        event.preventDefault();
        setRfidNo('');
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
    const  handleChangeRfidNo = async (event) => {
		event.preventDefault();
        try {
            const response = await axios.get('http://127.0.0.1:8000/api/GetRFID');
            const data = response.data;
            setRfidNo(data);
        } catch (error) {
            console.error('Error fetching data:', error);
        }
	};

    const handleSubmit1 = (event) => {
        event.preventDefault();
        if (rfid === '' || item === '') {
            Swal.fire({
                title: "Fill out?",
                text: "please fill out all content",
                icon: "question"
              });
        } 
        else {

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
        Swal.fire({
            title: "Success",
            text: "Item has been registered",
            icon: "success"
          });
    }}
    const handleSubmit2 = (event) => {
        event.preventDefault();
        if (studentName === '' || studentNo === '' || rfidNo ==='') {
            Swal.fire({
                title: "Fill out?",
                text: "please fill out all content",
                icon: "question"
              });
        } 
        else {

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
        Swal.fire({
            title: "Success",
            text: "Student has been registered",
            icon: "success"
          });

    }}

return  (
    <div id = "dashboard">
        <Sidebar/>
        <div>
        <form id = "registerform" >
            <div className='registerform'>
                <h1>Register Item</h1>
                <label>RFID No:
                    <>   </>
                    <input  readOnly type="Integer"  value={rfid} onChange={handleChangeRfid} />
                    <>     </>
                    <button  onClick={handleChangeRfid} >Read</button>
                    <>     </>
                    <button  onClick={handleClearRfid} >Clear</button>
                </label> 
                <label>Item Name:
                    <>   </>
                    <input  type="text"  value={item} onChange={handleChangeItem} />
                </label>

               <Button  onClick={handleSubmit1} className='submit_button'>Submit</Button>
            </div>
           
        </form>
        <div className='card'>
            <div className='form-group'>
                <h1 className='form-title'>Register Item</h1>
                <div className='input-group mb-5 w-100'>
                    <label>RFID No:</label>
                    <input type="Integer" class="form-control pe-5" value={rfid} placeholder="Readonly input here…" onChange={handleChangeRfid} readonly /> 
                    <Button onClick={handleChangeRfid}>Read</Button>
                    <Button onClick={handleClearRfid}>Clear</Button>
                </div>
                <div className='card-text'>
                    <label>Item Name:</label>
                    <input type="text" value={item} onChange={handleChangeItem} />
                </div>
                <Button onClick={handleSubmit1} className='submit_button'>Submit</Button>
            </div>
        </div>
        <form id = "registerform" onSubmit={handleSubmit2}>
            <div className='registerform'>
                <h1>Register Student</h1>
                <label>RFID No:
                    <>   </>
                    <input  type="Integer"  value={rfidNo} onChange={handleChangeRfidNo} />
                    <>     </>
                    <button  onClick={handleChangeRfidNo} >Read</button>
                    <>     </>
                    <button  onClick={handleClearRfidNo} >Clear</button>
                </label> 
                <label>Student No:
                    <>   </>
                    <input  type="Integer"  value={studentNo} onChange={handleChangeStudentNo} />
                </label> 
                <label>Student Name:
                    <>   </>
                    <input  type="text"  value={studentName} onChange={handleChangeStudentName} />
                </label>
                
               <Button type = "submit" className='submit_button'>Submit</Button>
            </div>
           
        </form>
        </div>
    </div>

)};