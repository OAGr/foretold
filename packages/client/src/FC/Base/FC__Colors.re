/* // this is used to show hex color; */
let removeHex = Js.String.sliceToEnd(~from=1);
let r = c => c->removeHex->(e => `hex(e));

let white = "#FFFFFF"->r;
let black = "#000000"->r;
let greyO4 = "#00000044"->r;
let whiteO2 = "#ffffff20"->r;
let whiteO4 = "#ffffff40"->r;
let whiteOc = "#ffffffc0"->r;
let clear = "#00000000"->r;

let textDarker = "#333333"->r;
let textDark = "#5f6d7d"->r;
let textMedium = "#9a9ea7"->r;
let smokeWhite = "#F0F1F3"->r;
let lightGrayBackground = "#f4f6f9"->r;
let lighterGrayBackground = "#fbfcfd"->r;
let grayBackground = "#dcdee0"->r;
let greydisabled = "#e3e4e6"->r;
let accentBlue = "#8C9EB5"->r;
let accentBlueO8 = "#8C9EB540"->r;
let accentBlue1a = "#8c9eb530"->r;
let mainBlue = "#347296"->r;
let link = "#4a72b7"->r;
let linkAccent = "#437bff"->r;
let darkLink = "#1a2e45"->r;
let darkAccentBlue = "#5C6E95"->r;
let grey1 = "#868686"->r;
let border = "#D5D7DA"->r;
let primary = mainBlue;

module FontWeights = {
  let light = Css.fontWeight(`num(300));
  let regular = Css.fontWeight(`num(400));
  let heavy = Css.fontWeight(`num(700));
  let veryHeavy = Css.fontWeight(`num(900));
};

module BorderRadius = {
  let medium = `px(5);
};

module Statuses = {
  let green = "#689533"->r;
  let yellow = "#C09C66"->r;
  let resolved = accentBlue;
};

module Text = {
  module LightBackground = {
    let main = textDark;
    let p = "#777c88"->r;
    let light = accentBlue;
    let active = "#3562AE"->r;
  };
};