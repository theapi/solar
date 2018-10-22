const dgram = require('dgram');
const socket = dgram.createSocket('udp4');
const multi_addr = '239.0.0.57';

socket.bind(12345, multi_addr, () => {
    socket.addMembership(multi_addr);
});

socket.on('message', (buf, rinfo) => {
    const msg_type = buf.readUInt8(1);
    let payload = {'msg_type': msg_type};
    switch (msg_type) {
      case 50:
        payload.msg_id = buf.readUInt8(2);
        payload.vcc = buf.readInt16BE(3);
        console.log(payload);
      break;

      case 55:
        payload.device_id = buf.readUInt8(2),
        payload.msg_id = buf.readUInt8(3);
        payload.vcc = buf.readInt16BE(5);
        console.log(payload);
      break;

    }
});
