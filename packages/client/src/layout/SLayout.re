open Utils;

module Styles = {
  open Css;

  let width100 = style([width(`percent(100.))]);
  let headerText = style([color(`hex("486474"))]);
  let descriptionText = style([color(`hex("aaa"))]);

  let seriesText =
    style([
      color(`hex("3d547b")),
      fontSize(`em(2.0)),
      float(`left),
      fontWeight(`medium),
    ]);

  let channelText =
    style([
      color(`hex("2c436e")),
      fontSize(`em(1.15)),
      fontWeight(`num(600)),
      marginTop(`px(3)),
      float(`left),
    ]);

  let container = style([maxWidth(`px(1170)), margin(`auto)]);

  let backHover = style([fontSize(`em(1.3))]);

  let dash =
    style([
      float(`left),
      marginTop(`em(0.2)),
      marginRight(`em(0.3)),
      marginLeft(`em(0.3)),
      fontSize(`em(1.5)),
      color(`hex("ccc")),
    ]);
};

module Header = {
  let component = ReasonReact.statelessComponent("Header");
  let textDiv = text =>
    <Fc.FC__PageCard.HeaderRow.Title>
      {text |> ste}
    </Fc.FC__PageCard.HeaderRow.Title>;

  let make = children => {
    ...component,
    render: _ =>
      <Fc.FC.PageCard.HeaderRow> ...children </Fc.FC.PageCard.HeaderRow>,
  };
};

module Spin = {
  module Styles = {
    open Css;
    let centerBlock = style([textAlign(`center), padding(`em(2.))]);
    let clear = style([clear(`both)]);
  };

  let component = ReasonReact.statelessComponent("Spin");

  let make = _children => {
    ...component,
    render: _ =>
      <>
        <div className=Styles.clear />
        <div className=Styles.centerBlock>
          <Antd.Spin tip="Loading..." />
        </div>
      </>,
  };
};

module Error = {
  module Styles = {
    open Css;
    let centerBlock = style([textAlign(`center), padding(`em(2.))]);
    let clear = style([clear(`both)]);
  };

  let component = ReasonReact.statelessComponent("Error");

  let make = (~e, _children) => {
    ...component,
    render: _ => <> <div> {"Error: " ++ e |> Utils.ste} </div> </>,
  };
};

module NothingToShow = {
  module Styles = {
    open Css;
    let centerBlock = style([textAlign(`center), margin(`em(2.))]);
    let clear = style([clear(`both)]);
  };

  let component = ReasonReact.statelessComponent("NothingToShow");

  let make = _children => {
    ...component,
    render: _ =>
      <>
        <div className=Styles.clear />
        <div className=Styles.centerBlock>
          {"Nothing to show" |> Utils.ste}
        </div>
      </>,
  };
};

module LayoutConfig = {
  type t = {
    head: ReasonReact.reactElement,
    body: ReasonReact.reactElement,
  };
  let make = (~head, ~body) => {head, body};
};

module FullPage = {
  let component = ReasonReact.statelessComponent("Page");
  let make = ({head, body}: LayoutConfig.t) => {
    ...component,
    render: _ =>
      <Fc.FC.Base.Div
        className=Css.(
          style(
            [
              marginTop(`em(2.0)),
              marginTop(`em(2.)),
              width(`percent(100.)),
              boxSizing(`borderBox),
            ]
            @ Fc.FC.Base.BaseStyles.fullWidthFloatLeft,
          )
        )>
        <div className=Styles.container>
          <Fc.FC.PageCard>
            <Fc.FC.PageCard.HeaderRow> head </Fc.FC.PageCard.HeaderRow>
            <Fc.FC.PageCard.Body> body </Fc.FC.PageCard.Body>
          </Fc.FC.PageCard>
        </div>
      </Fc.FC.Base.Div>,
  };
  let makeWithEl = (t: LayoutConfig.t) => t |> make |> E.React.el;
};

let channelBack = (~onClick, ()) =>
  <Fc.FC__Button onClick size=Median>
    {"< Back" |> ReasonReact.string}
  </Fc.FC__Button>;

let channelLink = (c: Types.channel) =>
  <Link.Jsx2
    linkType={Internal(ChannelShow(c.id))} className=Styles.channelText>
    {switch (c.id) {
     | "home" =>
       Primary.Channel.presentGlobal(
         ~symbolClassName=Primary.Channel.Styles.globe,
         (),
       )
     | _ => c |> Primary.Channel.present
     }}
  </Link.Jsx2>;

let channelEditLink = (c: Types.channel) =>
  <Link.Jsx2
    linkType={Internal(ChannelEdit(c.id))} className=Styles.headerText>
    {"edit" |> ste}
  </Link.Jsx2>;

let seriesHead = (channel: Types.channel, seriesName) =>
  <>
    <div className=Styles.seriesText>
      <Icon.Icon icon="LAYERS" />
      {seriesName |> ste}
    </div>
  </>;