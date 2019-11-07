const { ContinuousDistribution } = require('./continuousDistribution');
const { ContinuousDistributionCombination } = require('./continuousDistributionCombination');

class Cdf extends ContinuousDistribution{
  // TODO: This convertToNewLength function is obviously messy, could likely be refactored. I wanted to make a uniform distribution.
  createUniform2({min, max, sampleCount}) {
    let importantPoints = p => p.splice(0, 2).concat(p[p.length - 1]); 
    const dist = new Cdf([min, max], [0,1]).convertToNewLength(sampleCount);
    return new Cdf(importantPoints(dist.xs), importantPoints(dist.ys));
  }

  /**
   *
   * @return {Pdf}
   */
  toPdf() {
    let newYs = [this.ys[0]];
    for (let i = 1; i < this.ys.length; i++) {
      newYs.push((this.ys[i] - this.ys[i - 1]) / (this.xs[i] - this.xs[i - 1]))
    }
    return new (require('./pdf').Pdf)(this.xs, newYs);
  }

  toUniform(sampleCount) {
    return this.createUniform2({min: this.xs[0], max: this.xs[this.xs.length - 1], sampleCount})
  }

  combineWithUniformOfCdf({cdf, uniformWeight, sampleCount}) {
    let _uniform = cdf.toUniform(sampleCount);
    let combination = new ContinuousDistributionCombination([this, _uniform]);
    let normalize = 1/(1+uniformWeight);
    let newFn = combination.combineYsWithFn(
      sampleCount,
      (r =>(r[0] + (r[1]*uniformWeight))*normalize)
    );
    return new Cdf(newFn.xs, newFn.ys)
  }
}

module.exports = {
  Cdf,
};
