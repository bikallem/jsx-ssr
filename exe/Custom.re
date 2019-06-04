open Ppx_lab;

let createElement = (~className, ~id, ~children) => {
  <div className id> ...children </div>;
};
