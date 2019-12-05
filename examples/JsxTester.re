open Jsx_ssr;

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

Html.pp(Format.std_formatter, d);

let e = <span className="test2" />;
Html.pp(Format.std_formatter, e);

let m =
  <Custom name="James Bond" className="hello" id="customId1">
    <p> "Hello world" </p>
  </Custom>;
Html.pp(Format.std_formatter, m);
