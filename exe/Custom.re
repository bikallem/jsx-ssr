open Jsx_ssr;

let createElement = (~name, ~className, ~id, ~children) => {  
  <div className id>     
    ...[<HelloWorld name/>, ...children]
  </div>;  
};
