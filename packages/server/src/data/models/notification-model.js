const models = require('./definitions');
const { ModelPostgres } = require('./model-postgres');

/**
 * @implements {Layers.Models.ModelGeneric}
 */
class NotificationModel extends ModelPostgres {
  constructor() {
    super({
      model: models.Notification,
      sequelize: models.sequelize,
    });
  }
}

module.exports = {
  NotificationModel,
};
