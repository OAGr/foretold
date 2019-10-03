const _ = require('lodash');

const data = require('../../data');
const {
  MEASUREMENT_COMPETITOR_TYPE,
} = require('../../enums/measurement-competitor-type');

class MeasurablesStateMachine {
  constructor() {
    this.data = data;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async toJudgementPending() {
    const measurables = await this.data.measurables.needsToBePending();

    _.each(measurables, (measurable) => {
      measurable.judgementPending();
    });

    return true;
  }

  /**
   * @public
   * @return {Promise<boolean>}
   */
  async toResolving() {
    const measurables = await this.data.measurables.needsResolutionResponse();

    _.each(measurables, (measurable) => {
      this.data.measurables.processResolution(measurable);
    });

    return true;
  }

  /**
   * @param {Models.Measurement} measurement
   * @returns {Promise<boolean>}
   */
  async measurableStateTransition(measurement) {
    const { competitorType } = measurement;
    const measurable = await measurement.getMeasurable();

    if (competitorType === MEASUREMENT_COMPETITOR_TYPE.OBJECTIVE) {
      await measurable.judged();
      return true;
    }

    if (competitorType === MEASUREMENT_COMPETITOR_TYPE.UNRESOLVED) {
      await measurable.closedAsUnresolved();
      return true;
    }

    return false;
  }
}

module.exports = {
  MeasurablesStateMachine,
};