const char* htmlHomePage PROGMEM = R"HTMLHOMEPAGE(
<!DOCTYPE html>
<html>
  <head>
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
    <style>
    .arrows {
      font-size:70px;
      color:AliceBlue;
    }
    .circularArrows {
      font-size:80px;
      color:Cornsilk;
    }
    td {
      background-color:black;
      border-radius:25%;
      box-shadow: 5px 5px #888888;
    }
    td:active {
      transform: translate(5px,5px);
      box-shadow: none; 
    }
    .sensor {
        font-size: 24px;
        margin-top: 20px;
    }
    .noselect {
      -webkit-touch-callout: none; /* iOS Safari */
        -webkit-user-select: none; /* Safari */
         -khtml-user-select: none; /* Konqueror HTML */
           -moz-user-select: none; /* Firefox */
            -ms-user-select: none; /* Internet Explorer/Edge */
                user-select: none; /* Non-prefixed version, currently
                                      supported by Chrome and Opera */
    }
    </style>
  </head>
  <body class="noselect" align="center" style="background-color:white">
     
    <h1 style="color: teal;text-align:center;">AGV &#128663; Web Controller</h1>
    
    <div class="sensor">Temperature &#127777;:  <span id="tempValue">50</span></div>
    <div class="sensor">Humidity &#127783;: <span id="humiValue">100</span></div>
    <div class="sensor">Light &#9728;: <span id="ldrValue">100</span> Ultra &#128677;: <span id="UltraValue">100</span></div>
    
    <table id="mainTable" style="width:400px;margin:auto;table-layout:fixed" CELLSPACING=10>
      <tr>
        <td ontouchstart='onTouchStartAndEnd("5")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows" >&#11017;</span></td>
        <td ontouchstart='onTouchStartAndEnd("1")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows" >&#8679;</span></td>
        <td ontouchstart='onTouchStartAndEnd("6")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows" >&#11016;</span></td>
      </tr>
      
      <tr>
        <td ontouchstart='onTouchStartAndEnd("3")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows" >&#8678;</span></td>
        <td ontouchstart='onTouchStartAndEnd("11")' ontouchend='onTouchStartAndEnd("12")'><span class="arrows" >&#9835;</span></td>   
        <td ontouchstart='onTouchStartAndEnd("4")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows" >&#8680;</span></td>
      </tr>
      
      <tr>
        <td ontouchstart='onTouchStartAndEnd("7")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows" >&#11019;</span></td>
        <td ontouchstart='onTouchStartAndEnd("2")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows" >&#8681;</span></td>
        <td ontouchstart='onTouchStartAndEnd("8")' ontouchend='onTouchStartAndEnd("0")'><span class="arrows" >&#11018;</span></td>
      </tr>
    
      <tr>
        <td ontouchstart='onTouchStartAndEnd("9")' ontouchend='onTouchStartAndEnd("0")'><span class="circularArrows" >&#8634;</span></td>
        
        <td style="background-color:white;box-shadow:none"><h2 style="color: DarkMagenta;text-align:center;">Via &#128640; IDektep</h2></td>
        <td ontouchstart='onTouchStartAndEnd("10")' ontouchend='onTouchStartAndEnd("0")'><span class="circularArrows" >&#8635;</span></td>
      </tr>
    </table>

    <script>
      var webSocketUrl = "ws:\/\/" + window.location.hostname + "/ws";
      var websocket;
      
      function initWebSocket() 
      {
        websocket = new WebSocket(webSocketUrl);
        websocket.onopen    = function(event){};
        websocket.onclose   = function(event){setTimeout(initWebSocket, 2000);};
        websocket.onmessage = function(event){};
      }

      function onTouchStartAndEnd(value) 
      {
        websocket.send(value);
      }
          
      window.onload = initWebSocket;
      document.getElementById("mainTable").addEventListener("touchend", function(event){
        event.preventDefault()
      });      
    </script>
    <script>
      function toggleCheckbox(x) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/" + x, true);
        xhr.send();
      }
      function updateHumidity(value) {
        document.getElementById('humiValue').innerText = value;
      }
      function updateTemperature(value) {
        document.getElementById('tempValue').innerText = value;
      }
      function updateLight(value) {
        document.getElementById('ldrValue').innerText = value;
      }
      function updateUltra(value) {
        document.getElementById('UltraValue').innerText = value;
      }
      
      setInterval(function () {
        var tempXhr = new XMLHttpRequest();
        tempXhr.onreadystatechange = function () {
          if (tempXhr.readyState == 4 && tempXhr.status == 200) {
            var tempValue = parseFloat(tempXhr.responseText);
            updateTemperature(tempValue);
          }
        };
        tempXhr.open("GET", "/temperature", true);
        tempXhr.send();

        var humiXhr = new XMLHttpRequest();
        humiXhr.onreadystatechange = function () {
          if (humiXhr.readyState == 4 && humiXhr.status == 200) {
            var humiValue = parseFloat(humiXhr.responseText);
            updateHumidity(humiValue);
          }
        };
        humiXhr.open("GET", "/humidity", true);
        humiXhr.send();

        var ldrXhr = new XMLHttpRequest();
        ldrXhr.onreadystatechange = function () {
          if (ldrXhr.readyState == 4 && ldrXhr.status == 200) {
            var ldrValue = parseFloat(ldrXhr.responseText);
            updateLight(ldrValue);
          }
        };
        ldrXhr.open("GET", "/light", true);
        ldrXhr.send();
        
        var UltraXhr = new XMLHttpRequest();
        UltraXhr.onreadystatechange = function () {
          if (UltraXhr.readyState == 4 && UltraXhr.status == 200) {
            var UltraValue = parseFloat(UltraXhr.responseText);
            updateUltra(UltraValue);
          }
        };
        UltraXhr.open("GET", "/Ultra", true);
        UltraXhr.send();
        
      }, 1000);
    </script>
  </body>
</html> 

)HTMLHOMEPAGE";

void handleRoot(AsyncWebServerRequest *request)
{
  request->send_P(200, "text/html", htmlHomePage);

  server.on("/temperature", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(temp));
  });

  server.on("/humidity", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(humi));
  });
  server.on("/light", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(ldr));
  });
  
  server.on("/Ultra", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(Ultra));
  });
}

void handleNotFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "File Not Found");
}

void onWebSocketEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    // client->text(getRelayPinsStatusJson(ALL_RELAY_PINS_INDEX));
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    processAGVMovement("0");
    break;
  case WS_EVT_DATA:
    AwsFrameInfo *info;
    info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
      std::string myData = "";
      myData.assign((char *)data, len);
      processAGVMovement(myData.c_str());
    }
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  default:
    break;
  }
}
