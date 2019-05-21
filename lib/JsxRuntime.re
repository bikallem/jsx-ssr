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

/* Sample DOM element creation */
let input = createElement("input", [("value", "foo"), ("type", "text")], ());
let a = createElement("a", [("href", "/bar")], ~children=[str("bar")], ());
let span = createElement("span", [("onclick", "myFunction('james')")], ~children=[str("Click Me!")], ());
let button = createElement("button", [("onclick", "alert('Hello World')")], ~children=[str("Click Me!")], ());

let block1 = createElement("div", [("id", "container1")], ~children=[input,a,span,button], ());
