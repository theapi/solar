const fs = require('fs');
const fsPromises = fs.promises;

module.exports = class SolarPayloadHandler {

  constructor(socket, client) {
    this.msg_id = 999;
    this.client = client;

    socket.on('message', (input, rinfo) => {
      if (input.readUInt8(0) === 9) {
        // Remove the tab from the UDP message.
        const buf = input.slice(1);
        if (buf.readUInt8(0) === 55) {
          if (this.msg_id !== buf.readUInt8(2)) {
            let payload = this.unserialize(buf);
            payload.timestamp = new Date().getTime();
            console.log(payload);

            // Keep a record in a file.
            let now = new Date();
            let month = now.getUTCMonth() + 1;
            let dir = __dirname + '/log/solar/' + now.getUTCFullYear();
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

}
