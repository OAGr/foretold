module Columns = {
  type record = Types.leaderboardItem;
  type column = Table.column(record);

  let getName = (r: record) =>
    switch (r.measurable) {
    | Some(measurable) =>
      <Link
        linkType={
          Internal(MeasurableShow(measurable.channelId, measurable.id))
        }>
        {measurable.name |> E.S.default("Question") |> Utils.ste}
      </Link>
    | _ => "Question" |> Utils.ste
    };

  let measurable =
    Table.Column.make(
      ~name="Question" |> Utils.ste,
      ~render=(r: record) => r |> getName,
      ~flex=4,
      (),
    );

  let agent =
    Table.Column.make(
      ~name="Member" |> Utils.ste,
      ~render=
        (r: record) =>
          r.agent
          |> E.O.fmap((agent: Types.agent) => <AgentLink agent />)
          |> E.O.default("Member" |> Utils.ste),
      (),
    );

  let channel =
    Table.Column.make(
      ~name="Community" |> Utils.ste,
      ~render=
        (r: record) =>
          r.channel
          |> E.O.fmap((channel: Types.channel) =>
               <Link linkType={Internal(ChannelShow(channel.id))}>
                 {channel.name |> Utils.ste}
               </Link>
             )
          |> E.O.default("Community" |> Utils.ste),
      (),
    );

  let score =
    Table.Column.make(
      ~name="Score" |> Utils.ste,
      ~render=
        (r: record) =>
          r.pointScore
          |> E.O.fmap(E.Float.with3DigitsPrecision)
          |> E.O.default("")
          |> Utils.ste,
      (),
    );

  let timeAveragedScore =
    Table.Column.make(
      ~name="Time-Average Score" |> Utils.ste,
      ~help=
        Some({
          headerContent: "Time-Average Score" |> Utils.ste,
          bodyContent:
            "The average difference in log error between this member's predictions and the aggregate prediction, over the course of the life of the question."
            |> Utils.ste,
        }),
      ~render=
        (r: record) =>
          r.pointScore
          |> E.O.fmap(E.Float.with3DigitsPrecision)
          |> E.O.default("")
          |> Utils.ste,
      (),
    );

  let timeAveragedScore2 =
    Table.Column.make(
      ~name="Time-Average Score" |> Utils.ste,
      ~help=
        Some({
          headerContent: "Participant Time Average Score" |> Utils.ste,
          bodyContent:
            "The average difference in log error between this member's predictions and the aggregate prediction, over the course of the life of user's participation."
            |> Utils.ste,
        }),
      ~render=
        (r: record) =>
          r.timeAverageScore
          |> E.O.fmap((r: Types.timeAverageScore) => r.score)
          |> E.O.fmap(E.Float.with2DigitsPrecision)
          |> E.O.default("")
          |> Utils.ste,
      (),
    );

  let timeActivityRatio =
    Table.Column.make(
      ~name="Percent of Time Active" |> Utils.ste,
      ~help=
        Some({
          headerContent: "Percent of Time Active" |> Utils.ste,
          bodyContent:
            "The percentage of the question's open time when the agent was forecasting."
            |> Utils.ste,
        }),
      ~render=
        (r: record) =>
          r.timeAverageScore
          |> E.O.fmap((r: Types.timeAverageScore) => r.timeActivityRatio)
          |> E.O.fmap(e => e *. 100.)
          |> E.O.fmap(E.Float.with3DigitsPrecision)
          |> E.O.fmap(e => e ++ "%")
          |> E.O.default("")
          |> Utils.ste,
      (),
    );

  let totalScore =
    Table.Column.make(
      ~name="Total Net Log Score" |> Utils.ste,
      ~help=
        Some({
          headerContent: "Total Time-Averaged Log Score" |> Utils.ste,
          bodyContent:
            "The sum of all time-averaged scores by this member." |> Utils.ste,
        }),
      ~render=
        (r: record) =>
          r.pointScore
          |> E.O.fmap(E.Float.with3DigitsPrecision)
          |> E.O.default("")
          |> Utils.ste,
      (),
    );

  let predictionCount =
    Table.Column.make(
      ~name="Predictions" |> Utils.ste,
      ~render=
        (r: record) =>
          r.predictionCountTotal
          |> E.O.fmap(E.I.toString)
          |> E.O.default("")
          |> Utils.ste,
      (),
    );

  let predictedMeasurablesCount =
    Table.Column.make(
      ~name="Predicted Questions" |> Utils.ste,
      ~render=
        (r: record) =>
          r.numberOfQuestionsScored
          |> E.O.fmap(E.I.toString)
          |> E.O.default("")
          |> Utils.ste,
      (),
    );

  let totalVotesReceived =
    Table.Column.make(
      ~name="Total Upvotes" |> Utils.ste,
      ~render=
        (r: record) =>
          r.totalVotesReceived
          |> E.O.fmap(E.I.toString)
          |> E.O.default("")
          |> Utils.ste,
      (),
    );

  let getMeasurement = measurement => {
    let bounds = MeasurementsTable.Helpers.bounds([|measurement|]);
    <MeasurementsTable.Helpers.SmallDistribution
      measurement
      bounds
      width=75
      height=30
    />;
  };

  let competitiveMeasurement =
    Table.Column.make(
      ~name="Prediction" |> Utils.ste,
      ~render=
        (r: record) =>
          r.competitiveMeasurement
          |> E.O.fmap(getMeasurement)
          |> E.O.default("" |> Utils.ste),
      (),
    );

  let aggregationMeasurement =
    Table.Column.make(
      ~name="Recent Aggregate" |> Utils.ste,
      ~render=
        (r: record) =>
          r.aggregationMeasurement
          |> E.O.fmap(getMeasurement)
          |> E.O.default("" |> Utils.ste),
      (),
    );

  let measurables = [|
    agent,
    measurable,
    timeAveragedScore2,
    timeActivityRatio,
    predictionCount,
    totalVotesReceived,
  |];

  let measurables' = [|
    agent,
    timeAveragedScore2,
    timeActivityRatio,
    predictionCount,
    totalVotesReceived,
  |];

  let notebooks = [|agent, timeAveragedScore2, timeActivityRatio|];

  let members = [|
    agent,
    totalScore,
    predictedMeasurablesCount,
    predictionCount,
    totalVotesReceived,
  |];

  let members' = [|
    channel,
    totalScore,
    predictedMeasurablesCount,
    predictionCount,
    totalVotesReceived,
  |];
};

[@react.component]
let make = (~items, ~columns=Columns.measurables) =>
  <Table columns rows=items />;