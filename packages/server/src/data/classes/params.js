const utils = require('../../lib/utils');

/**
 * See "filter.js" comments.
 */
class Params {
  /**
   * @param {Layers.DataSourceLayer.params} [params]
   */
  constructor(params = {}) {
    const list = [
      'id',
      'agentId',
      'name',
      'auth0Id',
    ];
    utils.extend(this.constructor.name, params, list, this);
    utils.diff(this.constructor.name, params, list);
  }
}

module.exports = {
  Params,
};
