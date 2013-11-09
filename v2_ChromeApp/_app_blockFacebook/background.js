
// TODO: add timer to chrome localStorage

console.log('backgroud.js is running');

// Update timer data every UPDATE_INTERVAL seconds
var UPDATE_INTERVAL = 1;
setInterval(updateData, UPDATE_INTERVAL * 1000);
var timeOnFacebook = 0;
var limitTime = 300;
var tabID;

// Update the data
function updateData() {
	// Only count time if Chrome has focus
	chrome.windows.getLastFocused(function (window) {
		if (window === undefined) {
			console.log("facebook is close");
			httpGet("http://localhost:8080/?on_view=N");
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
			  			var temp = activeTab.match("facebook");
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
			  				httpGet("http://localhost:8080/?on_view=Y");
			  			}
			  			else {
			  				console.log("facebook is close");
			  				httpGet("http://localhost:8080/?on_view=N");
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

function checkTurnOffSandTimer(){
	var req = new XMLHttpRequest();
    req.open('GET', 'http://localhost:8080/', false); 
    req.send();
    if(req.status == 200) {
        //console.log(req.responseText);
        //chrome.tabs.remove(tabID); //close
        chrome.tabs.update(null, {url: "http://itp.nyu.edu/~ps2409/BlockFacebook/BlockFacebook.html"}); // redirect
    }
}