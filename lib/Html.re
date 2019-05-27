type htmlElement =
  | Text(string)
  | Element(string, list(attribute), list(htmlElement))

and attribute =
  | KeyValue(string, string)
  | Boolean(string);

let attr = (key, value) => KeyValue(key, value);
let flag = key => Boolean(key);

let text = txt => Text(txt);
let element = (tag, attributes, ~children=[], ()) =>
  Element(tag, attributes, children);

module ViewBuilder = {
  let (+=) = (buf, text) => {
    Buffer.add_string(buf, text);
    buf;
  };
  let (+!) = (buf, text) => Buffer.add_string(buf, text);

  let bufSize = 1024;

  let builder = {
    as self;
    pub buildElementTag = (buf, tag, attributes) => {
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
    pub buildElement = (buf, element) => {
      switch (element) {
      | Text(s) => buf +! s
      | Element(tag, attributes, children) =>
        self#buildElementTag(buf, tag, attributes);
        List.iter(elem => self#buildElement(buf, elem), children);
      };
    };
    pub renderHtmlDocument = element => {
      let buf = Buffer.create(bufSize);
      buf +! "<!DOCTYPE html>";
      self#buildElement(buf, element);
      Buffer.contents(buf);
    };
    pub renderHtmlElement = element => {
      let buf = Buffer.create(bufSize);
      self#buildElement(buf, element);
      Buffer.contents(buf);
    }
  };
};

let renderHtmlDocument = ViewBuilder.builder#renderHtmlDocument;
let renderHtmlElement = ViewBuilder.builder#renderHtmlElement;

/* Sample DOM element creation of the following html element.
      <div id="container">
        <input value="foo" type="text"/>
        <a href="/bar">bar</a>
        <span onclick="myFunction('james')">Click Me!</span>
        <button onclick="alert('Hello world!')">Click Me!</button>
      </div>;
   */
let input1 =
  element("input", [attr("value", "foo"), attr("type", "text")], ());
let input2 =
  element("input", [attr("type", "checkbox"), flag("checked")], ());
let a = element("a", [attr("href", "/bar")], ~children=[text("bar")], ());
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
let block1 =
  element(
    "div",
    [attr("id", "container1")],
    ~children=[input1, input2, a, span, button],
    (),
  );

/* let () = Printf.printf("%s", ViewBuilder.renderHtmlDocument(block1)); */
