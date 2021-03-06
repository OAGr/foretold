module Query = [%graphql
  {|
             mutation measurableUnarchive($id: String!) {
                 measurableUnarchive(id: $id) {
                   id
                 }
             }
     |}
];

module Mutation = ReasonApollo.CreateMutation(Query);

let mutate = (mutation: Mutation.apolloMutation, id: string) => {
  let m = Query.make(~id, ());
  mutation(
    ~variables=m##variables,
    ~refetchQueries=[|
      "agent",
      "measurables",
      "measurablesStateStats",
    |],
    (),
  )
  |> ignore;
};