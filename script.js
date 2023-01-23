var firebaseConfig = {
  apiKey: "AIzaSyBQ03yTMt0QRLb5MMy9UefOt8shsgxoXmg",
  authDomain: "remote-patient-monitorin-8e7b1.firebaseapp.com",
  databaseURL: "https://remote-patient-monitorin-8e7b1-default-rtdb.firebaseio.com",
  projectId: "remote-patient-monitorin-8e7b1",
  storageBucket: "remote-patient-monitorin-8e7b1.appspot.com",
  messagingSenderId: "619095473620",
  appId: "1:619095473620:web:21dd41131b9dad2dbc2a41",
  measurementId: "G-WP93SDYLD5"
 };

firebase.initializeApp(firebaseConfig);

$(document).ready(function(){
var database=firebase.database();

var bpm;
var spo2;
var temp;
var z;
var em;


database.ref().on("value",function(snap){
 bpm=snap.val().BPM;
 spo2=snap.val().SpO2;
 temp=snap.val().Temperature;
 z=snap.val().z;
 em=snap.val().Emergency;

$(".t").text(temp)
if(temp<38){
  $(".tt-n").text("Temperature Normal")
}
else{
  $(".tt-n").text("Temperature Abnormal")
}
if(z==1){
  $(".h").text(bpm)
  $(".s").text(spo2)

  if((bpm>=60)&&(bpm<=100)){
    $(".hh-n").text("BPM Normal")
  }
  else{
    $(".hh-n").text("BPM Abnormal")
  }
  if((spo2>=95)&&(spo2<=100)){
    $(".oo-n").text("SpO2 Normal")
  }
  else{
    $(".oo-n").text("SpO2 Abnormal")
  }
}
else{
  $(".h").text("0")
  $(".s").text("0")
  $(".hh-n").text("BPM Not Detected")
  $(".oo-n").text("SpO2 Not Detected")
}
if(z==1){
  $(".sen").text("BPM & SPO2 Sensor CONNECTED");

}
else{
  $(".sen").text("BPM & SPO2 Sensor DISCONNECTED");
}
if(em==1){
  $(".emer").css("background-color","red");
  $(".emer").css("color","white");
}
else{
  $(".emer").css("background-color","white");
  $(".emer").css("color","black");
}


});

});
