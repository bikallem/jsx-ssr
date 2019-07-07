let encodingRequired = text => {
  let rec loop = (text, i, l) =>
    if (i < l) {
      switch (text.[i]) {
      | '"'
      | '&'
      | '\''
      | '/'
      | '<'
      | '>' => (true, i)
      | '\t'
      | '\n'
      | '\r'
      | ' '..'~' => loop(text, i + 1, l)
      | _ => (true, i)
      };
    } else {
      (false, (-1));
    };

  loop(text, 0, String.length(text));
};

let encodeHtml = text => {
  let len = String.length(text);
  switch (encodingRequired(text)) {
  | (true, i) =>
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
              /* Illegal characters in html
                 http://en.wikipedia.org/wiki/Character_encodings_in_HTML
                 http://www.w3.org/TR/html5/syntax.html */
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
  | (false, _) => text
  };
};
