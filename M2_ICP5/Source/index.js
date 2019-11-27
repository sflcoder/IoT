var hostname = "broker.mqttdashboard.com";
var port = 8000;
var clientid = "zealmqttid"+Math.floor(Math.random() * 100) + 1;;
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
	console.log(message.payloadString);
	document.getElementById("recv_msg").innerHTML = message.payloadString;
	if (message.payloadString == "obstacle"){
	document.getElementById("recv_msg").style = "color:red"}
	else{document.getElementById("recv_msg").style = "color:green"}
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
function moveup(){
publish('forward','iot_class_icp4',2);
var temp = document.getElementById("sent_msg");
temp.innerHTML = 'forward';
}
function movedown(){
publish('backward','iot_class_icp4',2);
var temp = document.getElementById("sent_msg");
temp.innerHTML = 'backward';
}
function moveleft(){
publish('left','iot_class_icp4',2);
var temp = document.getElementById("sent_msg");
temp.innerHTML = 'left';
}
function moveright(){
publish('right','iot_class_icp4',2);
var temp = document.getElementById("sent_msg");
temp.innerHTML = 'right';
}
function end(){
publish('end','iot_class_icp4',2);
var temp = document.getElementById("sent_msg");
temp.innerHTML = 'end';
}
