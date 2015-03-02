var setPebbleToken = 'KD3H';
var appUUID = '60a604d4-73c2-4fab-893b-592456edcc01';
var ready = false;

function versionCheck(uuid, current_version) {
  var url = 'http://pblweb.com/api/v1/version/' + uuid + '.json?current=';// + current_version;
  var req = new XMLHttpRequest();
  req.open('GET', url, true);
  req.onload = function (e) {
		console.log('readyState: ' + req.readyState);
		console.log('status: ' + req.status);
    if (req.readyState == 4 && req.status == 200)
		{
			var response = JSON.parse(req.responseText);
			var version = response.version;
			console.log('Newest Version: ' + version + '\n');
			//var newer = response.newer;
			if (current_version != version) {
				Pebble.showSimpleNotificationOnPebble('New version!', 'A new version (' + version + ') of Drug Wars is available.');
			}
    }
  };
  req.send(null);
}

Pebble.addEventListener('ready', function(e) {
	ready = true;
});
	
Pebble.addEventListener('appmessage', function(e) {
	if (ready)
	{
		if (typeof(e.payload.version) != 'undefined')
		{
			console.log('This Version: ' + e.paylod.version + '\n');
			versionCheck(appUUID, e.payload.version);    
		}
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
});

Pebble.addEventListener('showConfiguration', function(e) {
  Pebble.openURL('http://x.SetPebble.com/' + setPebbleToken + '/' + Pebble.getAccountToken());
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