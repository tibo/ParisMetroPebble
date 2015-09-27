var parisTransportAPI = module.exports;

var baseURL = 'https://secure-atoll-4691.herokuapp.com';

parisTransportAPI.fetchStations = function(latitude, longitude, callback) {

  var url = baseURL + '/stations';

  if (latitude && longitude) {
    url = url + '?ll=' + latitude + ',' + longitude + '&limit=5&device=pebble';
  }
  else {
    url = url + '?device=pebble';
  }
  
  var req = new XMLHttpRequest();
  req.open('GET', url, true);
  req.onload = function () {
    if (req.readyState === 4) {
      if (req.status === 200) {
        var response = JSON.parse(req.responseText);
        var stations = response.stations;
        callback(stations, null);
      } 
      else {
        callback(null, 'error');
      }
    }
  };
  req.send(null);
},
parisTransportAPI.fetchLinesForStation= function(station_key, callback) {
  var url = baseURL + '/metro/stations/' + station_key.replace(/ /g,'%20') + '/lines?device=pebble';

  var req = new XMLHttpRequest();
  req.open('GET', url, true);
  req.onload = function () {
    if (req.readyState === 4) {
      if (req.status === 200) {
        var response = JSON.parse(req.responseText);
        var lines = response.lines;
        callback(lines, null);
      } 
      else {
        callback(null, 'error');
      }
    }
  };
  req.send(null);
},
parisTransportAPI.fetchSchedules = function(station_key, line, direction, callback) {

  var url = baseURL + '/metro/stations/' + station_key.replace(/ /g,'%20') + '/lines/' + line + '/directions/' + direction + '/schedules?device=pebble';

  var req = new XMLHttpRequest();
  req.open('GET', url, true);
  req.onload = function () {
    if (req.readyState === 4) {
      if (req.status === 200) {
        var response = JSON.parse(req.responseText);
        var schedules = response.schedules;
        callback(schedules,null);
      } 
      else {
        callback(null,'error');
      }
    }
  };
  req.send(null);
};