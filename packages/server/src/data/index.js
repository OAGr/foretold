const { MeasurablesData } = require('./measurables-data');
const { MeasurementsData } = require('./measurements-data');
const { UsersData } = require('./users-data');
const { SeriesData } = require('./series-data');
const { ChannelsData } = require('./channels-data');
const { ChannelMembershipsData } = require('./channel-memberships-data');
const { AgentsData } = require('./agents-data');
const { BotsData } = require('./bots-data');
const { TokensData } = require('./tokens-data');
const { PreferencesData } = require('./preferences-data');
const { NotificationsData } = require('./notifications-data');
const { TemplatesData } = require('./templates-data');
const { NotificationStatusesData } = require('./notification-statuses-data');
const { InvitationsData } = require('./invitations-data');
const { FeedItemsData } = require('./feed-items-data');
const { GlobalSettingsData } = require('./global-settings-data');
const { AgentMeasurablesData } = require('./agent-measurables-data');
const { AgentChannelsData } = require('./agent-channels-data');
const { MutexesData } = require('./mutexes-data');
const { NotebooksData } = require('./notebooks-data');

// @todo: fix, do not use instance
const globalSetting = new GlobalSettingsData();

module.exports = {
  globalSetting,
  MeasurablesData,
  MeasurementsData,
  UsersData,
  SeriesData,
  ChannelsData,
  ChannelMembershipsData,
  AgentsData,
  BotsData,
  TokensData,
  PreferencesData,
  NotificationsData,
  TemplatesData,
  NotificationStatusesData,
  InvitationsData,
  FeedItemsData,
  GlobalSettingsData,
  AgentMeasurablesData,
  AgentChannelsData,
  MutexesData,
  NotebooksData,
};
