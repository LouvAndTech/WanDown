//Recieve http post from the sensor 
const express = require('express')
const app = express()
const port = 3000
app.use(express.json());

//Send a notification to the user
const IFTTT = require('node-ifttt-maker');
const ifttt = new IFTTT('<IFTTT WEBHOOK KEY>');
const event = 'wan_loss';
//params for the notification
let params = {
    'value1': 'empty'
}

let last_state = 1

//Timer functions
//Timeout occured
const failFun = () => {
    console.log('timed out')
    //Send a notification to the user elouan
    params = {
        'value1': 'Internet is Down'
    }
    ifttt
        .request({event,params})
            .then((response) => {})
                .catch((err) => {});
    console.log('notification sent')
    //and update the state 
    last_state = 0
}
//Init the timer
const timoutTime = 30 * 1000;
let myTimeout = setTimeout(failFun, timoutTime);
//Reset the timer
function restartTimer() {
    //Remove the timer
    clearTimeout(myTimeout);
    //Timer
    myTimeout = setTimeout(failFun, timoutTime);
}


//On http post request
app.post('/', (req, res) => {
    //Log in the server console
    console.log('Hello World server side !')
    res.json(req.body);
    console.log(req.body);
    //check if the state changed
    if (last_state == 0){
        //Send a notification to the user elouan
        params = {
            'value1': 'Internet is back'
        }
        ifttt
            .request({event,params})
                .then((response) => {})
                    .catch((err) => {});
        //and update the state
        last_state = 1
    }

    //Restart the timer
    restartTimer();
})

//Listen for http post on port 3000
app.listen(port, () => {
  console.log(`App listening on port ${port}`)
})



