/* ReasonML converts <div className="test"/> to (div ~className:test ~children:[] ())[@JSX]. */
open Ppx_lab;

let testAttr = "test";

let strAttr = "hello3";

let d = <div className=testAttr testcol2=232 test3='c' test4=123.33 strAttr />;

print_endline(Html.renderHtmlDocument(d));

let e = <span className="test2" />;

print_endline(Html.renderHtmlDocument(e));
