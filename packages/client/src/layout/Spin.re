open Utils;

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
      <div className=Styles.centerBlock> <Antd.Spin tip="Loading..." /> </div>
    </>,
};