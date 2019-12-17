const _ = require('lodash');

const { FeedItemsData } = require('../../data');

const { Pagination } = require('../../data/classes');
const { Options } = require('../../data/classes');
const { Filter } = require('../../data/classes');
const { structures } = require('../../data/classes');

const { HOME_CHANNEL_ID } = require('../../../config/well-known');

/**
 *
 * Do not make wrong conclusions about "Filters"
 * and "Restrictions". "Filters" are used to filter
 * objects. However, "Restrictions" are used to
 * restrict an access to objects.
 *
 * @param {*} _root
 * @param {object} args
 * @param {Models.ChannelID} args.channelId
 * @param {Models.AgentID} args.agentId
 * @param {string} args.after
 * @param {string} args.before
 * @param {number} args.last
 * @param {number} args.first
 * @param {Schema.Context} context
 * @param {object} _info
 * @returns {Promise<*>}
 */
async function all(_root, args, context, _info) {
  /** @type {string} */
  const channelId = _.get(args, 'channelId', null);
  /** @type {string} */
  const agentId = _.get(args, 'agentId', null);
  /** @type {string} */
  const currentAgentId = _.get(context, 'agent.id', null);

  const withinJoinedChannels = channelId === HOME_CHANNEL_ID && !!currentAgentId
    ? structures.withinJoinedChannelsByChannelId(currentAgentId) : null;

  const filter = new Filter({ agentId, channelId, withinJoinedChannels });
  const pagination = new Pagination(args);
  const options = new Options({ currentAgentId });

  return new FeedItemsData().getConnection(
    filter,
    pagination,
    options,
  );
}

module.exports = {
  all,
};
