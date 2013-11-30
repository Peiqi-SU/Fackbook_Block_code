
// TODO: add timer to chrome localStorage

console.log('extension backgroud.js is running');

// Update timer data every UPDATE_INTERVAL seconds
var UPDATE_INTERVAL = 1;
setInterval(updateData, UPDATE_INTERVAL * 1000);
var timeOnFacebook = 0;
var limitTime = 3000;
var tabID;
var resp;
var appID = "aoemllogbdhoioamkopdemdenamfcbip";

// Update the data
function updateData() {
	// Only count time if Chrome has focus
	chrome.windows.getLastFocused(function (window) {
		if (window === undefined) {
			console.log("facebook is close");
			httpGet("http://itp.nyu.edu/~ps2409/sinatra/blockFacebook_server/AN");
			return;
		}
		else if (window.focused) {
			// Only count time if system has not been idle for 30 seconds
			chrome.idle.queryState(30, function (state) {
				if (state === "active") {
					chrome.tabs.query({'active': true, 'lastFocusedWindow': true}, function (arrayOfTabs) {
						var activeTab = arrayOfTabs[0].url;
						tabID = arrayOfTabs[0].id;
			  			// console.log('current tab: ', activeTab);
			  			var temp = activeTab.match("www.facebook");
			  			if(temp != null ){
			  				console.log("facebook is open");
			  				timeOnFacebook += UPDATE_INTERVAL;
			  				console.log("sec: ", timeOnFacebook);

			  				// redirect to another page
			  				if(timeOnFacebook > limitTime){
			  					console.log("redirect");
			  					//chrome.tabs.update(null, {url: "http://www.this-page-intentionally-left-blank.org/"}); // redirect
			  					chrome.tabs.remove(tabID); //close
			  				}

			  				// send httpGetRequest to server
			  				httpGet("http://itp.nyu.edu/~ps2409/sinatra/blockFacebook_server/AY");// change to BY

			  				// send a message to the app
			  				chrome.runtime.sendMessage(appID, {facebookOpen: true},
								function(response) {
									console.log("app got the message");
							});

			  			}
			  			else {
			  				console.log("facebook is close");
			  				httpGet("http://itp.nyu.edu/~ps2409/sinatra/blockFacebook_server/AN");// change to BN

			  				// send a message to the app
			  				chrome.runtime.sendMessage(appID, {facebookOpen: false},
								function(response) {
									console.log("app got the message");
							});
			  			}
			  		});
}
});
}
});
// check if turn off the sand timer
checkTurnOffSandTimer();
}

function httpGet(theUrl){
	var xmlHttp = null;
	xmlHttp = new XMLHttpRequest();
	xmlHttp.open( "GET", theUrl, false );
	xmlHttp.send( null );
} 

// redirect to a new page if overtime
function checkTurnOffSandTimer(){
	var req = null;
	req = new XMLHttpRequest();
	req.open("GET", "http://itp.nyu.edu/~ps2409/sinatra/blockFacebook_server/update", true);
	req.onreadystatechange = function() {
		if (req.readyState == 4) {
			resp = req.responseText;
			console.log("resp------ "+resp);
			var redirectFb = resp.charAt(resp.indexOf('A')+2);
			if(redirectFb == 'C'){
				chrome.tabs.update(null, {url: "http://itp.nyu.edu/~ps2409/BlockFacebook/BlockFacebook.html"}); // redirect
			}
		}
	}
	req.send();
}

// listing to the incoming msg
chrome.runtime.onMessageExternal.addListener(
	function(request, sender, sendResponse){
		console.log("function added addListener");
		if(request.closeFacebook){
			console.log("app turn on sand timer");
			chrome.tabs.update(null, {url: "http://itp.nyu.edu/~ps2409/BlockFacebook/BlockFacebook.html"}); // redirect
		}
	});


