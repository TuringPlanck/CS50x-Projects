/**
 * service.js
 *
 * Computer Science 50
 * Problem Set 8
 *
 * Implements a shuttle service.
 */

// default height
var HEIGHT = 0.8;

// default latitude
var LATITUDE = 42.3745615030193;

// default longitude
var LONGITUDE = -71.11803936751632;

// default heading
var HEADING = 1.757197490907891;

// default number of seats
var SEATS = 35;

// default velocity
var VELOCITY = 50;

// default points
var points = 0;

// global arrays for populate variables
var PLACEMARKS = [];
var MARKERS    = [];
var LONGLAT    = [];

// global variable for map population 
var people = 0;

// global variable for passengers picked up
var passengers = 0;

// global reference to shuttle's marker on 2D map
var bus = null;

// global reference to 3D Earth
var earth = null;

// global reference to 2D map
var map = null;

// global reference to shuttle
var shuttle = null;

// load version 1 of the Google Earth API
google.load("earth", "1");

// load version 3 of the Google Maps API
google.load("maps", "3", {other_params: "sensor=false"});

// once the window has loaded
$(window).load(function() {

    // listen for keydown anywhere in body
    $(document.body).keydown(function(event) {
        return keystroke(event, true);
    });

    // listen for keyup anywhere in body
    $(document.body).keyup(function(event) {
        return keystroke(event, false);
    });

    // listen for click on Drop Off button
    $("#dropoff").click(function(event) {
        dropoff();
    });

    // listen for click on Pick Up button
    $("#pickup").click(function(event) {
        pickup();
    });
    
    // listen for click on SpeedUp button
    $("#speedup").click(function(event) {
        speedup();
    });
    
    // listen for click on SlowDown button
    $("#slowdown").click(function(event) {
        slowdown();
    });

    // load application
    load();
});

// unload application
$(window).unload(function() {
    unload();
});

/**
 * Renders seating chart.
 */
function chart()
{
    var html = "<ol start='0'>";
    for (var i = 0; i < shuttle.seats.length; i++)
    {
        if (shuttle.seats[i] == null)
        {
            html += "<li>Empty Seat</li>";
        }
        else
        {
            html += "<li>" + shuttle.seats[i].name + "  ("+ shuttle.seats[i].house + ")</li>";
        }
    }
    html += "</ol>";
    $("#chart").html(html);
}

/**
 * Drops up passengers if their stop is nearby.
 */
function dropoff()
{
    // declare boolean for success/failure
    var success = false;
    
    // for every seat in the shuttle
    for (var i = 0; i < shuttle.seats.length; i++)
    {
        // if there is a passenger
        if (shuttle.seats[i] != null)
        {
            // if their house is nearby
            var house = shuttle.seats[i].house;
            if (shuttle.distance(HOUSES[house].lat, HOUSES[house].lng) < 30)
            {
                // dropoff passenger
                shuttle.seats[i] = null;
                success = true;
                people--;
                points++;               
                
                // if all passengers are dropped off
                if (people == 0)
                {
                    announce("All passengers dropped off!");
                    alert("Congratulations! You win!");
                    disableButtons();
                    break;
                }                
            }
        }
    }
    
    // announce state of dropoff
    if (success)
    {
        announce("Successful dropoff!  " + "points: " + points);
    }
    else
    {
        announce("There are no passengers with nearby houses");
    }
        
    chart();
}

/**
 * Called if Google Earth fails to load.
 */
function failureCB(errorCode) 
{
    // report error unless plugin simply isn't installed
    if (errorCode != ERR_CREATE_PLUGIN)
    {
        alert(errorCode);
    }
}

/**
 * Handler for Earth's frameend event.
 */
function frameend() 
{
    shuttle.update();
}

/**
 * Called once Google Earth has loaded.
 */
function initCB(instance) 
{
    // retain reference to GEPlugin instance
    earth = instance;

    // specify the speed at which the camera moves
    earth.getOptions().setFlyToSpeed(100);

    // show buildings
    earth.getLayerRoot().enableLayerById(earth.LAYER_BUILDINGS, true);

    // disable terrain (so that Earth is flat)
    earth.getLayerRoot().enableLayerById(earth.LAYER_TERRAIN, false);

    // prevent mouse navigation in the plugin
    earth.getOptions().setMouseNavigationEnabled(false);

    // instantiate shuttle
    shuttle = new Shuttle({
        heading: HEADING,
        height: HEIGHT,
        latitude: LATITUDE,
        longitude: LONGITUDE,
        planet: earth,
        seats: SEATS,
        velocity: VELOCITY
    });

    // synchronize camera with Earth
    google.earth.addEventListener(earth, "frameend", frameend);

    // synchronize map with Earth
    google.earth.addEventListener(earth.getView(), "viewchange", viewchange);

    // update shuttle's camera
    shuttle.updateCamera();

    // show Earth
    earth.getWindow().setVisibility(true);

    // render seating chart
    chart();

    // populate Earth with passengers and houses
    populate();
    
    // add buttons to window
    addButtons();
    
    passengers = people;
}

