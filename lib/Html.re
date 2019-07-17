module Util = {
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

  let printIndex = text => {
    let index = indextOfFirstEncodingChar(text);
    Fmt.(pr("%a", option(~none=nop, int), index));
  };

  let%expect_test "indexOfFirstEncodingChar(\"Hello world\")" = {
    printIndex("Hello, world!");
    %expect
    {||};
  };

  let%expect_test "indexOfFirstEncodingChar(\"Hello, <div>world</div>\")" = {
    printIndex("Hello, <div>world</div>");
    %expect
    {| 7 |};
  };

  let isControlChar = code => {
    code <= 31
    && code != 9
    && code != 10
    && code != 13
    || code >= 127
    && code <= 159
    || code
    land 0xFFFF == 0xFFFE
    || code
    land 0xFFFF == 0xFFFF;
  };

  let encodeChar = buffer => {
    fun
    | `Uchar(u) =>
      switch (Uchar.to_int(u)) {
      | 38 => Buffer.add_string(buffer, "&amp;")
      | 60 => Buffer.add_string(buffer, "&lt;")
      | 62 => Buffer.add_string(buffer, "&gt;")
      | 34 => Buffer.add_string(buffer, "&quot;")
      | 39 => Buffer.add_string(buffer, "&#x27;")
      | 47 => Buffer.add_string(buffer, "&#x2F;")
      | code when isControlChar(code) =>
        Buffer.add_utf_8_uchar(buffer, Uutf.u_rep)
      | _ => Buffer.add_utf_8_uchar(buffer, u)
      }
    | `Malformed(_) => Buffer.add_utf_8_uchar(buffer, Uutf.u_rep);
  };

  let encodeHtml = text => {
    let len = String.length(text);
    switch (indextOfFirstEncodingChar(text)) {
    | Some(i) =>
      let buffer = Buffer.create(len);
      Uutf.String.fold_utf_8(
        ~pos=i,
        ~len,
        (_, _, ch) => encodeChar(buffer, ch),
        (),
        text,
      );
      Buffer.contents(buffer);
    | None => text
    };
  };
};

module U = Util;

module Attribute = {
  type t =
    | KeyValue({
        key: string,
        value: string,
      })
    | Boolean(string);

  let attr = (key, value) => KeyValue({key, value: U.encodeHtml(value)});
  let flag = key => Boolean(key);
  let toString =
    Printf.(
      fun
      | KeyValue({key, value}) => sprintf("%s=\"%s\"", key, value)
      | Boolean(k) => sprintf("%s", k)
    );
};
module A = Attribute;

module Element = {
  type t =
    | Text(string)
    | Element({
        tag: string,
        attributes: list(Attribute.t),
        children: list(t),
      });

  let text = txt => Text(U.encodeHtml(txt));
  let rawText = txt => Text(txt);
  let emptyText = rawText("");
  let comment = txt =>
    Text(U.encodeHtml(txt) |> Printf.sprintf("<!-- %s -->"));
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

  let renderElementTag = (buf, tag, attributes) => {
    switch (attributes) {
    | [] => buf += "<" += tag +! ">"
    | _ =>
      buf += "<" +! tag;
      List.iter(a => buf += " " +! A.toString(a), attributes);
      buf +! ">";
    };
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
        List.iter(
          elem => renderElement(indentLevel + 1, buf, elem),
          children,
        );
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
};

module E = Element;
