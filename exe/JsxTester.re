/* ReasonML convert <div className="test"/> to
   (div ~className:test ~children:[])[@JSX],

   let div = (~className as _, ~children as _, ()) => 43; */

let d = <div className="test" />;

print_endline(d); /* div */

let e = <span className="test2" />;

print_endline(e); /* span */
