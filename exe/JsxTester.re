let div = (~className, ~children as _, ()) =>
  Printf.printf("div className=%s%!", className);

let d = <div className="test" />;

let () = print_endline("JSX");
