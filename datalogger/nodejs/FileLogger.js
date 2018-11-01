const fs = require('fs');
const fsPromises = fs.promises;

// Keep a record of the payload in a file.
module.exports = class FileLogger {

  constructor(name) {
    this.name = name;
  }

  log(payload) {
    let now = new Date();
    let month = now.getUTCMonth() + 1;
    let dir = __dirname + '/log/' + this.name + '/' + now.getUTCFullYear();
    // {recursive: true} requires > 10.12.0
    fsPromises.mkdir(dir, {recursive: true})
    .then(() => {
      return fsPromises.appendFile(
        dir + '/' + month + '.json',
        JSON.stringify(payload) + "\n"
      );
    })
    .catch(console.error);
  }

}
