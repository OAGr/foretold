[@bs.config {jsx: 3}];

open Antd;
open BsReform;

module FormConfig = [%lenses type state = {email: string}];

module Form = ReForm.Make(FormConfig);

let schema =
  Form.Validation.Schema([|
    Email(Email),
    Custom(
      Email,
      values =>
        Js.String.length(values.email) > 64
          ? ReSchema.Error("Keep it short!") : Valid,
    ),
    Custom(
      Email,
      values =>
        Js.String.length(values.email) < 3
          ? Error("The name too short.") : Valid,
    ),
  |]);

module FormComponent = {
  [@react.component]
  let make =
      (
        ~reform: Form.api,
        ~result: ReasonApolloHooks.Mutation.controledVariantResult('a),
      ) => {
    let onSubmit = event => {
      ReactEvent.Synthetic.preventDefault(event);
      reform.submit();
    };

    <Form.Provider value=reform>
      {switch (result) {
       | Error(_error) => <p> {"Something went wrong..." |> Utils.ste} </p>
       | Data(_) => <AntdAlert message=Lang.memberInvited type_="success" />
       | _ =>
         <Antd.Form onSubmit>
           <Form.Field
             field=FormConfig.Email
             render={({handleChange, error, value}) =>
               <>
                 <Antd.Form.Item label={"Email*" |> Utils.ste}>
                   <Antd.Input
                     value
                     onChange={event =>
                       ReactEvent.Form.target(event)##value |> handleChange
                     }
                   />
                   {E.React2.showIf(
                      error->Belt.Option.getWithDefault("") != "",
                      <AntdAlert
                        message={error->Belt.Option.getWithDefault("")}
                        type_="warning"
                      />,
                    )}
                 </Antd.Form.Item>
               </>
             }
           />
           <Antd.Form.Item>
             <Button
               _type=`primary onClick=onSubmit icon=Antd.IconName.usergroupAdd>
               {"Email an Invitation" |> Utils.ste}
             </Button>
           </Antd.Form.Item>
         </Antd.Form>
       }}
    </Form.Provider>;
  };
};

module Create = {
  [@react.component]
  let make = (~channelId) => {
    let (mutate, result, _) = InvitationCreate.Mutation.use();

    let reform =
      Form.use(
        ~validationStrategy=OnDemand,
        ~schema,
        ~onSubmit=
          ({state}) => {
            mutate(
              ~variables=
                InvitationCreate.Query.make(
                  ~input={
                    "email": state.values.email,
                    "channelId": channelId,
                  },
                  (),
                )##variables,
              (),
            )
            |> ignore;

            None;
          },
        ~initialState={email: ""},
        (),
      );

    <FormComponent reform result />;
  };
};

[@react.component]
let make = (~channelId: string) => {
  <SLayout head={SLayout.Header.textDiv("Invite Member")}>
    <FC.PageCard.BodyPadding> <Create channelId /> </FC.PageCard.BodyPadding>
  </SLayout>;
};