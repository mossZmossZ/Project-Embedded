import logo from './Comnarin.jpeg';
import './App.css';

function App() {
  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <p>
          Welcome Com narin to Frontend Developer
        </p>
        <a
          className="App-link"
          href="https://www.instagram.com/comnarin/"
          target="_blank"
          rel="noopener noreferrer"
        >
          Instagram
        </a>
      </header>
    </div>
  );
}

export default App;
