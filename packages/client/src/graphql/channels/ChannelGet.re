open Utils;

module Query = [%graphql
  {|
    query channel ($id: String!) {
      channel (id: $id) {
        id
        name
        description
        isArchived
        isPublic
        requireVerification
        membershipCount
        notebooksCount
        myRole
        knowledgeGraph
        bookmarksCount
        isBookmarked
        permissions {
          mutations {
            allow
          }
        }
      }
    }
  |}
];

module QueryComponent = ReasonApollo.CreateQuery(Query);

let toChannel = channel => {
  let allowMutations =
    channel##permissions##mutations##allow |> E.A.O.concatSome |> E.A.to_list;

  let permissions = Primary.Permissions.make(allowMutations);

  Primary.Channel.make(
    ~id=channel##id,
    ~name=channel##name |> E.J.toString,
    ~description=channel##description |> E.J.O.toString,
    ~isArchived=channel##isArchived,
    ~isPublic=channel##isPublic,
    ~notebooksCount=Some(channel##notebooksCount),
    ~myRole=Some(channel##myRole),
    ~membershipCount=Some(channel##membershipCount),
    ~permissions=Some(permissions),
    ~knowledgeGraph=channel##knowledgeGraph |> E.J.O.toString,
    ~bookmarksCount=Some(channel##bookmarksCount),
    ~isBookmarked=channel##isBookmarked,
    ~requireVerification=channel##requireVerification,
    (),
  );
};

let component = (~id, fn) => {
  let query = Query.make(~id, ());
  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> ApolloUtils.apolloResponseToResult
      |> E.R.bind(_, e =>
           switch (e##channel |> E.O.fmap(toChannel)) {
           | Some(r) => Ok(r)
           | None => Error("Community Not Found" |> ste)
           }
         )
      |> E.R.fmap(fn)
      |> E.R.id
    }
  </QueryComponent>;
};

let getChannelByIdAsComponent = (~id, innerFn) => {
  let query = Query.make(~id, ());
  <QueryComponent variables=query##variables>
    ...{({result}) =>
      result
      |> HttpResponse.fromApollo
      |> HttpResponse.fmap(e => e##channel |> E.O.fmap(toChannel))
      |> HttpResponse.optionalToMissing
      |> innerFn
    }
  </QueryComponent>;
};

let component2 = (~id, innerFn) => {
  switch (id) {
  | "" => HttpResponse.Success(Primary.Channel.getGlobalChannel()) |> innerFn
  | _ => getChannelByIdAsComponent(~id, innerFn)
  };
};