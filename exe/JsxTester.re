let div = (~className as _, ~children as _, ()) => 43;
/*     Printf.printf("div className=%s\n%!", className); */

let d = <div className="test" />;

print_endline(string_of_int(d));
