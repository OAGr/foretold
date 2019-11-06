[@bs.config {jsx: 3}];

module ReducerConfig = {
  type itemType = Types.agentChannel;
  type callFnParams = (option(string), option(string));

  let getId = (e: itemType) => e.id;
  let callFn = ((channelId, agentId): callFnParams) =>
    AgentChannelsGet.component(~channelId, ~agentId, ());

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

[@react.component]
let make =
    (
      ~channelId=None,
      ~agentId=None,
      ~head=Leaderboard.head(~subTab=ByMember),
      ~columns=LeaderboardTable.Columns.members,
      ~children=<Null />,
    ) => {
  let subComponent = (reducerParams: Reducer.Types.reducerParams) => {
    let items =
      switch (reducerParams.response) {
      | Success(connection) => connection.edges
      | _ => [||]
      };

    let items = items |> E.A.fmap(Primary.LeaderboardItem.fromAgentChannel);

    let body =
      switch (reducerParams.response) {
      | Success(_) =>
        Array.length(items) > 0
          ? <FC.PageCard.Body>
              <LeaderboardTable items columns />
            </FC.PageCard.Body>
          : <NothingToShow />
      | _ => <Spin />
      };

    let head =
      head(
        ~channelId,
        ~paginationPage=Reducer.Components.paginationPage(reducerParams),
        (),
      );

    <SLayout head isFluid=true> body </SLayout>;
  };

  <Reducer callFnParams=(channelId, agentId) subComponent />;
};