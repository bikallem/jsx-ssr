open Jsx_ssr;

module V = Html.View;

let createElement = (~name, ~children as _) => {
  <div style="color:red"> {Html.V.text("Hello World " ++ name ++ "!")} </div>;
};
