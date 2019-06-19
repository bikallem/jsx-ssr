open Jsx_ssr;

/* Sample DOM createElement creation of the following html createElement.
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
    createElement(
      "input",
      [attr("value", "foo"), attr("type", "text")],
      (),
    );
  let input2 =
    createElement(
      "input",
      [attr("type", "checkbox"), flag("checked")],
      (),
    );
  let a =
    createElement(
      "a",
      [attr("href", "/bar")],
      ~children=[text("bar")],
      (),
    );
  let span =
    createElement(
      "span",
      [attr("onclick", "myFunction('james')")],
      ~children=[text("Click Me!")],
      (),
    );
  let button =
    createElement(
      "button",
      [attr("onclick", "alert('Hello World')")],
      ~children=[text("Click Me!")],
      (),
    );
  createElement(
    "div",
    [attr("id", "container1")],
    ~children=[input1, input2, a, span, button],
    (),
  );
};

print_endline(Html.renderDocument(block1));
