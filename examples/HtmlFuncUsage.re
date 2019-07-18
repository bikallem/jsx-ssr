open Jsx_ssr.Html;

/* Sample DOM createElement creation of the following html createElement.
      <div id="container">
        <input value="foo" type="text"/>
        <a href="/bar">bar</a>
        <span onclick="myFunction('james')">Click Me!</span>
        <button onclick="alert('Hello world!')">Click Me!</button>
      </div>;
   */
let block1 = {
  let input1 =
    E.createElement(
      "input",
      [A.attr("value", "foo"), A.attr("type", "text")],
      (),
    );
  let input2 =
    E.createElement(
      "input",
      [A.attr("type", "checkbox"), A.flag("checked")],
      (),
    );
  let a =
    E.createElement(
      "a",
      [A.attr("href", "/bar")],
      ~children=[E.text("bar")],
      (),
    );
  let span =
    E.createElement(
      "span",
      [A.attr("onclick", "myFunction('james')")],
      ~children=[E.text("Click Me!")],
      (),
    );
  let button =
    E.createElement(
      "button",
      [A.attr("onclick", "alert('Hello World')")],
      ~children=[E.text("Click Me!")],
      (),
    );
  E.createElement(
    "div",
    [A.attr("id", "container1")],
    ~children=[input1, input2, a, span, button],
    (),
  );
};

print_endline(E.renderAsDoc(block1));