/**
 * Handles keystrokes.
 */
function keystroke(event, state)
{
    // ensure we have event
    if (!event)
    {
        event = window.event;
    }

    // left arrow
    if (event.keyCode == 37)
    {
        shuttle.states.turningLeftward = state;
        return false;
    }

    // up arrow
    else if (event.keyCode == 38)
    {
        shuttle.states.tiltingUpward = state;
        return false;
    }

    // right arrow
    else if (event.keyCode == 39)
    {
        shuttle.states.turningRightward = state;
        return false;
    }

    // down arrow
    else if (event.keyCode == 40)
    {
        shuttle.states.tiltingDownward = state;
        return false;
    }

    // A, a
    else if (event.keyCode == 65 || event.keyCode == 97)
    {
        shuttle.states.slidingLeftward = state;
        return false;
    }

    // D, d
    else if (event.keyCode == 68 || event.keyCode == 100)
    {
        shuttle.states.slidingRightward = state;
        return false;
    }
  
    // S, s
    else if (event.keyCode == 83 || event.keyCode == 115)
    {
        shuttle.states.movingBackward = state;     
        return false;
    }

    // W, w
    else if (event.keyCode == 87 || event.keyCode == 119)
    {
        shuttle.states.movingForward = state;    
        return false;
    }
  
    return true;
}

/**
 * Loads application.
 */
function load()
{
    // embed 2D map in DOM
    var latlng = new google.maps.LatLng(LATITUDE, LONGITUDE);
    map = new google.maps.Map($("#map").get(0), {
        center: latlng,
        disableDefaultUI: true,
        mapTypeId: google.maps.MapTypeId.ROADMAP,
        scrollwheel: false,
        zoom: 17,
        zoomControl: true
    });

    // prepare shuttle's icon for map
    bus = new google.maps.Marker({
        icon: "https://maps.gstatic.com/intl/en_us/mapfiles/ms/micons/bus.png",
        map: map,
        title: "you are here"
    });

    // embed 3D Earth in DOM
    google.earth.createInstance("earth", initCB, failureCB);
}

// increase velocity
function speedup()
{
    shuttle.velocity += VELOCITY;
    
}

// decrease velocity
function slowdown()
{
    shuttle.velocity -= VELOCITY;
}

// gets available seats in shuttle
function getSeat()
{
    for (var i = 0; i < shuttle.seats.length; i++)
    {
        if (shuttle.seats[i] == null)
        {
            return i;
        }
    }
    return shuttle.seats.length;
}


/**
 * Picks up nearby passengers.
 */
function pickup()
{
    // declare boolean for success/failure
    var success = false;
    
    // index for available seat in shuttle
    var seat = getSeat();
    
    // check that there is room onboard shuttle
    if (seat == shuttle.seats.length)
    {
        announce("There is no more room in the shuttle!");
        return;
    }
    
    // for all possible passengers
    var features = earth.getFeatures();
    for (var i = 0; i < PASSENGERS.length; i++)
    {
        // if seats are available
        if (seat != shuttle.seats.length)
        {
            // and passengers are nearby
            if (shuttle.distance(LONGLAT[i].latitude, LONGLAT[i].longitude) < 15)
            {
                // ensure we are not picking up freshmen
                for (var j in HOUSES)
                {
                    if (PASSENGERS[i].house == HOUSES[j])
                    {
                        break;
                    }
                    else if (j == HOUSES.length - 1)
                    {
                        announce("No freshmen allowed!!");
                        return;
                    }
                }
                // pickup passenger
                features.removeChild(PLACEMARKS[i]);
                PLACEMARKS[i] = null;
                MARKERS[i].setMap(null);
                shuttle.seats[seat] = PASSENGERS[i];
                passengers--;
                success = true;
            }
            // if all passengers are picked up
            if (passengers == 0)
            {
                announce("All passengers picked up!");
                break;
            }
        }
        else
        {
            announce("Shuttle full!");
            break;
        }
                       
        seat = getSeat();
    }
    
    // announce state of pickup
    if (success)
    {
        announce("Passengers Ready!");
    }
    else
    {
        announce("No Harvardians within range....<br>People must be within 15 meters to get onboard!");
    }
       
    chart();
}

