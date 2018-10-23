module.exports = class GardenPayloadHandler {

  constructor(socket) {
    this.msg_id = 999;

    socket.on('message', (buf, rinfo) => {
      if (buf.readUInt8(0) === 9 && buf.readUInt8(1) === 50) {
        if (this.msg_id !== buf.readUInt8(2)) {
          let payload = this.unserialize(buf);
          console.log(payload);
        }
      }
    });
  }

  unserialize(buf) {
    let payload = {};
    payload.msg_type = buf.readUInt8(1);
    payload.device_id = 1;
    payload.msg_id = buf.readUInt8(2);
    this.msg_id = payload.msg_id;
    payload.vcc = buf.readInt16BE(3);
    payload.mv = buf.readInt16BE(5);
    payload.ma = buf.readInt16BE(7);
    payload.light = buf.readInt16BE(9);
    payload.cpu_temperature = buf.readInt16BE(11);
    payload.temperature = buf.readInt16BE(13);
    payload.rssi = buf.readInt16BE(15);
    payload.snr = buf.readInt16BE(17);
    payload.freg_error = buf.readInt16BE(19);

    return payload;
  }

}
