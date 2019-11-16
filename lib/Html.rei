/** Element represents a HtmlJSX view engine. */
type t;

let text: string => t;
let rawText: string => t;
let emptyText: t;
let comment: string => t;
let char: char => t;
let int: int => t;
let float: float => t;
let createElement:
  (string, list(Attribute.t), ~children: list(t)=?, unit) => t;
let renderAsDoc: t => string;
let render: t => string;
