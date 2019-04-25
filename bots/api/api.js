const graphqlClient = require('graphql-client');

const queries = require('./queries');
const config = require('../config');

class API {
  /**
   * @param {string} token
   */
  constructor(token) {
    this.token = token;
    this.graphqlClient = graphqlClient;
    this.queries = queries;
    this.config = config;

    this.client = this.getClient();
  }

  /**
   * @protected
   * @return {{query}}
   */
  getClient() {
    return this.graphqlClient({
      url: this.config.API_URL,
      headers: {
        Authorization: 'Bearer ' + this.token,
      }
    });
  }

  /**
   * @protected
   * @param query
   * @param {object} [variables]
   * @return {*}
   */
  query(query, variables = {}) {
    return this.client.query(query, variables, (req, res) => {
      if (res.status === 401) {
        throw new Error('Not authorized');
      }
    }).then((body) => {
      console.log(body);
      return body;
    }).catch((err) => {
      console.log(err.message);
      return Promise.reject(err);
    });
  }

  /**
   * @public
   * @return {*}
   */
  measurables() {
    return this.query(this.queries.measurables);
  }

  /**
   * @public
   * @return {*}
   */
  measurementCreate() {
    return this.query(this.queries.measurementCreate, {
      input: {
        value: {
          floatPoint: 10.111
        },
        competitorType: "COMPETITIVE",
        measurableId: "4dda3bb8-e036-4820-9975-ae7d330f9cb0",
      },
    });
  }
}

module.exports = {
  API,
};
