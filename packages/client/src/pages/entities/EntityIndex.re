open Utils;

module ColumnsFunctor = (Ken: KenTools.KenModule) => {
  type record = BsKen.Graph_T.T.thing;
  type column = Table.column(BsKen.Graph_T.T.thing);

  let nameColumn =
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=
        (r: record) =>
          <Link linkType={Internal(EntityShow(r |> BsKen.Graph_T.Thing.id))}>
            {r |> Ken.getName |> ste}
          </Link>,
      ~flex=2,
      (),
    );

  let instanceOf =
    Table.Column.make(
      ~name="Instance Of" |> ste,
      ~render=(r: record) => r |> Ken.getInstanceOfName |> ste,
      (),
    );

  let idColumn =
    Table.Column.make(
      ~name="Name" |> ste,
      ~render=(r: record) => r |> BsKen.Graph_T.Thing.id |> ste,
      (),
    );

  let all = [|nameColumn, instanceOf, idColumn|];
};

[@react.component]
let make = () => {
  let context = React.useContext(Providers.app);
  module Config = {
    let globalSetting = context.globalSetting;
  };
  module Ken = KenTools.Functor(Config);
  module Columns = ColumnsFunctor(Ken);

  <SLayout head={<SLayout.TextDiv text="All Entities" />}>
    <Table columns=Columns.all rows=Ken.dataSource />
  </SLayout>;
};