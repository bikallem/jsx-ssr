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
  let input1 =
    Html.createElement(
      "input",
      [Attribute.attr("value", "foo"), Attribute.attr("type", "text")],
      (),
    );
  let input2 =
    Html.createElement(
      "input",
      [Attribute.attr("type", "checkbox"), Attribute.flag("checked")],
      (),
    );
  let a =
    Html.createElement(
      "a",
      [Attribute.attr("href", "/bar")],
      ~children=[Html.text("bar")],
      (),
    );
  let span =
    Html.createElement(
      "span",
      [Attribute.attr("onclick", "myFunction('james')")],
      ~children=[Html.text("Click Me!")],
      (),
    );
  let button =
    Html.createElement(
      "button",
      [Attribute.attr("onclick", "alert('Hello World')")],
      ~children=[Html.text("Click Me!")],
      (),
    );
  Html.createElement(
    "div",
    [Attribute.attr("id", "container1")],
    ~children=[input1, input2, a, span, button],
    (),
  );
};

print_endline(Html.renderAsDoc(block1));
