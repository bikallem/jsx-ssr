module Util: {
  /* Encodes the given text.
     Additionally, illegal characters as defined in
     http://en.wikipedia.org/wiki/Character_encodings_in_HTML and
     http://www.w3.org/TR/html5/syntax.html
     are normalised.*/
  let encodeHtml: string => string;
};

module Attribute: {
  type t;

  /* Create a attribue from a given tuple, eg. [attr(("id", "id1"))] creates
     an attribute 'id' with value 'id1'*/
  let attr: (string, string) => t;

  /* Creates a flag attribute, i.e. attributes that don't have a value, eg.
     'checked'. */
  let flag: string => t;

  let toString: t => string;
};

module A = Attribute;

/** Element represents a HtmlJSX view engine. **/

module Element: {
  /* Represents a html node, e.g. div, span, p etc. */
  type t;

  /* Create a html text t from a given string. The result html is html
     encoded/escapded. */
  let text: string => t;

  /* Create a html text t without any html encoding. Warning: use with
     care since using this function may expose you to XSS attacks. */
  let rawText: string => t;

  let emptyText: t;

  let comment: string => t;

  /* Creates a html t from char. */
  let char: char => t;

  /* Creates a html t from int. */
  let int: int => t;

  /* Creates a html t from float. */
  let float: float => t;

  /* Creates a html ts with given html tag name, attributes and children.
     e.g. [createt('div', attrs, ~children)] creates a html div ts
     with attributes 'attrs' and children as 'children'. */
  let createElement:
    (string, list(Attribute.t), ~children: list(t)=?, unit) => t;

  /* Renders a given t as a Html document string, i.e. html doc
     declaration are printed, eg. "<!DOCTYPE html>" */
  let renderAsDoc: t => string;

  /* Renders a given t to a string. */
  let render: t => string;
};

module E = Element;
