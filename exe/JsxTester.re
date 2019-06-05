/* ReasonML converts <div className="test"/> to (div ~className:test ~children:[] ())[@JSX]. */
open Jsx_ssr;

let testAttr = "test";

let strAttr = "hello3";

let d =
  <div className=testAttr testcol2=232 test3='c' test4=123.33 strAttr>
    {Html.text("hello")}
    {Html.char('c')}
    {Html.int(1234)}
    {Html.float(1234.)}
    123
    "hello world"
    "&nbsp;"
    1234.0
    'Z'
    {Html.text(strAttr)}
    <span class_=""> <p> "hello" </p> </span>
  </div>;

print_endline(Html.renderHtmlDocument(d));

let e = <span className="test2" />;
print_endline(Html.renderHtmlDocument(e));

let m =
  <Custom className="hello" id="customId1"> <p> "Hello world" </p> </Custom>;
print_endline(Html.renderHtmlDocument(m));
