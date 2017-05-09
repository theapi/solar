
const char index_html[] PROGMEM = R"=====(

<html>
<head>
<script type="text/javascript">
var data_obj;
</script>


    <script type="text/javascript">
      var data_json = '{"msg_type":0,"msg_id":0,"vcc":0,"solar_mv":0,"charge_ma":0,"light":0,"soil":0,"temperature":0}';
      data_obj = JSON.parse(data_json);
    </script>
   <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
      <script type="text/javascript">
      google.charts.load('current', {'packages':['gauge']});
      google.charts.setOnLoadCallback(drawChart);

      function drawChart() {
        // Message id
        if (data_obj.garden.msg_id) {
          var msg_id_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['msg_id', 0],
          ]);
          var msg_id_chart = new google.visualization.Gauge(document.getElementById('chart_msg_id'));
          msg_id_data.setValue(0, 1, data_obj.garden.msg_id);
          var msg_id_options = {
            min: 0, max: 254,
            width: 150, height: 150,
            majorTicks: [0,64,127,192,254], minorTicks: 10
          };
          msg_id_chart.draw(msg_id_data, msg_id_options);
        }

        // Soil
        if (data_obj.garden.soil) {
          var soil_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['soil', 0],
          ]);
          var soil_chart = new google.visualization.Gauge(document.getElementById('chart_soil'));
          soil_data.setValue(0, 1, data_obj.garden.soil);
          var soil_options = {
            min: 0, max: 1023,
            width: 150, height: 150,
            minorTicks: 10
          };
          soil_chart.draw(soil_data, soil_options);
        }

        // Temperature
        if (data_obj.garden.temperature) {
          var temperature_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['°C', 0],
          ]);
          var temperature_chart = new google.visualization.Gauge(document.getElementById('chart_temperature'));
          temperature_data.setValue(0, 1, data_obj.garden.temperature);
          var temperature_options = {
            min: 0, max: 50,
            width: 150, height: 150,
            redFrom: 40, redTo: 50,
            yellowFrom:30, yellowTo: 40,
            minorTicks: 10
          };
          temperature_chart.draw(temperature_data, temperature_options);
        }

        // VCC
        if (data_obj.garden.vcc) {
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
        if (data_obj.garden.light) {
          var light_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['Light', 0],
          ]);
          var light_chart = new google.visualization.Gauge(document.getElementById('chart_light'));
          light_data.setValue(0, 1, data_obj.garden.light);
          var light_options = {
            min: 0, max: 2000,
            width: 200, height: 200,
            redFrom: 0, redTo: 10,
            yellowFrom:10, yellowTo: 100,
            minorTicks: 10
          };
          light_chart.draw(light_data, light_options);
        }

        // Solar mV
        if (data_obj.garden.solar_mv) {
          var solar_mv_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['solar mV', 0],
          ]);
          var solar_mv_chart = new google.visualization.Gauge(document.getElementById('chart_solar_mv'));
          solar_mv_data.setValue(0, 1, data_obj.garden.solar_mv);
          var solar_mv_options = {
            min: 0, max: 10000,
            width: 200, height: 200,
            yellowFrom:5500, yellowTo: 6000,
            greenFrom:6000, greenTo: 10000,
            majorTicks: [0,1,2,3,4,5,6,7,8,9,10], minorTicks: 10
          };
          solar_mv_chart.draw(solar_mv_data, solar_mv_options);
        }

        // Charge mA
        if (data_obj.garden.charge_ma) {
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
        if (data_obj.signal.rssi) {
          var rssi_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['RSSI', 0],
          ]);
          var rssi_chart = new google.visualization.Gauge(document.getElementById('chart_rssi'));
          rssi_data.setValue(0, 1, data_obj.signal.rssi);
          var rssi_options = {
            min: -150, max: 0,
            width: 150, height: 150,
            minorTicks: 10
          };
          rssi_chart.draw(rssi_data, rssi_options);
        }

        // SNR
        if (data_obj.signal.snr) {
          var snr_data = google.visualization.arrayToDataTable([
            ['Label', 'Value'],
            ['SNR', 0],
          ]);
          var snr_chart = new google.visualization.Gauge(document.getElementById('chart_snr'));
          snr_data.setValue(0, 1, data_obj.signal.snr);
          var snr_options = {
            min: 0, max: 100,
            width: 150, height: 150,
            minorTicks: 10
          };
          snr_chart.draw(snr_data, snr_options);
        }

      }
    </script>
    
<script type="text/javascript">
  var connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
  connection.onopen = function () {
      connection.send('Connect ' + new Date());
  };
  connection.onerror = function (error) {
      console.log('WebSocket Error ', error);
  };
  connection.onmessage = function (e) {
      //console.log('Server: ', e.data);
      data_obj = JSON.parse(e.data);
      console.log(data_obj);
  };
</script>

</head>
<body>

<div style="margin: auto; width: 450;">
    <span id="chart_msg_id" style="float:left;"></span>
    <span id="chart_soil" style="float:left;"></span>
    <span id="chart_temperature" ></span>
</div>
<div style="margin: auto; width: 800;">
    <span id="chart_vcc" style="float:left;"></span>
    <span id="chart_light" style="float:left;"></span>
    <span id="chart_solar_mv" style="float:left;"></span>
    <span id="chart_ma" style="float:left;"></span>
</div>
<div style="margin: auto; width: 300;">
    <span id="chart_rssi" style="float:left;"></span>
    <span id="chart_snr" style="float:left;"></span>
</div>

</body>
</html>

)=====";

