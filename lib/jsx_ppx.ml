open Migrate_parsetree
open Ast_407
open Ast_mapper
open Parsetree
open Ast_helper
open Asttypes

let strExpr s = [%expr [%e Exp.constant (Pconst_string (s, None))]]

let mapConstExpr mapper expr =
  match expr with
  | {pexp_desc = Pexp_ident {txt = Lident ident; _}; pexp_attributes; _} as e
    -> (
    let isRawLiteral = function
      | {txt = "reason.raw_literal"; _}, _ -> true
      | _ -> false
    in
    match List.exists isRawLiteral pexp_attributes with
    | true -> mapper @@ strExpr ident
    | false -> e )
  | {pexp_desc = Pexp_constant (Pconst_integer _); _} as c ->
    mapper @@ [%expr string_of_int [%e c]]
  | {pexp_desc = Pexp_constant (Pconst_char _); _} as c ->
    mapper @@ [%expr String.make 1 [%e c]]
  | {pexp_desc = Pexp_constant (Pconst_float _); _} as c ->
    mapper @@ [%expr string_of_float [%e c]]
  | {pexp_desc = Pexp_constant (Pconst_string _); _} as c ->
    mapper @@ [%expr [%e c]]
  | _ -> expr

let rec mapChildren e =
  match e with
  | { pexp_desc =
        Pexp_construct
          ( ({txt = Lident "::"; _} as cons)
          , Some ({pexp_desc = Pexp_tuple tuple; _} as a) )
    ; _ } as e ->
    let tuple =
      match tuple with
      | [({pexp_desc = Pexp_constant _; _} as car); cdr] ->
        let mapper expr = [%expr Html.text [%e expr]] in
        [mapConstExpr mapper car; mapChildren cdr]
      | [car; cdr] -> [car; mapChildren cdr]
      | x -> x
    in
    { e with
      pexp_desc =
        Pexp_construct (cons, Some {a with pexp_desc = Pexp_tuple tuple}) }
  | x -> x

let mapAttributeName = function
  | "className" -> "class"
  | "htmlFor" -> "for"
  | "type_" -> "type"
  | "to_" -> "to"
  | "open_" -> "open"
  | "begin_" -> "begin"
  | "end_" -> "end"
  | "in_" -> "in"
  | attr -> attr

let mapExpression mapper e =
  match e with
  | { pexp_attributes = [({txt = "JSX"; loc = _}, PStr [])]
    ; pexp_desc =
        Pexp_apply
          ({pexp_desc = Pexp_ident {txt = Lident html_tag; loc = _}; _}, args)
    ; pexp_loc = _ } ->
    let attributes =
      List.fold_right
        (fun attr acc ->
          match attr with
          | Nolabel, [%expr ()] -> acc
          | Labelled "children", _ -> acc
          | Labelled attr, value ->
            let key = attr |> mapAttributeName |> strExpr in
            let value = mapConstExpr (fun e -> e) value in
            [%expr Attribute.attr [%e key] [%e value] :: [%e acc]]
          | Nolabel, _
           |Optional _, _ ->
            failwith "Invalid attribute")
        args [%expr []]
    in
    let children =
      List.find
        (function
          | Labelled "children", _ -> true
          | _ -> false)
        args
      |> snd
      |> mapChildren
    in
    [%expr
      Html.createElement [%e strExpr html_tag] [%e attributes]
        ~children:[%e default_mapper.expr mapper children]
        ()]
  | { pexp_attributes = [({txt = "JSX"; _}, PStr [])]
    ; pexp_desc =
        Pexp_apply
          ( ( {pexp_desc = Pexp_ident {txt = Ldot (_, "createElement"); _}; _}
            as expr )
          , args )
    ; pexp_loc } ->
    let args =
      List.fold_right
        (fun arg args' ->
          match arg with
          | (Labelled "children" as lbl), children ->
            (lbl, default_mapper.expr mapper children) :: args'
          | Nolabel, [%expr ()] -> args'
          | x -> x :: args')
        args []
    in
    Pexp_apply (expr, args) |> Exp.mk ~loc:pexp_loc
  | e -> default_mapper.expr mapper e

let mapper _ _ = {default_mapper with expr = mapExpression}
let () = Driver.register ~name:"JSX" Versions.ocaml_407 mapper
