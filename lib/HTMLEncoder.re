let encodingRequired = text => {
  let rec loop = (text, i, l) =>
    i < l
    && (
      switch (text.[i]) {
      | '"'
      | '&'
      | '\''
      | '/'
      | '<'
      | '>' => true
      | '\t'
      | '\n'
      | '\r'
      | ' '..'~' => loop(text, i + 1, l)
      | _ => true
      }
    );

  loop(text, 0, String.length(text));
};
