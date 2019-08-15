let toChannel = ch =>
  Primary.Channel.make(
    ~id=ch##id,
    ~name=ch##name,
    ~isArchived=false,
    ~isPublic=ch##isPublic,
    (),
  );

let toPreference = a =>
  Primary.Preference.make(
    ~id=a##id,
    ~stopAllEmails=a##stopAllEmails,
    ~enableExperimentalFeatures=a##enableExperimentalFeatures,
    (),
  );

let toAgent = a =>
  Primary.Agent.make(
    ~id=a##id,
    ~name=a##name,
    ~channelMemberships=None,
    ~preference=a##preference |> E.O.fmap(toPreference),
    (),
  );

let toBots = bots =>
  bots
  |> E.A.O.concatSome
  |> Array.map(bot =>
       Primary.Bot.make(
         ~id=bot##id,
         ~name=Some(bot##name),
         ~agent=
           bot##agent
           |> E.O.fmap(agent =>
                Primary.Agent.make(~id=agent##id, ~name=agent##name, ())
              ),
         (),
       )
     )
  |> E.O.some;

let toUser = a =>
  Primary.User.make(
    ~id=a##id,
    ~auth0Id=a##auth0Id,
    ~email=a##email,
    ~picture=a##picture,
    ~description=a##description,
    ~score=a##score,
    ~agent=a##agent |> E.O.fmap(toAgent),
    ~bots=a##bots |> toBots,
    (),
  );

module Query = [%graphql
  {|
    query user ($auth0Id: String) {
        user:
          user(auth0Id: $auth0Id) {
            id
            name
            email
            picture
            description
            auth0Id
            agentId
            score
            agent: Agent {
              id
              name
              preference: Preference {
                id
                stopAllEmails
                enableExperimentalFeatures
              }
            }
            bots: Bots {
              id
              name
              agent: Agent {
                id
                name
              }
            }
        }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let inner = innerComponentFn => {
  <QueryComponent>
    ...{({result}) =>
      result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(e => e##user |> E.O.fmap(toUser))
      |> HttpResponse.optionalToMissing
      |> (
        e =>
          switch (e) {
          | Success(c) =>
            innerComponentFn(Me.WithTokensAndUserData({userData: c}))
          | _ =>
            innerComponentFn(
              Me.WithTokensAndUserLoading({loadingUserData: e}),
            )
          }
      )
    }
  </QueryComponent>;
};