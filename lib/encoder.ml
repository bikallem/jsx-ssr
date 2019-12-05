let is_control_char code =
  (code <= 31 && code <> 9 && code <> 10 && code <> 13)
  || (code >= 127 && code <= 159)
  || code land 0xFFFF = 0xFFFE
  || code land 0xFFFF = 0xFFFF

let encode_char buffer ch =
  match ch with
  | `Uchar u -> (
    match Uchar.to_int u with
    | 38 -> Buffer.add_string buffer "&amp;"
    | 60 -> Buffer.add_string buffer "&lt;"
    | 62 -> Buffer.add_string buffer "&gt;"
    | 34 -> Buffer.add_string buffer "&quot;"
    | 39 -> Buffer.add_string buffer "&#x27;"
    | 47 -> Buffer.add_string buffer "&#x2F;"
    | code when is_control_char code ->
      Buffer.add_utf_8_uchar buffer Uutf.u_rep
    | _ -> Buffer.add_utf_8_uchar buffer u )
  | `Malformed _ -> Buffer.add_utf_8_uchar buffer Uutf.u_rep

let encode_html text =
  let buffer = Buffer.create @@ String.length text in
  Uutf.String.fold_utf_8 (fun _ _ ch -> encode_char buffer ch) () text;
  Buffer.contents buffer
