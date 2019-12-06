type t =
  | Text(string)
  | Element({
      tag: string,
      attributes: list(Attribute.t),
      children: list(t),
    });

let createElement = (tag, attributes, ~children=[], ()) =>
  Element({tag, attributes, children});

let text = txt => Text(Encoder.encodeHtml(txt));
let raw = txt => Text(txt);

let comment = txt =>
  Text(Encoder.encodeHtml(txt) |> Printf.sprintf("<!-- %s -->"));

let char = char => text @@ String.make(1, char);
let int = int => text @@ string_of_int(int);
let float = float => text @@ string_of_float(float);

let ppElement = (fmt, tag, attributes) =>
  switch (attributes) {
  | [] => Format.fprintf(fmt, "<%s>", tag)
  | _ =>
    Format.fprintf(fmt, "<%s", tag);
    List.iter(a => Format.fprintf(fmt, " %a", Attribute.pp, a), attributes);
    Format.fprintf(fmt, ">");
  };

let rec pp = (fmt, t) => {
  let ppCloseTag = (fmt, tag) => Format.fprintf(fmt, "</%s>", tag);
  switch (t) {
  | Text(s) => Format.fprintf(fmt, "%s", s)
  | Element({tag, attributes, children}) =>
    ppElement(fmt, tag, attributes);
    switch (children) {
    | [] => ppCloseTag(fmt, tag)
    | _ =>
      List.iter(elem => pp(fmt, elem), children);
      ppCloseTag(fmt, tag);
    };
  };
};
