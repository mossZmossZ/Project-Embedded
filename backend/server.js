const express = require('express')
const app = express()
const cors = require("cors")
const morgan = require("morgan")
const HelloWorldRoute = require('./routes/HelloWorld')

//middleware 
app.use(express.json())
app.use(cors())
app.use(morgan("dev"))
app.use((req,res,next)=>{
    res.setHeader('Access-Control-Allow-Origin','*');
    res.setHeader(
        'Access-Control-Allow-Headers',
        'Origin, X-Requestes-With, Content-Type, Accept, Authorization'
    );
    res.setHeader('Access-Control-Allow-Methods','Get, POST , PATCH, DELETE');

    next();
})

//route
app.use('/api',HelloWorldRoute);


const PORT = process.env.PORT || 8080
app.listen(PORT, () => {
    console.log(`Server is running on port : ${PORT}`)
})
//app.listen(8080, '192.168.0.73')
