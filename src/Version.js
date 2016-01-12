var setPebbleToken = 'PHZE';
var ready = false;

Pebble.addEventListener('ready', function(e) {
  ready = true;
  console.log("Ready! Acct Token: " + Pebble.getAccountToken());
  try
  {
  	var settings = localStorage.getItem(setPebbleToken);
  
  	var request = new XMLHttpRequest();
  	request.open('GET', 'http://x.SetPebble.com/api/' + setPebbleToken + '/' + Pebble.getAccountToken(), false);
  	request.onload = function(e) {
  		if (request.readyState == 4)
  			if (request.status == 200)
  				if (typeof(settings) == 'string')
  				{
  					if (settings != request.responseText)
  					{
  						try { Pebble.sendAppMessage(JSON.parse(request.responseText)); }
  						catch (f) {console.log(f);}
  					}
  					else
  					{
  						try	{ Pebble.sendAppMessage(JSON.parse(settings)); } 
  						catch (f)	{ console.log(f); }
  					}
  				}
  	};
  	request.send(null);
  }
  catch(f) {console.log(f);}
});
	
Pebble.addEventListener('appmessage', function(e) {
		
});

Pebble.addEventListener('showConfiguration', function(e) {
  try  { Pebble.openURL('http://x.SetPebble.com/' + setPebbleToken + '/' + Pebble.getAccountToken()); }
  catch(f) { console.log(f); }
});
	
Pebble.addEventListener('webviewclosed', function(e) {
  if ((typeof(e.response) == 'string') && (e.response.length > 0)) {
    try
		{
      Pebble.sendAppMessage(JSON.parse(e.response));
      localStorage.setItem(setPebbleToken, e.response);
    }
		catch(f) {console.log(f);}
  }
});