/* This ppx converts the given jsx into a htmlElement instance. */

open Migrate_parsetree;
open Ast_407;

open Ast_mapper;
open Parsetree;
open Ast_helper;
open Asttypes;

let strExpr = s => [%expr [%e Exp.constant(Pconst_string(s, None))]];

let mapConstToStrExpr = (mapper, expr) =>
  switch (expr) {
  | {pexp_desc: Pexp_ident({txt: Lident(ident), _}), pexp_attributes, _} as e =>
    let isRawLiteral = (
      fun
      | ({txt: "reason.raw_literal", _}, _) => true
      | _ => false
    );

    List.exists(isRawLiteral, pexp_attributes)
      ? mapper @@ strExpr(ident) : e;
  | {pexp_desc: Pexp_constant(Pconst_integer(_)), _} as c =>
    mapper @@ [%expr string_of_int([%e c])]
  | {pexp_desc: Pexp_constant(Pconst_char(_)), _} as c =>
    mapper @@ [%expr String.make(1, [%e c])]
  | {pexp_desc: Pexp_constant(Pconst_float(_)), _} as c =>
    mapper @@ [%expr string_of_float([%e c])]
  | {pexp_desc: Pexp_constant(Pconst_string(_)), _} as c =>
    mapper @@ [%expr [%e c]]
  | _ => expr
  };

let rec mapChildren = e =>
  switch (e) {
  | {
      pexp_desc:
        Pexp_construct(
          {txt: Lident("::"), _} as cons,
          Some({pexp_desc: Pexp_tuple(tuple), _} as a),
        ),
      _,
    } as e =>
    let tuple =
      switch (tuple) {
      | [{pexp_desc: Pexp_constant(_), _} as car, cdr] =>
        let mapper = expr => [%expr Html.text([%e expr])];
        [mapConstToStrExpr(mapper, car), mapChildren(cdr)];
      | [car, cdr] => [car, mapChildren(cdr)]
      | x => x
      };

    {
      ...e,
      pexp_desc:
        Pexp_construct(cons, Some({...a, pexp_desc: Pexp_tuple(tuple)})),
    };
  | x => x
  };

/* Given a list of args, it returns a tuple of attributes and children. */
let mapArgs = args => {
  let attributes =
    List.fold_right(
      (attr, acc) =>
        switch (attr) {
        | (Nolabel, [%expr ()]) => acc
        | (Labelled("children"), _) => acc
        | (Labelled(propName), value) =>
          let key = strExpr(propName);
          let value = mapConstToStrExpr(e => e, value);
          %expr
          [Html.attr([%e key], [%e value]), ...[%e acc]];
        | (Nolabel, _)
        | (Optional(_), _) => failwith("Invalid attribute")
        },
      args,
      [%expr []],
    );

  let children =
    List.find(
      fun
      | (Labelled("children"), _) => true
      | _ => false,
      args,
    )
    |> snd
    |> mapChildren;
  (attributes, children);
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
      let (attributes, children) = mapArgs(args);

      %expr
      Html.element(
        [%e strExpr(html_tag)],
        [%e attributes],
        ~children=[%e default_mapper.expr(mapper, children)],
        (),
      );
    | {
        pexp_attributes: [({txt: "JSX", _}, PStr([]))],
        pexp_desc:
          Pexp_apply(
            {pexp_desc: Pexp_ident({txt: Ldot(_, "createElement"), _}), _} as expr,
            args,
          ),
        pexp_loc,
      } =>
      let args =
        List.fold_right(
          (arg, args') =>
            switch (arg) {
            | (Labelled("children") as lbl, children) => [
                (lbl, default_mapper.expr(mapper, children)),
                ...args',
              ]
            | (Nolabel, [%expr ()]) => args'
            | x => [x, ...args']
            },
          args,
          [],
        );
      Pexp_apply(expr, args) |> Exp.mk(~loc=pexp_loc);
    | e => default_mapper.expr(mapper, e)
    };
  };

  {...default_mapper, expr};
};

let () = Driver.register(~name="JSX", Versions.ocaml_407, mapper);
