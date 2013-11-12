
// TODO: add timer to chrome localStorage

console.log('backgroud.js is running');

// Update timer data every UPDATE_INTERVAL seconds
var UPDATE_INTERVAL = 1;
setInterval(updateData, UPDATE_INTERVAL * 1000);
var resp;
var port;
var timerA = 'N';
var timerB = 'N';
var timerA0 = timerA;
var timerB0 = timerB;
var connectionId = -1;
var portOpen = false;

// check the ports
chrome.serial.getPorts(function(ports) {
	port = ports[5];
	console.log(port);

	// open the prots
	chrome.serial.open(port, function onOpen(openInfo) {
		connectionId = openInfo.connectionId;
			console.log("connectionId"+connectionId);
	  		if (connectionId == -1) {
	    		console.log('Could not open');
	    		return;
	  		}
		console.log("serial port open ");
		portOpen = true;
	});
});


// Update the data
function updateData() {
	// console.log("interval");
	var request = new XMLHttpRequest();
	request.open("GET","http://itp.nyu.edu/~ps2409/sinatra/blockFacebook_server/update",true);
	request.onreadystatechange = function() {
		if (request.readyState == 4) {
			resp = request.responseText;
			//console.log(resp);
			if (portOpen){
			control_timer();
		}
		}
	}
	request.send();

	// read the data from servo
	if (connectionId != -1) {
		chrome.serial.read(connectionId, 1, onRead);
	}
}

function control_timer(){
	// the status of A
	timerA = resp.charAt(resp.indexOf('A')+2);
	if(timerA != timerA0){
		timerA0 = timerA;
  		// send value to serial port
  		console.log("send value to serial port;  timerA: "+timerA);
  		var buffer = new ArrayBuffer(timerA.length*2); // 2 bytes for N/Y
  		var bufView = new Uint16Array(buffer);
		for (var i=0, strLen=timerA.length; i<strLen; i++) {
		    bufView[i] = timerA.charCodeAt(i);
		}
  		chrome.serial.write(connectionId, buffer, function afterSent(){
  			console.log("sent A");
  		});
	}

	// the status of B
	timerB = resp.charAt(resp.indexOf('B')+2);
	if(timerB != timerB0){
		timerB0 = timerB;
  		// send value to serial port
  		console.log("send value to serial port;  timerB: "+timerB);
  		var bufferB = new ArrayBuffer(timerB.length*2); // 2 bytes for N/Y
  		var bufViewB = new Uint16Array(bufferB);
		for (var i=0, strLen=timerB.length; i<strLen; i++) {
		    bufViewB[i] = timerB.charCodeAt(i);
		}
  		chrome.serial.write(connectionId, bufferB, function afterSent(){
  			console.log("sent B");
  		});
	}	
}

function onRead(readInfo){
	var dataFromTimer = String.fromCharCode.apply(null, new Uint8Array(readInfo.data));
	if(dataFromTimer.length !=0){
		console.log("dataFromTimer:"+dataFromTimer+"+!");
		if(dataFromTimer == "C"){
			// send httpGetRequest to server
			httpGet("http://itp.nyu.edu/~ps2409/sinatra/blockFacebook_server/AC");// change to BC
			console.log("AC");
		}
		if(dataFromTimer == "c"){
			// send httpGetRequest to server
			httpGet("http://itp.nyu.edu/~ps2409/sinatra/blockFacebook_server/BC");// change to BC
			console.log("BC");
		}
	}
}

function httpGet(theUrl){
	var xmlHttp = null;
	xmlHttp = new XMLHttpRequest();
	xmlHttp.open( "GET", theUrl, true );
	xmlHttp.send();
} 