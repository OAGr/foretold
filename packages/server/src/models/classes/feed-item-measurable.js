const assert = require('assert');
const _ = require('lodash');
const Mustache = require('mustache');

const { FeedItemCommon } = require('./feed-item-common');
const { FEED_ITEM_BODY } = require('../enums/feed-item-body');

class FeedItemMeasurable extends FeedItemCommon {
  /**
   * @public
   * @param {object} options
   * @param {string} options.item
   * @param {string} options.description
   * @param {string} options.measurableId
   */
  constructor(options) {
    super(options);
    assert(_.isString(options.measurableId), 'MeasurableId should be a string');
    this.measurableId = options.measurableId;
  }

  /**
   * @public
   * @return {string}
   */
  getName() {
    return FEED_ITEM_BODY.measurable;
  }

  /**
   * @public
   * @return {string}
   */
  getMeasurableId() {
    return this.measurableId;
  }

  /**
   * @public
   * @param {object} replacements
   * @return {FeedItem}
   */
  mutate(replacements) {
    const item = Mustache.render(this.item, replacements);
    const description = Mustache.render(this.description, replacements);
    const measurableId = Mustache.render(this.measurableId, replacements);

    return new FeedItemMeasurable({
      item,
      description,
      measurableId,
    });
  }
}

module.exports = {
  FeedItemMeasurable,
};
