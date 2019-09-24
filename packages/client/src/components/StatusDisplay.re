let statusShow = (~measurable: Types.measurable) =>
  switch (
    Primary.Measurable.toStatus(measurable),
    measurable.expectedResolutionDate,
    measurable.stateUpdatedAt,
  ) {
  | (`OPEN, Some(resDate), _) =>
    FC.StateStatus.make(~state=OPEN(resDate), ~fontSize=`em(0.85), ())
  | (`JUDGEMENT_PENDING, Some(resDate), _) =>
    FC.StateStatus.make(~state=PENDING(resDate), ~fontSize=`em(0.85), ())
  | (`CLOSED_AS_UNRESOLVED, _, Some(resolvedAt))
  | (`JUDGED, _, Some(resolvedAt)) =>
    FC.StateStatus.make(
      ~state=RESOLVED(resolvedAt),
      ~fontSize=`em(0.85),
      (),
    )
  | (_, _, _) =>
    Js.log("Missing data in statusShow");
    ReasonReact.null;
  };

let component = ReasonReact.statelessComponent("StatusDisplay");

let make = (~measurable: Types.measurable, _children) => {
  ...component,
  render: _self => statusShow(~measurable),
};