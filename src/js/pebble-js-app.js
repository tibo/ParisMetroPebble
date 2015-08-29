var baseURL = 'https://secure-atoll-4691.herokuapp.com';

var messagesQueue = Array();

function sendNextMessage(){
  if (messagesQueue.length == 0)
    return;

  var message = messagesQueue[0];
  
  Pebble.sendAppMessage(message, 
    function(data){
      messagesQueue.shift();
      sendNextMessage();
    }, 
    function(data, error) {
      sendNextMessage();
    });
};

function fetchStations(latitude, longitude) {

  var url = baseURL + '/metro/stations';

  if (latitude && longitude) {
    url = url + '?ll=' + latitude + ',' + longitude + '&limit=5';
  }

  var req = new XMLHttpRequest();
  req.open('GET', url, true);
  req.onload = function () {
    if (req.readyState === 4) {
      if (req.status === 200) {
        // console.log(req.responseText);
        var response = JSON.parse(req.responseText);
        var stations = response['stations']
        stations.forEach(function(station){
          messagesQueue.push({'NEW_STATION_KEY' : station.key});
        });
        messagesQueue.push({'END_STATIONS_KEY' : 1});

        sendNextMessage();
      } 
      else {
        console.log('Error');
      }
    }
  };
  req.send(null);
}

function locationSuccess(pos) {
  fetchStations(pos.coords.latitude, pos.coords.longitude);
}

function locationError(err) {
  fetchStations();
}

Pebble.addEventListener("ready", function(e) {
      var locationOptions = {
        'timeout': 15000,
        'maximumAge': 60000
      };
      window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
});

Pebble.addEventListener('appmessage', function(e) {
  console.log('AppMessage received: ' + JSON.stringify(e.payload));
  
  if (e.payload['SELECTED_STATION_KEY']) {
    console.log('selected station: ' + e.payload['SELECTED_STATION_KEY']);
  };
});
