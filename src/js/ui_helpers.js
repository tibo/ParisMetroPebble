var ui_helpers = module.exports;

ui_helpers.colorForLine = function (line) {
  switch(line){
    case '1':
      return 'pastelYellow';
    case '2':
      return 'cobaltBlue';
    case '3':
      return 'brass';
    case '3b':
    case '13':
      return 'pictonBlue';
    case '4':
      return 'jazzberryJam';
    case '5':
      return 'rajah';
    case '6':
    case '7b':
      return 'mayGreen';
    case '7':
      return 'melon';
    case '8':
      return 'purpureus';
    case '9':
      return 'limerick';
    case '10':
      return 'chromeYellow';
    case '11':
      return 'windsorTan';
    case '12':
      return 'darkGreen';
    case '14':
      return 'indigo';
  }
  
  return 'tiffanyBlue';
};