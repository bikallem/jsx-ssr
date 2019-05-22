type htmlElement =
  | Text(string)
  | Normal(string, list(attribute), list(htmlElement))

and attribute =
  | KeyValue(string, string)
  | Boolean(string);

let str = txt => Text(txt);
let attr = (key, value) => KeyValue(key, value);
let flag = key => Boolean(key);

let createElement = (tag, attributes, ~children=[], ()) => {
  let attributes =
    List.map(((key, value)) => attr(key, value), attributes);
  Normal(tag, attributes, children);
};

/**
  * WIP html renderer. For the moment it just returns a parent node tag name.
  * TBC.
  */
module ViewBuilder = {
  let (+=) = (buf, text) => {
    Buffer.add_string(buf, text);
    buf;
  };
  let (+!) = (buf, text) => Buffer.add_string(buf, text);

  let render = htmlElement => {
    let buildHtmlElement = (buf, tag, attributes) => {
      Printf.(
        switch (attributes) {
        | [] => buf += "<" += tag +! ">"
        | _ =>
          buf += "<" +! tag;
          List.iter(
            attr =>
              switch (attr) {
              | KeyValue(k, v) => buf += " " += k += "=\"" += v +! "\""
              | Boolean(k) => buf += " " +! k
              },
            attributes,
          );
          buf +! ">";
        }
      );
    };

    let buildNormalTag = (buf, tag, attributes, children) => {
      buildHtmlElement(buf, tag, attributes);
      List.iter((child =>  
    };

    switch (htmlElement) {
    | Text(s) => s
    | Normal(tag, _, _) => tag
    };
  };
};
/* Sample DOM element creation of the following html element.
      <div id="container">
        <input value="foo" type="text"/>
        <a href="/bar">bar</a>
        <span onclick="myFunction('james')">Click Me!</span>
        <button onclick="alert('Hello world!')">Click Me!</button>
      </div>;
   */
let input =
  createElement("input", [("value", "foo"), ("type", "text")], ());
let a =
  createElement("a", [("href", "/bar")], ~children=[str("bar")], ());
let span =
  createElement(
    "span",
    [("onclick", "myFunction('james')")],
    ~children=[str("Click Me!")],
    (),
  );
let button =
  createElement(
    "button",
    [("onclick", "alert('Hello World')")],
    ~children=[str("Click Me!")],
    (),
  );
let block1 =
  createElement(
    "div",
    [("id", "container1")],
    ~children=[input, a, span, button],
    (),
  );

let () = Printf.printf("%B", render(block1) == "div");
