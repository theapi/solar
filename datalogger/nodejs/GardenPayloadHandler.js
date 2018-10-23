module.exports = class GardenPayloadHandler {

  constructor(socket) {
    socket.on('message', (buf, rinfo) => {
      if (buf.readUInt8(0) === 9 && buf.readUInt8(1) === 50) {
        payload = this.unserialize(buf);
        console.log(payload);
      }
    });
  }

  unserialize(buf) {
    let payload = {'msg_type': buf.readUInt8(1)};
    payload.msg_id = buf.readUInt8(2);
    payload.vcc = buf.readInt16BE(3);

    return payload;
  }

}
