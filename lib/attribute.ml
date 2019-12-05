type t =
  | KeyValue of
      { key : string
      ; value : string }
  | Boolean of string

let attr key value = KeyValue {key; value = Encoder.encode_html value}
let flag key = Boolean key

let pp fmt = function
  | KeyValue {key; value} -> Format.fprintf fmt "%s=\"%s\"" key value
  | Boolean k -> Format.fprintf fmt "%s" k
