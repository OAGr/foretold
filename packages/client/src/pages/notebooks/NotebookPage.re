[@bs.config {jsx: 3}];

open Style.Grid;

type tab =
  | Show
  | Edit
  | Details;

module ShowIfSameUser = {
  [@react.component]
  let make = (~agentId, ~children) => {
    <Providers.AppContext.Consumer>
      ...{({loggedUser}) => {
        let isCorrect =
          loggedUser
          |> E.O.fmap((r: Types.user) => r.agentId == agentId)
          |> E.O.default(false);
        isCorrect ? children : ReasonReact.null;
      }}
    </Providers.AppContext.Consumer>;
  };
};

module Tabs = {
  open FC;

  [@react.component]
  let make = (~switchTab, ~tab, ~notebook: Types.notebook) => {
    <Providers.AppContext.Consumer>
      ...{({loggedUser}) =>
        <Div>
          <Div
            styles=[
              Css.style([
                FC.Base.BaseStyles.floatLeft,
                Css.paddingTop(`em(0.2)),
              ]),
            ]>
            <TabButton isActive={tab == Show} onClick={_ => switchTab(Show)}>
              {"Notebook" |> Utils.ste}
            </TabButton>
            <TabButton
              isActive={tab == Details} onClick={_ => switchTab(Details)}>
              {"Markdown" |> Utils.ste}
            </TabButton>
            <ShowIfSameUser agentId={notebook.ownerId}>
              <TabButton
                isActive={tab == Edit} onClick={_ => switchTab(Edit)}>
                {"Edit" |> Utils.ste}
              </TabButton>
            </ShowIfSameUser>
          </Div>
        </Div>
      }
    </Providers.AppContext.Consumer>;
  };
};

[@react.component]
let make = (~channelId: string, ~notebookId: string) => {
  let (tab, setTab) = React.useState(() => Show);
  let switchTab = tabToSwitch => setTab(_ => tabToSwitch);

  NotebookGet.component(~id=notebookId, notebook =>
    switch (notebook) {
    | Some(notebook) =>
      <>
        <SLayout head={<Tabs switchTab tab notebook />} isFluid=true>
          {switch (tab) {
           | Show =>
             <div
               className=Css.(
                 style([marginTop(`em(2.0)), marginBottom(`em(2.0))])
               )>
               <NotebookHeader notebook />
               <Markdown
                 source={notebook.body}
                 supportForetoldJs=true
                 channelId
               />
             </div>
           | Details =>
             <FC__PageCard.BodyPadding>
               <Antd.Input.TextArea
                 style={ReactDOMRe.Style.make(~minHeight="30em", ())}
                 value={notebook.body}
               />
             </FC__PageCard.BodyPadding>
           | Edit => <div> <NotebookUpdate notebook /> </div>
           }}
        </SLayout>
      </>
    | _ => <NotFoundPage />
    }
  );
};