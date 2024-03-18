const express = require("express")
const router = express.Router()
const {HelloWorld} = require('../controllers/HelloWorldController')


router.get('/HelloWorld',HelloWorld);

module.exports = router