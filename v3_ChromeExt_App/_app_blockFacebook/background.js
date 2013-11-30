
// TODO: add timer to chrome localStorage

console.log('app backgroud.js is running');

// Update timer data every UPDATE_INTERVAL seconds
var UPDATE_INTERVAL = 1;
setInterval(updateData, UPDATE_INTERVAL * 1000);
var resp;
var port;
var timerA = 'N';
var timerB = 'N';
var timerA0 = timerA;
var timerB0 = timerB;
var timerA1 = 'N';
var timerB1 = 'N';
var connectionId = -1;
var portOpen = false;
var selftimer = false;
var selftimer0 = false;
var extID = "pkglhoppgefihhdkggakeccaippnbbhd";

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
	// request.open("GET","http://itp.nyu.edu/~ps2409/sinatra/blockFacebook_server/update",true);
	request.open("GET","",true);
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

// listing to the incoming msg
chrome.runtime.onMessageExternal.addListener(
	function(request, sender, sendResponse){
		console.log("function added addListener");
		if(request.facebookOpen){
			// console.log("app turn on sand timer");
			selftimer0 = selftimer;
			selftimer = true;
		}
		else if(!request.facebookOpen){
			// console.log("app turn off sand timer");
			selftimer0 = selftimer;
			selftimer = false;
		}
		if(selftimer != selftimer0){
			// console.log("control_self_timer");
			control_self_timer();} 
	});

function control_self_timer(){
	if(selftimer){
		console.log("app turns on sand timer");
		timerA1 = 'Y';
	}
	else {
		console.log("app turns off sand timer");
		timerA1 = 'N';
	}
	var buffer = new ArrayBuffer(timerA1.length*2); // 2 bytes for N/Y
	var bufView = new Uint16Array(buffer);
	for (var i=0, strLen=timerA1.length; i<strLen; i++) {
	    bufView[i] = timerA1.charCodeAt(i);
	}
	chrome.serial.write(connectionId, buffer, function afterSent(){
		console.log("sent A - selftimer");
	});
}

function control_timer(){
	// the status of A -- comment on A
	// timerA = resp.charAt(resp.indexOf('A')+2);
	// if(timerA != timerA0){
	// 	timerA0 = timerA;
 //  		// send value to serial port
 //  		console.log("send value to serial port;  timerA: "+timerA);
 //  		var buffer = new ArrayBuffer(timerA.length*2); // 2 bytes for N/Y
 //  		var bufView = new Uint16Array(buffer);
	// 	for (var i=0, strLen=timerA.length; i<strLen; i++) {
	// 	    bufView[i] = timerA.charCodeAt(i);
	// 	}
 //  		chrome.serial.write(connectionId, buffer, function afterSent(){
 //  			console.log("sent A");
 //  		});
	// }

	// the status of B -- comment on B
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
			// send msg to extension:
			chrome.runtime.sendMessage(extID, {closeFacebook: true},
				function(response) {
					console.log("ext got the message");
				});

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