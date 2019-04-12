open SLayout;
open Utils;
open Style.Grid;
open Rationale.Function.Infix;

module MemberTab = {
  type t =
    | View
    | Invite;

  let toS = (t: t) =>
    switch (t) {
    | View => "V"
    | Invite => "I"
    };

  let toUrl = (t: t, id: string): Context__Routing.Url.t =>
    switch (t) {
    | View => ChannelMembers(id)
    | Invite => ChannelInvite(id)
    };
};

module InfoTab = {
  type t =
    | NewSeries
    | Edit;

  let toS = (t: t) =>
    switch (t) {
    | NewSeries => "N"
    | Edit => "E"
    };

  let toUrl = (t: t, id: string): Context__Routing.Url.t =>
    switch (t) {
    | NewSeries => SeriesNew(id)
    | Edit => ChannelEdit(id)
    };
};

module TopTab = {
  type t =
    | Measurables
    | Members(MemberTab.t)
    | Options(InfoTab.t);

  let toS = (t: t) =>
    switch (t) {
    | Measurables => "M"
    | Members(_) => "Me"
    | Options(_) => "O"
    };

  let toUrl = (t: t, id: string): Context__Routing.Url.t =>
    switch (t) {
    | Measurables => ChannelShow(id)
    | Members(_) => ChannelMembers(id)
    | Options(_) => ChannelEdit(id)
    };
};

let topOption = (page: Context.Routing.Route.t): option(TopTab.t) =>
  switch (page) {
  | ChannelEdit(_) => Some(Options(Edit))
  | SeriesNew(_) => Some(Options(NewSeries))
  | ChannelInvite(_) => Some(Members(Invite))
  | ChannelMembers(_) => Some(Members(View))
  | MeasurableNew(_) => Some(Measurables)
  | ChannelShow(_) => Some(Measurables)
  | Series(_, _) => Some(Measurables)
  | _ => None
  };

module Config = {
  type t = {
    head: ReasonReact.reactElement,
    body: ReasonReact.reactElement,
  };
};

let component = ReasonReact.statelessComponent("Page");

let button = (value, toUrl, str, id) =>
  <Antd_Radio_Button
    value onClick={_ => Context.Routing.Url.push(toUrl(id))}>
    {str |> ste}
  </Antd_Radio_Button>;

let tabs = (o: TopTab.t, channel: Context.Primary.Channel.t) =>
  TopTab.(
    <Antd.Radio.Group defaultValue="" value={o |> toS} onChange={e => ()}>
      {
        button(
          Measurables |> toS,
          Measurables |> toUrl,
          "Measurables",
          channel.id,
        )
      }
      {
        button(
          Members(View) |> toS,
          Members(View) |> toUrl,
          (
            channel.membershipCount
            |> E.O.fmap(string_of_int)
            |> E.O.fmap(e => e ++ " ")
            |> E.O.default("")
          )
          ++ "Members",
          channel.id,
        )
      }
      {
        button(
          Options(Edit) |> toS,
          Options(Edit) |> toUrl,
          "Settings",
          channel.id,
        )
      }
    </Antd.Radio.Group>
  );

let memberTabs = (o: MemberTab.t, channel: Context.Primary.Channel.t) =>
  MemberTab.(
    <Antd.Radio.Group defaultValue="" value={o |> toS} onChange={e => ()}>
      {button(View |> toS, View |> toUrl, "List", channel.id)}
      {button(Invite |> toS, Invite |> toUrl, "Invite", channel.id)}
    </Antd.Radio.Group>
  );

let editTabs = (o: InfoTab.t, channel: Context.Primary.Channel.t) =>
  InfoTab.(
    <Antd.Radio.Group defaultValue="foo" value={o |> toS} onChange={e => ()}>
      {button(Edit |> toS, Edit |> toUrl, "Edit", channel.id)}
      {button(NewSeries |> toS, NewSeries |> toUrl, "New Series", channel.id)}
    </Antd.Radio.Group>
  );

