
// Index the payload to Elasticsearch.
module.exports = class ElasticsearchLogger {

  constructor(client, index) {
    this.client = client;
    this.index = index;
  }

  log(payload) {
    this.client.index({
      index: this.index,
      type: '_doc',
      body: payload
    })
    .catch ((err) => {
      console.error('failed to index: ' + err);
    })
    ;
  }

}
