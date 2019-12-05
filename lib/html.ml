type t =
  | Text of string
  | Element of
      { tag : string
      ; attributes : Attribute.t list
      ; children : t list }

let createElement tag attributes ?(children = []) () =
  Element {tag; attributes; children}

let text txt = Text (Encoder.encode_html txt)
let raw txt = Text txt

let comment txt = Text (Encoder.encode_html txt |> Printf.sprintf "<!-- %s -->")

let char char = text @@ String.make 1 char
let int int = text @@ string_of_int int
let float float = text @@ string_of_float float

let pp_element_tag fmt tag attributes =
  match attributes with
  | [] -> Format.fprintf fmt "<%s>" tag
  | _ ->
    Format.fprintf fmt "<%s" tag;
    List.iter (fun a -> Format.fprintf fmt " %a" Attribute.pp a) attributes;
    Format.fprintf fmt ">"

let rec pp fmt t =
  let pp_close_tag fmt tag = Format.fprintf fmt "</%s>" tag in
  match t with
  | Text s -> Format.fprintf fmt "%s" s
  | Element {tag; attributes; children} -> (
    pp_element_tag fmt tag attributes;
    match children with
    | [] -> pp_close_tag fmt tag
    | _ ->
      List.iter (fun elem -> pp fmt elem) children;
      pp_close_tag fmt tag )
