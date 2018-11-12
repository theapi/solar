
var client = require('./connection.js');
const config = require('config');
const elasticsearch = config.get('elasticsearch');

client.indices.create({
  "index": elasticsearch.solar.index,
  "body": {
    "mappings": {
      "_doc": {
        "properties": {
          "cpu_temperature": {
            "type": "float"
          },
          "device_id": {
            "type": "short"
          },
          "flag_battery": {
            "type": "boolean"
          },
          "flag_switch": {
            "type": "boolean"
          },
          "flag_temperature": {
            "type": "boolean"
          },
          "flag_water": {
            "type": "boolean"
          },
          "flag_switch": {
            "type": "boolean"
          },
          "flags": {
            "type": "short"
          },
          "frq_error": {
            "type": "short"
          },
          "light": {
            "type": "integer"
          },
          "ma": {
            "type": "short"
          },
          "msg_id": {
            "type": "short"
          },
          "msg_type": {
            "type": "short"
          },
          "mv": {
            "type": "short"
          },
          "rssi": {
            "type": "short"
          },
          "snr": {
            "type": "short"
          },
          "temperature": {
            "type": "float"
          },
          "timestamp": {
            "type": "date"
          },
          "vcc": {
            "type": "short"
          }
        }
      }
    }
  }
})
.then((msg) => {
  console.log(msg);
})
.catch ((err) => {
  console.error('failed: ' + err);
})
;
