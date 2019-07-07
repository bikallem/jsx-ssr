let encodingRequired = text => {
  let rec loop = (text, i, l) =>
    switch (text.[i]) {
    | '"'
    | '&'
    | '\''
    | '/'
    | '<'
    | '>' => (true, Some(i))
    | '\t'
    | '\n'
    | '\r'
    | ' '..'~' =>
      if (i < l) {
        loop(text, i + 1, l);
      } else {
        (false, None);
      }
    | _ => (true, Some(i))
    };

  loop(text, 0, String.length(text));
};
