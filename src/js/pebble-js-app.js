var baseURL = 'https://secure-atoll-4691.herokuapp.com';

function fetchStations(latitude, longitude) {

  var url = baseURL + '/metro/stations';

  if (latitude && longitude) {
    url = url + '?ll=' + latitude + ',' + longitude;
  }

  var req = new XMLHttpRequest();
  req.open('GET', url, true);
  req.onload = function () {
    if (req.readyState === 4) {
      if (req.status === 200) {
        console.log(req.responseText);
        var response = JSON.parse(req.responseText);
        Pebble.sendAppMessage(response);
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

Pebble.addEventListener("ready",
    function(e) {
      console.log('ready from js');

      return;
      var locationOptions = {
        'timeout': 15000,
        'maximumAge': 60000
      };
      window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
    }
);
