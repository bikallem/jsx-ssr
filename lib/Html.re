type t =
  | Text(string)
  | Element({
      tag: string,
      attributes: list(Attribute.t),
      children: list(t),
    });

let text = txt => Text(Encoder.encodeHtml(txt));
let rawText = txt => Text(txt);
let emptyText = rawText("");
let comment = txt =>
  Text(Encoder.encodeHtml(txt) |> Printf.sprintf("<!-- %s -->"));
let char = char => text @@ String.make(1, char);
let int = int => text @@ string_of_int(int);
let float = float => text @@ string_of_float(float);

let createElement = (tag, attributes, ~children=[], ()) =>
  Element({tag, attributes, children});

let (+=) = (buf, text) => {
  Buffer.add_string(buf, text);
  buf;
};

let (+!) = (buf, text) => Buffer.add_string(buf, text);
let bufSize = 1024;

let renderElementTag = (buf, tag, attributes) =>
  switch (attributes) {
  | [] => buf += "<" += tag +! ">"
  | _ =>
    buf += "<" +! tag;
    List.iter(a => buf += " " +! Attribute.toString(a), attributes);
    buf +! ">";
  };

let rec renderElement = (indentLevel, buf, element) => {
  let indentSize = 4;
  let sp = String.make(indentSize * indentLevel, ' ');
  let closeTag = (buf, tag) => buf += "</" += tag +! ">\n";
  switch (element) {
  | Text(s) => buf += sp += s +! "\n"
  | Element({tag, attributes, children}) =>
    renderElementTag(buf += sp, tag, attributes);
    switch (children) {
    | [] => closeTag(buf, tag)
    | _ =>
      buf +! "\n";
      List.iter(elem => renderElement(indentLevel + 1, buf, elem), children);
      buf +! sp;
      closeTag(buf, tag);
    };
  };
};

let renderAsDoc = element => {
  let buf = Buffer.create(bufSize);
  buf +! "<!DOCTYPE html>\n";
  renderElement(0, buf, element);
  Buffer.contents(buf);
};

let render = element => {
  let buf = Buffer.create(bufSize);
  renderElement(0, buf, element);
  Buffer.contents(buf);
};
