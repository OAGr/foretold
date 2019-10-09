const _ = require('lodash');
const assert = require('assert');

const { DataBase } = require('./data-base');

const { ChannelMembershipsData } = require('./channel-memberships-data');
const { UsersData } = require('./users-data');

const { InvitationModel } = require('../models-abstract');

const { Data, Params, Filter, Query } = require('./classes');

/**
 * @implements {Layers.DataSourceLayer.DataSource}
 * @property {InvitationModel} InvitationModel
 */
class InvitationsData extends DataBase {
  constructor() {
    super();
    this.model = new InvitationModel();
    this.channelMemberships = new ChannelMembershipsData();
    this.users = new UsersData();
  }

  /**
   * @public
   * @todo: Use transactions here.
   * @todo: Always send invitations! Never mess the invitations flow
   * @todo: with direct member adding into channel.
   * @param {object} input
   * @param {string} input.email
   * @param {string} input.channelId
   * @param {string} input.inviterAgentId
   * @return {Promise<Object>}
   */
  async invite(input) {
    try {
      const channelId = _.get(input, 'channelId');
      const email = _.get(input, 'email');
      const inviterAgentId = _.get(input, 'inviterAgentId');

      assert(_.isString(email), 'Email should be a string');
      assert(_.isString(channelId), 'Channel Id should be a string');
      assert(_.isString(inviterAgentId), 'Inviter Agent Id should be a string');

      const added = await this.addMemberships(
        email,
        channelId,
        inviterAgentId,
      );

      if (added) {
        return null;
      }

      return this.upsert(email, channelId, inviterAgentId);
    } catch (e) {
      console.error('Invitation Err', e);
    }
    return null;
  }

  /**
   * @param {string} email
   * @param {Models.ObjectID} channelId
   * @param {Models.ObjectID} inviterAgentId
   * @returns {Promise<*>}
   */
  async upsert(email, channelId, inviterAgentId) {
    const params = new Params({ email, channelId });
    const query = new Query();
    const data = new Data({ email, inviterAgentId, channelId });
    return await this.upsertOne(params, query, data);
  }

  /**
   * @param {string} email
   * @param {Models.ObjectID} channelId
   * @param {Models.ObjectID} inviterAgentId
   * @returns {Promise<boolean>}
   */
  async addMemberships(email, channelId, inviterAgentId) {
    const user = await this.getVerified(email);

    if (!!user) {
      const agentId = _.get(user, 'agentId', null);
      const data = new Data({ channelId, agentId });
      const memberships = await this.channelMemberships.createOne(data);

      return !!memberships;
    }

    return false;
  }
}

module.exports = {
  InvitationsData,
};
