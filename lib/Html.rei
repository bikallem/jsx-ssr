/* Html module represents a HtmlJSX view engine. */
type element;

type attribute;

let attr: (string, string) => attribute;
let flag: string => attribute;

let text: string => element;
let rawText: string => element;
let char: char => element;
let int: int => element;
let float: float => element;
let createElement:
  (string, list(attribute), ~children: list(element)=?, unit) => element;

let renderDocument: element => string;
let renderElement: element => string;
