open Jsx_ssr;

let testAttr = "test";

let strAttr = "hello3";

let d =
  <div className=testAttr testcol2=232 test3='c' test4=123.33 strAttr>
    {Html.V.text("hello")}
    {Html.V.char('c')}
    {Html.V.int(1234)}
    {Html.V.float(1234.)}
    123
    "hello world"
    "&nbsp;"
    1234.0
    'Z'
    {Html.V.text(strAttr)}
    <span class_=""> <p> "hello" </p> </span>
  </div>;

print_endline(Html.V.renderAsDoc(d));

let e = <span className="test2" />;
print_endline(Html.V.renderAsDoc(e));

let m =
  <Custom name="James Bond" className="hello" id="customId1">
    <p> "Hello world" </p>
  </Custom>;
print_endline(Html.V.render(m));
