/* TODO: Allow for cases where user is not logged in */

let makeWithPage =
    (channelPage: Routing.ChannelPage.t, loggedInUser: option(Types.user))
    : ReasonReact.reactElement => {
  let channelId = channelPage.channelId;

  switch (loggedInUser) {
  | Some(loggedInUser) =>
    let loadChannel = ChannelGet.component2(~id=channelId);

    let successFn = (channel: Types.channel) => {
      let layout =
        Channel_Layout_C.makeWithEl(
          channelPage,
          loggedInUser,
          Some(channel),
        );

      switch (channelPage.subPage) {
      | Measurables(searchParams) =>
        <MeasurableIndex
          channelId
          searchParams
          loggedInUser
          itemsPerPage=20
          layout
        />
      | Measurable(measurableId) =>
        <ChannelMeasurable measurableId loggedInUser layout />
      | Series(id) => <SeriesShow id channelId loggedInUser layout />
      | NewMeasurable => <MeasurableNew channelId loggedInUser layout />
      | Members => <ChannelMembers channelId layout channel />
      | FeedItems => <FeedItems channelId={Some(channelId)} layout />
      | Leaderboard(ByMeasurement) =>
        <Leaderboard channelId={Some(channelId)} layout />
      | Leaderboard(ByMeasurable) =>
        <LeaderboardMeasurables channelId={Some(channelId)} layout />
      | AddMember => <ChannelAddMember channelId loggedInUser layout />
      | InviteMember => <ChannelInviteMember channelId loggedInUser layout />
      | Settings => <ChannelEdit channelId loggedInUser layout />
      | NewSeries => <SeriesNew channelId loggedInUser layout />
      };
    };

    let errorFn = _ => {
      let layout =
        Channel_Layout_C.makeWithEl(channelPage, loggedInUser, None);

      SLayout.LayoutConfig.make(
        ~head=<div />,
        ~body=<div> {"No channel." |> ReasonReact.string} </div>,
      )
      |> layout;
    };

    let loadingFn = () => {
      let layout =
        Channel_Layout_C.makeWithEl(channelPage, loggedInUser, None);

      SLayout.LayoutConfig.make(~head=<div />, ~body=<SLayout.Spin />)
      |> layout;
    };

    loadChannel(result =>
      result |> HttpResponse.flatten(successFn, errorFn, loadingFn)
    );
  | None => <Home />
  };
};