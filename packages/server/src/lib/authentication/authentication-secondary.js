const { Jwt } = require('../jwt');
const { Auth0 } = require('../auth0');

const { UsersData } = require('../../data/users-data');
const { AgentsData } = require('../../data/agents-data');
const { TokensData } = require('../../data/tokens-data');
const { BotsData } = require('../../data/bots-data');

const { Params } = require('../../data/classes');

const {
  NotAuthenticatedError,
  TokenIsInvalidError,
  NoAgentIdError,
} = require('./errors');

class AuthenticationSecondary {
  constructor() {
    this.Jwt = new Jwt();
    this.auth0 = new Auth0();

    this.users = new UsersData();
    this.agents = new AgentsData();
    this.bots = new BotsData();
    this.tokens = new TokensData();
  }

  /**
   * @public
   * @param {string} token
   * @return {Promise<{
   *  agent: Models.Agent,
   *  creator: Models.Creator,
   *  bot: Models.Bot,
   *  user: Models.Bot,
   * }>}
   */
  async authenticate(token = '') {
    if (this.tokens.validate(token)) {
      return this._byToken(token);
    }

    if (this.Jwt.validate(token)) {
      return this._byJwt(token);
    }

    throw new TokenIsInvalidError();
  }

  /**
   * @protected
   * @param {string} token
   * @return {Promise<{
   *  agent: Models.Agent,
   *  creator: Models.Creator,
   *  bot: Models.Bot,
   *  user: Models.Bot,
   * }>}
   */
  async _byJwt(token) {
    const decoded = this.Jwt.decodeJwtToken(token);
    const agentId = decoded.sub;
    return this._getContext(agentId);
  }

  /**
   * @protected
   * @param {string} token
   * @return {Promise<{
   *  agent: Models.Agent,
   *  creator: Models.Creator,
   *  bot: Models.Bot,
   *  user: Models.Bot,
   * }>}
   */
  async _byToken(token) {
    const agentId = await this.tokens.getAgentId(token);
    return this._getContext(agentId);
  }

  /**
   * @protected
   * @param {Defs.AgentID} agentId
   * @return {Promise<{
   *  agent: Models.Agent,
   *  creator: Models.Creator,
   *  bot: Models.Bot,
   *  user: Models.User,
   *  botUserOwner: Models.User,
   * }>}
   */
  async _getContext(agentId) {
    if (!agentId) {
      throw new NoAgentIdError();
    }

    const agent = await this.agents.getOne(new Params({ id: agentId }));
    if (!agent) {
      throw new NotAuthenticatedError();
    }

    const bot = await this.bots.getOne(new Params({ agentId }));
    const user = await this.users.getOne(new Params({ agentId }));
    const creator = bot || user;

    const botUserOwner = !!bot
      ? await this.users.getOne(new Params({ id: bot.userId }))
      : null;

    return {
      agent, bot, user, creator, botUserOwner,
    };
  }
}

module.exports = {
  AuthenticationSecondary,
};
