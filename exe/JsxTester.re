/* ReasonML convert <div className="test"/> to
   (div ~className:test ~children:[])[@JSX],

   let div = (~className as _, ~children as _, ()) => 43; */
open Ppx_lab;

let testAttr = "test";

let d = <div className=testAttr testcol2=232 test3='c' test4=123.33 />;

print_endline(Html.renderHtmlDocument(d)); /* div */

let e = <span className="test2" />;

print_endline(Html.renderHtmlDocument(e)); /* span */
