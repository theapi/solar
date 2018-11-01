
var client = require('./connection.js');
const config = require('config');
const elasticsearch = config.get('elasticsearch');

client.indices.create({
  "index": elasticsearch.garden.index,
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
