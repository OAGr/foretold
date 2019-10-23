type content = {
  headerContent: ReasonReact.reactElement,
  bodyContent: ReasonReact.reactElement,
};

module Overlay = {
  module Styles = {
    open Css;
    let className = style([maxWidth(`em(30.))]);
  };

  [@react.component]
  let make = (~content) =>
    <div className=Styles.className>
      <FC__PageCard.Jsx2>
        <FC__PageCard.HeaderRow.Jsx2>
          <FC__Div.Jsx2 float=`left>
            <FC__PageCard.HeaderRow.Title.Jsx2>
              <span
                className=Css.(
                  style([marginRight(`em(0.4)), opacity(0.5)])
                )>
                <FC__Icon.Questionmark.Jsx2 isInteractive=false />
              </span>
              {content.headerContent}
            </FC__PageCard.HeaderRow.Title.Jsx2>
          </FC__Div.Jsx2>
        </FC__PageCard.HeaderRow.Jsx2>
        <FC__PageCard.Body.Jsx2>
          <FC__PageCard.BodyPadding.Jsx2 v={`em(0.5)}>
            <span
              className=Css.(
                style([
                  color(FC__Settings.Text.LightBackground.p),
                  lineHeight(`em(1.5)),
                ])
              )>
              {content.bodyContent}
            </span>
          </FC__PageCard.BodyPadding.Jsx2>
        </FC__PageCard.Body.Jsx2>
      </FC__PageCard.Jsx2>
    </div>;

  module Jsx2 = {
    let make = (~content, children) =>
      ReasonReactCompat.wrapReactForReasonReact(
        make,
        makeProps(~content, ()),
        children,
      );
  };
};

let staticOverlay = content => <Overlay.Jsx2 content />;

[@react.component]
let make = (~content) =>
  <FC__Dropdown overlay={staticOverlay(content)} trigger=FC__Dropdown.Hover>
    <span> <FC__Icon.Questionmark.Jsx2 isInteractive=true /> </span>
  </FC__Dropdown>;

module Jsx2 = {
  let make = (~content, children) =>
    ReasonReactCompat.wrapReactForReasonReact(
      make,
      makeProps(~content, ()),
      children,
    );
};
