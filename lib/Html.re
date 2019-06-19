type element =
  | Text(string)
  | Element{
      tag: string,
      attributes: list(attribute),
      children: list(element),
    }

and attribute =
  | KeyValue{
      key: string,
      value: string,
    }
  | Boolean(string);

let attr = (key, value) => KeyValue({key, value});
let flag = key => Boolean(key);

let text = txt => Text(txt);
let char = char => text @@ String.make(1, char);
let int = int => text @@ string_of_int(int);
let float = float => text @@ string_of_float(float);
let createElement = (tag, attributes, ~children=[], ()) =>
  Element({tag, attributes, children});

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
      | KeyValue({key, value}) => buf += " " += key += "=\"" += value +! "\""
      | Boolean(k) => buf += " " +! k,
      attributes,
    );
    buf +! ">";
  };
}

and buildElement = (indentLevel, buf, element) => {
  let indentSize = 4;
  let sp = String.make(indentSize * indentLevel, ' ');
  let closeTag = (buf, tag) => buf += "</" += tag +! ">\n";

  switch (element) {
  | Text(s) => buf += sp += s +! "\n"
  | Element({tag, attributes, children}) =>
    buildElementTag(buf += sp, tag, attributes);
    switch (children) {
    | [] => closeTag(buf, tag)
    | _ =>
      buf +! "\n";
      List.iter(elem => buildElement(indentLevel + 1, buf, elem), children);
      buf +! sp;
      closeTag(buf, tag);
    };
  };
};

let renderDocument = element => {
  let buf = Buffer.create(bufSize);
  buf +! "<!DOCTYPE html>\n";
  buildElement(0, buf, element);
  Buffer.contents(buf);
};

let renderElement = element => {
  let buf = Buffer.create(bufSize);
  buildElement(0, buf, element);
  Buffer.contents(buf);
};
