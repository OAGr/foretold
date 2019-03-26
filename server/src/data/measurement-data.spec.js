const models = require('../models');
const { MeasurementData } = require('./measurement-data');

describe('tests Measurement Data layer', () => {

  it('class should be a constructor', () => {
    expect(MeasurementData).toBeInstanceOf(Function);
  });

  const instance = new MeasurementData();

  describe('getOne()', () => {
    const id = 'id2';
    beforeEach(() => {
      jest.spyOn(models.Measurement, 'findOne').mockReturnValue(Promise.resolve(true));
    });
    it('finds a measurement', () => {
      return instance.getOne(id).then((result) => {
        expect(models.Measurement.findOne).toHaveBeenCalledWith({
          "where": { "id": "id2" },
        });
        expect(result).toBe(true);
      });
    });
  });


});
