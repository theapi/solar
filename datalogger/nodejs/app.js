const SolarPayloadHandler = require('./SolarPayloadHandler.js');
const GardenPayloadHandler = require('./GardenPayloadHandler.js');
const client = require('./elasticsearch/connection.js');
const dgram = require('dgram');
const socket = dgram.createSocket('udp4');
const multi_addr = '239.0.0.57';

socket.bind(12345, multi_addr, () => {
    socket.addMembership(multi_addr);
});

let garden = new GardenPayloadHandler(socket);
let solar = new SolarPayloadHandler(socket, client);

// Just to show the serializer working :)
//console.log(solar.unserialize(Buffer.from([0x09, 0x37, 0x02, 0x03, 0x04, 0x05, 0x06])));
