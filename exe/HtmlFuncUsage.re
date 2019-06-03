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
