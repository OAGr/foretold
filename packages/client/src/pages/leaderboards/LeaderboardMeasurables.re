module ReducerConfig = {
  type itemType = Types.agentMeasurable;
  type callFnParams = (option(string), option(string));

  let getId = (e: itemType) => e.id;
  let callFn = ((channelId, measurableId): callFnParams) =>
    AgentMeasurablesGet.component(~channelId, ~measurableId, ());

  let isEqual = (a: itemType, b: itemType) => {
    a.id == b.id;
  };
};

module Reducer = PaginationFunctor.Make(ReducerConfig);

let component = ReasonReact.statelessComponent("LeaderboardMeasurables");
let make =
    (
      ~channelId=None,
      ~measurableId=None,
      ~head=Leaderboard.head(~subTab=ByMeasurable),
      ~columns=LeaderboardTable.Columns.measurables,
      _children,
    ) => {
  ...component,
  render: _ => {
    let subComponent = (reducerParams: Reducer.Types.reducerParams) => {
      let items =
        switch (reducerParams.response) {
        | Success(connection) => connection.edges
        | _ => [||]
        };

      let items =
        items |> E.A.fmap(Primary.LeaderboardItem.fromAgentMeasurable);

      let body =
        switch (reducerParams.response) {
        | Success(_) =>
          Array.length(items) > 0
            ? <FC.PageCard.Body>
                <LeaderboardTable.Jsx2 items columns />
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

      <SLayout head> body </SLayout>;
    };

    <Reducer callFnParams=(channelId, measurableId) subComponent />;
  },
};