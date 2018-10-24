
var elasticsearch = require('elasticsearch');

var client = new elasticsearch.Client( {
  hosts: [
    '192.168.0.2:9200',
  ]
});

module.exports = client;