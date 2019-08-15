const events = require('../async/events');
const emitter = require('../async/emitter');

const { AGENT_TYPE } = require('./enums/agent-type');
const { MEASUREMENT_COMPETITOR_TYPE } = require('./enums/measurement-competitor-type');
const { MEASURABLE_STATE } = require('./enums/measurable-state');

/**
 * Try to keep all "async" hooks in one place.
 * For what do you want to ask?
 *
 * Server side application architecture is being building with separated,
 * decomposed layers (Model Definitions, Models Abstractions, Data Layer.
 * Middleware Layer, Authorizers Layer, Async Flows Layer, GrqphQL
 * Layer ans its resolvers).
 *
 * Models are just "data type definitions" which are similar with "grapqhl
 * types". "Sequelize" is a just lib which plays role of "DB Driver".
 *
 * So this is why we keep "async" part of the application here, and
 * "associations" definitions too. It does not matter where the "associations"
 * are defined and placed (either DB or JS files). The main purpose
 * is to give to developers to read this base definitions without
 * duplications and keep in a mind only a small part of this.
 *
 */

function addHooks(db) {
  db.Measurable.addHook('afterUpdate', (instance) => {
    try {
      emitter.emit(events.MEASURABLE_CHANGED, instance);
    } catch (e) {
      console.log('Hook MEASURABLE_CHANGED', e);
    }
  });
  db.Measurement.addHook('afterCreate', (instance) => {
    try {
      emitter.emit(events.NEW_MEASUREMENT, instance);
    } catch (e) {
      console.log('Hook NEW_MEASUREMENT', e);
    }
  });
  db.Measurable.addHook('afterCreate', (instance) => {
    try {
      emitter.emit(events.NEW_MEASURABLE, instance);
    } catch (e) {
      console.log('Hook NEW_MEASURABLE', e);
    }
  });
  db.ChannelMemberships.addHook('afterCreate', (instance) => {
    try {
      emitter.emit(events.NEW_MEMBERSHIP, instance);
    } catch (e) {
      console.log('Hook', e);
    }
  });

  db.Bot.addHook('beforeCreate', async (event) => {
    try {
      const agent = await db.sequelize.models.Agent.create({
        type: AGENT_TYPE.BOT,
      });
      event.agentId = agent.dataValues.id;
    } catch (e) {
      console.log('Hook', e);
    }
  });

  db.User.addHook('beforeCreate', async (event) => {
    try {
      const agent = await db.sequelize.models.Agent.create({
        type: AGENT_TYPE.USER,
      });
      event.agentId = agent.dataValues.id
    } catch (e) {
      console.log('Hook', e);
    }
  });

  db.Series.addHook('afterCreate', async (series) => {
    try {
      await series.createMeasurables();
    } catch (e) {
      console.log('Hook', e);
    }
  });

  db.Measurable.addHook('beforeUpdate', async (instance) => {
    try {
      await instance.watchExpectedResolutionDate(instance);
    } catch (e) {
      console.log('Hook', e);
    }
  });

  db.Measurement.addHook('beforeValidate', async (instance) => {
    try {
      if (instance.dataValues.relevantAt == null) {
        instance.relevantAt = Date.now();
      }
    } catch (e) {
      console.log('Hook', e);
    }
  });

  db.Measurement.addHook('afterCreate', async (instance) => {
    try {
      const competitorType = instance.dataValues.competitorType;
      const isJudgable = [
        MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE,
        MEASUREMENT_COMPETITOR_TYPE.UNRESOLVED,
      ].includes(competitorType);

      if (isJudgable) {
        const measurable = await instance.getMeasurable();
        await measurable.judged();
      }
    } catch (e) {
      console.log('Hook', e);
    }
  });
}

module.exports = {
  addHooks,
};