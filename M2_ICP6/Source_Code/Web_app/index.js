var hostname = "broker.mqttdashboard.com";
var port = 8000;
var clientid = "zealmqtttproject2"+Math.floor(Math.random() * 100) + 1;;
var client = new Messaging.Client(hostname, port, clientid);

var options = {
     timeout: 3,
     onSuccess: function () {
         console.log("Connected to broker");
		 var stat = document.getElementById("status");
		 stat.innerHTML = "Connected";
		 stat.style = "color:green";
     },
     onFailure: function (message) {
         console.log("Connection failed: " + message.errorMessage);
     }
 };
//Attempt to connect
client.connect(options);

client.onConnectionLost = function (responseObject) {
     //Depending on your scenario you could implement a reconnect logic here
     console.log("connection lost: " + responseObject.errorMessage);
	 var stat = document.getElementById("status");
	 stat.innerHTML = "Not Connected";
	 stat.style = "color:red";
 };

 //Gets called whenever you receive a message for your subscriptions
 client.onMessageArrived = function (message) {
     //Do something with the push message you received
	 var in_msg = parseInt(message.payloadString);
	console.log(in_msg);
	document.getElementById("recv_msg").innerHTML = in_msg;
	if (in_msg == 3){
		extender_slider.disabled = false;
	} else if (in_msg == 4) {
		rotate_slider.disabled = false;
	} else if (in_msg == 5) {
		grabber_slider.disabled = false;
	} else if (in_msg == 6) {
		height_slider.disabled = false;
	}
 };

 //Creates a new Messaging.Message Object and sends it to the HiveMQ MQTT Broker
 var publish = function (payload, topic, qos) {
     //Send your message (also possible to serialize it as JSON or protobuf or just use a string, no limitations)
     var message = new Messaging.Message(payload);
     message.destinationName = topic;
     message.qos = qos;
     client.send(message);
 }

//this is on button click script
function extender(){
this.disabled = true;
publish('forward','iot_roboticarm_app',2);
publish(this.value,'iot_roboticarm_app/ext',2);
}
function rotate(){
publish('backward','iot_roboticarm_app',2);
var temp = document.getElementById("sent_msg");
temp.innerHTML = 'backward';
}
function grabber(){
publish('left','iot_roboticarm_app',2);
var temp = document.getElementById("sent_msg");
temp.innerHTML = 'left';
}
function height(){
publish('right','iot_roboticarm_app',2);
var temp = document.getElementById("sent_msg");
temp.innerHTML = 'right';
}


function moveup(){
publish('up','iot_roboticarm_app',2);
var temp = document.getElementById("sent_msg");
temp.innerHTML = 'up';
}
function movedown(){
publish('down','iot_roboticarm_app',2);
var temp = document.getElementById("sent_msg");
temp.innerHTML = 'down';
}
function end(){
publish('stop','iot_roboticarm_app',2);
var temp = document.getElementById("sent_msg");
temp.innerHTML = 'stop';
}
