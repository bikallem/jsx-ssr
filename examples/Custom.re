open Jsx_ssr;

let createElement = (~name, ~className, ~id, ~children) => {
  let elems = [<HelloWorld name />, ...children];
  <div className id type_="test"> ...elems </div>;
};
