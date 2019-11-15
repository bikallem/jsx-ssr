type t = KeyValue of {key: string; value: string} | Boolean of string

let attr key value = KeyValue {key; value= Encoder.encodeHtml value}
let flag key = Boolean key

let toString =
  let open Printf in
  function
  | KeyValue {key; value} -> sprintf "%s=\"%s\"" key value
  | Boolean k -> sprintf "%s" k
