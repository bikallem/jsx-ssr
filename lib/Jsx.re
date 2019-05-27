/* This ppx just converts the given jsx into a html tag name expression. */

open Migrate_parsetree;
open Ast_407;

open Ast_mapper;
open Parsetree;
open Ast_helper;
open Asttypes;

let ocaml_version = Versions.ocaml_407;

let str_expr = s => [%expr [%e Exp.constant(Pconst_string(s, None))]];

let map_attribute = attributes => {
  let map_value = value =>
    switch (value) {
    | {pexp_desc: Pexp_ident({txt: Lident(ident), _}), pexp_attributes, _} =>
      if (List.exists(
            fun
            | ({txt: "reason.raw_literal", _}, _) => true
            | _ => false,
            pexp_attributes,
          )) {
        str_expr(ident);
      } else {
        value;
      }
    | _ => str_expr("hello")
    };

  List.map(
    fun
    | (Nolabel, _) => failwith("Nolabel arg not supported yet")
    | (Optional(_), _) => failwith("Optional arg not supported yet")
    | (Labelled(lbl), value) => {
        let key = str_expr(lbl);
        let value = map_value(value);
        %expr
        Html.attr([%e key], [%e value]);
      },
    attributes,
  );
};

let mapper = (_, _) => {
  let expr = (mapper, e) => {
    switch (e) {
    | {
        pexp_attributes: [({txt: "JSX", loc: _}, PStr([]))],
        pexp_desc:
          Pexp_apply(
            {pexp_desc: Pexp_ident({txt: Lident(html_tag), loc: _}), _},
            _args,
          ),
        pexp_loc: _,
      } =>
      %expr
      Html.element([%e str_expr(html_tag)], [], ())
    | _ => default_mapper.expr(mapper, e)
    };
  };

  {...default_mapper, expr};
};

let () = Driver.register(~name="JSX", ocaml_version, mapper);
