export namespace Models {

  export interface Channel {
    id: string;
    isPublic: boolean;
  }

  export interface Bot {
    id: string;
    name: string;
    getAgent(): Models.Agent;
  }

  export interface User {
    id: string;
    name: string;
    auth0Id: string;
    agentId: string;
    getAgent(): Models.Agent;
  }

  export interface Measurable {
    id: string;
    creationNotification(creator: Models.Creator): any;
    updateNotifications(creator: Models.Creator): any;
  }

  export interface Measurement {
    id: string;
    creationNotification(creator: Models.Creator): any;
  }

  export interface Agent {
    id: string;
    getBot(): Models.Bot;
    getUser(): Models.User;
  }

  export interface Series {
    id: string;
  }

  export type ChannelMembershipRole = "ADMIN" | "VIEWER";

  export interface ChannelMemberships {
    agentId: string;
    channelId: string;
    role: ChannelMembershipRole;
  }

  export type Creator = Models.User | Models.Bot;
}

export namespace Schema {
  export interface Context {
    user?: Models.User;
    bot?: Models.Bot;
    agent: Models.Agent;
    creator?: Models.User | Models.Bot;
    channel?: Models.Channel;
    channelMembership?: Models.ChannelMemberships;
    channelMembershipsAdmins?: Models.ChannelMemberships[];
    channelMembershipsRole?: Models.ChannelMembershipRole;
    total?: number;
  }

  export interface ChannelsInput {
    name: string;
    description: string;
    isArchived: boolean;
    isPublic: boolean;
  }
}

export namespace Layers {
  namespace DataSourceLayer {
    type compoundId = object;
    type id = string | compoundId;
    type data = object;
    type options = object;
    type filter = object;
    type pagination = object;
    type query = object;
    type params = object;
    type response = { data: any };
    type responseList = { data: any, total: number };

    interface DataSource {
      createOne(
        data,
        options,
      ): response;

      getOne(
        params,
        query,
        options,
      ): response;

      updateOne(
        params,
        data,
        options,
      ): response;

      deleteOne(
        params,
        options,
      ): response;

      getAll(
        filter,
        pagination,
        options,
      ): responseList;
    }
  }

  namespace AbstractModelsLayer {
    type compoundId = object;
    type id = string | compoundId;

    type data = object;
    type restrictions = object;
    type filter = object;
    type pagination = object;
    type query = object;
    type params = object;
    type response = { data: any };
    type responseList = { data: any, total: number };

    interface AbstractModel {
      deleteOne(
        params: params,
        restrictions: restrictions,
      ): response;

      updateOne(
        params: params,
        data: data,
        restrictions: restrictions,
      ): response;

      createOne(
        data: data,
        restrictions: restrictions,
      ): response;

      getOne(
        params: params,
        query: query,
        restrictions: restrictions,
      ): response;

      getAll(
        filter: filter,
        pagination: pagination,
        restrictions: restrictions,
      ): responseList;
    }
  }
}
