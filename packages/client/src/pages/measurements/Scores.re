open Style.Grid;

let head = (~channelId, ~paginationPage, ()) =>
  <PaginationX paginationPage />;

let component = ReasonReact.statelessComponent("Scores");
let make = (~measurableId: string, ~loggedInUser: Types.user, _children) => {
  ...component,
  render: _ => {
    <LeaderboardMeasurables channelId=None head />;
  },
};