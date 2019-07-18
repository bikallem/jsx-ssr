open Jsx_ssr;
open Jsx_ssr.Html;

let testAttr = "test";

let strAttr = "hello3";

let d =
  <div
    className=testAttr
    testcol2=232
    test3='c'
    test4=123.33
    strAttr
    onClock="alert('hello')">
    {E.text("hello")}
    {E.char('c')}
    {E.int(1234)}
    {E.float(1234.)}
    123
    "hello world"
    "&nbsp;"
    1234.0
    'Z'
    {E.text(strAttr)}
    <span class_=""> <p> "hello" </p> </span>
  </div>;

print_endline(E.renderAsDoc(d));

let e = <span className="test2" />;
print_endline(E.renderAsDoc(e));

let m =
  <Custom name="James Bond" className="hello" id="customId1">
    <p> "Hello world" </p>
  </Custom>;
print_endline(E.render(m));