let leaveButton = (agent, channelId) =>
  agent
  |> E.O.fmap((e: Context.Primary.Agent.t) => e.id)
  |> E.O.React.fmapOrNull(C.Channel.SimpleHeader.leaveChannel(channelId));

let joinButton = (agent, channelId) =>
  agent
  |> E.O.fmap((e: Context.Primary.Agent.t) => e.id)
  |> E.O.React.fmapOrNull(C.Channel.SimpleHeader.joinChannel(channelId));

let make =
    (
      channelId: string,
      loggedInUser: Context.Primary.User.t,
      topOption: option(TopTab.t),
      {head, body}: LayoutConfig.t,
    ) => {
  ...component,
  render: _ => {
    let topOption =
      topOption
      |> E.O.toExn(
           "The top option should always be SOME in Channel_Layout.make",
         );

    let loadChannel =
      Foretold__GraphQL.Queries.Channel.component2(~id=channelId);
    let top =
      loadChannel(
        E.HttpResponse.fmap((channel: Context.Primary.Channel.t) =>
          <>
            <Div float=`left> {channelink(channel)} </Div>
            <Div float=`right> {tabs(topOption, channel)} </Div>
          </>
        )
        ||> E.HttpResponse.withReactDefaults,
      );

    let foo =
      loadChannel(
        E.HttpResponse.fmap((channel: Context.Primary.Channel.t) =>
          switch (topOption) {
          | Members(r) =>
            <MainSection>
              <Div float=`right> {memberTabs(r, channel)} </Div>
            </MainSection>
          | Options(r) =>
            <MainSection>
              <Div float=`right>
                {leaveButton(loggedInUser.agent, channel.id)}
                {joinButton(loggedInUser.agent, channel.id)}
                {editTabs(r, channel)}
              </Div>
            </MainSection>
          | _ => E.React.null
          }
        )
        ||> E.HttpResponse.withReactDefaults,
      );

    <Layout__Component__FillWithSidebar
      channelId={Some(channelId)} loggedInUser>
      <Header> top </Header>
      foo
      <MainSection> head </MainSection>
      <MainSection> body </MainSection>
    </Layout__Component__FillWithSidebar>;
  },
};

let makeWithEl = (channelId, loggedInUser, topOption, t: LayoutConfig.t) =>
  make(channelId, loggedInUser, topOption, t) |> E.React.el;

let makeWithPage =
    (page: Context.Routing.Route.t, loggedInUser): ReasonReact.reactElement => {
  let topOption = topOption(page);

  let layout = (channelId, fn) =>
    makeWithEl(channelId, loggedInUser, topOption) |> fn |> E.React.makeToEl;

  switch (page) {
  | ChannelShow(channelId) =>
    MeasurableIndex.make(
      ~channelId,
      ~loggedInUser,
      ~itemsPerPage=20,
      ~layout=_,
    )
    |> layout(channelId)
  | Series(channelId, id) =>
    SeriesShow.make(~id, ~channelId, ~loggedInUser, ~layout=_)
    |> layout(channelId)
  | MeasurableNew(channelId) =>
    MeasurableNew.make(~channelId, ~layout=_) |> layout(channelId)
  | ChannelMembers(channelId) =>
    ChannelMembers.make(~channelId, ~loggedInUser, ~layout=_)
    |> layout(channelId)
  | ChannelInvite(channelId) =>
    ChannelInvite.make(~channelId, ~loggedInUser, ~layout=_)
    |> layout(channelId)
  | ChannelEdit(channelId) =>
    ChannelEdit.make(~channelId, ~layout=_) |> layout(channelId)
  | SeriesNew(channelId) =>
    SeriesNew.make(~channelId, ~loggedInUser, ~layout=_) |> layout(channelId)
  | _ => E.React.null
  };
};