let toAgent = c => Primary.Agent.make(~id=c##id, ~name=c##name, ());

let toSeries = c =>
  Primary.Series.make(
    ~id=c##id,
    ~description=c##description,
    ~name=c##name,
    (),
  );

let toMeasurable = m => {
  let channel =
    m##channel
    |> E.O.fmap(channel =>
         Primary.Channel.make(
           ~id=channel##id,
           ~name=channel##name,
           ~isArchived=channel##isArchived,
           ~isPublic=channel##isPublic,
           (),
         )
       );

  let outcome =
    m##outcome
    |> E.O.fmap(measurement =>
         Primary.Measurement.make(
           ~id=measurement##id,
           ~valueText=measurement##valueText,
           ~value=measurement##value |> MeasurementValue.decodeGraphql,
           (),
         )
       );

  let previousAggregate =
    m##previousAggregate
    |> E.O.fmap(measurement =>
         Primary.Measurement.make(
           ~id=measurement##id,
           ~valueText=measurement##valueText,
           ~value=measurement##value |> MeasurementValue.decodeGraphql,
           (),
         )
       );

  Primary.Measurable.make(
    ~id=m##id,
    ~name=m##name,
    ~channelId=m##channelId,
    ~channel,
    ~isArchived=Some(m##isArchived),
    ~valueType=m##valueType,
    ~labelCustom=m##labelCustom,
    ~resolutionEndpoint=m##resolutionEndpoint,
    ~measurementCount=m##measurementCount,
    ~measurerCount=m##measurerCount,
    ~labelSubject=m##labelSubject,
    ~labelOnDate=m##labelOnDate,
    ~labelProperty=m##labelProperty,
    ~createdAt=Some(m##createdAt),
    ~updatedAt=Some(m##updatedAt),
    ~expectedResolutionDate=m##expectedResolutionDate,
    ~state=Some(m##state),
    ~stateUpdatedAt=m##stateUpdatedAt,
    ~creator=E.O.fmap(toAgent, m##creator),
    ~series=E.O.fmap(toSeries, m##series),
    ~iAmOwner=Some(m##iAmOwner),
    ~min=m##min,
    ~max=m##max,
    ~outcome,
    ~previousAggregate,
    (),
  );
};

module Query = [%graphql
  {|
    query getMeasurables (
        $states: [measurableState]!
        $channelId: String
        $seriesId: String
        $creatorId: String
        $first: Int
        $last: Int
        $after: String
        $before: String
    ) {
        measurables: measurables(
            states: $states
            channelId: $channelId
            seriesId: $seriesId
            creatorId: $creatorId
            first: $first
            last: $last
            after: $after
            before: $before
        ) {
          total
          pageInfo{
            hasPreviousPage
            hasNextPage
            startCursor
            endCursor
          }
          edges{
            node {
              id
              name
              channelId
              isArchived
              channel: Channel {
                id
                name
                description
                isPublic
                isArchived
              }
              labelCustom
              resolutionEndpoint
              valueType
              measurementCount
              measurerCount
              labelSubject
              labelProperty
              iAmOwner
              min
              max
              state
              labelOnDate @bsDecoder(fn: "E.J.O.toMoment")
              stateUpdatedAt @bsDecoder(fn: "E.J.O.toMoment")
              expectedResolutionDate @bsDecoder(fn: "E.J.O.toMoment")
              createdAt @bsDecoder(fn: "E.J.toMoment")
              updatedAt @bsDecoder(fn: "E.J.toMoment")
              creator {
                id
                name
              }
              series {
                id
                name
                description
              }
              outcome {
                  id
                  valueText
                  value {
                      floatCdf { xs ys }
                      floatPoint
                      percentage
                      binary
                      unresolvableResolution
                      comment
                  }
              }
              previousAggregate {
                  id
                  valueText
                  value {
                      floatCdf { xs ys }
                      floatPoint
                      percentage
                      binary
                      unresolvableResolution
                      comment
                  }
              }
            }
          }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let unpackEdges = a =>
  a##measurables |> E.O.fmap(Primary.Connection.fromJson(toMeasurable));

type measurableStates = Types.measurableState;

type direction = Primary.Connection.direction;

let queryDirection =
    (
      ~states: Js.Array.t(option(measurableStates)),
      ~seriesId=?,
      ~channelId=?,
      ~creatorId=?,
      ~pageLimit,
      ~direction,
      (),
    ) => {
  let fn = Query.make(~seriesId?, ~channelId?, ~creatorId?, ~states);
  switch ((direction: direction)) {
  | None => fn(~first=pageLimit, ())
  | After(after) => fn(~first=pageLimit, ~after, ())
  | Before(before) => fn(~last=pageLimit, ~before, ())
  };
};

let componentMaker = (query, innerComponentFn) =>
  <QueryComponent variables=query##variables>
    ...{o =>
      o.result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(unpackEdges)
      |> HttpResponse.optionalToMissing
      |> innerComponentFn
    }
  </QueryComponent>;

let component =
    (
      ~creatorId=None,
      ~seriesId=None,
      ~channelId=None,
      ~pageLimit,
      ~states,
      ~direction: direction,
      ~innerComponentFn,
      (),
    ) => {
  let query =
    queryDirection(
      ~channelId?,
      ~seriesId?,
      ~creatorId?,
      ~pageLimit,
      ~direction,
      ~states,
      (),
    );
  componentMaker(query, innerComponentFn);
};