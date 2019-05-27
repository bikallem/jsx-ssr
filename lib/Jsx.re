/* This ppx converts the given jsx into a htmlElement instance. */

open Migrate_parsetree;
open Ast_407;

open Ast_mapper;
open Parsetree;
open Ast_helper;
open Asttypes;

let ocaml_version = Versions.ocaml_407;

let str_expr = s => [%expr [%e Exp.constant(Pconst_string(s, None))]];

let map_attributes = attributes => {
  let rec map_value = value =>
    switch (value) {
    | {pexp_desc: Pexp_ident({txt: Lident(ident), _}), pexp_attributes, _} =>
      let isRawLiteral = (
        fun
        | ({txt: "reason.raw_literal", _}, _) => true
        | _ => false
      );
      List.exists(isRawLiteral, pexp_attributes) ? str_expr(ident) : value;
    | {pexp_desc: Pexp_constant(_), _} as c => map_constant(c)
    | _ => value
    }

  and map_constant = const => {
    switch (const) {
    | {pexp_desc: Pexp_constant(Pconst_integer(_)), _} =>
      %expr
      string_of_int([%e const])
    | _ => const
    };
  };

  List.fold_right(
    (attr, acc) =>
      switch (attr) {
      | (Nolabel, [%expr ()]) => acc
      | (Labelled("children"), _) => acc
      | (Labelled(propName), value) =>
        let key = str_expr(propName);
        let value = map_value(value);
        %expr
        [Html.attr([%e key], [%e value]), ...[%e acc]];
      | _ => failwith("Invalid attribute")
      },
    attributes,
    [%expr []],
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
            args,
          ),
        pexp_loc: _,
      } =>
      let attributes = map_attributes(args);
      %expr
      Html.element([%e str_expr(html_tag)], [%e attributes], ());
    | _ => default_mapper.expr(mapper, e)
    };
  };

  {...default_mapper, expr};
};

let () = Driver.register(~name="JSX", ocaml_version, mapper);
