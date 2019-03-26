module Route = {
  type t =
    | Home
    | AgentIndex
    | Redirect
    | Login
    | Profile
    | EntityShow(string)
    | AgentShow(string)
    | AgentMeasurables(string)
    | ChannelShow(string)
    | ChannelIndex
    | ChannelNew
    | MeasurableEdit(string)
    | MeasurableNew(string)
    | Series(string, string)
    | NotFound;

  let fromUrl = (url: ReasonReact.Router.url) =>
    switch (url.path) {
    | [] => Home
    | ["login"] => Login
    | ["callback"] =>
      Context__Auth.CallbackUrlToAuthTokens.make(url)
      |> E.O.fmap(Context__Auth.AuthTokens.set)
      |> E.O.default();
      Redirect;
    | ["redirect"] => Redirect
    | ["agents"] => AgentIndex
    | ["profile"] => Profile
    | ["agents", id] => AgentShow(id)
    | ["entities", ...id] => EntityShow(String.concat("/", id))
    | ["agents", id, "measurables"] => AgentMeasurables(id)
    | ["channels", "new"] => ChannelNew
    | ["channels"] => ChannelIndex
    | ["c"] => ChannelIndex
    | ["c", id] => ChannelShow(id)
    | ["c", id, "new"] => MeasurableNew(id)
    | ["measurables", id, "edit"] => MeasurableEdit(id)
    | ["c", channel, "s", id] => Series(channel, id)
    | _ => NotFound
    };
};

module Url = {
  type t =
    | Home
    | AgentIndex
    | Profile
    | EntityShow(string)
    | AgentShow(string)
    | AgentMeasurables(string)
    | ChannelShow(string)
    | ChannelNew
    | ChannelIndex
    | SeriesShow(string, string)
    | MeasurableEdit(string)
    | MeasurableNew(string);

  let toString = (r: t) =>
    switch ((r: t)) {
    | Home => "/"
    | AgentIndex => "/agents"
    | Profile => "/profile/"
    | EntityShow(id) => "/entities/" ++ id
    | AgentShow(id) => "/agents/" ++ id
    | AgentMeasurables(id) => "/agents/" ++ id ++ "/measurables"
    | ChannelNew => "/channels/" ++ "new"
    | ChannelShow(id) => "/c/" ++ id
    | ChannelIndex => "/channels"
    | MeasurableEdit(id) => "/measurables/" ++ id ++ "/edit"
    | MeasurableNew(channel) => "/c/" ++ channel ++ "/new"
    | SeriesShow(channel, id) => "/c/" ++ channel ++ "/s/" ++ id
    };

  let push = (r: t) => r |> toString |> ReasonReact.Router.push;
};