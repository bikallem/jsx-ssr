open Jsx_ssr;

let createElement = (~name, ~children as _) => {
  <div style="color:red"> {Html.text("Hello World " ++ name ++ "!")} </div>;
};
