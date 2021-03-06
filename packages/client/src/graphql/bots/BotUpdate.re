module Query = [%graphql
  {|
        mutation botUpdate ($id: String!, $input: BotInput!) {
            botUpdate (id: $id, input: $input) {
             id
            }
        }
    |}
];

module Mutation = ReasonApolloHooks.Mutation.Make(Query);