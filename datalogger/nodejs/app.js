

const config = require('config');
const dgram = require('dgram');

const FileLogger = require('./FileLogger.js');
const ElasticsearchLogger = require('./ElasticsearchLogger.js');
const SolarPayloadHandler = require('./SolarPayloadHandler.js');
const GardenPayloadHandler = require('./GardenPayloadHandler.js');
const esClient = require('./elasticsearch/connection.js');

// Connect to the UDP service.
const udpSocket = dgram.createSocket({ type: "udp4", reuseAddr: true });
const udp = config.get('udp');
udpSocket.bind(udp.port);
udpSocket.on("listening", function() {
  // Once connected, join the multicast group.
  udpSocket.addMembership(udp.address);
  const address = udpSocket.address();
  console.log(
    `UDP socket listening on ${address.address}:${address.port} pid: ${
      process.pid
    }`
  );
});

// Start the payload handlers.
const esConfig = config.get('elasticsearch');
new GardenPayloadHandler(
  udpSocket,
  [ // The loggers for the handler to use.
    console,
    new FileLogger('garden'),
    new ElasticsearchLogger(esClient, esConfig.garden.index)
  ]
);

new SolarPayloadHandler(
  udpSocket,
  [ // The loggers for the handler to use.
    console,
    new FileLogger('solar'),
    new ElasticsearchLogger(esClient, esConfig.solar.index)
  ]
);
