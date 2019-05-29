/* This ppx converts the given jsx into a htmlElement instance. */

open Migrate_parsetree;
open Ast_407;

open Ast_mapper;
open Parsetree;
open Ast_helper;
open Asttypes;

let ocaml_version = Versions.ocaml_407;

let str_expr = s => [%expr [%e Exp.constant(Pconst_string(s, None))]];

let map_constant = value =>
  switch (value) {
  | {pexp_desc: Pexp_ident({txt: Lident(ident), _}), pexp_attributes, _} =>
    let isRawLiteral = (
      fun
      | ({txt: "reason.raw_literal", _}, _) => true
      | _ => false
    );
    List.exists(isRawLiteral, pexp_attributes) ? str_expr(ident) : value;
  | {pexp_desc: Pexp_constant(Pconst_integer(_)), _} as c =>
    %expr
    string_of_int([%e c])
  | {pexp_desc: Pexp_constant(Pconst_char(_)), _} as c =>
    %expr
    String.make(1, [%e c])
  | {pexp_desc: Pexp_constant(Pconst_float(_)), _} as c =>
    %expr
    string_of_float([%e c])
  | _ => value
  };

let map_attributes = attributes => {
  List.fold_right(
    (attr, acc) =>
      switch (attr) {
      | (Nolabel, [%expr ()]) => acc
      | (Labelled("children"), _) => acc
      | (Labelled(propName), value) =>
        let key = str_expr(propName);
        let value = map_constant(value);
        %expr
        [Html.attr([%e key], [%e value]), ...[%e acc]];
      | _ => failwith("Invalid attribute")
      },
    attributes,
    [%expr []],
  );
};

let rec map_children =
  fun
  | {
      pexp_desc:
        Pexp_construct(
          {txt: Lident("::"), _} as cons,
          Some({pexp_desc: Pexp_tuple(tuple), _} as a),
        ),
      _,
    } as e => {
      let tuple =
        switch (tuple) {
        | [{pexp_desc: Pexp_constant(_), _} as car, cdr] => [
            map_constant(car),
            map_children(cdr),
          ]
        | [car, cdr] => [car, map_children(cdr)]
        | x => x
        };

      {
        ...e,
        pexp_desc:
          Pexp_construct(cons, Some({...a, pexp_desc: Pexp_tuple(tuple)})),
      };
    }
  | x => x;

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
      let children =
        List.find(
          fun
          | (Labelled("children"), _) => true
          | _ => false,
          args,
        )
        |> snd
        |> map_children;

      %expr
      Html.element(
        [%e str_expr(html_tag)],
        [%e attributes],
        ~children=[%e default_mapper.expr(mapper, children)],
        (),
      );
    | _ => default_mapper.expr(mapper, e)
    };
  };

  {...default_mapper, expr};
};

let () = Driver.register(~name="JSX", ocaml_version, mapper);
