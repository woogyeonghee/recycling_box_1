//modules
//const io = require('socket.io')(http);
const express  = require('express');
const app      = express();
const http     = require('http').Server(app);
const SerialPort = require('serialport');

//push msg
const FCM = require('fcm-node');
const serverKey = 'AAAA3biyxVc:APA91bGX1a_APtyAtK1DoCDiZCmh6jSeFqM9lFfCdqqGLo4lkxKV889D7fF4grgJwnBtVY1lChzb40_WJLdicnEvADbVSDyIkxzwIM9_1ocw91Q75AybhOfiL8NBQ2lNoSMGJ4TJdqT0';
const token = 'eX2oX7LtRTSyH9EQ1UDrsJ:APA91bFP7IVB8ek9sECMNakb_2v2YiK-ZnVXdUHG3XBKI69oiRHE9fcb_tjpJWnSa6V-whsF0yZTPGAMcQbNuo4pjDc502r80hBM4r94FhKMnfzMiCSEc21PXl8KCNvf5mPkmY71ULEV';
const fcm = new FCM(serverKey);

const can_message = {
		to: token,
		notification: {
			title: 'Can Box Full',   //
			body: 'Can Box FULL'  
		},
		};
const bottle_message = { 
		to: token,
		notification: {
			title: 'bottle Box Full',   
			body: 'ºbottle Box Full' 
		},
		};

//serial
var can_Bool = false;
var bottle_Bool = false;

const sp = new SerialPort('/dev/sendUsbPort', {
  baudRate: 9600,
  parser: new SerialPort.parsers.Readline('\r\n')
});

sp.on('open', () => console.log('serialPort open'));
sp.on('data', (data)=>
{ 
	if(data == "C" && !can_Bool){	
		//io.emit('adc', adcValue);
		fcm.send(can_message, function(err, response){
			if (err) {
        			console.log("Something has gone wrong!");
			} else {
				console.log('full can box push msg...');
        			console.log("Successfully sent with response: ", 					response);
				can_Bool = true;
			}
		});
	}else if(data == "B" && !bottle_Bool){
		fcm.send(bottle_message, function(err, response){
			if (err) {
        			console.log("Something has gone wrong!");
			} else {
				console.log('full bottle box push msg...');
        			console.log("Successfully sent with response: ", 					response);
				bottle_Bool = true;
			}
		});
	}else if(data == "N" && can_Bool){
		can_Bool = false;
		console.log('empty box push msg...');
	}
	
});
//connect client
http.listen(3000, function() {
    console.log('listening on *:' + 3000);
});
