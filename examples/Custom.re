open Jsx_ssr;

let createElement = (~name, ~className, ~id, ~children) => {  
  <div className id type_="test">
    ...[<HelloWorld name/>, ...children]
  </div>;  
};