/**
 * Populates Earth with passengers and houses.
 */
function populate()
{
    // mark houses
    for (var house in HOUSES)
    {
        // plant house on map
        new google.maps.Marker({
            icon: "https://google-maps-icons.googlecode.com/files/home.png",
            map: map,
            position: new google.maps.LatLng(HOUSES[house].lat, HOUSES[house].lng),
            title: house
        });
    }

    // get current URL, sans any filename
    var url = window.location.href.substring(0, (window.location.href.lastIndexOf("/")) + 1);

    // scatter passengers
    for (var i = 0; i < PASSENGERS.length; i++)
    {
        // ensure passengers are not pseudorandomly positioned at their own houses
        var building;
        do 
        {
            // pick a random building
            building = BUILDINGS[Math.floor(Math.random() * BUILDINGS.length)];
        }
        while ((building.lat - PASSENGERS[i].house.lat < Math.abs(0.0015)) || 
        (building.lng - PASSENGERS[i].house.lng < Math.abs(0.0015)))

        // prepare placemark
        var placemark = earth.createPlacemark("");
        placemark.setName(PASSENGERS[i].name + " to " + PASSENGERS[i].house);

        // prepare icon
        var icon = earth.createIcon("");
        icon.setHref(url + "/img/" + PASSENGERS[i].username + ".jpg");

        // prepare style
        var style = earth.createStyle("");
        style.getIconStyle().setIcon(icon);
        style.getIconStyle().setScale(4.0);

        // prepare stylemap
        var styleMap = earth.createStyleMap("");
        styleMap.setNormalStyle(style);
        styleMap.setHighlightStyle(style);

        // associate stylemap with placemark
        placemark.setStyleSelector(styleMap);

        // prepare point
        var point = earth.createPoint("");
        point.setAltitudeMode(earth.ALTITUDE_RELATIVE_TO_GROUND);
        point.setLatitude(building.lat);
        point.setLongitude(building.lng);
        point.setAltitude(0.0);

        // associate placemark with point
        placemark.setGeometry(point);

        // add placemark to Earth
        earth.getFeatures().appendChild(placemark);

        // add marker to map
        var marker = new google.maps.Marker({
            icon: "https://maps.gstatic.com/intl/en_us/mapfiles/ms/micons/man.png",
            map: map,
            position: new google.maps.LatLng(building.lat, building.lng),
            title: PASSENGERS[i].name + " at " + building.name
        });

        // TODO: remember passenger's placemark and marker for pick-up's sake
        MARKERS.push(marker);       
        PLACEMARKS.push(placemark);
        LONGLAT.push({longitude: placemark.getGeometry().getLongitude(), latitude: placemark.getGeometry().getLatitude()})
        people++;
    }
}

/**
 * Handler for Earth's viewchange event.
 */
function viewchange() 
{
    // keep map centered on shuttle's marker
    var latlng = new google.maps.LatLng(shuttle.position.latitude, shuttle.position.longitude);
    map.setCenter(latlng);
    bus.setPosition(latlng);
}

/**
 * Unloads Earth.
 */
function unload()
{
    google.earth.removeEventListener(earth.getView(), "viewchange", viewchange);
    google.earth.removeEventListener(earth, "frameend", frameend);
}

function announce(s)
{
    document.getElementById("announcements").innerHTML = s;
}


function addButtons()
{
    document.getElementById("controls").innerHTML = 
        "<input onclick=\"pickup();\" type=\"button\" value=\"Pick Up\">" +
        "<input onclick=\"dropoff();\" type=\"button\" value=\"Drop Off\">" +
        "<input onclick=\"speedup();\" type=\"button\" value=\"SpeedUp\">" +
        "<input onclick=\"slowdown();\" type=\"button\" value=\"Slowdown\">";
}

function disableButtons()
{
    document.getElementById("controls").innerHTML = 
        "<input onclick=\"pickup();\" type=\"button\" disabled=\"disabled\" value=\"Pick Up\">" +
        "<input onclick=\"dropoff();\" type=\"button\" disabled=\"disabled\" value=\"Drop Off\">"
        "<input onclick=\"speedup();\" type=\"button\" disabled=\"disabled\" value=\"SpeedUp\">" +
        "<input onclick=\"slowdown();\" type=\"button\" disabled=\"disabled\" value=\"Slowdown\">";
}
