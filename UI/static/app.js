// Initialize Firebase
var firebaseConfig = {
    apiKey: "AIzaSyDOJPAGj9hJOIrceNeZN84Nmvcnc3DhrbA",
    authDomain: "feesh-93f92.firebaseapp.com",
    databaseURL: "https://feesh-93f92-default-rtdb.asia-southeast1.firebasedatabase.app",
    projectId: "feesh-93f92",
    storageBucket: "feesh-93f92.appspot.com",
    messagingSenderId: "595866266582",
    appId: "1:595866266582:web:16caf0ea0c707a025d1277"
};
firebase.initializeApp(firebaseConfig);

function setSchedule() {
    var scheduleInput = document.getElementById("schedule");
    var scheduleValue = scheduleInput.value;

    // Update the "times/time" key in Firebase
    firebase.database().ref('times').set({
        time: scheduleValue
    });
}

function turnOnFeed() {
    // Set the "feed" key to 1 in Firebase
    firebase.database().ref('feed').set(1);
}

function getCurrentSchedule() {
    var timesRef = firebase.database().ref('times');

    // Real-time listener for changes in the "times" node
    timesRef.on('value', function(snapshot) {
        var schedule = snapshot.val().time;
        document.getElementById("currentSchedule").innerHTML = "" + schedule;
    });
}

// Function to update the current clock on the webpage
function updateClock() {
    var currentTime = new Date();
    var hours = currentTime.getHours();
    var minutes = currentTime.getMinutes();
    var seconds = currentTime.getSeconds();

    // Add leading zeros if needed
    minutes = (minutes < 10 ? '0' : '') + minutes;
    seconds = (seconds < 10 ? '0' : '') + seconds;

    var formattedTime = hours + ':' + minutes + ':' + seconds;

    document.getElementById("currentClock").innerHTML = "" + formattedTime;

    // Update every second
    setTimeout(updateClock, 1000);
}

function getCurrentTemperature() {
    var temperatureRef = firebase.database().ref('Temperature');

    // Real-time listener for changes in the "Temperature" node
    temperatureRef.on('value', function(snapshot) {
        var temperature = snapshot.val().toFixed(2); // Limit to two decimal places
        document.getElementById("currentTemperature").innerHTML = "" + temperature + " °C";
    });
}

// Start updating the clock
updateClock();

getCurrentSchedule();

getCurrentTemperature();
