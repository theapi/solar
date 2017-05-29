
const char index_html[] PROGMEM = R"=====(

<html>
<head>
<script type="text/javascript">
var data_obj;
var timer = 0;
</script>


    <script type="text/javascript">
       var data_json = '{"garden":{"msg_type":0,"msg_id":0,"vcc":0,"solar_mv":0,"charge_ma":0,"light": 0,"cpu":0,"temperature":0}, "signal":{"rssi":0,"snr":0}, "connection":{"status":0}}';
       data_obj = JSON.parse(data_json);
    </script>
   <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
      <script type="text/javascript">
      google.charts.load('current', {'packages':['gauge', 'corechart']});
      google.charts.setOnLoadCallback(drawChart);

      // Update the timer every second.
      setInterval(drawTimer, 1000);

      function drawTimer() {
        // Timer
        var timer_data = google.visualization.arrayToDataTable([
          ['Label', 'Value'],
          ['timer', 0],
        ]);
        var timer_chart = new google.visualization.Gauge(document.getElementById('chart_timer'));
        timer_data.setValue(0, 1, ++timer);
        var timer_options = {
          min: 0, max: 180,
          width: 180, height: 180,
          redFrom: 160, redTo: 180,
          yellowFrom:30, yellowTo: 160,
          majorTicks: [0, 20, 40, 60, 80, 100, 120, 140, 160, 180], minorTicks: 10
        };
        timer_chart.draw(timer_data, timer_options);
      }

      function drawChart() {

        // Connection status
        if (typeof data_obj.connection !== 'undefined') {
          var connection_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['0', 0],
            ['1', 0],
            ['2', 0],
          ]);
          var connection_chart = new google.visualization.PieChart(document.getElementById('chart_connection'));
          if (data_obj.connection.status == 1) {
            connection_data.setValue(0, 1, 0);
            connection_data.setValue(1, 1, 1);
            connection_data.setValue(2, 1, 0);
          } else if (data_obj.connection.status == 2) {
            connection_data.setValue(0, 1, 0);
            connection_data.setValue(1, 1, 0);
            connection_data.setValue(2, 1, 1);
          } else {
            connection_data.setValue(0, 1, 1);
            connection_data.setValue(1, 1, 0);
            connection_data.setValue(2, 1, 0);
          }

          var connection_options = {
            min: 0, max: 2,
            width: 30, height: 30,
            colors:['blue', 'green' , 'red'],
            pieHole: 0.6,
            pieSliceText: 'none',
            enableInteractivity: false,
            legend: {position: 'none'},
            //majorTicks: ['0', '1', '2'], minorTicks: 0
          };
          connection_chart.draw(connection_data, connection_options);
        }

        // Message id
        if (typeof data_obj.garden !== 'undefined') {
          var msg_id_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['msg', 0],
          ]);
          var msg_id_chart = new google.visualization.Gauge(document.getElementById('chart_msg_id'));
          msg_id_data.setValue(0, 1, data_obj.garden.msg_id);
          var msg_id_options = {
            min: 0, max: 255,
            width: 140, height: 140,
            minorTicks: 10
          };
          msg_id_chart.draw(msg_id_data, msg_id_options);

          // Reset the timer
          timer = 0;
          drawTimer();
        }

        // Cpu temperature
        if (typeof data_obj.garden !== 'undefined') {
          var cpu_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['Cpu \u00B0C', 0],
          ]);
          var cpu_chart = new google.visualization.Gauge(document.getElementById('chart_cpu'));
          cpu_data.setValue(0, 1, data_obj.garden.cpu);
          var cpu_options = {
            min: -10, max: 100,
            width: 180, height: 180,
            redFrom: 80, redTo: 100,
            yellowFrom:70, yellowTo: 80,
            majorTicks: [-10,'0',10,20,30,40,50,60,70,80,90,100], minorTicks: 10
          };
          cpu_chart.draw(cpu_data, cpu_options);
        }

        // Temperature
        if (typeof data_obj.garden !== 'undefined') {
          var temperature_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['\u00B0C', 0],
          ]);
          var temperature_chart = new google.visualization.Gauge(document.getElementById('chart_temperature'));
          temperature_data.setValue(0, 1, data_obj.garden.temperature);
          var temperature_options = {
            min: -10, max: 50,
            width: 180, height: 180,
            redFrom: 40, redTo: 50,
            yellowFrom:30, yellowTo: 40,
            majorTicks: [-10,'0',10,20,30,40,50], minorTicks: 10
          };
          temperature_chart.draw(temperature_data, temperature_options);
        }

        // VCC
        if (typeof data_obj.garden !== 'undefined') {
          var vcc_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['VCC', 0],
          ]);
          var vcc_chart = new google.visualization.Gauge(document.getElementById('chart_vcc'));
          vcc_data.setValue(0, 1, data_obj.garden.vcc);
          var vcc_options = {
            min: 0, max: 6000,
            width: 200, height: 200,
            redFrom: 5500, redTo: 6000,
            yellowFrom:3000, yellowTo: 3300,
            greenFrom:0, greenTo: 3000,
            greenColor: "#de3800",
            majorTicks: [0,1,2,3,4,5,6], minorTicks: 10
          };
          vcc_chart.draw(vcc_data, vcc_options);
        }

        // Light
        if (typeof data_obj.garden !== 'undefined') {
          var light_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['light', 0],
          ]);
          var light_chart = new google.visualization.Gauge(document.getElementById('chart_light'));
          light_data.setValue(0, 1, data_obj.garden.light);
          var light_options = {
            min: 0, max: 5000,
            width: 200, height: 200,
            redFrom: 0, redTo: 1000,
            redColor: "#2e2d4f",
            yellowFrom:2500, yellowTo: 3500,
            greenFrom:3500, greenTo: 5000,
            greenColor: "#ffee38",
            majorTicks: [0,1,2,3,4,5], minorTicks: 10
          };
          light_chart.draw(light_data, light_options);
        }

        // Solar mV
        if (typeof data_obj.garden !== 'undefined') {
          var solar_mv_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['mV', 0],
          ]);
          var solar_mv_chart = new google.visualization.Gauge(document.getElementById('chart_solar_mv'));
          solar_mv_data.setValue(0, 1, data_obj.garden.solar_mv);
          var solar_mv_options = {
            min: 0, max: 8000,
            width: 200, height: 200,
            yellowFrom:5500, yellowTo: 6000,
            greenFrom:6000, greenTo: 8000,
            majorTicks: [0,1,2,3,4,5,6,7,8], minorTicks: 10
          };
          solar_mv_chart.draw(solar_mv_data, solar_mv_options);
        }

        // Charge mA
        if (typeof data_obj.garden !== 'undefined') {
          var ma_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['mA', 0],
          ]);
          var ma_chart = new google.visualization.Gauge(document.getElementById('chart_ma'));
          ma_data.setValue(0, 1, data_obj.garden.charge_ma);
          var ma_options = {
            min: 0, max:200,
            width: 200, height: 200,
            redFrom: 0, redTo: 5,
            yellowFrom:5, yellowTo: 20,
            majorTicks: [0,50,100,150,200],minorTicks: 5
          };
          ma_chart.draw(ma_data, ma_options);
        }

        // RSSI
        if (typeof data_obj.signal !== 'undefined') {
          var rssi_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['RSSI', 0],
          ]);
          var rssi_chart = new google.visualization.Gauge(document.getElementById('chart_rssi'));
          rssi_data.setValue(0, 1, data_obj.signal.rssi);
          var rssi_options = {
            min: -140, max: 0,
            width: 140, height: 140,
            majorTicks: [-140, -112, -84, -56, -28, 0],  minorTicks: 10
          };
          rssi_chart.draw(rssi_data, rssi_options);
        }

        // SNR
        if (typeof data_obj.signal !== 'undefined') {
          var snr_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['SNR', 0],
          ]);
          var snr_chart = new google.visualization.Gauge(document.getElementById('chart_snr'));
          snr_data.setValue(0, 1, data_obj.signal.snr);
          var snr_options = {
            min: 0, max: 100,
            width: 140, height: 140,
            majorTicks: [0,20,40,60,80,100], minorTicks: 10
          };
          snr_chart.draw(snr_data, snr_options);
        }

      }
    </script>

