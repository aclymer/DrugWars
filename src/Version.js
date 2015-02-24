var setPebbleToken = 'FYQN';
var appUUID = '60a604d4-73c2-4fab-893b-592456edcc01';

function versionCheck(uuid, version) {
  var url = 'http://pblweb.com/api/v1/version/' + uuid + '.json?current=' + version;
  var req = new XMLHttpRequest();
  req.open('GET', url, true);
  req.onload = function (e) {
    if (req.readyState == 4 && req.status == 200) {
      if (req.status == 200) {
        var response = JSON.parse(req.responseText);
        var version = response.version;
				console.log("Newest Version: " + e.paylod.version + "\n");
        var newer = response.newer;
        if (newer) {
          Pebble.showSimpleNotificationOnPebble('New version!', 'A new version (' + version + ') of Drug Wars is available.');
        }
      }
    }
  };
  req.send(null);
}

Pebble.addEventListener('ready', function(e) {
});
	
Pebble.addEventListener('appmessage', function(e) {
	console.log("This Version: " + e.paylod.version + "\n");
	if (typeof(e.payload.version) != 'undefined')
	{
		versionCheck(appUUID, e.payload.version);
    var settings = localStorage.getItem(setPebbleToken);
    if (typeof(settings) == 'string') {
      try	{ Pebble.sendAppMessage(JSON.parse(settings)); } 
			catch (e)	{}
    }
    var request = new XMLHttpRequest();
    request.open('GET', 'http://x.SetPebble.com/api/' + setPebbleToken + '/' + Pebble.getAccountToken(), true);
    request.onload = function(e) {
      if (request.readyState == 4)
        if (request.status == 200)
          try { Pebble.sendAppMessage(JSON.parse(request.responseText)); }
					catch (e) {}
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
		catch(e) {}
  }
});