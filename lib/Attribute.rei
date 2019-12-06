type t;

let attr: (string, string) => t;
let flag: string => t;
let pp: (Format.formatter, t) => unit;
