/* Html module represents a HtmlJSX view engine. */
type t;

type attribute;

let attr: (string, string) => attribute;
let flag: string => attribute;

let text: string => t;
let char: char => t;
let int: int => t;
let float: float => t;
let createElement:
  (string, list(attribute), ~children: list(t)=?, unit) => t;

let renderDocument: t => string;
let renderElement: t => string;
