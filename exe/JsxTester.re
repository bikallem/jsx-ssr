/* ReasonML converts <div className="test"/> to (div ~className:test ~children:[] ())[@JSX]. */
open Ppx_lab;

/* Sample DOM element creation of the following html element.
      <div id="container">
        <input value="foo" type="text"/>
        <a href="/bar">bar</a>
        <span onclick="myFunction('james')">Click Me!</span>
        <button onclick="alert('Hello world!')">Click Me!</button>
      </div>;
   */
let block1 = {
  open Html;

  let input1 =
    element("input", [attr("value", "foo"), attr("type", "text")], ());
  let input2 =
    element("input", [attr("type", "checkbox"), flag("checked")], ());
  let a =
    element("a", [attr("href", "/bar")], ~children=[text("bar")], ());
  let span =
    element(
      "span",
      [attr("onclick", "myFunction('james')")],
      ~children=[text("Click Me!")],
      (),
    );
  let button =
    element(
      "button",
      [attr("onclick", "alert('Hello World')")],
      ~children=[text("Click Me!")],
      (),
    );
  element(
    "div",
    [attr("id", "container1")],
    ~children=[input1, input2, a, span, button],
    (),
  );
};

print_endline(Html.renderHtmlDocument(block1));

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
