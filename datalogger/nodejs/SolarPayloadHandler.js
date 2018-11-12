const fs = require('fs');
const fsPromises = fs.promises;

module.exports = class SolarPayloadHandler {

  constructor(socket, loggers) {
    this.msg_id = 999;
    this.loggers = loggers;

    socket.on('message', (input, rinfo) => {
      if (input.readUInt8(0) === 9) {
        // Remove the tab from the UDP message.
        const buf = input.slice(1);
        if (buf.readUInt8(0) === 55) {
          if (this.msg_id !== buf.readUInt8(2)) {
            let payload = this.unserialize(buf);
            payload.timestamp = new Date().getTime();

            // Extract the flags to fields of their own.
            // bit 0 = pump_switch
            // bit 1 = water
            // bit 2 = temperature
            // bit 3 = battery
            payload.flag_switch = this.bitRead(payload.flags, 0);
            payload.flag_water = this.bitRead(payload.flags, 1);
            payload.flag_temerature = this.bitRead(payload.flags, 2);
            payload.flag_battery = this.bitRead(payload.flags, 3);

            // Log to all the loggers.
            for (let i = 0, len = this.loggers.length; i < len; i++) {
              this.loggers[i].log(payload);
            }

          }
        }
      }
    });
  }

  unserialize(buf) {
    let payload = {};
    payload.msg_type = buf.readUInt8(0);
    payload.device_id = buf.readUInt8(1);
    payload.msg_id = buf.readUInt8(2);
    this.msg_id = payload.msg_id;
    payload.flags = buf.readUInt8(3);
    payload.vcc = buf.readInt16BE(4);
    payload.mv = buf.readInt16BE(6);
    payload.ma = buf.readInt16BE(8);
    payload.light = buf.readInt16BE(10);
    payload.cpu_temperature = buf.readInt16BE(11);
    payload.temperature = buf.readInt16BE(14) / 10;
    payload.rssi = buf.readInt16BE(16);
    payload.snr = buf.readInt16BE(18);
    payload.freg_error = buf.readInt16BE(20);

    return payload;
  }


  bitRead(value, bit) {
    return ((value >> bit) & 0x01);
  }

}
