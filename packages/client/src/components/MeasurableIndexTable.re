module Styles = {
  open Css;

  let mainColumn =
    style([flex(`num(4.)), display(`flex), flexDirection(`column)]);

  let mainColumnTop =
    style([
      flex(`num(1.)),
      paddingLeft(px(2)),
      selector(" p", [marginTop(`px(3)), marginBottom(`px(8))]),
    ]);
  let rightColumn = style([flex(`num(1.))]);
};

let component = ReasonReact.statelessComponent("MeasurableIndexTable");
let make =
    (
      ~measurables: array(Types.measurable),
      ~showExtraData: bool,
      ~onSelect=(_m: Types.measurable) => (),
      ~channelId: option(string)=None,
      _children,
    ) => {
  ...component,
  render: _self =>
    measurables |> E.A.length > 0
      ? <>
          <Fc.FC.Table.HeaderRow>
            <Fc.FC.Table.Cell flex={`num(3.)}>
              {"Name & Status" |> ReasonReact.string}
            </Fc.FC.Table.Cell>
            <Fc.FC.Table.Cell flex={`num(1.5)}>
              {"Aggregate and resolution" |> ReasonReact.string}
            </Fc.FC.Table.Cell>
            <Fc.FC.Table.Cell flex={`num(1.)}>
              {"Details" |> ReasonReact.string}
            </Fc.FC.Table.Cell>
          </Fc.FC.Table.HeaderRow>
          {measurables
           |> E.A.fmap((m: Types.measurable) => {
                let iAmOwner = m.iAmOwner == Some(true);

                <Fc.FC.Table.Row onClick={_e => onSelect(m)} key={m.id}>
                  <Fc.FC.Table.Cell
                    flex={`num(3.)}
                    className=Css.(
                      style([
                        paddingTop(`em(0.3)),
                        paddingBottom(`em(0.3)),
                      ])
                    )>
                    <div className=Styles.mainColumn>
                      <div className=Styles.mainColumnTop>
                        {MeasurableItems.link(~m)}
                      </div>
                    </div>
                    <div className=Styles.rightColumn>
                      <StatusDisplay measurable=m />
                    </div>
                  </Fc.FC.Table.Cell>
                  <Fc.FC.Table.Cell
                    flex={`num(1.5)}
                    className=Css.(style([paddingTop(`em(0.5))]))>
                    <MeasurementItems.AggregationResolution measurable=m />
                  </Fc.FC.Table.Cell>
                  <Fc.FC.Table.Cell
                    flex={`num(1.)}
                    className=Css.(style([paddingTop(`em(0.5))]))>
                    {E.React.showIf(
                       channelId == Some("home"),
                       MeasurableItems.channelLink(~m),
                     )}
                    {E.React.showIf(
                       showExtraData,
                       MeasurableItems.series(~m, ~channelId, ())
                       |> E.O.React.defaultNull,
                     )}
                    {E.React.showIf(
                       showExtraData,
                       MeasurableItems.creatorLink(~m)
                       |> E.O.React.defaultNull,
                     )}
                    {MeasurableItems.measurements(~m) |> E.O.React.defaultNull}
                    {MeasurableItems.measurers(~m) |> E.O.React.defaultNull}
                    {E.React.showIf(iAmOwner, MeasurableItems.editLink(~m))}
                  </Fc.FC.Table.Cell>
                </Fc.FC.Table.Row>;
              })
           |> ReasonReact.array}
        </>
      : <SLayout.NothingToShow />,
};