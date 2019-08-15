const assert = require('assert');
const _ = require('lodash');

class Restrictions {
  /**
   * @param {Layers.AbstractModelsLayer.restrictions} [options]
   */
  constructor(options) {
    const list = {
      'isAdmin': v => _.isBoolean(v) || _.isUndefined(v),
      'agentId': v => _.isString(v),
      'measuredByAgentId': v => _.isString(v) || _.isUndefined(v),
      'userId': v => _.isString(v),
      'channelId': v => _.isBoolean(v),
      'measurableId': v => _.isBoolean(v),
      'channelIdAsId': v => _.isBoolean(v),
      'channelMemberId': v => _.isString(v),
    };

    _.each(list, (_test, name) => {
      if (_.has(options, name)) {
        this[name] = _.get(options, name);
      }
    });

    _.each(list, (test, name) => {
      if (_.has(this, name)) {
        assert(
          test(this[name]),
          `Restrictions."${name}" assert failed, ` +
          `type is "${typeof _.get(this, name)}".`);
      }
    });
  }
}

module.exports = {
  Restrictions,
};