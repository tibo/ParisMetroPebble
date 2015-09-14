// events queue
var messagesQueue = Array();
var numberOfRetry = 5;

function sendNextMessage(){
  if (messagesQueue.length == 0)
  {
    console.log('empty message queue');
    return;
  }

  var message = messagesQueue[0];
  
  Pebble.sendAppMessage(message, 
    function(data){
      messagesQueue.shift();
      numberOfRetry = 5;
      sendNextMessage();
    }, 
    function(data) {
      console.log('error sending message: ' + data.data.error.message);
      numberOfRetry --;
      if (numberOfRetry > 0) {
        sendNextMessage();
      }
    });
};

//
var selected_station_key = null;
var selected_line = null;
var selected_direction = null;

// API
var baseURL = 'https://secure-atoll-4691.herokuapp.com';

function fetchStations(latitude, longitude) {

  var url = baseURL + '/metro/stations';

  if (latitude && longitude) {
    url = url + '?ll=' + latitude + ',' + longitude + '&limit=5';
  }
  // debug Chatelet
  // url = url + '?ll=48.858706,2.347456&limit=1';

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
          messagesQueue.push({'NEW_STATION_NAME' : station.name});
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

function fetchLinesForStation(station_key) {
  selected_station_key = station_key;
  var url = baseURL + '/metro/stations/' + station_key.replace(/ /g,'%20') + '/lines';

  var req = new XMLHttpRequest();
  req.open('GET', url, true);
  req.onload = function () {
    if (req.readyState === 4) {
      if (req.status === 200) {
        var response = JSON.parse(req.responseText);
        var lines = response['lines']
        lines.forEach(function(line){
          messagesQueue.push({'NEW_LINE_KEY' : line.line});
          var destinations = line['destinations'];
          destinations.forEach(function(destination){
            messagesQueue.push({'NEW_DESTINATION_DIRECTION_KEY' : destination.direction});
            messagesQueue.push({'NEW_DESTINATION_NAME_KEY' : destination.name});
          });
        });
        messagesQueue.push({'END_DESTINATIONS_KEY' : 1});

        sendNextMessage();
      } 
      else {
        console.log('Error');
      }
    }
  };
  req.send(null);
}

function fetchSchedules() {
  if (!selected_station_key) {
    console.log('error: lost track of the selected station');
    return;
  }

  if (!selected_line || !selected_direction) {
    return;
  }

  var url = baseURL + '/metro/stations/' + selected_station_key.replace(/ /g,'%20') + '/lines/' + selected_line + '/directions/' + selected_direction + '/schedules';

  var req = new XMLHttpRequest();
  req.open('GET', url, true);
  req.onload = function () {
    if (req.readyState === 4) {
      if (req.status === 200) {
        console.log(req.responseText);
        var response = JSON.parse(req.responseText);
        var schedules = response['schedules']
        schedules.forEach(function(schedule){
          messagesQueue.push({'NEW_SCHEDULE_DESTINATION_KEY' : schedule.destination});
          messagesQueue.push({'NEW_SCHEDULE_TIME_KEY' : schedule.arriving});
        });
        messagesQueue.push({'END_SCHEDULES_KEY' : 1});

        sendNextMessage();
      } 
      else {
        console.log('Error');
      }
    }
  };
  req.send(null);
}

// geolocation
function locationSuccess(pos) {
  fetchStations(pos.coords.latitude, pos.coords.longitude);
}

function locationError(err) {
  fetchStations();
}

// event observers
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
    fetchLinesForStation(e.payload['SELECTED_STATION_KEY']);
  }

  if (e.payload['SELECTED_LINE_KEY']) {
    selected_line = e.payload['SELECTED_LINE_KEY'];
    fetchSchedules();
  }
  
  if (e.payload['SELECTED_DIRECTION_KEY']) {
    selected_direction = e.payload['SELECTED_DIRECTION_KEY'];
    fetchSchedules();
  }
});
