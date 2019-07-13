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

/* Returns Some(i) if the given text contains a character that needs to be
   html encoded. 'i' is the 0-based index where the first such character is
   found. */
let indextOfFirstEncodingChar = text => {
  let rec loop = (text, i, l) =>
    if (i < l) {
      switch (text.[i]) {
      | '"'
      | '&'
      | '\''
      | '/'
      | '<'
      | '>' => Some(i)
      | '\t'
      | '\n'
      | '\r'
      | ' '..'~' => loop(text, i + 1, l)
      | _ => Some(i)
      };
    } else {
      None;
    };

  loop(text, 0, String.length(text));
};

let encodeHtml = text => {
  let len = String.length(text);
  switch (indextOfFirstEncodingChar(text)) {
  | Some(i) =>
    let buffer = Buffer.create(len);
    Uutf.String.fold_utf_8(
      ~pos=i,
      ~len,
      (_, _, d) =>
        switch (d) {
        | `Uchar(u) =>
          switch (Uchar.to_int(u)) {
          | 38 => Buffer.add_string(buffer, "&amp;")
          | 60 => Buffer.add_string(buffer, "&lt;")
          | 62 => Buffer.add_string(buffer, "&gt;")
          | 34 => Buffer.add_string(buffer, "&quot;")
          | 39 => Buffer.add_string(buffer, "&#x27;")
          | 47 => Buffer.add_string(buffer, "&#x2F;")
          | code =>
            let u =
              if (code <= 31
                  && code != 9
                  && code != 10
                  && code != 13
                  || code >= 127
                  && code <= 159
                  || code
                  land 0xFFFF == 0xFFFE
                  || code
                  land 0xFFFF == 0xFFFF) {
                Uutf.u_rep;
              } else {
                u;
              };
            Buffer.add_utf_8_uchar(buffer, u);
          }
        | `Malformed(_) => Buffer.add_utf_8_uchar(buffer, Uutf.u_rep)
        },
      (),
      text,
    );
    Buffer.contents(buffer);
  | None => text
  };
};
let attr = (key, value) => KeyValue({key, value: encodeHtml(value)});

let flag = key => Boolean(key);

let text = txt => Text(encodeHtml(txt));
let rawText = txt => Text(txt);
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

let renderAsDoc = element => {
  let buf = Buffer.create(bufSize);
  buf +! "<!DOCTYPE html>\n";
  buildElement(0, buf, element);
  Buffer.contents(buf);
};

let render = element => {
  let buf = Buffer.create(bufSize);
  buildElement(0, buf, element);
  Buffer.contents(buf);
};
