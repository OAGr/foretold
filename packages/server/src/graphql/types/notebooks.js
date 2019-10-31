const graphql = require('graphql');
const { resolver, DateType } = require('graphql-sequelize');

const models = require('../../models');
const scalars = require('./scalars');

const notebook = new graphql.GraphQLObjectType({
  name: 'Notebook',
  fields: () => ({
    id: { type: graphql.GraphQLNonNull(scalars.notebookId) },
    name: { type: graphql.GraphQLNonNull(scalars.string3to255) },
    createdAt: { type: graphql.GraphQLNonNull(DateType.default) },
    updatedAt: { type: graphql.GraphQLNonNull(DateType.default) },
    agentId: { type: graphql.GraphQLNonNull(scalars.agentId) },
    channelId: { type: graphql.GraphQLNonNull(scalars.channelId) },
    body: { type: graphql.GraphQLNonNull(scalars.string3to16K) },
    agent: {
      type: require('./agents').agent,
      resolve: resolver(models.Notebook.Agent),
    },
    channel: {
      type: require('./channels').channel,
      resolve: resolver(models.Notebook.Channel),
    },
  }),
});

const notebookCreateInput = new graphql.GraphQLInputObjectType({
  name: 'NotebookCreateInput',
  fields: () => ({
    name: { type: graphql.GraphQLNonNull(scalars.string3to255) },
    body: { type: graphql.GraphQLNonNull(scalars.string3to16K) },
  }),
});

const notebookEdge = new graphql.GraphQLObjectType({
  name: 'NotebookEdge',
  fields: () => ({
    node: { type: require('./notebooks').notebook },
    cursor: { type: graphql.GraphQLNonNull(graphql.GraphQLString) },
  }),
});

const notebooksConnection = new graphql.GraphQLObjectType({
  name: 'NotebooksConnection',
  fields: () => ({
    total: {
      type: graphql.GraphQLInt,
    },
    pageInfo: {
      type: graphql.GraphQLNonNull(require('./common').pageInfoConnection),
    },
    edges: {
      type: graphql.GraphQLList(require('./notebooks').notebookEdge),
    },
  }),
});

module.exports = {
  notebook,
  notebookCreateInput,
  notebookEdge,
  notebooksConnection,
};