const { AgentsData } = require('../../data');
const { MeasurablesData } = require('../../data');
const { ChannelMembershipsData } = require('../../data');

const { Data } = require('../../data/classes');
const { Params } = require('../../data/classes');
const { Query } = require('../../data/classes');

const { MEASURABLE_VALUE_TYPE } = require('../../enums');
const { CHANNEL_MEMBERSHIP_ROLES } = require('../../enums');
const { AGENT_TYPE } = require('../../enums');
const { MEASURABLE_STATE } = require('../../enums');

class Creators {
  constructor() {
    this.agents = new AgentsData();
    this.measurables = new MeasurablesData();
    this.channelMemberships = new ChannelMembershipsData();
  }

  /**
   * @param {Defs.Series} series
   * @returns {Promise<void>}
   */
  async createMeasurables(series) {
    for (const subject of series.subjects) {
      for (const property of series.properties) {
        for (const date of series.dates) {
          console.log(
            'Making Measurable for Series:',
            subject,
            property,
            date,
          );
          await this.measurables.createOne(new Data({
            name: '',
            labelSubject: subject,
            labelProperty: property,
            labelOnDate: date,
            expectedResolutionDate: date,
            seriesId: series.id,
            creatorId: series.creatorId,
            channelId: series.channelId,
            valueType: MEASURABLE_VALUE_TYPE.FLOAT,
          }));
        }
      }
    }
  }

  /**
   * @param {Defs.Channel} channel
   * @returns {Promise<boolean>}
   */
  async createChannelMembership(channel) {
    await this.channelMemberships.upsertOne(new Params({
      channelId: channel.id,
      agentId: channel.creatorId,
    }), new Query({}), new Data({
      role: CHANNEL_MEMBERSHIP_ROLES.ADMIN,
      channelId: channel.id,
      agentId: channel.creatorId,
    }));
    return true;
  }

  /**
   * @param {Defs.Bot} instance
   * @returns {Promise<boolean>}
   */
  async createBotAgent(instance) {
    const data = new Data({ type: AGENT_TYPE.BOT });
    const agent = await this.agents.createOne(data);
    instance.agentId = agent.id;
    return true;
  }

  /**
   * @param {Defs.User} instance
   * @returns {Promise<boolean>}
   */
  async createUserAgent(instance) {
    const data = new Data({ type: AGENT_TYPE.USER });
    const agent = await this.agents.createOne(data);
    instance.agentId = agent.id;
    return true;
  }

  /**
   * @param {Defs.Measurable} instance
   * @returns {Promise<boolean>}
   */
  async checkMeasurableState(instance) {
    if (instance.changed('expectedResolutionDate')) {
      if (instance.expectedResolutionDate >= new Date()) {
        await instance.set('state', MEASURABLE_STATE.OPEN);
      }
    }
    return true;
  }

  /**
   * @param {Defs.Measurement} instance
   * @returns {Promise<boolean>}
   */
  async checkMeasurement(instance) {
    if (instance.relevantAt == null) {
      instance.relevantAt = Date.now();
    }
    return true;
  }
}

module.exports = {
  Creators,
};
