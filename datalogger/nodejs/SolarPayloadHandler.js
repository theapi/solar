module.exports = class SolarPayloadHandler {

  constructor(socket) {
    socket.on('message', (buf, rinfo) => {
      if (buf.readUInt8(0) === 9 && buf.readUInt8(1) === 55) {
        payload = this.unserialize(buf);
        console.log(payload);
      }
    });
  }

  unserialize(buf) {
    let payload = {'msg_type': buf.readUInt8(1)};
    payload.device_id = buf.readUInt8(2),
    payload.msg_id = buf.readUInt8(3);
    payload.vcc = buf.readInt16BE(5);

    return payload;
  }

}
