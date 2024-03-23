import React from 'react';
import ReactDOM from 'react-dom/client';
import './index.css';
import App from './App';
import Slidebar from './Components/Sidebar';
import { BrowserRouter, Routes,Route} from 'react-router-dom';
import Register from './Register';

const root = ReactDOM.createRoot(document.getElementById('root'));
root.render(
  <BrowserRouter>
    <Routes>
      <Route path="/" element={<App />} />
      <Route path="/register" element={<Register />} />
    </Routes>
  </BrowserRouter>
);

// If you want to start measuring performance in your app, pass a function
// to log results (for example: reportWebVitals(console.log))
// or send to an analytics endpoint. Learn more: https://bit.ly/CRA-vitals

