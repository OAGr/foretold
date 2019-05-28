open FC__Base;
open Colors.Text;

let activeStyles =
  Css.[
    borderBottom(`px(2), `solid, LightBackground.active),
    color(LightBackground.active),
    hover([color(LightBackground.active)]),
  ];
let inactiveStyles =
  Css.[
    borderBottom(`px(2), `solid, Colors.clear),
    color(LightBackground.main),
    hover([color(LightBackground.active)]),
  ];
let allStyles =
  Css.[
    paddingBottom(`em(0.8)),
    paddingTop(`em(0.8)),
    paddingLeft(`em(0.4)),
    paddingRight(`em(0.4)),
    marginRight(`em(1.8)),
    BaseStyles.floatLeft,
  ];

let component = ReasonReact.statelessComponent("Tab");
let make = (~isActive=false, ~onClick=?, children) => {
  ...component,
  render: _self =>
    <Link
      isDisabled=false
      ?onClick
      className={
        Css.style((isActive ? activeStyles : inactiveStyles) @ allStyles)
      }>
      ...children
    </Link>,
};