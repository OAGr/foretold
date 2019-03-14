open Utils;
open Rationale;
open Result.Infix;

type state = {
  page: int,
  selected: option(int),
};
type action =
  | NextPage
  | LastPage
  | Select(option(int))
  | SelectIncrement
  | SelectDecrement;

let component = ReasonReact.reducerComponent("MeasurableIndex");

let itemsPerPage = 20;

let indexChannelHeader =
    (channel: option(string), onForward, onBackward, isAtStart, isAtEnd) =>
  <SLayout.Header>
    {
      channel
      |> E.O.fmap(c =>
           <div>
             <h1>
               <a href={Urls.mapLinkToUrl(ChannelShow(c))}>
                 {"#" ++ c |> ste}
               </a>
             </h1>
             <Antd.Button onClick={_ => onBackward()} disabled=isAtStart>
               <Icon.Icon icon="ARROW_LEFT" />
             </Antd.Button>
             <Antd.Button onClick={_ => onForward()} disabled=isAtEnd>
               <Icon.Icon icon="ARROW_RIGHT" />
             </Antd.Button>
             {SLayout.button(c)}
           </div>
         )
      |> E.O.React.defaultNull
    }
  </SLayout.Header>;

let itemHeader =
    (
      channel: option(string),
      onForward,
      onBackward,
      onBack,
      isAtStart,
      isAtEnd,
    ) =>
  <SLayout.Header>
    {
      channel
      |> E.O.fmap(c =>
           <div>
             <Antd.Button onClick={_ => onBack()}>
               <Icon.Icon icon="ARROW_LEFT" />
             </Antd.Button>
             <h1>
               <a href={Urls.mapLinkToUrl(ChannelShow(c))}>
                 {"#" ++ c |> ste}
               </a>
             </h1>
             <Antd.Button onClick={_ => onBackward()} disabled=isAtStart>
               <Icon.Icon icon="ARROW_LEFT" />
             </Antd.Button>
             <Antd.Button onClick={_ => onForward()} disabled=isAtEnd>
               <Icon.Icon icon="ARROW_RIGHT" />
             </Antd.Button>
             {SLayout.button(c)}
           </div>
         )
      |> E.O.React.defaultNull
    }
  </SLayout.Header>;

let make = (~channel: string, ~loggedInUser: GetUser.t, _children) => {
  ...component,
  initialState: () => {page: 0, selected: None},
  reducer: (action, state) =>
    switch (action) {
    | NextPage => ReasonReact.Update({...state, page: state.page + 1})
    | LastPage => ReasonReact.Update({...state, page: state.page - 1})
    | Select((num: option(int))) =>
      ReasonReact.Update({...state, selected: num})
    | SelectIncrement =>
      ReasonReact.Update({
        ...state,
        selected: state.selected |> E.O.fmap(r => r + 1),
      })
    | SelectDecrement =>
      ReasonReact.Update({
        ...state,
        selected: state.selected |> E.O.fmap(r => r - 1),
      })
    },
  render: ({state, send}) =>
    (
      (measurables: array(DataModel.measurable)) =>
        <div>
          {
            switch (state.selected) {
            | Some(index) =>
              let measurable = E.A.get(measurables, index);
              let itemsOnPage = measurables |> Array.length;
              <div>
                {
                  itemHeader(
                    Some(channel),
                    () => send(SelectIncrement),
                    () => send(SelectDecrement),
                    () => send(Select(None)),
                    index == 0,
                    index == itemsOnPage - 1,
                  )
                }
                <div className=SLayout.Styles.mainSection>
                  {
                    switch (measurable) {
                    | Some(m) =>
                      <MeasurableShow__Component id={m.id} loggedInUser />
                    | None => "Item not found" |> ste
                    }
                  }
                </div>
              </div>;
            | _ =>
              <div>
                {
                  indexChannelHeader(
                    Some(channel),
                    () => send(NextPage),
                    () => send(LastPage),
                    state.page == 0,
                    measurables |> Array.length < itemsPerPage,
                  )
                }
                <div className=SLayout.Styles.mainSection>
                  <MeasurableIndex__Table
                    measurables
                    loggedInUser
                    showExtraData=true
                    onSelect=(
                      e =>
                        send(
                          Select(
                            Some(
                              measurables
                              |> Array.to_list
                              |> Rationale.RList.findIndex(
                                   (r: DataModel.measurable) =>
                                   r.id == e.id
                                 )
                              |> E.O.toExn(""),
                            ),
                          ),
                        )
                    )
                  />
                </div>
              </div>
            }
          }
        </div>
    )
    |> GetMeasurables.component(channel, state.page, itemsPerPage),
};