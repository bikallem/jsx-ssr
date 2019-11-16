let isControlChar = code =>
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

let encodeChar = (buffer, ch) =>
  switch (ch) {
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
  | `Malformed(_) => Buffer.add_utf_8_uchar(buffer, Uutf.u_rep)
  };

let encodeHtml = text => {
  let buffer = Buffer.create @@ String.length(text);
  Uutf.String.fold_utf_8((_, _, ch) => encodeChar(buffer, ch), (), text);
  Buffer.contents(buffer);
};
