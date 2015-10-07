var UI = require('ui');
var ui_helpers = require('ui_helpers');
var API = require('paris_transport_API');

var selected_station;

// UI
var loadingScreen = new UI.Card({
  fullscreen: true,
  backgroundColor: ui_helpers.colorForLine(null),
  textColor: 'white',
  body: 'Loading...',
  banner: 'IMAGE_METRO_140',
  scrollable: false
});

function showError(error_message){
  var card = new UI.Card({
    fullscreen: true,
    backgroundColor: 'red',
    textColor: 'white',
    title: 'Error',
    body: (error_message?error_message:"Something went wrong!"),
    scrollable: false
  });
  
  card.show();
}

function showStationsList(stations){
  var stations_list = new UI.Menu({
    fullscreen: true,
    backgroundColor: 'black',
    textColor: 'white',
    highlightBackgroundColor: ui_helpers.colorForLine(null),
    highlightTextColor: 'white',
    sections: [{
      items: stations
    }]
  });
  
  stations_list.show();
  
  stations_list.on('select', function(e) {
    selected_station = e.item.key;
    loadingScreen.show();
    API.fetchLinesForStation(selected_station, function(lines, error){
      loadingScreen.hide();
      if(error) {
        showError(error);
      }
      else {
        showLinesList(lines);
      }
    });
  });
}

function showLinesList(lines){
  var lines_list = new UI.Menu({
    fullscreen: true,
    backgroundColor: 'black',
    textColor: 'white',
    highlightBackgroundColor: ui_helpers.colorForLine(null),
    highlightTextColor: 'white',
    sections: lines
  });
  
  lines_list.show();
  
  lines_list.on('select', function(e) {
    loadingScreen.show();
    API.fetchSchedules(selected_station, e.section.title, e.item.direction, function(schedules, error){
      loadingScreen.hide();
      if (error){
        showError(error);
      }
      else {
        showSchedulesList(schedules);
      }
    });
  });
}

function showSchedulesList(schedules){
  var schedule_list = new UI.Menu({
    fullscreen: true,
    backgroundColor: 'black',
    textColor: 'white',
    highlightBackgroundColor: ui_helpers.colorForLine(null),
    highlightTextColor: 'white',
    sections: schedules
  });
  
  schedule_list.show();
}

// geolocation
function locationSuccess(pos) {
  API.fetchStations(pos.coords.latitude, pos.coords.longitude, function(stations, error){
    loadingScreen.hide();
    if (error){
      showError(error);
    }
    else {
      showStationsList(stations);  
    }
    
  });
}

function locationError(err) {
  API.fetchStations(null, null, function(stations, error){
    loadingScreen.hide();
    if (error) {
      showError(error);
    }
    else {
      showStationsList(stations);
    }
  });
}

loadingScreen.show();

var locationOptions = {
  'timeout': 15000
};

navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);