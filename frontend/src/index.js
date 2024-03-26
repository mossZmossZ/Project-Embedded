import React from 'react';
import ReactDOM from 'react-dom/client';
import App from './Pages/App';
import Borrowlist from './Pages/BorrowList';
import { BrowserRouter, Routes,Route} from 'react-router-dom';
import ItemList from './Pages/Itemlist';
import Students from './Pages/Students';
import Register from './Pages/Register';

const root = ReactDOM.createRoot(document.getElementById('root'));
root.render(
  <BrowserRouter>
      <Routes>
        <Route path="/" element={<App />} />
        <Route path="/register" element={<Register />} />
        <Route path="/itemlist" element={<ItemList />} />
        <Route path="/borrowList" element={<Borrowlist />} />
        <Route path="/Students" element={<Students />} />
      </Routes>
  </BrowserRouter>
);

// If you want to start measuring performance in your app, pass a function
// to log results (for example: reportWebVitals(console.log))
// or send to an analytics endpoint. Learn more: https://bit.ly/CRA-vitals

