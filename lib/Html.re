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
  switch (element) {
  | Text(s) => buf +! s
  | Element(tag, attributes, children) =>
    buildElementTag(buf, tag, attributes);
    switch (children) {
    | [] => buf +! Printf.sprintf("</%s>", tag)
    | _ => List.iter(elem => buildElement(buf, elem), children)
    };
  };
}

and renderHtmlDocument = element => {
  let buf = Buffer.create(bufSize);
  buf +! "<!DOCTYPE html>";
  buildElement(buf, element);
  Buffer.contents(buf);
}

and renderHtmlElement = element => {
  let buf = Buffer.create(bufSize);
  buildElement(buf, element);
  Buffer.contents(buf);
};
