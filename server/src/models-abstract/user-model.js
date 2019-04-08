const _ = require('lodash');

const models = require('../models');
const { ModelPostgres } = require('./model-postgres');

class UserModel extends ModelPostgres {

  constructor() {
    super({
      model: models.User,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  UserModel,
};
