type t;

let text: string => t;
let raw: string => t;
let comment: string => t;
let char: char => t;
let int: int => t;
let float: float => t;
let createElement:
  (string, list(Attribute.t), ~children: list(t)=?, unit) => t;
let pp: (Format.formatter, t) => unit;
