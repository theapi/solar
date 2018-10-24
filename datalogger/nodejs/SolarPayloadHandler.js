const fs = require('fs');
const fsPromises = fs.promises;

module.exports = class SolarPayloadHandler {

  constructor(socket, client) {
    this.msg_id = 999;
    this.client = client;

    socket.on('message', (buf, rinfo) => {
      if (buf.readUInt8(0) === 9 && buf.readUInt8(1) === 55) {
        if (this.msg_id !== buf.readUInt8(3)) {
          let payload = this.unserialize(buf);
          payload.timestamp = new Date().getTime();
          console.log(payload);

          // Keep a record in a file.
          let now = new Date();
          let month = now.getUTCMonth() + 1;
          let dir = __dirname + '/log/' + now.getUTCFullYear();
          // {recursive: true} requires > 10.12.0
          fsPromises.mkdir(dir, {recursive: true})
          .then(() => {
            return fsPromises.appendFile(
              dir + '/' + month + '.json',
              JSON.stringify(payload) + "\n"
            );
          })
          .catch(console.error);

          // Index into Elasticsearch.
          this.client.index({
            index: 'solar_0', //@todo not hard coded index name
            type: '_doc',
            body: payload
          })
          .catch ((err) => {
            console.error('failed to index: ' + err);
          })
          ;

        }
      }
    });
  }

  unserialize(buf) {
    let payload = {};
    payload.msg_type = buf.readUInt8(1);
    payload.device_id = buf.readUInt8(2);
    payload.msg_id = buf.readUInt8(3);
    this.msg_id = payload.msg_id;
    payload.flags = buf.readUInt8(4);
    payload.vcc = buf.readInt16BE(5);
    payload.mv = buf.readInt16BE(7);
    payload.ma = buf.readInt16BE(9);
    payload.light = buf.readInt16BE(11);
    payload.cpu_temperature = buf.readInt16BE(13);
    payload.temperature = buf.readInt16BE(15) / 10;
    payload.rssi = buf.readInt16BE(17);
    payload.snr = buf.readInt16BE(19);
    payload.freg_error = buf.readInt16BE(21);

    return payload;
  }

}
