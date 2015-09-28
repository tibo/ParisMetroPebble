var ajax = require('ajax');

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
  
  ajax(
    {
      url: url,
      type: 'json'
    },
    function(data, status, request) {
      callback(data.stations, null);
    },
    function(error, status, request) {
      callback(null, error);
    }
  );
},
parisTransportAPI.fetchLinesForStation= function(station_key, callback) {
  var url = baseURL + '/metro/stations/' + station_key.replace(/ /g,'%20') + '/lines?device=pebble';
  
  ajax(
    {
      url: url,
      type: 'json'
    },
    function(data, status, request) {
      callback(data.lines, null);
    },
    function(error, status, request) {
      callback(null, error);
    }
  );
},
parisTransportAPI.fetchSchedules = function(station_key, line, direction, callback) {
  var url = baseURL + '/metro/stations/' + station_key.replace(/ /g,'%20') + '/lines/' + line + '/directions/' + direction + '/schedules?device=pebble';
  
  ajax(
    {
      url: url,
      type: 'json'
    },
    function(data, status, request) {
      callback(data.schedules, null);
    },
    function(error, status, request) {
      callback(null, error);
    }
  );
};