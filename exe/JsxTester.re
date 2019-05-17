let div = (~className, ~children as _, (), ()) =>
  Printf.printf("div className=%s\n%!", className);

let d = <div className="test" />;

d();
