type htmlElement =
  | Text(string)
  | Element(string, list(attribute), list(htmlElement))

and attribute =
  | KeyValue(string, string)
  | Boolean(string);

let attr = (key, value) => KeyValue(key, value);
let flag = key => Boolean(key);

let text = txt => Text(txt);
let char = char => text @@ String.make(1, char);
let int = int => text @@ string_of_int(int);
let float = float => text @@ string_of_float(float);
let element = (tag, attributes, ~children=[], ()) =>
  Element(tag, attributes, children);

/* View Rendering Functions */
let (+=) = (buf, text) => {
  Buffer.add_string(buf, text);
  buf;
};
let (+!) = (buf, text) => Buffer.add_string(buf, text);

let bufSize = 1024;

let rec buildElementTag = (buf, tag, attributes) => {
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
}

and buildElement = (buf, element) => {
  let closeTag = (buf, tag) => buf += "</" += tag +! ">";
  switch (element) {
  | Text(s) => buf +! s
  | Element(tag, attributes, children) =>
    buildElementTag(buf, tag, attributes);
    switch (children) {
    | [] => closeTag(buf, tag)
    | _ =>
      List.iter(elem => buildElement(buf, elem), children);
      closeTag(buf, tag);
    };
  };
};

let renderHtmlDocument = element => {
  let buf = Buffer.create(bufSize);
  buf +! "<!DOCTYPE html>";
  buildElement(buf, element);
  Buffer.contents(buf);
};

let renderHtmlElement = element => {
  let buf = Buffer.create(bufSize);
  buildElement(buf, element);
  Buffer.contents(buf);
};
