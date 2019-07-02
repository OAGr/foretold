module ReducerConfig = {
  type itemType = Context.Primary.Measurable.t;
  type callFnParams = string;
  let getId = (e: Context.Primary.Measurable.t) => e.id;
  let callFn = (e: callFnParams) =>
    Foretold__GraphQL.Queries.Measurables.componentWithCreator(~creatorId=e);
  let isEqual = (a: itemType, b: itemType) => a.id == b.id;
};

module Reducer = PaginationReducerFunctor.Make(ReducerConfig);

let component = ReasonReact.statelessComponent("AgentMeasurables");
type pageParams = {id: string};

let make =
    (
      ~pageParams: pageParams,
      ~loggedInUser: Context.Primary.User.t,
      ~layout=SLayout.FullPage.makeWithEl,
      _children,
    ) => {
  ...component,
  render: _ => {
    let lmake = SLayout.LayoutConfig.make;
    Reducer.make(
      ~itemsPerPage=20,
      ~callFnParams=pageParams.id,
      ~subComponent=selectWithPaginationParams =>
      lmake(
        ~head=
          switch (selectWithPaginationParams.selection) {
          | Some(_selection) =>
            <>
              {Reducer.Components.deselectButton(
                 selectWithPaginationParams.send,
               )}
              {Reducer.Components.correctButtonDuo(selectWithPaginationParams)}
            </>
          | None =>
            <>
              {Reducer.Components.correctButtonDuo(selectWithPaginationParams)}
            </>
          },
        ~body=
          switch (
            selectWithPaginationParams.response,
            selectWithPaginationParams.selection,
          ) {
          | (_, Some(measurable)) =>
            <C.Measurable.FullPresentation id={measurable.id} loggedInUser />
          | (Success(connection), None) =>
            <C.Measurables.BasicTable
              measurables={connection.edges}
              showExtraData=true
              onSelect={e =>
                Reducer.Components.sendSelectItem(
                  selectWithPaginationParams,
                  e.id,
                )
              }
            />
          | _ => <div />
          },
      )
      |> layout
    )
    |> E.React.makeToEl;
  },
};