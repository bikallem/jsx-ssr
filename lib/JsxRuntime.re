type htmlElement =
  | Text(string)
  | Element(string, list(attribute), list(htmlElement))

and attribute =
  | KeyValue(string, string)
  | Boolean(string);

let str = txt => Text(txt);
let attr = ((key, value)) => KeyValue(key, value);
let flag = key => Boolean(key);

let createElement = (tag, attributes, ~children=[], ()) => {
  let attributes = List.map(attr, attributes);
  Element(tag, attributes, children);
};

module ViewBuilder: {
  let renderHtmlDocument: htmlElement => string;
  let renderHtmlElement: htmlElement => string;
} = {
  let (+=) = (buf, text) => {
    Buffer.add_string(buf, text);
    buf;
  };
  let (+!) = (buf, text) => Buffer.add_string(buf, text);

  let bufSize = 1024;

  let builder = {
    as self;
    pub buildElement = (buf, tag, attributes) => {
      switch (attributes) {
      | [] => buf += "<" += tag +! ">"
      | _ =>
        buf += "<" +! tag;
        List.iter(
          fun
          | KeyValue(k, v) => buf += " " += k += "=\"" += v +! "\""
          | Boolean(k) => buf += " " +! k,
          attributes,
        );
        buf +! ">";
      };
    };
    pub renderElement = (buf, element) => {
      switch (element) {
      | Text(s) => buf +! s
      | Element(tag, attributes, children) =>
        self#buildElement(buf, tag, attributes);
        List.iter(elem => self#renderElement(buf, elem), children);
      };
    };
    pub renderHtmlDocument = element => {
      let buf = Buffer.create(bufSize);
      buf +! "<!DOCTYPE html>";
      self#renderElement(buf, element);
      Buffer.contents(buf);
    };
    pub renderHtmlElement = element => {
      let buf = Buffer.create(bufSize);
      self#renderElement(buf, element);
      Buffer.contents(buf);
    }
  };

  let renderHtmlDocument = builder#renderHtmlDocument;
  let renderHtmlElement = builder#renderHtmlElement;
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

let () = Printf.printf("%s", ViewBuilder.renderHtmlDocument(block1));