<script type="text/javascript">
  var connection = new WebSocket('ws://192.168.0.34:81/', ['arduino']);
  connection.onopen = function () {
      connection.send('Connect ' + new Date());
      data_obj.connection.status = 1;
  };
  connection.onerror = function (error) {
      console.log('WebSocket Error ', error);
      data_obj.connection.status = 2;
  };
  connection.onclose = function () {
      console.log('WebSocket closed');
      data_obj.connection.status = 0;
  };
  connection.onmessage = function (e) {
      console.log('Server: ', e.data);
      var json_obj = JSON.parse(e.data);
      console.log(json_obj);

      if (typeof json_obj.signal !== 'undefined') {
          data_obj.signal = json_obj.signal;
      }
      if (typeof json_obj.garden !== 'undefined') {
          data_obj.garden = json_obj.garden;
      }
      if (typeof json_obj.status !== 'undefined') {
          data_obj.status = json_obj.status;
      }

      drawChart();
  };
</script>

</head>
<body>

  <div>
    <span id="chart_connection" style="float:left;"></span>
  </div>
<div style="margin: auto; width: 550;">
    <span id="chart_timer" style="float:left;"></span>
    <span id="chart_cpu" style="float:left;"></span>
    <span id="chart_temperature" style="float:left;"></span>
</div>
<div style="margin: auto; width: 800; clear:both;">
    <span id="chart_vcc" style="float:left;"></span>
    <span id="chart_light" style="float:left;"></span>
    <span id="chart_solar_mv" style="float:left;"></span>
    <span id="chart_ma" style="float:left;"></span>
</div>
<div style="margin: auto; width: 420;">
    <span id="chart_msg_id" style="float:left;"></span>
    <span id="chart_rssi" style="float:left;"></span>
    <span id="chart_snr" style="float:left;"></span>
</div>

</body>
</html>



)=====";
