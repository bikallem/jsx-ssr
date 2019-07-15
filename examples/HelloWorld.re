open Jsx_ssr;
open Jsx_ssr.Html;

let createElement = (~name, ~children as _) => {
  <div style="color:red"> {E.text("Hello World " ++ name ++ "!")} </div>;
};
