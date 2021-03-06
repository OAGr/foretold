const { DataBase } = require('./data-base');
const { FeedItemModel } = require('./models');

/**
 * @implements {Layers.DataSource.DataGeneric}
 * @property {FeedItemModel} model
 */
class FeedItemsData extends DataBase {
  constructor() {
    super();
    this.model = new FeedItemModel();
  }

  /**
   * @protected
   * @param {Layers.DataSource.DataOptions} [options]
   * @return {Layers.Models.ModelRestrictions}
   */
  _getDefaultRestrictions(options = {}) {
    return {
      ...super._getDefaultRestrictionsForIncludedIntoChannel(options),
    };
  }
}

module.exports = {
  FeedItemsData,
};
