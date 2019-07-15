/** Html module represents a HtmlJSX view engine. **/

module View: {
  /* Represents a html node, e.g. div, span, p etc. */
  type element;

  /* Represents an attribute of a html node. */
  type attribute;

  /* Create a attribue from a given tuple, eg. [attr(("id", "id1"))] creates
     an attribute 'id' with value 'id1'*/
  let attr: (string, string) => attribute;

  /* Creates a flag attribute, i.e. attributes that don't have a value, eg.
     'checked'. */
  let flag: string => attribute;

  /* Create a html text element from a given string. The result html is html
     encoded/escapded. */
  let text: string => element;

  /* Create a html text element without any html encoding. Warning: use with
     care since using this function may expose you to XSS attacks. */
  let rawText: string => element;

  let emptyText: element;

  let comment: string => element;

  /* Creates a html element from char. */
  let char: char => element;

  /* Creates a html element from int. */
  let int: int => element;

  /* Creates a html element from float. */
  let float: float => element;

  /* Creates a html elements with given html tag name, attributes and children.
     e.g. [createElement('div', attrs, ~children)] creates a html div elements
     with attributes 'attrs' and children as 'children'. */
  let createElement:
    (string, list(attribute), ~children: list(element)=?, unit) => element;

  /* Html encodes the given text.

      Additionally, illegal characters as defined in
      http://en.wikipedia.org/wiki/Character_encodings_in_HTML and
      http://www.w3.org/TR/html5/syntax.html
      are normalised.
     */
  let encodeHtml: string => string;

  /* Renders a given element as a Html document string, i.e. html doc
     declaration are printed, eg. "<!DOCTYPE html>" */
  let renderAsDoc: element => string;

  /* Renders a given element to a string. */
  let render: element => string;
};

module V = View;
