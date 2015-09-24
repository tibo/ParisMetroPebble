var UI = require('ui');

// API
var baseURL = 'https://secure-atoll-4691.herokuapp.com';

function fetchStations(latitude, longitude, callback) {

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
        callback(stations);
      } 
      else {
        showError(null);
      }
    }
  };
  req.send(null);
}

function fetchLinesForStation(station_key, callback) {
  var url = baseURL + '/metro/stations/' + station_key.replace(/ /g,'%20') + '/lines';

  var req = new XMLHttpRequest();
  req.open('GET', url, true);
  req.onload = function () {
    if (req.readyState === 4) {
      if (req.status === 200) {
        var response = JSON.parse(req.responseText);
        var lines = response.lines;
        callback(lines);
      } 
      else {
        showError(null);
      }
    }
  };
  req.send(null);
}

function fetchSchedules(station_key, line, direction, callback) {

  var url = baseURL + '/metro/stations/' + station_key.replace(/ /g,'%20') + '/lines/' + line + '/directions/' + direction + '/schedules';

  var req = new XMLHttpRequest();
  req.open('GET', url, true);
  req.onload = function () {
    if (req.readyState === 4) {
      if (req.status === 200) {
        var response = JSON.parse(req.responseText);
        var schedules = response.schedules;
        callback(schedules);
      } 
      else {
        showError(null);
      }
    }
  };
  req.send(null);
}

// UI
var loadingScreen = new UI.Card({
  fullscreen: true,
  title: 'Paris Metro',
  body: 'Loading...',
  scrollable: false
});

function showError(error_message){
  var card = new UI.Card({
    title: 'Error',
    body: (error_message?error_message:"Something went wrong!"),
    scrollable: false
  });
  
  card.show();
}

function showStationsList(stations){
  var stations_list = new UI.Menu({
    fullscreen: true,
    backgroundColor: 'white',
    textColor: 'black',
    highlightBackgroundColor: 'tiffanyBlue',
    highlightTextColor: 'white',
    sections: [{
      title: 'Nearby',
      items: stations
    }]
  });
  
  stations_list.show();
  
  stations_list.on('select', function(e) {
    loadingScreen.show();
    fetchLinesForStation(e.item.key, function(lines, error){
      var ui_lines = Array();
      
      lines.forEach(function(line){
        var l = {'title' : line.line, 'station_key': e.item.key};
        var items = Array();
        line.destinations.forEach(function(destination){
          items.push({
            'title': destination.name,
            'direction': destination.direction
          });
        });
        l.items = items;
        
        ui_lines.push(l);
        setupLinesList(ui_lines);
      });
    });
  });
}

function setupLinesList(lines){
  var lines_list = new UI.Menu({
    fullscreen: true,
    backgroundColor: 'white',
    textColor: 'black',
    highlightBackgroundColor: 'tiffanyBlue',
    highlightTextColor: 'white',
    sections: lines
  });
  
  loadingScreen.hide();
  lines_list.show();
  
  lines_list.on('select', function(e) {
    loadingScreen.show();
    fetchSchedules(e.section.station_key, e.section.title, e.item.direction, function(schedules, error){
      var ui_schedules = Array();
      
      schedules.forEach(function(schedule){
        if (schedule.arriving) {
          ui_schedules.push({
            'title' : schedule.destination,
            'items' : [{
              'title' : schedule.arriving
            }]
          });
        }
        else {
          ui_schedules.push({
            'title' : '',
            'items' : [{
              'title' : schedule.destination
            }]
          });
        }
      });
      
      setupSchedulesList(ui_schedules);
    });
  });
}

function setupSchedulesList(schedules){
  var schedule_list = new UI.Menu({
    fullscreen: true,
    backgroundColor: 'white',
    textColor: 'black',
    highlightBackgroundColor: 'tiffanyBlue',
    highlightTextColor: 'white',
    sections: schedules
  });
  
  loadingScreen.hide();
  schedule_list.show();
}

// geolocation
function locationSuccess(pos) {
  fetchStations(pos.coords.latitude, pos.coords.longitude, function(stations){
    loadingScreen.hide();
    showStationsList(stations);
  });
}

function locationError(err) {
  fetchStations(null, null, function(stations){
    loadingScreen.hide();
    showStationsList(stations);
  });
}

loadingScreen.show();

var locationOptions = {
  'timeout': 15000,
  'maximumAge': 60000
};
window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);