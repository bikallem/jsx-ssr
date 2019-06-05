open Jsx_ssr;

let createElement = (~className, ~id, ~children) => {
  <div className id> ...children </div>;
};
