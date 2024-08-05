    // Konfigurasi Firebase
    var firebaseConfig = {
        apiKey: "AIzaSyD_8VkwW2IURyk2ICwYjLm3uUsddh-B8xk",
        authDomain: "vertikultur-7b393.firebaseapp.com",
        databaseURL: "https://vertikultur-7b393-default-rtdb.firebaseio.com",
        projectId: "vertikultur-7b393",
        storageBucket: "vertikultur-7b393.appspot.com",
        messagingSenderId: "68659489325",
        appId: "1:68659489325:web:80b899dcd6ef19e548a6a4",
        measurementId: "G-H6TQ470T93"
      };
  
      // Inisialisasi Firebase
      firebase.initializeApp(firebaseConfig);
  
      // Referensi ke Firebase Database
      var db = firebase.database().ref();
  
      // Fungsi untuk memperbarui tampilan dengan data dari Firebase
      function updateData() {
        db.on('value', function (snapshot) {
          var data = snapshot.val();
          document.getElementById('humidity').innerHTML = "Humidity: " + data.humidity + "%";
          document.getElementById('temperature').innerHTML = "Temperature: " + data.temperature + "°C";
          document.getElementById('tds').innerHTML = "TDS: " + data.tds + " ppm";
          var waterLevelElement = document.getElementById('waterLevel');
          if (data.waterLevel == 0) {
            waterLevelElement.innerHTML = "Water Level: Air Kurang";
          } else {
            waterLevelElement.innerHTML = "Water Level: Air Cukup";
          }
          document.getElementById('soilMoisture').innerHTML = "Soil Moisture: " + data.soilMoisture + "%";
        });
      }
  
      // Panggil fungsi updateData untuk memperbarui data saat halaman dimuat
      window.onload = function () {
        updateData();
      };