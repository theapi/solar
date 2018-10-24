
const PORT = 12345;
const MULTICAST_ADDR = "239.0.0.57";

const SolarPayloadHandler = require('./SolarPayloadHandler.js');
const GardenPayloadHandler = require('./GardenPayloadHandler.js');
const client = require('./elasticsearch/connection.js');
const dgram = require('dgram');
const socket = dgram.createSocket({ type: "udp4", reuseAddr: true });

socket.bind(PORT);

socket.on("listening", function() {
  socket.addMembership(MULTICAST_ADDR);
  const address = socket.address();
  console.log(
    `UDP socket listening on ${address.address}:${address.port} pid: ${
      process.pid
    }`
  );
});

let garden = new GardenPayloadHandler(socket);
let solar = new SolarPayloadHandler(socket, client);

// Just to show the serializer working :)
//console.log(solar.unserialize(Buffer.from([0x09, 0x37, 0x02, 0x03, 0x04, 0x05, 0x06])));
