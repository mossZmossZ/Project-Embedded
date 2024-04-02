import {useState} from 'react';
import Sidebar from '../Components/Sidebar.js';
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
            const response = await axios.get(`${process.env.REACT_APP_BACKEND_API}/api/GetRFID`);
            const data = response.data; // Assuming the response is an object with a 'lastRFID' property
            setRfid(data.lastRFID);
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
    const handleChangeRfidNo = async (event) => {
        event.preventDefault();
        try {
            const response = await axios.get(`${process.env.REACT_APP_BACKEND_API}/api/GetRFID`);
            const data = response.data; // Assuming the response is an object with a 'lastRFID' property
            setRfidNo(data.lastRFID);
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

        axios.post(`${process.env.REACT_APP_BACKEND_API}/api/register`, formData)
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

        axios.post(`${process.env.REACT_APP_BACKEND_API}/api/register2`, formData2)
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
        <div class='w-50 '>
            <div class="card mt-4 rounded-4 custom-register "style={{marginLeft: 8 + 'em'}} >
                <div class="card-body">
                <h1 class="card-title">Register Item</h1>
                <form>
                    <div class="form-floating input-group mt-4 mb-3">
                        
                        <input type="text" class="form-control"  value={rfid}  placeholder="RFID NO" onChange={handleChangeRfid} disabled readOnly/>
                        <label >RFID NO :</label> 
                        <button className='btn btn-outline-success ' onClick={handleChangeRfid} >Read</button>
                        <button className='btn btn-outline-danger ' onClick={handleClearRfid} >Clear</button>         
                    </div>
                    <div class="form-floating mt-4 mb-3">
                        <input  type="text"  class="form-control" placeholder="Item name" value={item} onChange={handleChangeItem} />
                        <label >Item Name: </label> 
                    </div>
                    <button className='btn btn-primary mb-2 ' onClick={handleSubmit1} >Submit</button>
                </form>
                </div>
            </div>
            <div class="card mt-4 rounded-4 custom-register"style={{marginLeft: 8 + 'em'}} >
                <div class="card-body ">
                <h1 class="card-title">Register Student</h1>
                <form>
                    <div class="form-floating input-group mt-4 mb-3">
                        <input type="text" class="form-control "  value={rfidNo}  placeholder="RFID NO" onChange={handleChangeRfidNo} disabled readOnly/>
                        <label >RFID NO :</label> 
                        <button className='btn btn-outline-success' onClick={handleChangeRfidNo} >Read</button>
                        <button className='btn btn-outline-danger' onClick={handleClearRfidNo} >Clear</button>         
                    </div>
                    <div class="form-floating mt-4 mb-3">
                        <input  type="text"  class="form-control" placeholder="Student name"  value={studentName} onChange={handleChangeStudentName} />
                        <label >Student Name : </label> 
                    </div>
                    <div class="form-floating mt-4 mb-3">
                        <input  type="text"  class="form-control" placeholder="Student no" value={studentNo} onChange={handleChangeStudentNo} />
                        <label >Student NO : </label> 
                    </div>
                    <button class='btn btn-primary' onClick={handleSubmit2}>Submit</button>
                </form>
                </div>
            </div>
        </div>
    </div>

)};